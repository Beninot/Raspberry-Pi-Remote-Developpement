# Remote_Developpement

## General Description
  This project is a simple comand line program that is meant to ease the use of the ssh protocol for remote developpment of linux embeded applications over network. I think it can be a really simple way for someone to get into remote developpement for things like embeded linux systems like the raspberry pi or the beaglebone black. It's best use case is to my eyes developping graphical user interfaces on the raspberry pi 7" lcd, this program allows you to edit the code on your big powerfull, elegant and reliable windows computer to then export those files to an embeded system using the pscp interface provided by putty on the windows client to the openssh linux server.

## System requirements
  Windows 7-10 based pc with Putty installed
  
  linux developpement platform running a ssh server, preferably loaded with make and gcc
  
## Usage

Place the .exe file in a folder, then run it in the command line with the argument -S to enter the Setup

After folowing the setup steps, you'll have to setup the ssh server authorized key to allow the program to connect, to do so, you should use PUTTYgen and place the result key in the same folder as the .exe file under the name key.ppk

Once this is done, you can now build the provided hello world program, to do so run the program with the argument -b to build it according to it's makefile, you'll have to run it twice with that argument as he will have to create the folder the first time, build result will be stored in the ./Results folder.

With the program build, you just have to run the -r argument to run the program

To retrieve the program output, just run the -s argument to sync, this will retrieve the output files as the default run command redirects the stdio and stderr to files in the ./results folder

You can kill the process using the -k argument

## Arguments
### -S to setup
enters the ip/user/path configuration, be carefull, this will overwrite any change to the Command Files
### -s to sync
synchronises the Results folder
### -b to build
transfers the build_environment folder to the server, runs make and retrieves the make result   
**note that it has to be ran twice the first time**
### -r to run
runs the process
### -k to kill
kills the process


## Contribution
**You are more than welcome to share your toughts / contribute to this project, please do so**
