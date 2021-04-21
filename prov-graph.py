import re
import pygraphviz as pgv


def trustR2(s1, s2):
  s1 = s1[9:] if s1.startswith('trustPath') else s1[5:]
  s2 = s2[9:] if s2.startswith('trustPath') else s2[5:]
  s1 = s1.split(':')[0]
  s2 = s2.split(':')[0]
  a1, a2 = s1.split('-')
  a3, a4 = s2.split('-')
  if a2==a3:
    return 'trustPath'+a1+'-'+a4
  else:
    return 'trustPath'+a3+'-'+a2


knowsDic = {'r2': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r1': lambda s1,s2 : 'know'+s1[4]+s2[4],
           'r13': lambda s1,s2 : 'know'+s1[4]+s2[5] if s1[5]==s2[4] else 'know'+s2[4]+s1[5],
           'rd': lambda s : 'know'+s[8]+s[9],
           'r0': lambda s : s[:-1]}

trustDic = {'ra': lambda s: 'trustEvent'+s[9:].split(':')[0],
            'r1': lambda s : 'trustPath'+s[5:].split(':')[0],
            'r2': trustR2,
            'r3': lambda s1, s2 : s2.replace('trustPath', 'mutualTrustPath').split(':')[0],
            'r0': lambda s: s.split(':')[0]}

nameDic = {0: 'ben', 1: 'Elena', 2: 'Dhanya', 3: 'Alex', 4:'Arti',
           5: 'Sabina', 6: 'Jay', 7: 'Steve'}


def argsNum(func):
  return len(func.func_code.co_varnames)

def parseFile(file_name):
  with open(file_name, 'r') as f:
    s = f.readline().strip()
    s = deleteRule(s, 'ra@n257')
    # s = preprocess(s)
    # s = deleteRule(s, 'r0@n257')
    d = parse(s)
    return d


def preprocess(s):
    s = deleteRule(s, 'ra@n257')
    s = deleteRule(s, 'rb@n257')
    s = deleteRule(s, 'rc@n257')
    s = deleteRule(s, 'rd@n257') 
    s = s.replace('knowEvent', 'know')
    s = s.replace('liveEvent', 'live')
    s = s.replace('likeEvent', 'like')
    s = s.replace('relation', 'know')
    return s

def firstLastMatch(s):
  count = 0
  for i in range(len(s)):
    if s[i]=='(':
      count += 1
    elif s[i]==')':
      count -= 1
      if count==0 and i<len(s)-1:
        return False
  return True

def checkParentheses(s):
  count = 0
  for c in s:
    if c=='(':
      count += 1
    elif c==')':
      count -= 1
  if count == 0:
    return True 
  else:
    return False 

def deleteParentheses(s):
  while s.startswith('(') and firstLastMatch(s):
    s = s[1:-1]
  return s


def parse(s):
  if s.startswith('(') and firstLastMatch(s):
    return parse(s[1:-1])

  rl, cl = splitRule(s)
  if '*' in cl:
    l = []
    for rule in rl:
      rule = deleteParentheses(rule)
      if not re.match(r'r.{1,2}@n257\(.+\)$', rule) and len(splitRule(rule)[0])==1:
        if rule=='1':
          return []
        l.append(rule)
      else:
        l.append(parse(rule))
    return l

  else:
    dic = {}
    for rule in rl:
      rule = deleteParentheses(rule)
      rule_name, rule_body = ruleParse(rule)
      while rule_name in dic:
        rule_name += '_1'
      if not rule_name.startswith('r0'):
        dic[rule_name] = parse(rule_body)
      else:
        if len(splitRule(rule_body)[0])==1:
          dic[rule_name] = rule_body
        else:
          dic[rule_name] = parse(rule_body)
    return dic


def splitRule(s):
  rl = []
  cl = []
  i = 0
  pre = 0
  count = 0
  while i<len(s):
    if s[i]=='*' or s[i]=='+':
      if count==0:
        cl.append(s[i])
        rl.append(s[pre:i])
        pre = i+1
    if s[i]=='(':
      count += 1
    elif s[i]==')':
      count -= 1
    i += 1
  rl.append(s[pre:i])
  return rl, cl

def ruleParse(rule):
  if re.match(r'r.{1,2}@n257\(.+\)$', rule):
    rule_name = rule.split('@')[0]
    pattern = re.search('r.{1,2}@n257\((.+)\)', rule)
    rule_body = pattern.group(1)
  else:
    rule_name = 'r0'
    rule_body = rule
  return rule_name, rule_body


def setRuleNode(G, node_name):
  G.add_node(node_name)
  n = G.get_node(node_name)
  n.attr['shape'] = 'oval'
  n.attr['color'] = 'red'
  n.attr['fontsize'] = 10
  n.attr['fontname'] = 'sans-serif'
  n.attr['margin'] = 0.1


