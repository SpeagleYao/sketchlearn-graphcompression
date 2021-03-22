import matplotlib.pyplot as plt

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


if __name__=='__main__':
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