This is the Linux release V3.4.0, rev. 1, 64bit (Ubuntu 14.04.1)

****************************
****************************
FROM THE COMMAND LINE, run

$ ./vrep.sh 

to launch V-REP
****************************
****************************



**********************************
Various issues you might run into:
**********************************

1. When trying to start V-REP, following message displays: "Error: could not find or correctly load the V-REP library"
	a) Make sure you started V-REP with "./vrep.sh" FROM THE COMMAND LINE
	b) check what dependency is missing by using the file "libLoadErrorCheck.sh"

2. You are using a dongle license key, but V-REP displays 'No dongle was found' at launch time.
	a) See below



***************
Using a dongle:
***************

There are 2 methods to have the dongle recognized by V-REP:

1. Without super-user rights:
	a) $ lsusb
	b) Make sure that the dongle is correctly plugged and recognized (VID:1bc0, PID:8100)
	c) $ sudo cp 92-SLKey-HID.rules /etc/udev/rules.d/
	d) Restart the computer
	e) $ ./vrep.sh

2. With super-user rights (not recommended: V-REP's user interfaces will slightly look different and there might be load problems with specific plugins):
	a) $ sudo -E ./vrep.sh

