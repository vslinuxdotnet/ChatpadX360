# ChatpadX360
OpenSource Keyboard for Xbox 360 Chatpad

This code convert the matrix pressed keys in pure serial uart codes.
- Support Blacklight with auto fade OFF/ON.
- Support most common baut rates

Funcion keys:<br>
  - Chatkey turn blacklight ON/OFF ON=fade out in 2 seconds<br>
  - Shift, Orange an Green keys works with all chars.<br>
  - The Orange Z and X keys was replaced by ESC and TAB.<br>
  <br>
Keyboard power ON options:<br>
 - key 1 change baut rate.<br>
 - key 0 Reset to defaults, baut rate=4800, blacklight=off.<br>
 - key 9 print about text.<br> 
<br>
This source code support the original: PIC16F883 and a alternative compatible pinout: PIC18F26k20.
<br>
To compile use Mikroc in https://www.mikroe.com/mikroc-pic or get the hex files in the hex folder.
<br>
Happy hacking!!!
