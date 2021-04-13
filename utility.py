import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

markers = ['o', 'x', '+', 'v', '^', '<', '>', 's', 'd', '.', ',']
colors = ['b', 'r', 'g', 'y']


def drawLineGraph(x, ys, title):
  count = 0
  for k in ys:
    plt.plot(x, ys[k], markers[count%11]+'-'+colors[count%4], label=k)
    count += 1
  plt.title(title)
  plt.xlabel("number of columns")
  plt.legend()
  plt.show()


def test2k10():
  x = ['10', '30', '50', '70', '90', '110', '130', '150', '170', '200']

  ys = {
    'large flow recall': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.017, 0.017, 0.0419, 0.131, 0.817, 0.773, 0.555, 0.676, 1, 1]
  }
  drawLineGraph(x, ys, "method 1, 405 flows")

  ys = {
    'all flow precision': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.017, 0.017, 0.0419, 0.131, 0.817, 0.773, 0.555, 0.676, 1, 1]
  }
  drawLineGraph(x, ys, "method 1, 405 flows")

  ys = {
    'large flow recall': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.017, 0.025, 0.096, 0.622, 0.580, 0.410, 0.493, 0.570, 0.758, 0.758]
  }
  drawLineGraph(x, ys, "method 2, 405 flows")

  ys = {
    'all flow precision': [1, 1, 0.907, 0.834, 0.880, 0.922, 0.889, 0.898, 0.761, 0.761],
    'all flow recall': [0.017, 0.025, 0.096, 0.622, 0.580, 0.410, 0.493, 0.570, 0.758, 0.758]
  }
  drawLineGraph(x, ys, "method 2, 405 flows")


def test2k2():
  x = ['10', '30', '50', '70', '90', '110', '130', '150', '170', '200', '400']

  ys = {
    'all flow precision': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.017, 0.017, 0.0296296, 0.0592593, 0.17037, 0.224691, 0.350617, 0.269136, 0.311111, 0.407407, 0.94321]
  }
  drawLineGraph(x, ys, "method 1, 405 flows, k=2")

  # ys = {
  #   'all flow precision': [1, 1, 0.907, 0.834, 0.880, 0.922, 0.889, 0.898, 0.761, 0.761],
  #   'all flow recall': [0.017, 0.025, 0.096, 0.622, 0.580, 0.410, 0.493, 0.570, 0.758, 0.758]
  # }
  # drawLineGraph(x, ys, "method 2, 405 flows")


def test6k2():
  x = ['50', '150', '250', '350', '450', '550', '650', '750', '850', '1000', '2000']

  ys = {
    'all flow precision': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.0156014, 0.0221439, 0.0583795, 0.178661, 0.172622, 0.419728, 0.29844, 0.366885, 0.421741, 0.502265, 0.97081]
  }
  drawLineGraph(x, ys, "method 1, 1987 flows, k=2")

  # ys = {
  #   'all flow precision': [1, 1, 0.907, 0.834, 0.880, 0.922, 0.889, 0.898, 0.761, 0.761],
  #   'all flow recall': [0.017, 0.025, 0.096, 0.622, 0.580, 0.410, 0.493, 0.570, 0.758, 0.758]
  # }
  # drawLineGraph(x, ys, "method 2, 405 flows")


def test6k10():
  x = ['50', '150', '250', '350', '450', '550', '650', '750', '850', '1000']

  ys = {
    'all flow precision': [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    'all flow recall': [0.0181178, 0.0382486, 0.217413, 0.511324, 0.817313, 0.964771, 0.696527, 0.778057, 1, 1]
  }
  drawLineGraph(x, ys, "method 1, 1987 flows, k=10")

  # ys = {
  #   'all flow precision': [1, 1, 0.907, 0.834, 0.880, 0.922, 0.889, 0.898, 0.761, 0.761],
  #   'all flow recall': [0.017, 0.025, 0.096, 0.622, 0.580, 0.410, 0.493, 0.570, 0.758, 0.758]
  # }
  # drawLineGraph(x, ys, "method 2, 405 flows")


def test8k3():
  x = ['50', '60', '70', '80', '90', '100', '110', '120', '130', '140']

  ys = {
    'number of large flows': [1, 4, 5, 9, 11, 15, 17, 25, 29, 31]
  }
  drawLineGraph(x, ys, "298 flows in total")


def draw3Dgraph(filename):
  x = np.array([2, 3, 4, 5, 6, 7, 8, 9, 10])
  y = np.array([10, 30, 50, 70, 90, 110, 130, 150, 170, 200])
  x, y = np.meshgrid(x, y)
  x = x.transpose()
  y = y.transpose()
  
  z = []
  f = open(filename, 'r')
  for line in f.readlines():
    if line.startswith("total flow recall:"):
      z.append(float(line.split()[-1]))
  z = np.array(z).reshape(9, 10)

  # print(x.shape)
  # print(y.shape)
  # print(z.shape)

  fig = plt.figure("recall of test2, 405 flows")
  ax = plt.axes(projection='3d')
  ax.set_xlabel("r")
  ax.set_ylabel("c")
  ax.plot_surface(x, y, z, cmap='coolwarm')

  plt.show()



def drawHistGraph(file_name):
  f = open(file_name, 'r')
  d = []
  for line in f.readlines():
    d.append(int(line.split()[-1]))
  plt.hist(x=d, bins=50)
  plt.grid(axis='y', alpha=0.75)
  plt.show()



if __name__=='__main__':
  # drawHistGraph("./data/testp1/testp20.prov")
  drawHistGraph("./data/trust_30/trustp_30_20.prov")
  # test8k3()
