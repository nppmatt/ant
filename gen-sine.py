#!/usr/bin/env python3

from math import sin, pi

def main(f, PRECISION, NAME):
    f.write("double %s[] = {\n" % NAME)
    j = 0
    p = 0.0
    while True:
        f.write("{:.20f}, ".format(sin(p)))
        j += 1
        p += PRECISION
        if p > 2*pi:
            break
    f.write("1.0 };\n")
    f.write("const int %s_size = %d;\n" % (NAME, j+1))


if __name__ == '__main__':
    includeDir = "src/include/"
    #main(open("costable_1.h", "w"), 1.0, "costable_1")
    #main(open("costable_0_1.h", "w"), 0.1, "costable_0_1")
    #main(open("costable_0_01.h", "w"), 0.01, "costable_0_01")
    #main(open("costable_0_001.h", "w"), 0.001, "costable_0_001")
    main(open(f"{includeDir}sintable_0_0001.h", "w"), 0.0001, "sintable_0_0001")
    main(open(f"{includeDir}sintable_0_00001.h", "w"), 0.00001, "sintable_0_00001")

