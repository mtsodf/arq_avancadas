import matplotlib.pyplot as plt
from math import sqrt
plt.style.use('ggplot')

def plot_sol(ax):

    f = open("../../ALL_tsp/att48.tsp")

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

    f = open("../../ALL_tsp/att48.opt.tour")

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

    ax.plot(xsol, ysol)

f = open("out.txt")

iters = []
costs = []
temperatures = []

for line in f:
    it, cost, t = line.split()
    it = int(it)
    cost = float(cost)
    iters.append(it)
    costs.append(cost)
    temperatures.append(float(t))


f.close()
fig, axis = plt.subplots(2,2, figsize=(20,15))
axis[0,0].plot(iters, costs)
axis[0,1].plot(iters, temperatures)



f = open("path.txt")
xs = []
ys = []


for line in f:
    id, x, y = line.split()
    xs.append(float(x))
    ys.append(float(y))

xs.append(xs[0])
ys.append(ys[0])

axis[1,0].scatter(xs, ys)
f.close()

axis[1,1].plot(xs, ys, marker="o", ms=5)
plot_sol(axis[1,0])


plt.show()