import random
import math

def parse(name, data):

    f_in = open("../" + name + ".txt", 'r')
    f_out = open("../new.txt", 'w+')

    edges = []

    t_min = math.inf
    t_max = 0

    n = 1
    v_map = {}

    for line in f_in.readlines():
        lst = line.split()

        u = lst[0]
        if u not in v_map:
            v_map[u] = n
            u = str(v_map[u])
            n += 1
        else:
            u = str(v_map[u])

        v = lst[1]
        if v not in v_map:
            v_map[v] = n
            v = str(v_map[v])
            n += 1
        else:
            v = str(v_map[v])

        t = int(lst[2])
        t_min = min(t_min, t)
        t_max = max(t_max, t)

        edge = [t, u, v]
        edges.append(edge)

    edges.sort()
    print(f"t_max - t_min = {t_max - t_min}")
    
    for i,e in enumerate(edges):
        f_out.write(e[1] + " " + e[2] + " " + str(random.randint(1,10)) + " " + str((e[0] - t_min) // data[name] + 1) + "\n")


        

data = {"mock_snap": 1,
        "CollegeMsg": 86715,
        "mathoverflow": 86412,
        }

parse("CollegeMsg", data)