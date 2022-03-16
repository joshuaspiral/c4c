#!/bin/sh

set -xe
cc -Wall -Wextra -ggdb -O3 -o twoplayer twoplayer.c -lm
cc -Wall -Wextra -ggdb -O3 -o ai ai.c -lm
cc -Wall -Wextra -ggdb -O3 -o ai_vs_ai ai_vs_ai.c -lm
