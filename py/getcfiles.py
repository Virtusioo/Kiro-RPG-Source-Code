
import pathlib
import sys

src = pathlib.Path(sys.argv[1])

for f in src.rglob("*.c"):
    print(f.as_posix())
