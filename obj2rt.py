#!/usr/bin/env python3
import sys
import math
import os.path
import PIL.Image
import subprocess
import random

vertices = []
faces = []
planes = []
cylinders = []
spheres = []
lights = []
materials = []
textures = []

camera = ((0, 0, 0), (1, 0, 0), 90)

mat_use = None

def parse_vertex(line, swap_yz=False):
    x = float(line[0])
    y = float(line[1])
    z = float(line[2])
    if swap_yz:
        return (x, z, y)
    else:
        return (x, y, z)

def str_float(f):
    decimals = 0
    result = "0"
    while float(result) != f:
        result = f"{f:.{decimals}f}"
        decimals += 1
    return result

def str_vertex(vertex):
    return f"{str_float(vertex[0])},{str_float(vertex[1])},{str_float(vertex[2])}"

def str_color(color):
    return f"{color[0]},{color[1]},{color[2]}"

def str_material(material):
    if type(material) is str:
        return material
    return str_color(material)

def str_texture(texture):
    if type(texture) is str:
        return texture
    return str_color(texture)

def use_material(material):
    global mat_use
    string = str_material(material)
    if string != mat_use:
        mat_use = string
        sys.stdout.write(f"mat_use {string}\n")

def add_vertex(vertex):
    try:
        return vertices.index(vertex)
    except ValueError:
        vertices.append(vertex)
        return len(vertices) - 1

def add_texture(path, flip):
    path = path.replace("\\", "/")
    filename = os.path.splitext(path)[0] + ".bmp"
    for texture in textures:
        if texture[1] == filename:
            return texture[0]
    if flip:
        args = ['magick', path, '-colorspace', 'sRGB', '-depth', '32', '-alpha', 'set', '-flip', filename]
    else:
        args = ['magick', path, '-colorspace', 'sRGB', '-depth', '32', '-alpha', 'set', filename]
    subprocess.call(args);
    name = f"tex_{os.path.splitext(os.path.basename(filename))[0]}_{random.randrange(2**32):08x}"
    textures.append((name, filename))
    return name

def mjoosten_material(color, name):
    full_name = f"mat_mjoosten_{color[0]:02x}{color[1]:02x}{color[2]:02x}_{name}"
    for material in materials:
        if material[0] == full_name:
            return full_name
    if name == "mirror":
        materials.append((full_name, (255, 255, 255), 0, None, (0, 0, 0), None))
    elif name == "glass":
        materials.append((full_name, (255, 255, 255), None, 1.5, (0, 0, 0), None))
    elif name == "metal":
        materials.append((full_name, color, 0.1, None, (0, 0, 0), None))
    else:
        assert False
    return full_name

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

def load_obj(filename, flip_textures):
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
                assert len(line) >= 4
                indices = []
                for v in line[1:]:
                    v = v.split("/")
                    if len(v) == 1:
                        vertex = obj_vert[int(v[0]) - 1]
                    if len(v) == 2:
                        vertex = obj_vert[int(v[0]) - 1] + obj_vtex[int(v[1]) - 1]
                    if len(v) == 3:
                        if v[1] != "":
                            vertex = obj_vert[int(v[0]) - 1] + obj_vtex[int(v[1]) - 1] + obj_norm[int(v[2]) - 1]
                        else:
                            vertex = obj_vert[int(v[0]) - 1] + (0, 0) + obj_norm[int(v[2]) - 1]
                    if vertex not in vertex_map:
                        vertex_map[vertex] = len(vertices)
                        vertices.append(vertex)
                    indices.append(vertex_map[vertex])
                for i in range(2, len(indices)):
                    face = (indices[0], indices[i - 1], indices[i])
                    if cur_mat is None:
                        faces.append((face, (255, 255, 255)))
                    else:
                        faces.append((face, cur_mat))
            if line[0] == "usemtl":
                cur_mat = f"mat_{line[1]}"
            if line[0] == "mtllib":
                with open(os.path.join(os.path.dirname(filename), line[1])) as mtl_file:
                    for line in mtl_file:
                        line = line.split()
                        if len(line) == 0:
                            continue
                        if line[0] == "newmtl":
                            materials.append((f"mat_{line[1]}", (0, 0, 0), None, None, (0, 0, 0), None))
                        if line[0] == "Ka":
                            mat = list(materials[-1])
                            if type(mat[4]) is not str:
                                mat[4] = tuple([int(min(float(x) * 256, 255)) for x in line[1:4]])
                            #materials[-1] = tuple(mat)
                        if line[0] == "Kd":
                            mat = list(materials[-1])
                            if type(mat[1]) is not str:
                                mat[1] = tuple([int(min(float(x) * 256, 255)) for x in line[1:4]])
                            materials[-1] = tuple(mat)
                        if line[0] == "map_Ka":
                            mat = list(materials[-1])
                            path = os.path.join(os.path.dirname(filename), line[1])
                            mat[4] = add_texture(path, flip_textures)
                            #materials[-1] = tuple(mat)
                        if line[0] == "map_Kd":
                            mat = list(materials[-1])
                            path = os.path.join(os.path.dirname(filename), line[1])
                            mat[1] = add_texture(path, flip_textures)
                            materials[-1] = tuple(mat)
                        if line[0] == "map_Bump":
                            mat = list(materials[-1])
                            path = os.path.join(os.path.dirname(filename), line[1])
                            mat[5] = add_texture(path, flip_textures)
                            materials[-1] = tuple(mat)


