
import subprocess, sys

commands = [
    ["git", "add", "."],
    ["git", "commit", "-m", sys.argv[1]],
    ["git", "push", "origin", "main"]
]

for c in commands:
    subprocess.run(c)