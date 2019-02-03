# shprep

## Meta
v0.1.0 -- First Beta Release  
Last update: Feb 2, 2019  

## Index
- [Description](https://github.com/BryanMorfe/shprep/blob/master/README.md#description)
- [License](https://github.com/BryanMorfe/shprep/blob/master/README.md#license)
- [Examples](https://github.com/BryanMorfe/shprep/blob/master/README.md#examples)
- [Installation](https://github.com/BryanMorfe/shprep/blob/master/README.md#installation)
- [OS Compatibility](https://github.com/BryanMorfe/shprep/blob/master/README.md#os-compatibility)
- [TODO](https://github.com/BryanMorfe/shprep/blob/master/README.md#todo)

## Description
Unix/Linux migration program -- Prepares shadow file for migration in incompatible systems.  
This program was created between a migration from Unix Solaris 10 to Ubuntu (details are confidential), and since the password encryption algorithms are different, something had to be done to transfer thousands of user accounts without compromising the functionality of the system.  
This program made everything much easier, and now if you need to do the same, you can simply use this program to speed up the process.

## License
MIT License

Copyright (c) 2019 Evoluti, Inc. (Bryan Morfe CEO)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

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
| | **Architecture** | **Master** |
|---|:---:|:---:|
| **Unix Solaris 10** | x86_64 |![Build Status](https://ci.swift.org/job/oss-swift-incremental-RA-osx/lastCompletedBuild/badge/icon) |
| **Ubuntu 18.04** | x86_64 | ![Build Status](https://ci.swift.org/job/oss-swift-incremental-RA-linux-ubuntu-18_04/lastCompletedBuild/badge/icon)|
| **CentOS 7** | x86_64 | ![Build Status](https://ci.swift.org/job/oss-swift-incremental-RA-linux-ubuntu-18_04/lastCompletedBuild/badge/icon)|

## TODO
- Spread the code into a few other source files (maybe?)
