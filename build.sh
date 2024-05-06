#!/usr/bin/env bash

bazel build //:test --config=clang_conf
bazel build //:ant_1d --config=clang_conf
bazel build //:ant_coarse --config=clang_conf
bazel build //:ant_bc --config=clang_conf
bazel build //:ant_directional --config=clang_conf
bazel build //:ant_mathc_cos --config=clang_conf
bazel build //:ant_infra_test --config=clang_conf
bazel build //:ant_mathc_sin --config=clang_conf
bazel build //:ant_fix_dir --config=clang_conf
bazel build //:ant_lerp3 --config=clang_conf
bazel build //:ant_lerp4 --config=clang_conf
bazel build //:ant_2d --config=clang_conf
bazel build //:ant_2d_bc --config=clang_conf
bazel build //:ant_array --config=clang_conf
bazel build //:ant_array4 --config=clang_conf

