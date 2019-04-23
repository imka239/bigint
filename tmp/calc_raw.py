#!/usr/bin/env python3
import sys
import time

tm = time.time()
a = input().replace("\n", " ").split(" ")

total = 0
for i in range(2, 2 * int(a[0]) + 1, 2):
    aa = int(a[3])
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
        a[3] = input()

total = time.time() - tm
if a[2] == "t":
    sys.stderr.write("PTHN\t: " + "{:.12f}".format(total / int(a[0])) + '\n');
