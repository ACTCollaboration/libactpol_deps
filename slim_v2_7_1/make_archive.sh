#!/bin/bash

# This is to keep Mac OS X from bundling ._* accessory files into the tar file.
export COPYFILE_DISABLE=1
VERSION=v2_7_1
SLIMDIR=slim_${VERSION}

# Temporarily make a softlink so ../slim_v2_7_1 points to ../slim
# Build the archive from .., and then remove the versioned link.
cd ..
ln -s slim ${SLIMDIR}
tar -cvz -f slim_${VERSION}.tgz -X slim/archive_exclude.txt  ${SLIMDIR}/*
zip slim_${VERSION}.zip ${SLIMDIR} -x slim/archive_exclude.txt 
rm ${SLIMDIR}
