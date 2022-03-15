#!/bin/sh

set -xe
cc -Wall -Wextra -ggdb -o twoplayer twoplayer.c -lm
cc -Wall -Wextra -ggdb -o ai ai.c -lm
cc -Wall -Wextra -ggdb -o ai_vs_ai ai_vs_ai.c -lm
