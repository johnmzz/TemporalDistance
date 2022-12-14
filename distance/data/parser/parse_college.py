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
        t = int(lst[2])

        edge = [t, u, v]
        edges.append(edge)

    edges.sort()
    
    new_t = 1
    for i,e in enumerate(edges):
        f_out.write(e[1] + " " + e[2] + " 1 " + str(new_t) + "\n")

        if i != 0 and i % 100 == 0:
            new_t += 1

parse("CollegeMsg")