#!/usr/bin/env bash

BIN_DIR=bazel-bin
OUT_DIR=out
options=( "$BIN_DIR"/* )
select option in "${options[@]}" "cancel"; do
	case $option in
		"cancel")
			break
			echo "Cancelled."
			;;
		*)
			echo "Generating from $option"
			./"$option" > "$OUT_DIR"/result.dat
			echo "Done."
			break
			;;
	esac
done

