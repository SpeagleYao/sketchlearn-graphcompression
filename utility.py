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



def drawScatterGraphs(file_name):
  f = open(file_name, 'r')
  ratios = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

  c_ratios = []
  suff_times = []
  sketch_times = []
  deltas = []
  terms = []

  for line in f.readlines():
    splits = line.strip().split(' ');
    if len(splits)==1 and splits[0]!='':
      c_ratios.append(float(splits[0]))
    elif line.startswith("Sufficient lineage time"):
      suff_times.append(float(splits[-1]))
    elif line.startswith("sketch extraction time"):
      sketch_times.append(float(splits[-1]))
    elif line.startswith("delta"):
      deltas.append(float(splits[-1]))
    elif line.startswith("ratio of pruned terms"):
      terms.append(float(splits[-1]))

  # print(c_ratios)
  # print(suff_times)

  fig, ax = plt.subplots()
  ax.scatter(c_ratios, suff_times, marker='o', color='blue', label="suff time", alpha=0.5)
  ax.scatter(c_ratios, sketch_times, marker='^', color='red', label="sketch time", alpha=0.5)
  ax.set_xlabel("compression ratio")
  ax.set_ylabel("time")
  ax.legend()
  plt.show()

  fig, ax = plt.subplots()
  ax.scatter(c_ratios, deltas, marker='^', color='red', alpha=0.5)
  ax.set_xlabel("compression ratio")
  ax.set_ylabel("deltas")
  plt.show()

  fig, ax = plt.subplots()
  ax.scatter(c_ratios, terms, marker='^', color='red', alpha=0.5)
  ax.set_xlabel("compression ratio")
  ax.set_ylabel("ratio of pruned literals")
  plt.show()


def drawMainBarGraph():
  samples = ["10", "20", "30", "40", "50"]
  orig_times = [0.256, 0.547, 0.857, 1.571, 1.75]
  sketch_times_1 = [0.225413, 0.17922, 0.952791, 3.01084, 3.49483]
  sketch_times_10 = [2.31669, 2.55956, 8.30721, 50.1642, 58.2097]
  sketch_times_100 = [24.9, 130.6, 176.76, 827.165, 931.186]
  size_of_prov_1 = [30, 21, 218, 657, 735]
  size_of_prov_10 = [183, 212, 473, 1539, 1727]
  size_of_prov_100 = [208, 552, 780, 1803, 2083]
  
  plt.figure()
  x = range(5)
  bar1 = plt.bar(left = [i - 0.3 for i in x], height = orig_times, width = 0.19,
               alpha = 0.8, color = 'r',label = 'baseline maintenance time')
  bar2 = plt.bar(left = [i-0.1 for i in x], height=sketch_times_1, width=0.19, alpha=0.5, color='b', label='1 round')
  bar3 = plt.bar([i+0.1 for i in x], sketch_times_10, width = 0.19,
               alpha = 0.7, color = 'b', label = '10 rounds') 
  bar4 = plt.bar([i+0.3 for i in x], height=sketch_times_100, width=0.19,
                alpha=0.9, color='b', label='100 rounds')
  plt.xticks(x,samples)
  plt.title('maintenance time')     
  plt.xlabel('sample size')       
  plt.ylabel('time')       
  plt.yscale('log')
  plt.legend()
  plt.show()

  plt.figure()
  bar1 = plt.bar(left=[i-0.2 for i in x], height=size_of_prov_1, width=0.19, alpha=0.5, color='b',
                label='1 rounds')
  bar1 = plt.bar(left=[i for i in x], height=size_of_prov_10, width=0.19, alpha=0.7, color='b',
                label='10 rounds')
  bar2 = plt.bar(left=[i+0.2 for i in x], height=size_of_prov_100, width=0.19, alpha=0.9, color='b',
                label='100 rounds')
  plt.xticks(x,samples)
  plt.title('provenance table size')     
  plt.xlabel('sample size')       
  plt.ylabel('size of prov table')       
  plt.yscale('log')
  plt.legend()
  plt.show()


def drawQueryBarGraph():
  unique_terms = ["15", "17", "31", "39"]
  orig_times = [1.723, 2.563, 42.41, 225]
  sketch_times = [0.01, 0.01, 0.18, 0.49]
  x = [0, 1, 5, 7]
  bar1 = plt.bar(left = [i - 0.2 for i in x], height = orig_times, width = 0.4,
               alpha = 0.8, color = 'r',label = 'baseline query time')
  bar2 = plt.bar([i + 0.2 for i in x], sketch_times, width = 0.4,
               alpha = 0.8, color = 'b', label = 'sketch query time') 

  plt.xticks(x,unique_terms)
  plt.title('provenance query time')     
  plt.xlabel('number of unique terms')       
  plt.ylabel('time')       
  plt.yscale('log')
  plt.legend()
  plt.show()


