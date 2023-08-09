from cs50 import get_float

cents = get_float("Change owed: ")
while cents < 0:
    cents = get_float("Change owed: ")

cents = cents * 100

sum_cash = 0
money = [25, 10, 5, 1]
for i in money:
    cash = int(cents / i)
    cents = cents % i
    sum_cash += cash
print(sum_cash)