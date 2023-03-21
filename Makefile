upload:
	sudo arduino-cli compile -u -b arduino:avr:uno -p /dev/ttyACM0 ./

compile:
	sudo arduino-cli compile -b arduino:avr:uno -p /dev/ttyACM0 ./

list:
	sudo arduino-cli board list

debug:
	alacritty -e sudo screen /dev/ttyACM0 9600
