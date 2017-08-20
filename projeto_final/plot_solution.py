from math import sqrt
f = open("./ALL_tsp/att48.tsp")

ler_nos = False

xs = []
ys = []
for line in f:
    if "NODE_COORD_SECTION" in line:
        ler_nos = True
        continue

    if "EOF" in line:
        break

    if ler_nos:
        id, x, y = line.split();
        x = float(x)
        y = float(y)
        xs.append(x)
        ys.append(y)

f.close()

n = len(xs)
print "Quantidade de nos ", n

f = open("./ALL_tsp/att48.opt.tour")

sol = []

ler_nos = False

for line in f:
    if "TOUR_SECTION" in line:
        ler_nos = True
        continue

    if "EOF" in line:
        break

    if ler_nos:
        if int(line) > 0:
            sol.append(int(line)-1)


cost = 0.0
for i in range(n):
    i1 = sol[i]
    i2 = sol[(i+1)%n]
    x1 = xs[i1]
    y1 = ys[i1]

    x2 = xs[i2]
    y2 = ys[i2]

    cost += sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))


f.close()

print "Custo ", cost