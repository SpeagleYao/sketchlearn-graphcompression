import numpy as np;
import matplotlib.pyplot as plt
import pygraphviz as pgv
import argparse
import random
import copy
from networkx.generators.random_graphs import erdos_renyi_graph


def setNode(G, node_name):
  G.add_node(node_name)
  n = G.get_node(node_name)
  n.attr['shape'] = 'oval'
  n.attr['color'] = 'blue'
  n.attr['fontsize'] = 15
  n.attr['height'] = 1.2
  n.attr['width'] = 1.2
  n.attr['fontname'] = 'sans-serif'
  n.attr['margin'] = 0.1


def drawGraph(g, save_name):
  G = pgv.AGraph(strict=True, directed=False)
  G.graph_attr['rankdir'] = 'LR'
  G.graph_attr['splines'] = "true"
  G.graph_attr['overlap'] = "false"
  for node in g.nodes:
    setNode(G, node+1)
  for a, b in g.edges:
    G.add_edge(a+1, b+1)
  G.layout()
  G.draw(save_name)


def buildRouteTable(g):
  nodes = g.nodes
  edges = g.edges
  adjacentTable = {ele:[] for ele in nodes}
  for a, b in edges:
    adjacentTable[a].append(b)
    adjacentTable[b].append(a)
  # print(adjacentTable)
  routes = {ele:{} for ele in nodes}
  for a, b in edges:
    routes[a][b] = [b, 1]
    routes[b][a] = [a, 1]
  while True:
    changed = False
    tmp = {}
    for k in routes:
      tmp[k] = routes[k]
    for node in nodes:
      for neightbor in adjacentTable[node]:
        for dest in routes[neightbor]:
          if dest!=node:
            if (not dest in routes[node]) or (1+routes[neightbor][dest][1]<routes[node][dest][1]):
              tmp[node][dest] = [neightbor, 1+routes[neightbor][dest][1]]
              changed = True
    if not changed:
      break
    routes = tmp
  return routes


def generatePackets(g, n):
  res = []
  nodes = g.nodes
  nodes = list(nodes)
  for i in range(n):
    random.shuffle(nodes)
    time = 0
    if random.random()<0.005:
      time = random.randint(900, 1000)
      print(time)
    else:
      time = random.randint(2, 4)
    for j in range(time):
      res.append([nodes[0], nodes[1], "word"])
  return res



if __name__=='__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument("-si", type=str, default="./data/images/test.png")
  parser.add_argument("-sf", type=str, default="./data/packet-forward/test1.db")
  parser.add_argument("-n", type=int, default=6)
  parser.add_argument("-p", type=float, default=0.5)
  args = parser.parse_args()

  f = open(args.sf, 'w')

  g = erdos_renyi_graph(args.n, args.p);
  # print(g.nodes);
  # print(g.edges);
  drawGraph(g, args.si)

  routes = buildRouteTable(g)
  for node in routes:
    for dest in routes[node]:
      f.write("route "+str(node+1)+" "+str(dest+1)+" "+str(routes[node][dest][0]+1)+"\n")

  packets = generatePackets(g, 200)
  for src, dst, word in packets:
    f.write("packet "+str(src+1)+" "+str(src+1)+" "+str(dst+1)+" "+word+"\n")

  f.close()
  