
import argparse


if __name__=='__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument("-f", type=str, default="./data/test2/test.txt")
  parser.add_argument("-s", type=str, default="./data/test2/test.prov")
  args = parser.parse_args()

  f = open(args.f, 'r')
  s = open(args.s, 'w')

  count = 0
  for line in f.readlines():
    splits = line.strip().split(' ')
    tuplename = splits[2].split('=')[-1]
    if tuplename=='recv':
      count += 1
    elif tuplename=='edge':
      v1 = splits[4].split('=')[-1]
      v2 = splits[5].split('=')[-1]
      if len(v1)!=40:
        print(v1, len(v1))
      for i in range(40-len(v1)):
        v1 = v1+'0'
      for i in range(40-len(v2)):
        v2 = v2+'0'
      # print(len(v1), len(v2))
      freq = splits[6].split('=')[-1]
      s.write(v1+v2+" "+str(freq)+"\n")
  # print(count)

  f.close()
  s.close()