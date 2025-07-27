
import sys, shutil
from pathlib import Path

src_dir = Path("dll") / sys.argv[1]
dst_dir = Path("build")

for file in src_dir.iterdir():
    if file.is_file():
        shutil.copy2(file, dst_dir)