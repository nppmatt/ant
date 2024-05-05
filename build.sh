#!/usr/bin/env bash

bazel build //:test --config=clang_conf
bazel build //:ant_1d --config=clang_conf
bazel build //:ant_coarse --config=clang_conf

