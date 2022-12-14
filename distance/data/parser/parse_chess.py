import random
import math

def parse(name):

    f_in = open("../" + name + ".txt", 'r')
    f_out = open("../new.txt", 'w+')

    edges = []

    for line in f_in.readlines():
        lst = line.split()

        u = lst[0]
        v = lst[1]
        d = int(lst[2])
        t = int(float(lst[3]))

        edge = [t, u, v, d]
        edges.append(edge)

    edges.sort()
    
    new_t = 0
    last_t = 0
    for i,e in enumerate(edges):
        if e[0] != last_t:
            last_t = e[0]
            new_t += 1
 
        f_out.write(e[1] + " " + e[2] + " " + str(e[3]) + " " + str(new_t) + "\n")

parse("chess")