#!/usr/bin/env python3

import os

for dirpath, _, filenames in os.walk("."):
    for filename in filenames:
        if not filename.endswith(".h") and not filename.endswith(".cpp"):
            continue

        fullname = os.path.join(dirpath, filename)
        os.system(f"clang-format -verbose -i {fullname}")
