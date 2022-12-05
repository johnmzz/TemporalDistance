import random

def parse(name):

    f_in = open("../" + name + ".txt", 'r')
    f_out = open("../new.txt", 'w+')

    for line in f_in.readlines():
        lst = line.split()

        u = lst[0]
        v = lst[1]
        t = lst[2]

        f_out.write(u + " " + v + " " + t + " " + str(random.randint(1,100)) + "\n")

parse("CollegeMsg")