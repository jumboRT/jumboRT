#!/usr/bin/env python3
import sys
import math
import os.path
import PIL.Image

vertices = []
faces = []
planes = []
cylinders = []
spheres = []
lights = []
materials = []

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

def str_color(color):
    return f"{color[0]},{color[1]},{color[2]}"

def str_material(material):
    if type(material) is str:
        return material
    return str_color(material)

def add_vertex(vertex):
    try:
        return vertices.index(vertex)
    except ValueError:
        vertices.append(vertex)
        return len(vertices) - 1

def mjoosten_material(color, name):
    full_name = f"mat_mjoosten_{color[0]:02x}{color[1]:02x}{color[2]:02x}_{name}"
    for material in materials:
        if material[0] == full_name:
            return full_name
    if name == "mirror":
        materials.append((full_name, (255, 255, 255), 0, None, (0, 0, 0)))
    elif name == "glass":
        materials.append((full_name, (255, 255, 255), None, 1.5, (0, 0, 0)))
    elif name == "metal":
        materials.append((full_name, color, 0.1, None, (0, 0, 0)))
    else:
        assert False
    return full_name

def convert_image(filename):
    image = PIL.Image.open(filename)
    filename = os.path.splitext(filename)[0] + ".bmp"
    image.save(filename)
    image.close()
    return filename

def load_jkoers(filename):
    with open(filename) as file:
        for line in file:
            line = line.split()
            if len(line) == 0:
                continue
            if line[0] == "pl":
                position = parse_vertex(line[1].split(","), True)
                rotation = parse_vertex(line[2].split(","), True)
                color = tuple(map(int, line[3].split(",")))
                planes.append((position, rotation, color))
            if line[0] == "tr":
                a = add_vertex(parse_vertex(line[1].split(","), True))
                b = add_vertex(parse_vertex(line[2].split(","), True))
                c = add_vertex(parse_vertex(line[3].split(","), True))
                color = tuple(map(int, line[4].split(",")))
                faces.append(((a, b, c), color))
            if line[0] == "c":
                position = parse_vertex(line[1].split(","), True)
                rotation = parse_vertex(line[2].split(","), True)
                fov = float(line[3])
                camera = (position, rotation, fov)
            if line[0] == "l":
                position = parse_vertex(line[1].split(","), True)
                brightness = float(line[2])
                color = tuple(map(int, line[3].split(",")))
                lights.append((position, brightness, color))

def load_mjoosten(filename):
    with open(filename) as file:
        for line in file:
            line = line.split()
            if len(line) == 0:
                continue
            if line[0] == "C":
                position = parse_vertex(line[1].split(","), True)
                rotation = parse_vertex(line[2].split(","), True)
                fov = float(line[3])
                camera = (position, rotation, fov)
            if line[0] == "L":
                position = parse_vertex(line[1].split(","), True)
                brightness = float(line[2])
                color = tuple(map(int, line[3].split(",")))
                lights.append((position, brightness, color))
            if line[0] == "pl":
                position = parse_vertex(line[1].split(","), True)
                rotation = parse_vertex(line[2].split(","), True)
                material = tuple(map(int, line[3].split(",")))
                if len(line) >= 5:
                    material = mjoosten_material(material, line[4])
                planes.append((position, rotation, material))
            if line[0] == "cy":
                position = parse_vertex(line[1].split(","), True)
                rotation = parse_vertex(line[2].split(","), True)
                diameter = float(line[3])
                height = float(line[4])
                material = tuple(map(int, line[5].split(",")))
                if len(line) >= 7:
                    material = mjoosten_material(material, line[6])
                cylinders.append((position, rotation, diameter, height, material))
            if line[0] == "sp":
                position = parse_vertex(line[1].split(","), True)
                diameter = float(line[2])
                material = tuple(map(int, line[3].split(",")))
                if len(line) >= 5:
                    material = mjoosten_material(material, line[4])
                spheres.append((position, diameter, material))

