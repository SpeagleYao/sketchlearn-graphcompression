import argparse
import random


intToHexTable = {
  0:'0', 1:'1', 2:'2', 3:'3', 4:'4', 5:'5', 6:'6', 7:'7',
  8:'8', 9:'9', 10:'a', 11:'b', 12:'c', 13:'d', 14:'e', 15:'f'
}


def generateRandomNode():
  res = ""
  n = random.randint(0, 20)
  n = hex(n)[2:]
  if len(n)==1:
    n = '0'+n
  res += n
  for i in range(40):
    res += intToHexTable[random.randint(0, 15)]
  return res


def generateRandomFreq():
  if random.random()<0.4:
    return random.randint(100, 150)
  else:
    return random.randint(1, 3)


def generateRandomEdge_v2():
  res = {}
  n1 = generateRandomNode()
  n = random.randint(1, 5)
  for i in range(n):
    n2 = generateRandomNode()
    freq = generateRandomFreq()
    res[n1+n2] = freq
  return res


def generateRandomEdge():
  n1 = generateRandomNode()
  n2 = generateRandomNode()
  freq = generateRandomFreq()
  return n1+n2, freq



if __name__=='__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument("-s", type=str)
  parser.add_argument("-n", type=int)
  args = parser.parse_args()

  f = open(args.s, 'w')

  for i in range(args.n):
    # k, v = generateRandomEdge()
    d = generateRandomEdge_v2()
    for k in d:
      f.write(k+' '+str(d[k])+'\n')

  f.close()