def drawPruneBarGraph():
  unique_terms = ["15", "17", "31", "39"]
  orig_times = [0.85, 2.19, 6.88, 114.7]
  sketch_times = [1.06, 1.55, 3.9, 4.46]
  x = [0, 1, 5, 7]
  bar1 = plt.bar(left = [i - 0.2 for i in x], height = orig_times, width = 0.4,
               alpha = 0.8, color = 'r',label = 'sufficient lieage')
  bar2 = plt.bar([i + 0.2 for i in x], sketch_times, width = 0.4,
               alpha = 0.8, color = 'b', label = 'sketch extraction (comp ratio=0)') 

  plt.xticks(x,unique_terms)
  plt.title('provenance prune time')     
  plt.xlabel('number of unique terms')       
  plt.ylabel('time')       
  plt.yscale('log')
  plt.legend()
  plt.show()


def drawTotalTermBarGraph():
  unique_terms = ["15", "17", "31", "39"]
  total_terms = [128, 139, 2978, 61958]
  sketch_times = [1.06, 1.55, 3.9, 4.46]
  x = [0, 1, 5, 7]
  bar = plt.bar(left=x, height=total_terms, width=0.4, alpha=0.8, color='g')
  plt.xticks(x,unique_terms)
  plt.title('number of total terms after converting to DNF')     
  plt.xlabel('number of unique terms')       
  plt.ylabel('number of total terms')       
  plt.yscale('log')
  plt.legend()
  plt.show()


def drawPruneBarGraphWithCompRatio():
  comp_ratios = ["suff", "0.0", "0.1", "0.2", "0.3", "0.4", "0.5"]
  times = [18.5, 3.51946, 3.61, 3.59, 3.66, 3.58843, 3.73995]
  x = range(7)
  bar = plt.bar(left=x, height=times, width=0.4, alpha=0.8, color='g')
  plt.xticks(x,comp_ratios)
  plt.title('prune time with different comp ratio')     
  plt.xlabel('comp ratio = (original_storage-sketch_size)/original_storage')       
  plt.ylabel('time') 
  plt.legend()
  plt.show()


def drawPruneTermsBarGraphWithCompRatio():
  comp_ratios = ["suff", "0.0", "0.1", "0.2", "0.3", "0.4", "0.5"]
  times = [0.956, 0, 0, 0, 0, 0.34, 0.951]
  deltas = [0.004, 0, 0, 0, 0, 0.0418, 0.1]
  x = range(7)
  bar = plt.bar(left=x, height=times, width=0.4, alpha=0.8, color='g')
  plt.xticks(x,comp_ratios)
  plt.title('ratio of pruned terms with comp ratio in average')     
  plt.xlabel('comp ratio')       
  plt.ylabel('ratio of pruned terms') 
  plt.legend()
  plt.show()

  bar = plt.bar(left=x, height=deltas, width=0.4, alpha=0.8, color='g')
  plt.xticks(x,comp_ratios)
  plt.title('delta with comp ratio in average')     
  plt.xlabel('comp ratio')       
  plt.ylabel('delta') 
  plt.legend()
  plt.show()


def drawScatterGraphs():
  pruned_terms_suff = [0.988, 0.95, 0.989807, 0.991113, 0.978829, 0.733926,  0.986932, 0.991113, 0.9885, 0.977261, 0.943544]
  delta_suff = [0.0004, 0.0009, 0.0096, 0.0044, 0.0053, 0.0086,  0.0015, 0.0041, 0.002, 0.004, 0.0075]
  pruned_terms_sketch = [0.99817, 0.99242, 0.995, 0.944851, 0.967852, 0.980397, 0.998693, 0.858599, 0.856247, 0.94, 0.925248]
  delta_sketch = [0.0628, 0.098, 0.08, 0.0126, 0.6554, 0.1017, 0.0637, 0.0013, 0.0159, 0.006, 0.072]

  print(np.mean(np.array(pruned_terms_suff)))
  print(np.mean(np.array(delta_suff)))
  print(np.mean(np.array(pruned_terms_sketch)))
  print(np.mean(np.array(delta_sketch)))

  fig, ax = plt.subplots()
  ax.scatter(pruned_terms_suff, delta_suff, marker='o', color='red', label="suff", alpha=0.5)
  ax.scatter(pruned_terms_sketch, delta_sketch, marker='^', color='blue', label="sketch", alpha=0.5)
  ax.set_title("performance of suff and sketch to prune (comp ratio=0.6)")
  ax.set_xlabel("ratio of pruned terms")
  ax.set_ylabel("delta")
  ax.legend()
  plt.show()


if __name__=='__main__':
  # drawHistGraph("./data/testp1/testp20.prov")
  # drawHistGraph("./data/trust_30/trustp_30_20.prov")
  # test8k3()
  # drawScatterGraphs("./data/sample10_new_10rounds/sample10_new_10rounds_mutualtrust1-2.log")
  drawMainBarGraph()
  # drawQueryBarGraph()
  # drawPruneBarGraph()
  # drawTotalTermBarGraph()
  # drawPruneBarGraphWithCompRatio()
  # drawScatterGraphs()
  # drawPruneTermsBarGraphWithCompRatio()