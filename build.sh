#!/bin/sh

set -xe
cc -Wall -Wextra -ggdb -o twoplayer twoplayer.c -lm
# cc -Wall -Wextra -ggdb -o ai ai.c -lm
