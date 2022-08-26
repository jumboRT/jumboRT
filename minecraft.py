import math

z = -1 / 3
a = math.sqrt(1 - z * z)

for i in range(3600):
    x = math.cos(i / 1800 * math.pi) * a
    y = math.sin(i / 1800 * math.pi) * a
    print(f"$ {i} C 61,-69,77 {x:.05f},{y:.05f},{z:.05f} 90")
