#!/usr/bin/env python3
import sys
import math

lines = sys.stdin.readlines()
vertices = []
faces = []
camera = ((0, 0, 0), (1, 0, 0), 90)
# write_bb = ((0.785739, -0.908510, 0.038250), (1.072875, -0.509230, 0.311250))
# write_bb = ((0.883179, -0.673730, 0.038250), (1.072875, -0.509230, 0.311250))
# write_bb = ((0.883179, -0.673730, 0.192250), (1.072875, -0.509230, 0.311250))
# write_bb = ((0.935566, -0.631730, 0.213250), (0.967875, -0.600230, 0.234250))
# write_bb = ((0.953736, -0.631730, 0.213250), (0.967875, -0.617730, 0.234250))

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
        if len(line) == 0:
            continue
        if line[0] == "v":
            vertex = parse_vertex(line[1:], True)
            vertices.append(vertex)
        if line[0] == "f":
            assert len(line) == 4
            a = int(line[1].split("/")[0]) - 1
            b = int(line[2].split("/")[0]) - 1
            c = int(line[3].split("/")[0]) - 1
            faces.append((a, b, c))
else:
    sys.stderr.write(f"invalid format {sys.argv[1]}\n")
    sys.exit(1)

def make_bb(vertices):
    bb_min = (math.inf, math.inf, math.inf)
    bb_max = (-math.inf, -math.inf, -math.inf)
    for vertex in vertices:
        bb_min = (min(bb_min[0], vertex[0]), min(bb_min[1], vertex[1]), min(bb_min[2], vertex[2]))
        bb_max = (max(bb_max[0], vertex[0]), max(bb_max[1], vertex[1]), max(bb_max[2], vertex[2]))
    return (bb_min, bb_max)

def intersect_bb(bb1, bb2):
    if bb1[0][0] < bb2[1][0] and bb2[0][0] < bb1[1][0]:
        if bb1[0][1] < bb2[1][1] and bb2[0][1] < bb1[1][1]:
            if bb1[0][2] < bb2[1][2] and bb2[0][2] < bb1[1][2]:
                return True
    return False

# new_faces = []
# for face in faces:
#     bb = make_bb([vertices[i] for i in face])
#     if intersect_bb(bb, write_bb):
#         new_faces.append(face)
# faces = new_faces

sys.stderr.write(f"{len(vertices)} vertices\n")
sys.stderr.write(f"{len(faces)} triangles\n")
sys.stdout.write(f"C {str_vertex(camera[0])} {str_vertex(camera[1])} {camera[2]:f}\n")
for vertex in vertices:
    sys.stdout.write(f"v {str_vertex(vertex)}\n")
for face in faces:
        sys.stdout.write(f"tr {face[0]} {face[1]} {face[2]}\n")
