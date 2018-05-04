#!/usr/bin/env python3

import os

EXCLUDE_DIRS = [".git", "build", "gsl", ".vscode"]


print(".*(", end="")

for dirpath, _, filenames in os.walk("."):
    excluded = False
    for exl_dir in EXCLUDE_DIRS:
        if exl_dir in dirpath:
            excluded = True
            break

    if excluded:
        continue

    for filename in filenames:
        if not filename.endswith(".h") and not filename.endswith(".cpp"):
            continue

        print(filename.replace(".", r"\.") + "|", end="")

print(r"Journey\.cpp)", end="")
