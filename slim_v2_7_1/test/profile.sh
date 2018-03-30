#!/bin/bash

expansion_profile ()
{
# Compress something
    ./slim -p -c1    -i -r16384 -dm2 /tmp/data.bin
    ./slim -p -c1100 -i -r1     -dm2 /tmp/mce.bin

# Profile expansion
    valgrind --tool=callgrind ./slim -px /tmp/data.bin.slm
    valgrind --tool=callgrind ./slim -px /tmp/mce.bin.slm

# Verify that it was done correctly
    echo
    echo 'Compare the original and compressed/expanded md5sums:'
    md5sum /tmp/data.bin /tmp/data.bin.raw
    md5sum /tmp/mce.bin /tmp/mce.bin.raw
}



compression_profile ()
{
# Profile compression
    valgrind --tool=callgrind ./slim -p -c1    -i -r16384 -dm2 /tmp/data.bin
    valgrind --tool=callgrind ./slim -p -c1100 -i -r1     -dm2 /tmp/mce.bin

# Verify that it was done correctly
    echo
    echo 'Compare the original and compressed/expanded md5sums:'
    ./slim -px /tmp/data.bin.slm
    md5sum /tmp/data.bin /tmp/data.bin.raw
    ./slim -px /tmp/mce.bin.slm
    md5sum /tmp/mce.bin /tmp/mce.bin.raw
}



make testdata
make all

case $1 in
    comp* )
        echo 'Compression profile'
        compression_profile ;;
    
    exp* )
        echo 'Expansion profile'  
        expansion_profile ;;

    * ) echo 'usage: profile.sh [expansion | compression]'
        exit
esac


