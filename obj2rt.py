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

class ObjMaterial:
    def __init__(self, name):
        self.name = name
        self.illum = 2
        self.ka = None
        self.kd = None
        self.ks = None
        self.tf = None
        self.d = None
        self.ns = 1.0
        self.ni = 1.0
        self.map_ka = None
        self.map_kd = None
        self.map_ks = None
        self.map_d = None
        self.bump = None

class Material:
    def __init__(self, name):
        self.name = name
        self.bsdfs = []
        self.emission = None
        self.bump = None
        self.alpha = None

    def write(self):
        sys.stdout.write(f"mat_beg {self.name}\n")
        for bsdf in self.bsdfs:
            sys.stdout.write(f"    {str(bsdf)}\n")
        if self.emission is not None:
            sys.stdout.write(f"    emission 1 {str_texture(self.emission)}\n")
        if self.bump is not None:
            sys.stdout.write(f"    bump {str_texture(self.bump)}\n")
        if self.alpha is not None:
            sys.stdout.write(f"    alpha {str_texture(self.alpha)}\n")
        sys.stdout.write(f"mat_end\n")

    def add(self, bsdf):
        self.bsdfs.append(bsdf)

    def add_urgently(self, bsdf):
        self.bsdfs = [b for b in self.bsdfs if type(b) is not type(bsdf)]
        self.bsdfs.append(bsdf)

    def add_trivially(self, bsdf):
        bsdfs = [b for b in self.bsdfs if type(b) is type(bsdf)]
        if len(bsdfs) == 0:
            self.bsdfs.append(bsdf)

class Diffuse:
    def __init__(self, weight, texture):
        self.weight = weight
        self.texture = texture

    def __str__(self):
        return f"diffuse {str_float(self.weight)} {str_texture(self.texture)}"

class Reflection:
    def __init__(self, weight, texture, fuzzy):
        self.weight = weight
        self.texture = texture
        self.fuzzy = fuzzy

    def __str__(self):
        return f"reflective {str_float(self.weight)} {str_texture(self.texture)} {str_float(self.fuzzy)}"

class Refraction:
    def __init__(self, weight, texture, ior):
        self.weight = weight
        self.texture = texture
        self.ior = ior

    def __str__(self):
        return f"transmissive {str_float(self.weight)} {str_texture(self.texture)} {str_float(self.ior)}"

def parse_vertex(line, swap_yz=False):
    x = float(line[0])
    y = float(line[1])
    z = float(line[2])
    if swap_yz:
        return (x, z, y)
    else:
        return (x, y, z)

def str_float(f):
    d = 0
    while True:
        r = "%.*f" % (d, f)
        if float(r) == f:
            return r
        d += 1

def str_vertex(vertex):
    return f"{str_float(vertex[0])},{str_float(vertex[1])},{str_float(vertex[2])}"

def str_color(color):
    if type(color[0]) is int:
        return f"{color[0]},{color[1]},{color[2]}"
    elif len(color) == 3:
        return f"({str_float(color[0])},{str_float(color[1])},{str_float(color[2])})"
    elif len(color) == 4:
        return f"({str_float(color[0])},{str_float(color[1])},{str_float(color[2])},{str_float(color[3])})"
    else:
        assert False

def str_material(material):
    if type(material) is str:
        return material
    return str_color(material)

def str_texture(texture):
    if type(texture) is str:
        return texture
    elif type(texture) is tuple and type(texture[0]) is not int and type(texture[0]) is not float:
        return f"filter {str_texture(texture[0])} {str_texture(texture[1])}"
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
    material = Material(full_name)
    materials.append(material)
    if name == "mirror":
        material.add(Reflection(1.0, (255, 255, 255), 0.0))
    elif name == "glass":
        material.add(Refraction(1.0, (255, 255, 255), 1.5))
    elif name == "metal":
        material.add(Reflection(1.0, color, 0.1))
    else:
        assert False
    return full_name

def obj_map(m, c):
    if m is None:
        if c == (0.0, 0.0, 0.0):
            return None
        return c
    if c is None:
        return m
    if c == (0.0, 0.0, 0.0):
        return None
    if c == (1.0, 1.0, 1.0):
        return m
    return (c, m)

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

