#!/usr/bin/env python3
import sys
import time

tm = time.time()
a = input().replace("\n", " ").split(" ")
a[2] = int(a[2])

total = 0
for i in range(2, 2 * int(a[0]) + 1, 2):
    aa = a[2]
    ba = int(input())
    if a[1] == '+':
        aa += ba
    elif a[1] == '-':
        aa -= ba
    elif a[1] == '*':
        aa *= ba
    elif a[1] == '/':
        aa //= ba
    elif a[1] == '%':
        aa %= ba
    if i < 2 * int(a[0]):
        a[2] = int(input())

total = time.time() - tm
sys.stderr.write("PTHN\t: " + "{:.12f}".format(total / int(a[0])) + '\n');