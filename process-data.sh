#!/usr/bin/env bash

echo "$(date) Job Selection" >> joblog
./gen-data.sh

echo "$(date) Generating data plots." >> joblog
./gen-plot.py
echo "$(date) Finished plotting." >> joblog

echo "$(date) Generating videos." >> joblog
./gen-video.sh
echo "$(date) Finished videos." >> joblog

