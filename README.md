# shprep

## Meta
v0.1.0 -- First Beta Release  
Last update: Feb 2, 2019  

## Index
- [Description](https://github.com/BryanMorfe/shprep/blob/master/README.md#description)
- [Examples](https://github.com/BryanMorfe/shprep/blob/master/README.md#examples)
- [Installation](https://github.com/BryanMorfe/shprep/blob/master/README.md#installation)
- [OS Compatibility](https://github.com/BryanMorfe/shprep/blob/master/README.md#os-compatibility)
- [TODO](https://github.com/BryanMorfe/shprep/blob/master/README.md#todo)

## Description
Unix/Linux migration tool -- Prepares shadow file for migration in incompatible systems.  

## Examples
In this hypothetical example I will be migrating from a Unix Solaris 10 system (IP Address 192.168.1.10) to linux Ubuntu 16 (IP Address 192.168.1.15).

### Step 1: Get Shadow File From Unix Solaris

#### Reverse Copy (From Ubuntu)
Run the following command on the **Ubuntu System** (note that you need an account in Solaris with root privileges):  
`$ scp root@192.168.1.10:/etc/shadow $HOME`  
`root@192.168.1.10's password:`  
  
After the password for root is entered, you should have a copy of the shadow file in the home directory for my user in Ubuntu.

#### Forward Copy (From Solaris)
Run the following command on the **Solaris System:** (sudo privileges needed on Solaris, but no root access is needed on Ubuntu):  
`$ sudo scp /etc/shadow user@192.168.1.15:~/`  
`password:`  
`user@192.168.1.15's password:`  
  
After both passwords are entered, you should have a copy of the shadow file for Solaris in the home directory of `user` on the Ubuntu System.  

### Step 2: Process Shadow File With shprep
Run the following commands on Ubuntu (assume pwd=$HOME):  
`$ mkdir processed`  
`$ shprep -c -r --shadow-file shadow --output-file processed/shadow -p D3fP@s5w0rD --hash-alg sha512`  
  
After that command, the generated shadow file will have default passwords for all users that originally had a password. Users that didn't have a password will stay that way. If no further processing is needed, this file could be copied into the /etc directory **AFTER A BACKUP OF THE ORIGINAL IS MADE**.  

#### Disclaimer
I am not responsible for any corrupted shadow files produced by this program. **ALWAYS** make a backup of the original shadow file before modifying it with this or any other software.  

## Installation

### Step 1: Get Source Code

#### Manually
Open terminal and run the following command:  
`$ git clone https://github.com/BryanMorfe/shprep`

#### Download Link
[Download](https://github.com/BryanMorfe/shprep/archive/master.zip) the zip version and decompress.  

### Step 2: Compile

**Note: If a compressed file was downloaded, decompress and then follow instructions here.**

Run the following command:  
`$ make`  

### Step 3: Install (Optional)
Run the following command:  
`$ make install`  

### Step 4: Clean (Optional)
Run the following command:  
`$ make clean`  

## OS Compatibility
Unix Solaris 10 - pass  
CentOS 7 - pass  
Raspbian - pass  
macOS Mojave - limited pass  

## TODO
- Spread the code into a few other source files (maybe?)
- That's comment the code a little
