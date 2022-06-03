#!/usr/bin/env python3
import sys

lines = sys.stdin.readlines()
vertices = []
faces = []

for line in lines:
        line = line.split(" ")
        if line[0] == "v":
                x = float(line[1])
                y = float(line[2])
                z = float(line[3])
                vertices.append((x, y, z))
        if line[0] == "f":
                assert len(line) == 4
                a = int(line[1].split("/")[0]) - 1
                b = int(line[2].split("/")[0]) - 1
                c = int(line[3].split("/")[0]) - 1
                faces.append((a, b, c))

sys.stdout.write(f"C 0,0,0 1,0,0 90\n")
sys.stdout.write(f"L -1,0,0 1.0 255,255,255\n")
for face in faces:
        a = ",".join(f"{v}" for v in vertices[face[0]])
        b = ",".join(f"{v}" for v in vertices[face[1]])
        c = ",".join(f"{v}" for v in vertices[face[2]])
        sys.stdout.write(f"tr {a} {b} {c} 255,255,255\n")
