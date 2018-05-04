#!/usr/bin/env bash
clang-tidy -analyze-temporary-dtors -checks=-*,cppcoreguidelines-*,clang-analyzer-*,-clang-analyzer-osx*,-clang-analyzer-optin.osx*,bugprone-*,modernize-*,performance-*,portability-*,readability-braces-around-statements -header-filter=$(./gen_header_filter.py) -p ./build "$@"
