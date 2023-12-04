#!/bin/sh
cd $(dirname "$0")/../Module
version=$(grep "version=" $(pwd)/module.prop | cut -d "=" -f2)
commit=$(grep "main" $(dirname "$0")/../.git/packed-refs | awk 'NR==1{print $1}' | cut -c 1-5)
rm $(pwd)/../output/irisHelper*.zip $(pwd)/*.bak $(pwd)/*/*.bak $(pwd)/*.log 2>/dev/null
/data/data/com.termux/files/usr/bin/zip -9 -rq "$(pwd)/../output/irisHelperV$version $commit.zip" .
echo "当前时间：$(date +%Y) 年 $(date +%m) 月 $(date +%d) 日 $(date +%H) 时 $(date +%M) 分 $(date +%S) 秒"
