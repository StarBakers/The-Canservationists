# The Canservationists - CanSat NL 2021 Competition
The Arduino code that we used for our participation to ESERO NL CanSat competition 2021. 

- cansat_code.ino is the code we uploadded to CanSat itself to fulfil its mission. 
- radio_receiver.ino is the code run by the radio receiver of the ground station. 
- radio_configuration.ino is only used for the initial configuration of the two radios (at Canat itself an at teh ground station). 

Please pay attention to the libraries needed for each program (included at the top od the code). 

The configuration of CanSat an of the radio receiver, as well as of the radios for their initial setup can be found at the pdf file Cansat Configuration - Canservationists.pdf. 

NOTE: Radio configuration needs to take place before using the devices.
Configuration takes place using the following syntax (without the dashes):
WR - Frequency -  RF Data Rate -  Output Power -  UART-Rate -  Series check, 
e.g. "WR 433900 1 9 3 0"
check the corresponding radio configuration code. 
Possible values for all these settings:
- Frequency: Unit is KHz,for example 434MHz is 434000
- RF Data Rate: 1,2,3 and 4 refer to 2400,4800,9600,19200bps
- Output Power: 0 to 9; 9 means 13dBm (20mW)
- UART Rate: 0,1,2,3,4,5 and 6 refers to 1200,2400,4800,9600, 19200,38400,57600bps
- Series Check: Series checkout:0 means no check,1 means even parity,2 means odd parity.

Using the above example the radios are onfigured at: 433.9MHz, 2400bps, 13dBm, 9600 bps, no check.

For questions: filvag04 AT gmail DOT com
Blogpost: https://atlasbros.blogspot.com/2021/06/using-cansat-to-monitor-local-gas.html (where you can also find the whole report)

Enjoy :-) 

The Canservationists
