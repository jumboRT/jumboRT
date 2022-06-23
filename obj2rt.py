#!/usr/bin/env python3
import sys

lines = sys.stdin.readlines()
vertices = []
faces = []

for line in lines:
	line = line.split(" ")
	if line[0] == "v":
		x = float(line[1])
		z = float(line[2])
		y = float(line[3])
		vertices.append((x, y, z))
	if line[0] == "f":
		assert len(line) == 4
		a = int(line[1].split("/")[0]) - 1
		b = int(line[2].split("/")[0]) - 1
		c = int(line[3].split("/")[0]) - 1
		faces.append((a, b, c))

sys.stdout.write(f"C 0,0,0 1,0,0 90\n")
for vertex in vertices:
    sys.stdout.write(f"v {vertex[0]},{vertex[1]},{vertex[2]}\n")
for face in faces:
    sys.stdout.write(f"tr {face[0]} {face[1]} {face[2]}\n")
