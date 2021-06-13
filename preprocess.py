import pandas as pd
import numpy as np
import argparse

# def parse():
#   parser = argparse.ArgumentParser()
#   parser.add_argument('-n', type=int, default=20)
#   parser.add_argument('-d', type=int, default=2)
#   parser.add_argument('-w', type=int, default=5)
#   return parser.parse_args()
parser = argparse.ArgumentParser()
parser.add_argument('-s', type=int, default=20)
#   parser.add_argument('-d', type=int, default=2)
#   parser.add_argument('-w', type=int, default=5)
args = parser.parse_args()


def sample(data, sampled, width, size, refer):
  '''
  data: original pd frame data
  sampled: sampled pd frame data
  width: max width to search during dfs
  size: total number of tuples to sample
  refer: start node
  '''
  # tmp = pd.DataFrame(columns=data.columns)
  if sampled.shape[0]>=size:
    return sampled
  left = data[data['node1']==refer]
  right = data[data['node2']==refer]
  whole = left.append(right)
  row = whole.shape[0]
  indice = np.array(whole.index)
  np.random.shuffle(indice)
  i = 0
  count = 0
  while count<width and i<row:
    # if indice[i] not in sampled.index and whole.loc[indice[i],'node1']+whole.loc[indice[i],'node2']<40:
    if indice[i] not in sampled.index:
      sampled = sampled.append(whole.loc[indice[i]])
      count += 1
      if sampled.shape[0]==size:
        return sampled
    i += 1
  r = np.random.choice(sampled[['node1', 'node2']].values.reshape(-1))
  return sample(data, sampled, width, size, r)


def preprocess(sampled):
  print(sampled)
  row = sampled.shape[0]
  for i in range(row):
    if (sampled.iloc[i, 2]<0.8):
      sampled.iloc[i, 2] = 0.1
  sampled[['node1', 'node2']].astype(int).round()
  return sampled


if __name__=='__main__':
  rawfile = './data/trust-new/bitcoinotc-prob.csv'
  savefile = './data/new_samples/sample_'+str(args.s)+'_new.csv'
  rawdata = pd.read_csv(rawfile, sep=',')
  rawdata = rawdata[rawdata['node1']<30][rawdata['node2']<30]
  # print(rawdata.shape[0])
  # print(rawdata)
  sampled = pd.DataFrame(columns=rawdata.columns)
  sampled = sample(rawdata, sampled, 5, args.s, 1)
  # sampled = preprocess(sampled)
  print(sampled)
  sampled.to_csv(savefile, encoding='gbk', index=False)


  # readfile = "./data/trust-raw/sample_100.csv"
  # writefile = "./data/trust-raw/sample_100_new.csv"
  # df = pd.read_csv(readfile)
  # row = df.shape[0]
  # print(row)
  # for i in range(row):
  #   if (df.iloc[i, 2]<=0.8):
  #     df.iloc[i, 2] = 0.1
  # df.to_csv(writefile, encoding='gbk', index=False)