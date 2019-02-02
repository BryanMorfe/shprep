/*
 * shprep Version 0.1.0
 * shprep is a simple unix/linux utility that processes shadow files
 * for purposes of migration. If two systems use incompatible encryptions
 * or hashing for the storage of user passwords, one cannot simply transfer
 * the shadow files since the system will not recognize the password. This
 * utility allows you to set a default password for all users and prepare your
 * shadow file for migration. Additionally, it allows you to specify that the
 * user should change their password the first time the login.
 *
 * Note: In order to make sure that the encryption algorithm is correct, you must
 * transfer the shadow file from the system you are migrating from to the system
 * you are migrating to.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define GOT_SHADOW_FILE     0x01
#define GOT_DEF_PASSWD      0x02
#define GOT_CHG_PASSWD      0x04
#define GOT_OUT_FPATH       0x08
#define GOT_RAND_SALT       0x10
#define GOT_HASH_ALG        0x20

#define HASH_MD5_VAL        1
#define HASH_BF1_VAL        2
#define HASH_BF2_VAL        3
#define HASH_SHA256_VAL     4
#define HASH_SHA512_VAL     5

#define HASH_MD5            "md5"
#define HASH_BF1            "blowfish"
#define HASH_BF2            "sksblowfish"
#define HASH_SHA256         "sha256"
#define HASH_SHA512         "sha512"

#define HASH_MD5_CD         "$1$"
#define HASH_BF1_CD         "$2$"
#define HASH_BF2_CD         "$2b$"
#define HASH_SHA256_CD      "$5$"
#define HASH_SHA512_CD      "$6$"

#define PASSWD_FIELD        1
#define PASSWD_CHG_FIELD    2

#define DEF_SALT            "$6$hxIzLpw"    /* Use sha-512 for hashing + salt */

void get_input(int argc, char *argv[], char *shadow_file_path, char *def_passwd, char *out_path);
void process_shadow_file(char *shadow_file_path, char *def_passwd, char *out_path);
int fpeekc(FILE *stream);
void display_help();
void display_ver();
void rand_salt(int len, char *salt);

int input_obtained = 0;
int hash_alg = 0;

int main(int argc, char *argv[])
{
    char shadow_fp[256], def_passwd[1024], out_path[256];
    
    /* Prepare for random salt generation */
    srand(time(NULL));
    
    get_input(argc, argv, shadow_fp, def_passwd, out_path);
    
    process_shadow_file(shadow_fp, def_passwd, out_path);
    
    return 0;
}

