import matplotlib.pyplot as plt
from math import sqrt
import imageio
plt.style.use('ggplot')


def cost(xs, ys):
    cost = 0.0

    for i in range(len(xs)-1):
        x1 = xs[i]; y1 = ys[i]
        x2 = xs[i+1]; y2 = ys[i+1]

        cost += sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))

    return cost


def readPath(file):
    f = open(file)
    xs = []
    ys = []

    n = int(f.next())

    for i in range(n):
        line = f.next()
        id, x, y = line.split()
        xs.append(float(x))
        ys.append(float(y))

    xs.append(xs[0])
    ys.append(ys[0])

    mini, minj = [int(x) for x in f.next().strip().split()]

    f.close()

    return xs, ys, mini, minj


cont = 0
mini = 0
minj = 0

for i in range(1, 52):
    mini_ant = mini
    minj_ant = minj

    xs, ys, mini, minj = readPath("att48/path_%.2d.txt"%i)

    if i != 1:
        fig, ax = plt.subplots(1, 1)
        ax.plot(xs, ys, marker="o", ms=5)
        ax.plot(xs[mini_ant:mini_ant+2], ys[mini_ant:mini_ant+2], marker="o", linewidth=3, c="blue")
        ax.plot(xs[minj_ant:minj_ant+2], ys[minj_ant:minj_ant+2], marker="o", linewidth=3, c="blue")
        ax.set_title("att48 - tamanho = %8.2f" % cost(xs,ys))
        plt.savefig("att48/path_%.3d.png"%cont)
        plt.close()
        cont += 1

    fig, ax = plt.subplots(1, 1)
    ax.plot(xs, ys, marker="o", ms=5)
    ax.set_title("att48 - tamanho = %8.2f" % cost(xs,ys))
    plt.savefig("att48/path_%.3d.png"%cont)
    plt.close()
    cont += 1

    fig, ax = plt.subplots(1, 1)
    ax.plot(xs, ys, marker="o", ms=5)
    ax.plot(xs[mini:mini+2], ys[mini:mini+2], marker="o", linewidth=3, c="blue")
    ax.plot(xs[minj:minj+2], ys[minj:minj+2], marker="o", linewidth=3, c="blue")
    ax.set_title("att48 - tamanho = %8.2f" % cost(xs,ys))
    plt.savefig("att48/path_%.3d.png"%cont)
    cont += 1
    plt.close()


with imageio.get_writer('att48/att48.gif', mode='I', duration=0.25, loop=False) as writer:
    for filename in ["att48/path_%.3d.png"%x for x in range(cont-3)]:
        image = imageio.imread(filename)
        writer.append_data(image)