import subprocess
import sys
import re

output = subprocess.check_output(["clinfo", "-l", "--raw"]).decode()
result = []

for line in output.split("\n"):
    parts = line.split(": ")
    if len(parts) < 2:
        continue
    cl_id = parts[0]
    parts = re.split(r':|\.', cl_id)
    if len(parts) < 2:
        continue
    result.extend(["-d", parts[0], parts[1]])

subprocess.run(["./miniRT"] + sys.argv[1:] + result)
