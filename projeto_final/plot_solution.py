from math import sqrt
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('ggplot')
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


cost_sol = 0.0
cost_init = 0.0
xsol = []
ysol = []
for i in range(n):
    i1 = sol[i]
    i2 = sol[(i+1)%n]
    x1 = xs[i1]
    y1 = ys[i1]

    x2 = xs[i2]
    y2 = ys[i2]

    cost_sol += sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))

    xsol.append(xs[i1])
    ysol.append(ys[i1])

    x1 = xs[i]
    y1 = ys[i]

    x2 = xs[(i+1)%n]
    y2 = ys[(i+1)%n]
    cost_init += sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))

xsol.append(xsol[0])
ysol.append(ysol[0])

f.close()



fig, ax = plt.subplots()

ax.scatter(xs,ys)
ax.scatter(xs[0],ys[0])
ax.plot(xsol, ysol)
plt.show()

print "Custo inicial", cost_init
print "Custo ", cost_sol