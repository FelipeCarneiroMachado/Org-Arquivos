import random

l = list(range(1, 101))
random.shuffle(l)

print("100 " + " ".join(map(str, l)))