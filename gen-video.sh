#!/usr/bin/env bash

PLOT_DIR=plot-export
VIDEO_DIR=video-export

echo "Generating 30fps video."
ffmpeg -hide_banner -loglevel error -r 30 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result30.mp4

echo "Generating 60fps video."
ffmpeg -hide_banner -loglevel error -r 60 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result60.mp4

echo "Generating 120fps video."
ffmpeg -hide_banner -loglevel error -r 120 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result120.mp4

echo "Generating 240fps video."
ffmpeg -hide_banner -loglevel error -r 240 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result240.mp4

echo "Generating 480fps video."
ffmpeg -hide_banner -loglevel error -r 480 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result480.mp4

echo "Generating 960fps video."
ffmpeg -hide_banner -loglevel error -r 960 -i $PLOT_DIR/plot-%d.png -c:v libx264 -pix_fmt yuv420p -y $VIDEO_DIR/result960.mp4

