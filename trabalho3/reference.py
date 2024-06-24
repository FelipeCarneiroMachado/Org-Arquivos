from sys import argv
f = open("debug.txt")
#print(list(map(lambda x: x.split(",")[0], f.read().split("\n")[:-1])))
l = list(map(int,  map(lambda x: x.split(",")[0], f.read().split("\n")[:-1])))
#print(l)
for i in range(int(argv[1])):
    if i not in l:
        print(i)