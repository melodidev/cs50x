# 4003600000000014
from cs50 import get_string
from sys import exit

credit = get_string("Card Number: ")
if len(credit) != 16:
    print("INVALID, shorter")
    exit(1)

sum_evens = 0
for i in range(0, 16, 2):
    evens = 2 * int(credit[i])

    if evens > 9:
        evens = str(evens)
        evens = int(evens[0]) + int(evens[1])
    sum_evens = sum_evens + evens

sum_odds = 0
for i in range(1, 17, 2):
    odds = int(credit[i])
    sum_odds = sum_odds + odds

sum_all = sum_evens + sum_odds
sum_all = str(sum_all)
if sum_all[1] == "0":
    print("VALID")
else:
    print("INVALID")