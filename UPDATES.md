# Updates File

## Description
This file contains metadata about the updates timeline of shprep.

## Index
- [Updates](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#updates)
	- [0.1.0](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#010)
		- [Known Bugs](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#known-bugs)
	- [0.1.1](https://github.com/BryanMorfe/shprep/blob/master/README.md#011)
		- [New Features/Fixes](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#new-featuresfixes)
		- [Known Bugs](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#known-bugs-1)
	- [0.1.2](https://github.com/BryanMorfe/shprep/blob/master/README.md#012)
		- [New Features/Fixes](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#new-featuresfixes-1)
		- [Known Bugs](https://github.com/BryanMorfe/shprep/blob/master/UPDATES.md#known-bugs-2)

## Updates

### 0.1.0
**First Public Release**  
**Build Date: February 2, 2019**  

#### Known Bugs
**As of February 7, 2019:**  
- If your gcc compiler requires `libcrypt` to be linked, it will not compile unless you add it manually (the how-to is on the [README](https://github.com/BryanMorfe/shprep/blob/master/README.md#installation)).
- Users with no password or no login had their password change field changed to zero. This could have cause some problems in some operating systems.
- Users whose password were changed had a problem when the password change field was requested to be changed. The first digit of the original value was left in there.

### 0.1.1
**Build Date: February 8, 2019**

#### New Features/Fixes
- Fixed bug with password change field having the first digit of the old value unchanged. Now it is properly set to zero (if requested), and user is prompted to change their password on login.
- Password change field is left unmodified on user accounts with no password or login disabled.

#### Known Bugs
**As of February 7, 2019:**  
- Makefile provided no option link the appropriate libraries (`-lcrypt`), should a particular system required it.

### 0.1.2
**Build Date: February 9, 2019**

#### New Features/Fixes
- Used autotools to create proper configure and installation scripts.

#### Known Bugs
**As of February 9, 2019:**  
- None.
