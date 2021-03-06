import numpy as np;
import matplotlib.pyplot as plt
import pygraphviz as pgv
import argparse
import random
import copy
import os
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
    if random.random()<0.5:
      time = random.randint(40, 50)
      print(time)
      # pass
    else:
      time = random.randint(1, 3)
    for j in range(time):
      res.append([nodes[0], nodes[1], "word"])
  return res



def generateProbPackets(g, n):
  res = []
  nodes = g.nodes
  nodes = list(nodes)
  l = []
  for i in range(n):
    random.shuffle(nodes)
    key = str(nodes[0])+'_'+str(nodes[1])
    if key in l:
      continue
    else:
      l.append(key)
    if random.random()<0.9:
      res.append([nodes[0], nodes[1], "word", str(random.random()/10)])
    else:
      res.append([nodes[0], nodes[1], "word", str(random.random()/10+0.9)])
  return res


if __name__=='__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument("-si", type=str, default="./data/images/test.png")
  parser.add_argument("-sf", type=str, default="./data/packet-forward/test1.db")
  parser.add_argument("-s", type=str)
  parser.add_argument("-n", type=int, default=6)
  parser.add_argument("-p", type=float, default=0.5)
  parser.add_argument("-pr", type=int, default=0)
  args = parser.parse_args()

  if not os.path.exists(args.s):
    os.mkdir(args.s)
  si = args.s+"/test.png"
  sf = args.s+"/test.db"
  
  f = open(sf, 'w')

  g = erdos_renyi_graph(args.n, args.p);
  # print(g.nodes);
  # print(g.edges);
  drawGraph(g, si)

  routes = buildRouteTable(g)
  for node in routes:
    for dest in routes[node]:
      if args.pr==0:
        f.write("route "+str(node+1)+" "+str(dest+1)+" "+str(routes[node][dest][0]+1)+"\n")
      else:
        p = random.random

  if args.pr==0:
    packets = generatePackets(g, 200)
    for src, dst, word in packets:
      f.write("packet "+str(src+1)+" "+str(src+1)+" "+str(dst+1)+" "+word+"\n")
  else:
    packets = generateProbPackets(g, 200)
    for src, dst, word, prob in packets:
      f.write("packet "+str(src+1)+" "+str(src+1)+" "+str(dst+1)+" "+word+" "+prob+"\n")

  f.close()
  