def load_mtl(filename, flip_textures):
    with open(filename) as mtl_file:
        obj_materials = []
        for line in mtl_file:
            line = line.split()
            if len(line) == 0:
                continue
            if line[0] == "newmtl":
                material = ObjMaterial(f"mat_{line[1]}")
                obj_materials.append(material)
            if line[0] == "illum":
                obj_materials[-1].illum = int(line[1])
            if line[0] == "Ka":
                obj_materials[-1].ka = tuple(map(float, line[1:4]))
            if line[0] == "Kd":
                obj_materials[-1].kd = tuple(map(float, line[1:4]))
            if line[0] == "Ks":
                obj_materials[-1].ks = tuple(map(float, line[1:4]))
            if line[0] == "Tf":
                obj_materials[-1].tf = tuple(map(float, line[1:4]))
            if line[0] == "d":
                obj_materials[-1].d = (0.0, 0.0, 0.0, float(line[1]))
            if line[0] == "Ns":
                obj_materials[-1].ns = float(line[1])
            if line[0] == "Ni":
                obj_materials[-1].ni = float(line[1])
            if line[0] == "map_Ka":
                path = os.path.join(os.path.dirname(filename), line[1])
                obj_materials[-1].map_ka = add_texture(path, flip_textures)
            if line[0] == "map_Kd":
                path = os.path.join(os.path.dirname(filename), line[1])
                obj_materials[-1].map_kd = add_texture(path, flip_textures)
            if line[0] == "map_Ks":
                path = os.path.join(os.path.dirname(filename), line[1])
                obj_materials[-1].map_ks = add_texture(path, flip_textures)
            if line[0] == "map_d":
                pass
                #path = os.path.join(os.path.dirname(filename), line[1])
                #obj_materials[-1].map_d = add_texture(path, flip_textures)
            if line[0] == "map_Bump":
                path = os.path.join(os.path.dirname(filename), line[1])
                obj_materials[-1].bump = add_texture(path, flip_textures)
        for mat_obj in obj_materials:
            mat_rt = Material(mat_obj.name)
            mat_rt.emission = obj_map(mat_obj.map_ka, mat_obj.ka)
            mat_rt.alpha = obj_map(mat_obj.map_kd, mat_obj.d)
            mat_rt.bump = mat_obj.bump
            diffuse = obj_map(mat_obj.map_kd, mat_obj.kd)
            specular = obj_map(mat_obj.map_ks, mat_obj.ks)
            if diffuse is not None:
                mat_rt.add(Diffuse(1.0, diffuse))
            if specular is not None:
                mat_rt.add(Reflection(1.0, specular, 0.1))
            if mat_obj.illum == 4:
                mat_rt.add(Refraction(1.0, mat_obj.tf, mat_obj.ni))
            materials.append(mat_rt)

def load_obj(filename, flip_textures):
    obj_vert = []
    obj_vtex = []
    obj_norm = []
    vertex_map = {}
    cur_mat = None
    mtl_files = set()

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
                pathname = os.path.join(os.path.dirname(filename), line[1])
                if pathname not in mtl_files:
                    load_mtl(pathname, flip_textures)
                    mtl_files.add(pathname)

if len(sys.argv) < 3:
    sys.stderr.write(f"usage: {sys.argv[0]} [format] [filename]\n")
    sys.exit(1)
elif sys.argv[1] == "jkoers":
    load_jkoers(sys.argv[2])
elif sys.argv[1] == "mjoosten":
    load_mjoosten(sys.argv[2])
elif sys.argv[1] == "obj" or sys.argv[1] == "obj-flip":    
    load_obj(sys.argv[2], sys.argv[1] == "obj-flip")
elif sys.argv[1] == "mtl" or sys.argv[1] == "mtl-flip":
    load_mtl(sys.argv[2], sys.argv[1] == "mtl-flip")
else:
    sys.stderr.write(f"invalid format {sys.argv[1]}\n")
    sys.exit(1)

sys.stderr.write(f"{len(vertices)} vertices\n")
sys.stderr.write(f"{len(faces)} triangles\n")
sys.stdout.write(f"C {str_vertex(camera[0])} {str_vertex(camera[1])} {str_float(camera[2])}\n")
for texture in textures:
    sys.stdout.write(f"tex_def {texture[0]} {texture[1]}\n")
for material in materials:
    material.write()
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
