#!/bin/bash

shopt -s globstar; clang-format --verbose -i ./Soup/Src/**/**.{h,cpp}
