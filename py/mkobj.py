
import os
import sys

SRC_DIR = sys.argv[1]
OBJ_DIR = 'obj'

for root, dirs, _ in os.walk(SRC_DIR):
    for d in dirs:
        src_path = os.path.join(root, d)
        rel_path = os.path.relpath(src_path, SRC_DIR)
        obj_path = os.path.join(OBJ_DIR, rel_path)
        os.makedirs(obj_path, exist_ok=True)