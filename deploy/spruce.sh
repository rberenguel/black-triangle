#!/bin/sh

# set stick to d-pad mode
killall -q -USR2 joystickinput

export LD_LIBRARY_PATH=/mnt/SDCARD/Roms/A30PORTS/blacktriangle:$LD_LIBRARY_PATH

cd /mnt/SDCARD/Roms/A30PORTS/blacktriangle
./black_triangle

# set stick to analog mode
killall -q -USR1 joystickinput
