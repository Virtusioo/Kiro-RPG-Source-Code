
import sys, pathlib

directory = pathlib.Path(sys.argv[1]) 
c_files = list(directory.rglob("*.c"))

for file in c_files:
    print(file)
