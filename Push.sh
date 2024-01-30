#!/bin/sh

rm -rf $(find $(pwd) -name "*.bak")

#通用推送模板
git gc --prune=all
git add .
git commit -m "$1"
git push -f origin cmake
