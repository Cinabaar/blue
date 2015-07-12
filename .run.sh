#!/bin/bash
if [[ $# > 0 ]]; then
    cd $1/build/bin
    optirun ./blue
fi
