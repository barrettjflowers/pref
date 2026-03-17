#!/bin/bash
set -e

cc -Wall -Wextra -std=c11 -O2 -o pref src/main.c src/defaults.c src/prefs.c

echo "Built pref binary"
