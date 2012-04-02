1. Requirements.
2. Compilation and Execution.

1. Requirements:
- Linux:
  Requires libpcap development libraries.
  - To install on Ubuntu type on terminal: sudo apt-get install libpcap-devel
  - For Fedora, type as su: yum install libpcap-devel
  - To compile from source:
    1. Download libpcap (http://www.tcpdump.org/release/libpcap-1.2.1.tar.gz) from http://www.tcpdump.org/
    2. Extract the tar file using 'tar -xvf libpcap-1.2.1.tar.gz' or use GUI based extractor.
    3. 'cd libpcap-1.2.1'
    4. './configure'
    5. 'make'
    6. 'sudo make install' or as su 'make install'
- Windows:
  - Download and install winpcap from http://www.winpcap.org/install/default.htm
  - Requires Visual Studio 2010. Express version is recommended.

2. Compilation:
- Linux:
  - Type 'make' in the root (current) directory to compile.
    - Default is 32 bit optimized executible.
    - To compile in 64bit mode add 'm64bit=1' to make command, e.g. 'make m64bit=1'
    - To compile in debug mode add 'dbg=1' to make command, e.g. 'make dbg=1'
  - 'make run' will run the default executible. Adding 'make run dbg=1' will run the debug exe.
  - To pass arguments to executible use 'make run ARG="arg1 argument2 arg3"' etc.
- Windows:
  Open the VS2010 solution file provided.