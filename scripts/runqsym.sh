#!/bin/bash

DIR="/qsym/output"
PROG="$DIR/build/djpeg-labyrinth"
PROG_NORMAL="$DIR/jpeg-9d/djpeg-normal"
SEED="$DIR/in"
OUTPUT="$DIR/out"
CMD="@@"
CFG_FILE="$DIR/jpeg-9d/new_cfg.dot"

if [ $1 == "M" ]; then
	/afl/afl-fuzz -t 1000+ -m none -M afl-master -c 5h -i $SEED -o $OUTPUT -- $PROG $CMD &
fi

if [ $1 == "S" ]; then
	/afl/afl-fuzz -t 1000+ -m none -S afl-slave -c 5h -i $SEED -o $OUTPUT -- $PROG $CMD &
fi

if [ $1 == "Q" ]; then
	/qsym/bin/run_qsym_afl.py -a laby-slave -o $OUTPUT -n qsym -g $CFG_FILE -- $PROG_NORMAL $CMD &
fi