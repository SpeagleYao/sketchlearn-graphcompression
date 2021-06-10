import pandas as pd
import numpy as np
import argparse

def parse():
  parser = argparse.ArgumentParser()
  parser.add_argument('-n', type=int, default=20)
  parser.add_argument('-d', type=int, default=2)
  parser.add_argument('-w', type=int, default=5)
  return parser.parse_args()

def sample(data, sampled, iters, refer, num):
  tmp = pd.DataFrame(columns=data.columns)
  if iters==0:
    return tmp
  if refer<0:
    l = np.array(data.index)
    np.random.shuffle(l)
    for i in range(num):
      if not l[i] in sampled.index:
        tmp = tmp.append(data.loc[l[i]:l[i]])
        r = np.random.choice(tmp[['node1', 'node2']].values.reshape(-1))
        tmp = tmp.append(sample(data, tmp, iters-1, r, num))
  else:
    key = np.random.choice(['node1', 'node2'])
    ndata = data[data[key]==refer]
    l = np.array(ndata.index)
    np.random.shuffle(l)
    for i in range(min(num, len(l))):
      if not l[i] in tmp.index and not l[i] in sampled.index:
        tmp = tmp.append(ndata.loc[l[i]:l[i]+1])
        r = np.random.choice(sampled[['node1', 'node2']].values.reshape(-1))
        tmp = tmp.append(sample(data, tmp, iters-1, r, num))
  return tmp


if __name__ == '__main__':
  args = parse()
  data = pd.read_csv('./data/trust/bitcoinotc-prob.csv', sep=',')
  data = data[data['node1']<args.n][data['node2']<args.n]
  print data.shape
  sampled = pd.DataFrame(columns=data.columns)
  sampled = sample(data, sampled, args.d, -1, args.w)
  print sampled
  sampled.to_csv('./data/trust/sample_'+np.str(args.n)+'.csv', encoding='gbk', index=False)
