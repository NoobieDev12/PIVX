# SELF-COMPILING PIVX CORE WALLET FOR WINDOWS USING WINDOWS SUBSYSTEM FOR LINUX (WSL)

These are detailed step-by-step instructions for beginners on how to self-compile PIVX Core Wallet on Windows 10 using Windows Subsystem for Linux (WSL) directly from Master branch.

This tutorial is intended for anyone who is willing to help with testing of latest features on **testnet** that PIVX Core Developers are planning to implement on PIVX mainnet. If you find any bugs, please report them directly on PIVX Github:

https://github.com/PIVX-Project/PIVX/issues

**NOTE:** I wouldn't recommend doing this on the same machine where you are running your main PIVX wallet, not to break something and lose/damage your wallet.dat file. If you are doing it, please make sure that you have backup of your wallet.dat file safely stored somewhere before starting this. It's safer to run it on a different machine or Virtual Machine.

---------------------------------------------------------

## Let's start...

## COMPILING THE WALLET FOR THE FIRST TIME:

1. Press Windows button on keyboard, search for **Windows PowerShell** and start it. Type the following command:
```
Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux
```

2. Go to Microsoft Store and download **Ubuntu 18.04 LTS**:

[Ubuntu 18.04 LTS](https://www.microsoft.com/store/productId/9N9TNGVNDL3Q)

3. Setup your username and password and write it down somewhere, you will need it later.

4. Run these commands line by line:
```
sudo dpkg --configure -a
sudo apt update && sudo apt upgrade -y
sudo apt install build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake curl git nsis -y
sudo apt install libssl-dev libgmp-dev libevent-dev libboost-all-dev software-properties-common -y
sudo add-apt-repository -y ppa:pivx/pivx
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install libdb4.8-dev libdb4.8++-dev libminiupnpc-dev libzmq3-dev libcurl4-openssl-dev -y
sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev -y
sudo apt-get install qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev libsodium-dev cargo -y
sudo apt-get update && sudo apt-get upgrade -y
git clone https://github.com/pivx-project/pivx.git PIVX
cd PIVX
```
5. **Next 2 steps are important**: after 2nd command, you will be asked and you have to set the default mingw32 g++ compiler option to **posix** (most probably it will be option with number 1):
```
sudo apt install g++-mingw-w64-x86-64
sudo update-alternatives --config x86_64-w64-mingw32-g++
```
**NOTE:** During the step 6, at step `make HOST=x86_64-w64-mingw32`, when you see list like - timer, - wave etc, **press the ENTER on keyboard to continue**...it looks like it gets paused, but after pressing ENTER it will resume the building process.
Also, even if you get the **conftest.ext - System Error**, don't worry, just **click OK** and it will continue with the process.

6. Run the following commands line by line:
```
PATH=$(echo "$PATH" | sed -e 's/:\/mnt.*//g')
cd depends
make HOST=x86_64-w64-mingw32
cd ~/PIVX/
```

**IMPORTANT NOTE:** Make sure before running the next steps that your Antivirus software is not blocking or even worse deleting the pivx-qt.exe when running command `make -j$(nproc)`. It's common that bitcoin miner is detected as a virus (false alarm) and antivirus is deleting it automatically. **The best approach is to exclude from detection PIVX folder path in your antivirus** (same as it is mentioned in the last line, but replace `YOUR_WINDOWS_USERNAME` with your Windows User name, and `YOUR_WSL_USERNAME`:
`C:\Users\YOUR_WINDOWS_USERNAME\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc\LocalState\rootfs\home\YOUR_WSL_USERNAME\PIVX`

7. Run the following commands line by line:
```
./autogen.sh && CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/ --disable-online-rust
make -j$(nproc)
```
**NOTE:** You have successfully compiled the wallet! But...you are now one step away from starting the wallet.
There are 2 ways of starting the wallet:
1) starting directly from command line
2) starting by opening pivx-qt.exe file

**1) starting directly from command line:**
I'll go with the first option now, but for some users, command isn't working and returning `cmd.exe command not found`, but not a problem since option 2 will surely work in worst case. Run the following command:
```
cmd.exe /C start "C:\Users\YOUR_WINDOWS_USERNAME\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc\LocalState\rootfs\home\YOUR_WSL_USERNAME\PIVX\src\qt\pivx-qt.exe" --testnet
```
**IMPORTANT NOTE:** Don't forget again to replace `YOUR_WINDOWS_USERNAME` with your Windows User name, and `YOUR_WSL_USERNAME` with username you used in Step 3 earlier.


**2) starting by opening pivx-qt.exe file:**
After you successfully compile the wallet, you can copy-paste the compiled .exe file to a "fast reachable" folder on your Local Disc, by using the following command after `make -j$(nproc)`:

```
sudo make install DESTDIR=/mnt/c/workspace/PIVX
```
This will create an executable files in Local Disk (C:) -> workspace -> PIVX folder.
To start the wallet open PIVX -> bin -> pivx-qt.exe

**NOTE:** This will start the wallet on **mainnet**, but you want it in **testnet**! So, you should go to Local Disk (C:) -> Users ->  YOUR_USERNAME -> AppData -> Roaming -> PIVX -> edit pivx.conf and add in it exactly:
`testnet=1`. Save it, close it and you can start the wallet again. It will be running on testnet now.

--------------------------------------------
**Congratulations, you have successfully compiled and started PIVX Qt Core Wallet!**

**That's it, enjoy playing around with the latest version of PIVX Core Wallet directly compiled from master branch!**

--------------------------------------------
## COMPILING IF YOU ALREADY COMPILED WALLET THIS WAY EARLIER:

In Terminal type:

```
rm -rf PIVX

Also, from Local Disk (C:) -> workspace -> Delete entire "PIVX" folder
(it is there if you did "2) starting by opening pivx-qt.exe file" while you were compiling for the first time).


GO BACK TO STEP 4. AND FOLLOW THE STEPS FROM THERE.
```

--------------------------------------------
#### **Congratulations, you have successfully compiled and started FRESH PIVX Qt Core Wallet!**

This tutorial was made and summarized following the official PIVX build guide for Windows. If you are interested in details and broader understanding of what you're doing in each step, [visit the official PIVX full build guide for Windows by clicking HERE](https://github.com/PIVX-Project/PIVX/blob/master/doc/build-windows.md).
