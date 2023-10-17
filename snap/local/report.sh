#!/bin/sh

$SNAP/usr/bin/keycounts report -f $SNAP_COMMON/keycounts.dat | sort -h
