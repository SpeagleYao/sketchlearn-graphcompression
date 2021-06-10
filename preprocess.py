import pandas as pd
import numpy as np

def sample(data, sampled, width, n, size, refer):
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
    if indice[i] not in sampled.index and whole.loc[indice[i],'node1']+whole.loc[indice[i],'node2']<40:
      sampled = sampled.append(whole.loc[indice[i]])
      n += 1
      count += 1
      if n==size:
        return sampled
    i += 1
  r = np.random.choice(sampled[['node1', 'node2']].values.reshape(-1))
  return sample(data, sampled, width, n, size, r)


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
  savefile = './data/trust-new/sample_20_new.csv'
  rawdata = pd.read_csv(rawfile, sep=',')
  # print(rawdata)
  sampled = pd.DataFrame(columns=rawdata.columns)
  sampled = sample(rawdata, sampled, 3, 0, 20, 1)
  sampled = preprocess(sampled)
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