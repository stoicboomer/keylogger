# keylogger

My first exprience in the malware development, for sure a fun journey :D

## Contents

A simple local keylogget that will save every keystroke in a given path and a chad remote
keylogger with a backup function that will log in a file the keystrokes not sent while 
waiting for a valid connection to send all.

- lib/client.h
	contains different functions for handling the client connection.	

- lib/vkConverts.h
	Virtual key convert functions, customizable on the switch.

- lib/keySavers.h
	Saving methods for the keystrokes (for now just the offline func and remote sender thread)	

## TODO

- Install the keylogger as a system service
- Encryption and ADS (alternate data stream)
- Git gud
- Make file

### Contact me

- Discord Askesis@0327
