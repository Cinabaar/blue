#!/bin/bash
if [[ $# > 0 ]]; then
    cd $1/bin
    optirun ./blue
fi
