#!/usr/bin/env bash

shopt -s nullglob extglob

BIN_DIR=bazel-bin
OUT_DIR=out

# Collect files without an extension
options=()
for file in "$BIN_DIR"/*; do
	if [[ -f "$file" && ! "$file" == *.* && ! "$file" == "$BIN_DIR"/.* ]]; then
		options+=("$file")
	fi
done

select option in "${options[@]}" "cancel"; do
	case $option in
		"cancel")
			echo "Cancelled."
			exit 1
			break
			;;
		*)
			echo "Generating from $option"
			./"$option" > "$OUT_DIR"/result.dat
			echo "Done."
			break
			;;
	esac
done
exit 0

