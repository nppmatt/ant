#!/usr/bin/env bash

echo "$(date) Job Selection" >> joblog
./gen-data.sh

if [ $? -eq 1 ]; then
	echo "Job selection cancellation received, stopping data processing."
	exit 0;
fi

echo "$(date) Generating data plots." >> joblog
rm plot-export/*
./gen-plot.py
echo "$(date) Finished plotting." >> joblog

echo "$(date) Generating videos." >> joblog
rm video-export/*
./gen-video.sh
echo "$(date) Finished videos." >> joblog

