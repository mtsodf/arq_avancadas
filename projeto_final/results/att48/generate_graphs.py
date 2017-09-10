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

costs = []
for i in range(1, 52):
    mini_ant = mini
    minj_ant = minj

    xs, ys, mini, minj = readPath("path_%.2d.txt"%i)
    costs.append(cost(xs,ys))
    if i != 1:
        fig, ax = plt.subplots(1, 2, figsize=(15,7))
        plt.suptitle("att48 - tamanho = %8.2f" % cost(xs,ys), fontsize=20)
        ax[0].plot(xs, ys, marker="o", ms=5)
        ax[0].plot(xs[mini_ant:mini_ant+2], ys[mini_ant:mini_ant+2], marker="o", linewidth=3, c="blue")
        ax[0].plot(xs[minj_ant:minj_ant+2], ys[minj_ant:minj_ant+2], marker="o", linewidth=3, c="blue")


        ax[1].set_xlim((0,52))
        ax[1].set_ylim((30000, 150000))
        ax[1].plot(range(1,i+1), costs, marker="o")
        ax[1].plot((1,52), (33523, 33523), color="blue")
        ax[1].set_xlabel("iteracao")
        plt.tight_layout(rect=(0,0,1,0.9))
        plt.savefig("path__%.3d.png"%cont)
        plt.close()
        cont += 1



    fig, ax = plt.subplots(1, 2, figsize=(15,7))
    plt.suptitle("att48 - tamanho = %8.2f" % cost(xs,ys), fontsize=20)
    ax[0].plot(xs, ys, marker="o", ms=5)
    ax[1].plot(range(1,i+1), costs, marker="o")
    ax[1].set_xlim((0,52))
    ax[1].set_ylim((30000, 150000))
    ax[1].plot((1,52), (33523, 33523), color="blue")
    ax[1].set_xlabel("iteracao")
    plt.tight_layout(rect=(0,0,1,0.9))
    plt.savefig("path__%.3d.png"%cont)
    plt.close()
    cont += 1


    fig, ax = plt.subplots(1, 2, figsize=(15,7))
    plt.suptitle("att48 - tamanho = %8.2f" % cost(xs,ys), fontsize=20)
    ax[0].plot(xs, ys, marker="o", ms=5)
    ax[0].plot(xs[mini:mini+2], ys[mini:mini+2], marker="o", linewidth=3, c="blue")
    ax[0].plot(xs[minj:minj+2], ys[minj:minj+2], marker="o", linewidth=3, c="blue")
    ax[1].plot(range(1,i+1), costs, marker="o")
    ax[1].set_xlim((0,52))
    ax[1].set_ylim((30000, 150000))
    ax[1].plot((1,52), (33523, 33523), color="blue")
    ax[1].set_xlabel("iteracao")
    plt.tight_layout(rect=(0,0,1,0.9))
    plt.savefig("path__%.3d.png"%cont)
    cont += 1
    plt.close()


with imageio.get_writer('att48_graf.gif', mode='I', duration=0.25, loop=False) as writer:
    for filename in ["path__%.3d.png"%x for x in range(cont-3)]:
        image = imageio.imread(filename)
        writer.append_data(image)