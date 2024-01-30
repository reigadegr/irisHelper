#!/bin/bash
#
# Copyright (C) 2021-2022 Matt Yang
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# stop on error
set -e

BASEDIR="$(dirname $(readlink -f "$0"))"
BUILD_DIR="$BASEDIR/build"
BUILD_TYPE="Release"
BUILD_TASKS="$1"
ARM64_PREFIX=aarch64-linux-android
C_COMPILER="/data/data/com.termux/files/usr/bin/aarch64-linux-android-clang"
CXX_COMPILER="/data/data/com.termux/files/usr/bin/aarch64-linux-android-clang++"

remove_file() {
	rm -rf $(find $BASEDIR/source/src -name "*.bak")
}

format_code() {
	echo "当前时间：$(date +%Y) 年 $(date +%m) 月 $(date +%d) 日 $(date +%H) 时 $(date +%M) 分 $(date +%S) 秒"
	nohup /data/data/com.termux/files/usr/bin/clang-format -i $BASEDIR/source/src/*.cpp >/dev/null 2>&1 &
    nohup /data/data/com.termux/files/usr/bin/clang-format -i $BASEDIR/source/src/include/*.h >/dev/null 2>&1 &
    echo "格式化代码完毕"
}

# $1:prefix $2:targets
build_targets() {
    mkdir -p $BUILD_DIR/$1
    cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DCMAKE_C_COMPILER=$C_COMPILER \
        -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -H$BASEDIR \
        -B$BUILD_DIR/$1 \
        -G "Unix Makefiles"
    cmake --build $BUILD_DIR/$1 --config $BUILD_TYPE --target $2 -j8
}

make_irisHelper() {
    echo ">>> Making irisHelper binaries"
    format_code
    build_targets $ARM64_PREFIX "irisHelper"
    remove_file
}

# $1:task
do_task() {
    case $1 in
    make)
        make_irisHelper
        ;;
    *)
        echo " ! Unknown task name $1"
        exit 1
        ;;
    esac
}

#start to working
do_task $BUILD_TASKS

ldd $(pwd)/build/aarch64-linux-android*/runnable/irisHelper
strip_tool="/data/data/com.termux/files/usr/bin/aarch64-linux-android-strip"
$strip_tool $(pwd)/build/aarch64-linux-android*/runnable/irisHelper
