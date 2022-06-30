#!/usr/bin/env python3
import sys

lines = sys.stdin.readlines()
vertices = []
faces = []
camera = ((0, 0, 0), (1, 0, 0), 90)

def parse_vertex(line, swap_yz=False):
    x = float(line[0])
    y = float(line[1])
    z = float(line[2])
    if swap_yz:
        return (x, z, y)
    else:
        return (x, y, z)

def str_vertex(vertex):
    return f"{vertex[0]:f},{vertex[1]:f},{vertex[2]:f}"

def add_vertex(vertex):
    try:
        return vertices.index(vertex)
    except ValueError:
        vertices.append(vertex)
        return len(vertices) - 1

if sys.argv[1] == "jkoers":
    for line in lines:
        line = line.split()
        if len(line) == 0:
            continue
        if line[0] == "tr":
            a = add_vertex(parse_vertex(line[1].split(","), True))
            b = add_vertex(parse_vertex(line[2].split(","), True))
            c = add_vertex(parse_vertex(line[3].split(","), True))
            faces.append((a, b, c))
        if line[0] == "c":
            position = parse_vertex(line[1].split(","), True)
            rotation = parse_vertex(line[2].split(","), True)
            fov = float(line[3])
            camera = (position, rotation, fov)
elif sys.argv[1] == "obj":    
    for line in lines:
        line = line.split()
        if line[0] == "v":
            vertex = parse_vertex(line[1:])
            vertices.append((x, y, z))
        if line[0] == "f":
            assert len(line) == 4
            a = int(line[1].split("/")[0]) - 1
            b = int(line[2].split("/")[0]) - 1
            c = int(line[3].split("/")[0]) - 1
            faces.append((a, b, c))
else:
    sys.stderr.write(f"invalid format {sys.argv[1]}\n")
    sys.exit(1)

sys.stderr.write(f"{len(vertices)} vertices\n")
sys.stderr.write(f"{len(faces)} triangles\n")
sys.stdout.write(f"C {str_vertex(camera[0])} {str_vertex(camera[1])} {camera[2]:f}\n")
for vertex in vertices:
    sys.stdout.write(f"v {str_vertex(vertex)}\n")
for face in faces:
    sys.stdout.write(f"tr {face[0]} {face[1]} {face[2]}\n")
