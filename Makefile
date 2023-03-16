upload:
	sudo arduino --board arduino:avr:uno --port /dev/ttyACM0 --upload ./code.ino

find:
	ls -lashF /dev | grep "ACM\|USB"
