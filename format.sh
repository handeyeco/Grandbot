#!/bin/bash

find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=chromium
node toc.js guide.md