if len(sys.argv) < 3:
    sys.stderr.write(f"usage: {sys.argv[0]} [format] [filename]\n")
    sys.exit(1)
elif sys.argv[1] == "jkoers":
    load_jkoers(sys.argv[2])
elif sys.argv[1] == "mjoosten":
    load_mjoosten(sys.argv[2])
elif sys.argv[1] == "obj" or sys.argv[1] == "obj-flip":    
    load_obj(sys.argv[2], sys.argv[1] == "obj-flip")
else:
    sys.stderr.write(f"invalid format {sys.argv[1]}\n")
    sys.exit(1)

sys.stderr.write(f"{len(vertices)} vertices\n")
sys.stderr.write(f"{len(faces)} triangles\n")
sys.stdout.write(f"C {str_vertex(camera[0])} {str_vertex(camera[1])} {str_float(camera[2])}\n")
for texture in textures:
    sys.stdout.write(f"tex_def {texture[0]} {texture[1]}\n")
for material in materials:
    sys.stdout.write(f"mat_beg {material[0]}\n")
    if material[1] != (0, 0, 0):
        sys.stdout.write(f"    diffuse {str_texture(material[1])}\n")
    if material[2] is not None:
        sys.stdout.write(f"    reflective 255,255,255 {str_float(material[2])}\n")
    if material[3] is not None:
        sys.stdout.write(f"    refractive 255,255,255 {str_float(material[3])}\n")
    if material[4] != (0, 0, 0):
        sys.stdout.write(f"    emission 1.0 {str_texture(material[4])}\n")
    if material[5] != None:
        sys.stdout.write(f"    bump {str_texture(material[5])}\n")
    sys.stdout.write(f"mat_end\n")
for plane in planes:
    use_material(plane[2])
    sys.stdout.write(f"pl {str_vertex(plane[0])} {str_vertex(plane[1])}\n")
for cylinder in cylinders:
    use_material(cylinder[4])
    sys.stdout.write(f"cy {str_vertex(cylinder[0])} {str_vertex(cylinder[1])} {str_float(cylinder[2])} {str_float(cylinder[3])}\n")
for sphere in spheres:
    use_material(sphere[2])
    sys.stdout.write(f"sp {str_vertex(sphere[0])} {str_float(sphere[1])}\n")
for vertex in vertices:
    if len(vertex) == 3:
        sys.stdout.write(f"v {str_vertex(vertex)}\n")
    elif len(vertex) == 5:
        sys.stdout.write(f"w {str_vertex(vertex[0:3])} {str_float(vertex[3])},{str_float(vertex[4])}\n")
    elif len(vertex) == 8:
        sys.stdout.write(f"W {str_vertex(vertex[0:3])} {str_float(vertex[3])},{str_float(vertex[4])} {str_vertex(vertex[5:8])}\n")
for face in faces:
    use_material(face[1])
    sys.stdout.write(f"f {face[0][0]} {face[0][1]} {face[0][2]}\n")
for light in lights:
    sys.stdout.write(f"L {str_vertex(light[0])} {str_float(light[1])} {str_color(light[2])}\n")
