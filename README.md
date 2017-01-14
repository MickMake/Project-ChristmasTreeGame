################################################################################
Mick's Christmas Tree Game.

Check out the video on my YouTube channel:
 MickMake YouTube channel:    https://www.youtube.com/c/MickMakes
 Video:                       https://www.youtube.com/watch?v=tHWXnCrZYtg
 Website article:             https://mickmake.com/archives/1550
 Game interface:              http://santa.mickmake.com/

This is a good example of how to create an internet accessible application that can potentially handle a huge load. The Apache web server will die long before the Teensy will.

+------------+            +-------------+                    +--------+
| Web server | >> 8080 >> | Linux socat | >> /dev/ttyACM0 >> | Teensy |
+------------+            +-------------+                    +--------+

index.php will use javascript to call "throw.php?elf=X", where X is the button number that someone has pressed.

throw.php - will initiate a socket connection to the Linux box.

socat - Make sure you have redirected inbound TCP:8080 to /dev/ttyACM0 using something like this:
nohup socat -gx TCP-LISTEN:1234,fork,max-children=10,reuseaddr OPEN:/dev/ttyACM0,b115200,flusho=1,raw,readbytes=1 </dev/null &
	fork - will fork sub-processes for inbound connections.
	max-children - we only have 10 elves!
	flushno=1 - flush all input.
	raw - we're just interested in raw data, no telnet handshakes.
	readbytes=1 - We should only ever see one byte, terminate once we get it.

Teensy - The Teensy will then respond to the single character data coming in on the seriaal port.


"Santa is running wild again, and it's the job of the Blue Elves to knock him off his sleigh.
See if you can get an Elf to hit Santa with a snowball.
Just click on any button to throw one.
When you hit Santa it'll make a mark.

Since Elf telemetry is so bad, there's a good 10 second delay between telling an Elf to throw a snowball and them actually doing it! (You can blame YouTube for that.) See if you can judge the delay.
"


################################################################################
# ChristmasTreeGame/
Source code running on the Teensy.


################################################################################
# html/
PHP/HTML files running on the internet facing server.
Make sure you change the TARGET_SERVER text to the Linux box connect to the Teensy.


################################################################################
# Schematics/
PDF and KiCad schematics. Pretty basic stuff, though.


