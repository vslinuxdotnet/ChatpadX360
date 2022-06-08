# ChatpadX360
OpenSource Keyboard for Xbox 360 Chatpad<br>
<img src="https://raw.githubusercontent.com/vslinuxdotnet/ChatpadX360/main/img/chatpadkeyboard.jpg">
<br><br>
This code convert the matrix pressed keys in pure serial uart codes that you can use with microcontrollers.
- Support Blacklight with auto fade OFF/ON.
- Support most common baud rates: 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200

Function keys:<br>
  - Chatkey turn blacklight ON/OFF ON=fade out in 2 seconds<br>
  - Shift, Orange an Green alt keys works with all chars.<br>
  - The Orange Z and X keys (æ & œ) was replaced by ESC and TAB keys.<br>
  <br>
Keyboard power ON options:<br>
 - key 1 change baud rate.<br>
 - key 0 Reset to defaults, baud rate=4800, blacklight=off.<br>
 - key 2 print about text with version.<br> 
<br>
This source code support the original: PIC16F883 and a alternative compatible pinout: PIC18F26k20.<br>
If you have the microsoft chip, you can replace it with one of this PIC's.<br>
To flash use the pickit3 programmer, see the pinout in img folder.<br>

The chatpad keyboard use 3.3v, and have a boost converter on the right side, to go at 3.8V.<br>
If you remove it, you can do a bypass on boost, and power it at 5V.<br>
To compile use Mikroc in https://www.mikroe.com/mikroc-pic or get the hex files in the hex folder.
<br><br>
To do in the future:
- Add support to PS2 in C7 and A7 pins (the only avaliable) Needed more flash space... move to PIC18F26k20.
- Add sleep mode option (100 nA).
- Add more key options
<br>
Please <a href="https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=ebay@vslinux.net&lc=PT&no_note=0&item_name=Open+Source+Software&cn=&curency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted">donate</a> to support this and other projects.
<br>
Thank you!
<br><br>
Happy hacking!!!
