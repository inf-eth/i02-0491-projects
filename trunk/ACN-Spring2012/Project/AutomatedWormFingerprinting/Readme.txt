# Readme.txt
#
# Copyright (c) 2012 Attique Dawood, Awais Aslam
#
# This file is part of Automated Worm Fingerprinting.
#
# Automated Worm Fingerprinting is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Automated Worm Fingerprinting is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################

1. Requirements.
2. Compilation and Execution.

1. Requirements:
- Linux:
  Requires libpcap and openssl development libraries.
  - To install on Ubuntu type on terminal: sudo apt-get install libpcap-devel openssl-devel
  - For Fedora, type as su: yum install libpcap-devel openssl-devel
  - To compile libpcap from source:
    1. Download libpcap (http://www.tcpdump.org/release/libpcap-1.2.1.tar.gz) from http://www.tcpdump.org/
    2. Extract the tar file using 'tar -xvf libpcap-1.2.1.tar.gz' or use GUI based extractor.
    3. 'cd libpcap-1.2.1'
    4. './configure'
    5. 'make'
    6. 'sudo make install' or as su 'make install'
- Windows:
  OpenSSL Development files:
  - Download openssl for windows from http://code.google.com/p/openssl-for-windows/downloads/detail?name=openssl-0.9.8k_WIN32.zip
  - Copy 'include' folder over 'C:\Program Files\Visual Studio 10.0\VC\include'
  - Copy 'lib' folder over 'C:\Program Files\Visual Studio 10.0\VC\lib'
  Winpcap development files:
  - Download and install winpcap dll (http://www.winpcap.org/install/bin/WinPcap_4_1_2.exe)
  - Download and install winpcap (http://www.winpcap.org/install/bin/WpdPack_4_1_2.zip)
  - Extract the archive. 
  - Copy all the files in 'include' folder to 'C:\Program Files\Visual Studio 10.0\VC\include'
  - Copy all the files in 'lib' folder to 'C:\Program Files\Visual Studio 10.0\VC\lib'
  - Requires Visual Studio 2010. Express edition is recommended.

2. Compilation:
- Linux:
  - Type 'make' in the root (current) directory to compile.
    - Default is 32 bit optimized executible.
    - To compile in 64bit mode add 'm64bit=1' to make command, e.g. 'make m64bit=1'
    - To compile in debug mode add 'dbg=1' to make command, e.g. 'make dbg=1'
  - 'make run' will run the default executible. Adding 'make run dbg=1' will run the debug exe.
  - To pass arguments to executible use 'make run ARG="arg1 argument2 arg3"' etc.
    For Example, 'make run ARG="wlan0 tcp"' will capture only tcp packets on wifi.
- Windows:
  Open the VS2010 solution file provided.
