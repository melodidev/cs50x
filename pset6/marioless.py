from cs50 import get_int

h = get_int("Height: ")
while not 0 < h < 9:
    h = get_int("Height: ")

for i in range(1, h + 1, 1):
    print(" " * (h - i), "#" * i, sep='')