#!/usr/bin/env python3

import os

EXCLUDE_DIRS = [".git", "build", "gsl", ".vscode"]

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

        fullname = os.path.join(dirpath, filename)
        os.system(f"clang-format -verbose -i {fullname}")
