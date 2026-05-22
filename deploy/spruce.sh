#!/bin/sh

# set stick to d-pad mode
killall -q -USR2 joystickinput

cd /mnt/SDCARD/Roms/PORTS/blacktriangle
./black_triangle

# set stick to analog mode
killall -q -USR1 joystickinput
