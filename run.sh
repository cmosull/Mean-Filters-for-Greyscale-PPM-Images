gcc -Wall 2Dconv.c -o 2dconv
gcc -Wall separable_filters.c -o sepfilters
./2dconv.exe bridge.ppm
./sepfilters.exe bridge.ppm