# Socket_Python_C++_Demo

>A simple demo for socket communication between c++ and python ,tested with Ubuntu 20.04.

## Usage

1. `chmod +x make.sh` and `sh make.sh`.
2. `python python_server.py` to run the python file.
3. `cd build` and `./tcp_client`.

## Description

I customized many conditions to make it easy to modify the program.

By default, the project executes a certain number of image numbers that need to be processed from the C++ project to the Python project, and then reversely transmits them back after completion.

The program uses two ports for one-way information transmission, so that it can run stably and easily.

If you have any questions or suggestions, please feel free to ask me.