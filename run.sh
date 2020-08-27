gcc -Wall 2Dconv.c -o 2dconv
gcc -Wall sepfilters.c -o sepfilters
gcc -Wall slidewindow.c -o slidewindow
./2dconv.exe bridge.ppm
./sepfilters.exe bridge.ppm
./slidewindow.exe bridge.ppm
