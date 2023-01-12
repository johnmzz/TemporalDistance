import random
import math

def parse(name, data):

    f_in = open("../" + name + ".txt", 'r')
    f_out = open("../new.txt", 'w+')

    edges = []

    t_min = math.inf
    t_max = 0

    for line in f_in.readlines():
        lst = line.split()

        u = lst[0]
        v = lst[1]
        t = int(lst[2])
        t_min = min(t_min, t)
        t_max = max(t_max, t)

        edge = [t, u, v]
        edges.append(edge)

    edges.sort()
    print(f"t_max - t_min = {t_max - t_min}")
    
    for i,e in enumerate(edges):
        f_out.write(e[1] + " " + e[2] + " 1 " + str((e[0] - t_min) // data[name] + 1) + "\n")


        

data = {"CollegeMsg": 86715,
        "email": 86499,
        "mathoverflow": 86412,
        }

parse("mathoverflow", data)