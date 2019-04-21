#!/usr/bin/env python3
import sys
import time

tm = time.time()
a = input().replace("\n", " ").split(" ")

total = 0
for i in range(2, 2 * int(a[0]) + 1, 2):
    aa = int(a[2])
    ba = int(input())
    if a[1] == '+':
        print (aa + ba)
    elif a[1] == '-':
        print (aa - ba)
    elif a[1] == '*':
        print (aa * ba)
    elif a[1] == '/':
        print (aa // ba)
    elif a[1] == '%':
        print (aa % ba)
    if i < 2 * int(a[0]):
        a[2] = input()

total = time.time() - tm
sys.stderr.write("PTHN\t: " + "{:.12f}".format(total / int(a[0])) + '\n');