def load_obj(filename):
    obj_vert = []
    obj_vtex = []
    obj_norm = []
    vertex_map = {}
    cur_mat = None

    with open(filename) as file:
        for line in file:
            line = line.split()
            if len(line) == 0:
                continue
            if line[0] == "v":
                vertex = parse_vertex(line[1:], True)
                obj_vert.append(vertex)
            if line[0] == "vt":
                x = float(line[1])
                y = float(line[2])
                obj_vtex.append((x, y))
            if line[0] == "vn":
                normal = parse_vertex(line[1:], True)
                obj_norm.append(normal)
            if line[0] == "f":
                assert len(line) == 4
                indices = []
                for v in line[1:]:
                    v = v.split("/")
                    if len(v) == 1:
                        vertex = obj_vert[int(v[0]) - 1]
                    if len(v) == 3:
                        vertex = obj_vert[int(v[0]) - 1] + obj_vtex[int(v[1]) - 1] + obj_norm[int(v[2]) - 1]
                    if vertex not in vertex_map:
                        vertex_map[vertex] = len(vertices)
                        vertices.append(vertex)
                    indices.append(vertex_map[vertex])
                if cur_mat is None:
                    faces.append((tuple(indices), (255, 255, 255)))
                else:
                    faces.append((tuple(indices), cur_mat))
            if line[0] == "usemtl":
                cur_mat = f"mat_{line[1]}"
            if line[0] == "mtllib":
                with open(os.path.join(os.path.dirname(filename), line[1])) as mtl_file:
                    for line in mtl_file:
                        line = line.split()
                        if len(line) == 0:
                            continue
                        if line[0] == "newmtl":
                            materials.append((f"mat_{line[1]}", (0, 0, 0), None, None, (0, 0, 0)))
                        if line[0] == "Ka":
                            mat = list(materials[-1])
                            if type(mat[4]) is not str:
                                mat[4] = tuple([int(min(float(x) * 256, 255)) for x in line[1:4]])
                            materials[-1] = tuple(mat)
                        if line[0] == "Kd":
                            mat = list(materials[-1])
                            if type(mat[1]) is not str:
                                mat[1] = tuple([int(min(float(x) * 256, 255)) for x in line[1:4]])
                            materials[-1] = tuple(mat)
                        if line[0] == "map_Ka":
                            mat = list(materials[-1])
                            mat[4] = convert_image(os.path.join(os.path.dirname(filename), line[1]))
                            materials[-1] = tuple(mat)
                        if line[0] == "map_Kd":
                            mat = list(materials[-1])
                            mat[1] = convert_image(os.path.join(os.path.dirname(filename), line[1]))
                            materials[-1] = tuple(mat)

if len(sys.argv) < 3:
    sys.stderr.write(f"usage: {sys.argv[0]} [format] [filename]\n")
    sys.exit(1)
elif sys.argv[1] == "jkoers":
    load_jkoers(sys.argv[2])
elif sys.argv[1] == "mjoosten":
    load_mjoosten(sys.argv[2])
elif sys.argv[1] == "obj":    
    load_obj(sys.argv[2])
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
for material in materials:
    sys.stdout.write(f"mat_beg {material[0]}\n")
    if type(material[1]) is str:
        sys.stdout.write(f"    albedo_tex {material[1]}\n")
    else:
        sys.stdout.write(f"    albedo {str_color(material[1])}\n")
    if material[2] is not None:
        sys.stdout.write(f"    fuzzy {material[2]:f}\n")
    if material[3] is not None:
        sys.stdout.write(f"    refractive {material[3]:f}\n")
    if type(material[4]) is str:
        sys.stdout.write(f"    emission_tex {material[4]}\n")
    else:
        sys.stdout.write(f"    emission {str_color(material[4])}\n")
    sys.stdout.write(f"mat_end\n")
for plane in planes:
    sys.stdout.write(f"pl {str_vertex(plane[0])} {str_vertex(plane[1])} {str_material(plane[2])}\n")
for cylinder in cylinders:
    sys.stdout.write(f"cy {str_vertex(cylinder[0])} {str_vertex(cylinder[1])} {cylinder[2]:f} {cylinder[3]:f} {str_material(cylinder[4])}\n")
for sphere in spheres:
    sys.stdout.write(f"sp {str_vertex(sphere[0])} {sphere[1]:f} {str_material(sphere[2])}\n")
for vertex in vertices:
    if len(vertex) == 3:
        sys.stdout.write(f"v {str_vertex(vertex)}\n")
    elif len(vertex) == 8:
        sys.stdout.write(f"vtn {str_vertex(vertex[0:3])} {vertex[3]:f},{vertex[4]:f} {str_vertex(vertex[5:8])}\n")
for face in faces:
    sys.stdout.write(f"tr {face[0][0]} {face[0][1]} {face[0][2]} {str_material(face[1])}\n")
for light in lights:
    sys.stdout.write(f"L {str_vertex(light[0])} {light[1]:f} {str_color(light[2])}\n")
