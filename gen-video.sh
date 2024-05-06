#!/usr/bin/env bash

PLOT_DIR=plot-export
VIDEO_DIR=video-export

echo "Generating 30fps video."
ffmpeg -hide_banner -loglevel error -i $PLOT_DIR/result_%5d.png -c:v libx264 -pix_fmt yuv420p -r 30 -y $VIDEO_DIR/result30.mp4

echo "Generating 60fps video."
ffmpeg -hide_banner -loglevel error -i $PLOT_DIR/result_%5d.png -c:v libx264 -pix_fmt yuv420p -r 60 -y $VIDEO_DIR/result60.mp4

echo "Generating 120fps video."
ffmpeg -hide_banner -loglevel error -i $PLOT_DIR/result_%5d.png -c:v libx264 -pix_fmt yuv420p -r 120 -y $VIDEO_DIR/result120.mp4

echo "Generating 240fps video."
ffmpeg -hide_banner -loglevel error -i $PLOT_DIR/result_%5d.png -c:v libx264 -pix_fmt yuv420p -r 240 -y $VIDEO_DIR/result240.mp4

echo "Generating 480fps video."
ffmpeg -hide_banner -loglevel error -i $PLOT_DIR/result_%5d.png -c:v libx264 -pix_fmt yuv420p -r 480 -y $VIDEO_DIR/result480.mp4

