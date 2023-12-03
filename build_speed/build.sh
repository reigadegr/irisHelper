#!/bin/sh
remove_file(){
    rm -rf $(pwd)/*.bak $(pwd)/include/*.bak $(pwd)/config/*.bak $(dirname "$0")/*.bak
}
FileName=irisHelper
format_code(){
    code_file="
        $(dirname "$0")/*.cpp
        $(dirname "$0")/include/*.h
    "

    for i in $code_file; do
        [  ! -f $i ] && continue
        echo "开始格式化$(basename $i)"
        /data/data/com.termux/files/usr/bin/clang-format -i $i
    done
}
compile_start(){
    #
   	/data/data/com.termux/files/usr/bin/aarch64-linux-android-clang++ \
    -flto=fill -flto-jobs=8 \
    -Wall -fomit-frame-pointer -std=c++23 -stdlib=libc++ -Os -flto \
    -fno-rtti -fvisibility=hidden -static-libgcc -static-libstdc++ \
    -fshort-enums -fmerge-all-constants -fno-exceptions \
    -fuse-ld=lld -mtune=native -march=native -pthread \
    -Bsymbolic -fdata-sections -ffunction-sections -fno-stack-protector \
    -Wl,-O3,--lto-O3,--gc-sections,--as-needed,--icf=all,-z,norelro,--pack-dyn-relocs=android+relr,-x,-s,--strip-all \
    $(pwd)/*.cpp -o $(dirname "$0")/$FileName && echo "*编译完成*" || exit 1
    /data/data/com.termux/files/usr/bin/aarch64-linux-android-strip $(dirname "$0")/$FileName
    chmod +x $(dirname "$0")/$FileName
    echo "当前时间：$(date +%Y) 年 $(date +%m) 月 $(date +%d) 日 $(date +%H) 时 $(date +%M) 分 $(date +%S) 秒"
}

echo "当前时间：$(date +%Y) 年 $(date +%m) 月 $(date +%d) 日 $(date +%H) 时 $(date +%M) 分 $(date +%S) 秒"

format_code
compile_start
remove_file 2>/dev/null
echo "------------开始运行...-------------------"
#./$FileName
echo "------------结束运行...-------------------"
ldd /data/data/com.termux/files/home/t/$FileName
