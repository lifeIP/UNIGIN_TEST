#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $2"
        echo "********************************************************************************"
        echo ""
        exit $1
    else
        echo ""
        echo "*** SUCCESS: $2"
        echo "********************************************************************************"
        echo ""
    fi
} 

cmake -H. -B junk/unigine_task_release_linux -DCMAKE_BUILD_TYPE=Release
check_ret $? "cmake configure"

cmake --build junk/unigine_task_release_linux --config Release
check_ret $? "cmake build"