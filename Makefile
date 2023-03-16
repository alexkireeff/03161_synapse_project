upload:
	sudo arduino-cli compile -u -b arduino:avr:uno -p /dev/ttyACM0 ./

list:
	sudo arduino-cli board list
