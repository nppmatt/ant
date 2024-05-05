#!/usr/bin/env bash

bazel build //:test --config=clang_conf
bazel build //:ant_1d --config=clang_conf
bazel build //:ant_coarse --config=clang_conf
bazel build //:ant_bc --config=clang_conf
bazel build //:ant_directional --config=clang_conf
bazel build //:ant_mathc_cos --config=clang_conf