void get_input(int argc, char *argv[], char *shadow_file_path, char *def_passwd, char *out_path)
{
    int i;
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--shadow-file") == 0)
        {
            strcpy(shadow_file_path, argv[++i]);
            input_obtained |= GOT_SHADOW_FILE;
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--default-passwd") == 0)
        {
            strcpy(def_passwd, argv[++i]);
            input_obtained |= GOT_DEF_PASSWD;
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output-file") == 0)
        {
            strcpy(out_path, argv[++i]);
            input_obtained |= GOT_OUT_FPATH;
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--change-on-login") == 0)
            input_obtained |= GOT_CHG_PASSWD;
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--random-salt") == 0)
            input_obtained |= GOT_RAND_SALT;
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--hash-alg") == 0)
        {
            input_obtained |= GOT_HASH_ALG;
            
            if (strcmp(argv[++i], HASH_MD5) == 0)
                hash_alg = HASH_MD5_VAL;
            else if (strcmp(argv[i], HASH_BF1) == 0)
                hash_alg = HASH_BF1_VAL;
            else if (strcmp(argv[i], HASH_BF2) == 0)
                hash_alg = HASH_BF2_VAL;
            else if (strcmp(argv[i], HASH_SHA256) == 0)
                hash_alg = HASH_SHA256_VAL;
            else if (strcmp(argv[i], HASH_SHA512) == 0)
                hash_alg = HASH_SHA512_VAL;
            else
            {
                fprintf(stderr, "\033[1;31m[-] Unrecognized hashing algorithm:\033[0m %s.\n", argv[i]);
                exit(1);
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            display_help();
            exit(1);
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            display_ver();
            exit(1);
        }
        else
        {
            fprintf(stderr, "\033[1;31m[-] Invalid option:\033[0m %s. Run \"shprep -h\" to display the help menu.\n", argv[i]);
            exit(1);
        }
    }
    
    int should_exit = 0;
    
    if ((~input_obtained & GOT_SHADOW_FILE) == GOT_SHADOW_FILE)
    {
        should_exit = 1;
        fprintf(stderr, "\033[1;31m[-] Input error:\033[0m shadow file not specified.\n");
    }
    
    if ((~input_obtained & GOT_RAND_SALT) == GOT_RAND_SALT && (input_obtained & GOT_HASH_ALG) == GOT_HASH_ALG)
    {
        should_exit = 1;
        fprintf(stderr, "\033[1;31m[-] Logic error:\033[0m Cannot assign hashing algorithm without random salt option.\n");
    }
    
    if ((~input_obtained & GOT_DEF_PASSWD) == GOT_DEF_PASSWD)
    {
        should_exit = 1;
        fprintf(stderr, "\033[1;31m[-] Input error:\033[0m default password not specified.\n");
    }
    
    if ((~input_obtained & GOT_OUT_FPATH) == GOT_OUT_FPATH)
    {
        should_exit = 1;
        fprintf(stderr, "\033[1;31m[-] Input error:\033[0m output shadow files path not specified.\n");
    }
    
    if (strcmp(out_path, shadow_file_path) == 0)
    {
        should_exit = 1;
        fprintf(stderr, "\033[1;31m[-] Input error:\033[0m For security purposes, this program cannot override the input shadow file. Your output shadow file must be different than your input shadow file.\n");
    }
    
    if (should_exit)
    {
        printf("Run \"shprep -h\" to display the help menu.\n");
        exit(1);
    }
    
    if (hash_alg == 0)
        hash_alg = HASH_SHA512_VAL;
    
}

void process_shadow_file(char *shadow_file_path, char *def_passwd, char *out_path)
{
    FILE *sfh = fopen(shadow_file_path, "r");
    
    if (sfh == NULL)
    {
        fprintf(stderr, "\033[1;31m[-]Error:\033[0m cannot read file %s.\n", shadow_file_path);
        exit(1);
    }
    
    FILE *sfh_prep = fopen(out_path, "w");
    
    if (sfh_prep == NULL)
    {
        fprintf(stderr, "\033[1;31m[-] Error:\033[0m cannot open file %s.\n", out_path);
        exit(1);
    }

    int ch;
    int field_num = 0;
    
    printf("\033[1;33m[+]\033[0m Generating processed shadow file...\n");
    while ((ch = fgetc(sfh)) != EOF)
    {
        fputc(ch, sfh_prep);
        
        if (ch == ':')
            field_num++;
        else if (ch == '\n')
            field_num = 0;
        
        /* Change the users with a password to the default password */
        int peeked_c = fpeekc(sfh);
        if (field_num == PASSWD_FIELD && peeked_c != '*' && peeked_c != '!' && peeked_c != ':')
        {
            char salt[11];
            
            if ((input_obtained & GOT_RAND_SALT) == GOT_RAND_SALT)
                rand_salt(10, salt);
            else
                strcpy(salt, DEF_SALT);
            
            fprintf(sfh_prep, "%s", crypt(def_passwd, salt));
            
            while ((ch = fgetc(sfh)) != ':');
            field_num++;
            fputc(':', sfh_prep);
        }
        else if (field_num == PASSWD_CHG_FIELD && (input_obtained & GOT_CHG_PASSWD) == GOT_CHG_PASSWD)
        {
            fprintf(sfh_prep, "0"); /* Indicate passwd should be changed next time they log in */
            while ((ch = fgetc(sfh)) != ':');
            field_num++;
            fputc(':', sfh_prep);
        }
    }
    
    printf("\033[1;32m[*]\033[0m Shadow file \"%s\" generated...\n", out_path);
    
    fclose(sfh);
    fclose(sfh_prep);
    
    printf("\033[1;32m[*]\033[0m Cleaning up...\n");
}

int fpeekc(FILE *stream)
{
    int ch = fgetc(stream);
    ungetc(ch, stream);
    return ch;
}

void display_help()
{
    printf("Usage: shprep [Options]\n");
    printf("Example: shprep --shadow-file /etc/shadow --output-file /home/processed/shadow -p D3f@ulTP@s5 -c -r\n\n");
    printf("Options:\n");
    printf("\t-a, --hash-alg\t\t\t\tChoose hashing algorithm (sha512 is default and recommended).\n");
    printf("\t\tAvailable algorithms: md, blowfish, eksblowfish, sha256, sha512\n");
    printf("\t-c, --change-on-login\t\t\tIndicates a password change is required at login.\n");
    printf("\t-f, --shadow-file <path>|<file>\t\tSpecifies the path of the shadow file to be processed.\n");
    printf("\t-h, --help\t\t\t\tDisplays this menu.\n");
    printf("\t-o, --output-file <path>|<file>\t\tSpecifies the output shadow file (after processing).\n");
    printf("\t-p, --default-passwd <defaultpassword>\tSpecifies the default password for users with a password.\n");
    printf("\t-r, --random-salt\t\t\tGenerate random salt for each user password (recommended).\n");
    printf("\t-v, --version\t\t\t\tDisplay the version of shprep.\n");
    printf("\t\tNote: Users without a password or with no login will stay that way.\n");
}

void display_ver()
{
    printf("shprep v0.1.0\n");
    printf("Build Date: February 2, 2019\n");
    printf("Copyright (c) 2019 Evoluti Inc.\n");
}

void rand_salt(int len, char *salt)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const char *hash_alg_arr[] = {HASH_MD5_CD, HASH_BF1_CD, HASH_BF2_CD, HASH_SHA256_CD, HASH_SHA512_CD};
    
    int charset_len = strlen(charset);
    strcpy(salt, hash_alg_arr[hash_alg - 1]);
    
    int start_val = strlen(hash_alg_arr[hash_alg - 1]);
    int i;
    for (i = start_val; i < len; ++i)
        salt[i] = charset[rand() % charset_len];
    salt[i] = '\0';
}
