
import pathlib, shutil

build = pathlib.Path("build")
objs = pathlib.Path("obj")

# Clean build/ but keep .gitignore
for f in build.iterdir():
    if f.name != ".gitignore":
        f.unlink()

# Clean obj/ but keep .gitignore
for f in objs.iterdir():
    if f.is_file() and f.name != ".gitignore":
        f.unlink()
    elif not f.is_file():
        shutil.rmtree(f.absolute())
