import random
import math

def parse(name):

    f_in = open("../" + name + ".txt", 'r')
    f_out = open("../new.txt", 'w+')

    edges = []

    m = 0
    t_min = math.inf
    t_max = 0

    for line in f_in.readlines():
        lst = line.split()

        u = lst[0]
        v = lst[1]
        t = int(lst[2])

        edge = [t, u, v]
        edges.append(edge)

        m += 1
        if t > t_max:
            t_max = t
        if t < t_min:
            t_min = t

    edges.sort()
    
    new_t = 1
    for i,e in enumerate(edges):
        f_out.write(e[1] + " " + e[2] + " " + str(new_t) + " " + "1" + "\n")

        if i % 100 == 0:
            new_t += 1

    print(f"m = {m}, t_max = {t_max}, t_min = {t_min}, num_t = {t_max-t_min}")
    print(f"new_t = {new_t}")

parse("CollegeMsg")