def setTupleNode(G, node_name):
  G.add_node(node_name)
  n = G.get_node(node_name)
  n.attr['shape'] = 'box'
  n.attr['fontsize'] = 10
  n.attr['fontname'] = 'sans-serif'
  n.attr['height'] = 0.3
  n.attr['width'] = 0.4
  n.attr['color'] = 'blue'


def giveNodeName(G, node_name, name_dic):
  if node_name not in G.nodes():
    name_dic[node_name] = 1
    return node_name
  else:
    name_dic[node_name] += 1
    return node_name+':'+str(name_dic[node_name]-1)

def drawGraphWithObj(G, obj, name_dic, func_dic):

  l = []
  if isinstance(obj, dict):
    query_name = None
    for key in obj.keys():
      rule = key.split('_')[0]
      # in case of r0
      if rule=='r0':
        node_name = giveNodeName(G, func_dic[rule](obj[key]), name_dic)
        setTupleNode(G, node_name)
        l.append(node_name)
        return l
      node_name = giveNodeName(G, rule, name_dic)
      setRuleNode(G, node_name)
      children = drawGraphWithObj(G, obj[key], name_dic, func_dic)
      for child in children:
        if child:
          G.add_edge(child, node_name)
      try:
        query_name = func_dic[rule](*(children[i] for i in range(func_dic[rule].__code__.co_argcount)))
      except:
        # print(rule, node_name)
        pass
      l.append(node_name)
    if query_name:
      node_name = giveNodeName(G, query_name, name_dic)
      setTupleNode(G, node_name)
      for rule in l:
        try:
          G.add_edge(rule, node_name)
        except:
          # print(rule, node_name)
          pass
      return [node_name]

  elif isinstance(obj, list):
    for o in obj:
      children = drawGraphWithObj(G, o, name_dic, func_dic)
      for child in children:
        l.append(child)

  else:
    if obj[:4]=='know':
      obj = obj[:-1]
    node_name = giveNodeName(G, obj, name_dic)
    setTupleNode(G, node_name)
    l.append(node_name)

  return l


def drawGraph(file_name, save_name, func_dic):
  d = parseFile(file_name)
  # print d
  d = prune(d)
  # print d
  # print toString(d)
  name_dic = {}
  G = pgv.AGraph(strict=True, directed=True)
  G.graph_attr['rankdir'] = 'BT'
  G.graph_attr['margin'] = 0.1
  G.graph_attr['mclimit'] = 0.1
  G.graph_attr['nodesep'] = 0.1
  drawGraphWithObj(G, d, name_dic, func_dic)
  G.layout()
  G.draw(save_name, prog='dot')


def prune(obj):
  if isinstance(obj, dict):
    d = {}
    for key in obj.keys():
      if obj[key]!=[]:
        p = prune(obj[key])
        if isinstance(p, list) and len(p)!=len(obj[key]):
          continue
        elif isinstance(p, dict) and len(p)==0:
          continue
        d[key] = p
    return d
  elif isinstance(obj, list):
    l = []
    for ele in obj:
      ele = prune(ele)
      if isinstance(ele, dict) and len(ele)==0:
        continue
      l.append(ele)
    return l
  else:
    return obj


def toString(obj):
  if isinstance(obj, dict):
    s = ''
    for key in obj.keys():
      s += '('+key.split('_')[0]+'@n257'+toString(obj[key])+')'+'+'
    if len(obj)!=1:
      return '('+s[:-1]+')'
    else:
      return s[:-1]
  elif isinstance(obj, list):
    for ele in obj:
      s = '('+toString(obj[0])+'*'+toString(obj[1])+')'
    return s
  else:
    return '('+obj+')'
    

def deleteRule(s, rule):
  n = len(rule)
  ans = ''
  i = 0
  pre = 0
  while i<len(s)-n:
    if s[i:i+n]==rule:
      ans += s[pre:i]
      leftIndex = i+n
      rightIndex = leftIndex
      count = 0
      for j in range(leftIndex, len(s)):
        if s[j]=='(':
          count += 1
        elif s[j]==')':
          count -= 1
          if count==0:
            rightIndex = j
            break
      tmp = deleteRule(s[leftIndex+1:rightIndex], rule)
      ans += tmp
      pre = rightIndex+1
      i = rightIndex
    i += 1
  ans += s[pre:len(s)]
  return ans


def checkRedundancy(s):
  if s[0]=='(' and s[1]=='(' and \
    s[-1]==')' and s[-2]==')' and \
    firstLastMatch(s[1:-1]):
    return True
  return False

if __name__ == '__main__':
  name = "mutualTrustPath1-2_10_new_100_prune"
  drawGraph('./data/trust/'+name+'.txt', './data/trust/'+name+'.png', trustDic)
  '''
  G = pgv.AGraph(strict=True, directed=True)
  G.add_node(1)
  G.add_edge(2, 3)
  print G.string()
  G.layout()
  G.draw('data/plot/test.png')
  '''
