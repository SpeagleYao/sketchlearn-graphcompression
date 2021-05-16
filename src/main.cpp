
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <math.h>
#include <random>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <boost/algorithm/string.hpp>
#include "multi-level-sketch.h"
#include "Suff.h"
#include "DNF.h"

using namespace std;


int readFromFile(string& filepath, unordered_map<string, int>& freqs) {
  cout << "reading from file " << filepath << endl;
  ifstream fp(filepath);
  string line;
  int count = 0;

  while (getline(fp, line)) {
    if (!line.empty()) {
      vector<string> splits;
      boost::split(splits, line, boost::is_any_of(" "));
      if (freqs.find(splits[0])==freqs.end()) {
        freqs[splits[0]] = stoi(splits[1]);
        count++;
      }
      else {
        cout << "duplicate edges!" << endl;
        cout << splits[0] << ' ' << splits[1] <<endl;
      }
    }
  }
  return count;
}


int readFromFile_v2(string& filepath, 
                    unordered_map<string, int>& provFreqs,
                    unordered_map<string, int>& ruleFreqs,
                    unordered_map<string, string>& idToContent, 
                    unordered_map<string, string>& contentToId) {
  cout << "reading from file " << filepath << endl;
  ifstream fp(filepath);
  string line;
  int count = 0;

  while (getline(fp, line)) {
    if (!line.empty()) {
      count++;
      vector<string> splits;
      boost::split(splits, line, boost::is_any_of(" "));
      string tablename = splits[0];
      string flowkey = splits[1];
      string id1 = flowkey.substr(0, 40);
      idToContent[id1] = splits[2];
      contentToId[splits[2]] = id1;
      string id2 = flowkey.substr(40, 80);
      idToContent[id2] = splits[3];
      contentToId[splits[3]] = id2;
      if (tablename=="provEdge") {
        provFreqs[flowkey] = stoi(splits[4]);
      }
      else {
        ruleFreqs[flowkey] = stoi(splits[4]);
      }
    }
  }
  return count;
}


void printVector(vector<int> v) {
  cout << '[';
  for (auto it=v.begin(); it!=v.end(); it++) {
    cout << *it << ' ';
  }
  cout << ']' << endl;
}


void print2DVector(vector<vector<int>> v) {
  cout << '[';
  for (int i=0; i<v.size(); i++) {
    printVector(v[i]);
  }
  cout << ']' << endl;
}


bool isEDB(string& toQuery) {
  vector<string> splits;
  boost::split(splits, toQuery, boost::is_any_of("-123456789"));
  if (splits[0]=="trust") {
    return true;
  }
  return false;
}


bool isVisited(string& toQuery, vector<string>& visited) {
  for (int i=0; i<visited.size(); i++) {
    if (visited[i]==toQuery) {
      return true;
    }
  }
  return false;
}


string extractRuleName(string& RC) {
  vector<string> splits;
  boost::split(splits, RC, boost::is_any_of("_"));
  return splits[0];
}


string generateRandomHexString(int length) {
  string s;
  for (int i=0; i<length; i++) {
    int t = rand()%16;
    s += hexTable[t];
  }
  return s;
}


map<string, double> setProbs(string sd) {
  // a map to store literal's name and probability
  map<string, double> p;

  //read trust data from files
  ifstream fin(sd);
  string line;
  int i = 0;
  while (getline(fin, line))
  {
    istringstream sin(line);
    vector<string> fields;
    string field;
    while (getline(sin, field, ','))
    {
        fields.push_back(field);
    }
    if (fields[0] == "node1")
    {
        continue;
    }
    i++;
    string name;
    name.append("trust").append(fields[0]).append("-").append(fields[1]);
    double prob = stod(fields[2]);
    p[name] = prob;
    //cout << name << " " << p[name] << endl;
  }
  cout << "total number of element: " << i << endl;

  p["ra"] = 1.0;
  p["rb"] = 1.0;
  p["r0"] = 1.0;
  p["r1"] = 1.0;
  p["r2"] = 1.0;
  p["r3"] = 1.0;
  p["r4"] = 1.0;
  p["r5"] = 1.0;
  p["r6"] = 1.0;
  return p;
}


string setProv(string sp) {
    //read prov from files
    ifstream pfin(sp);
    stringstream buffer;
    buffer << pfin.rdbuf();
    string prov = buffer.str();
    prov.erase(0,prov.find_first_not_of(" \t\r\n"));
    prov.erase(prov.find_last_not_of(" \t\r\n") + 1);
    return prov;
}


string buildProvenance(string& toQuery,
                      vector<string>& visited,
                      unordered_map<string, int>& provFreqs,
                      unordered_map<string, int>& ruleFreqs,
                      unordered_map<string, string>& idToContent,
                      unordered_map<string, string>& contentToId
                      ) 
{
  if (isEDB(toQuery)||isVisited(toQuery, visited)) {
    return toQuery;
  }
  visited.push_back(toQuery);
  string res = "";
  string QID = contentToId[toQuery];
  for (auto it : provFreqs) {
    string Q = it.first.substr(0, 40);
    if (Q==QID) {
      string RID = it.first.substr(40, 80);
      string RC = idToContent[RID];
      string rName = extractRuleName(RC);
      string tmp = "";
      for (auto itt : ruleFreqs) {
        string R = itt.first.substr(0, 40);
        if (R==RID) {
          string NQID = itt.first.substr(40, 80);
          string NQC = idToContent[NQID];
          string nQProv = buildProvenance(NQC, visited, provFreqs, ruleFreqs, idToContent, contentToId);
          if (nQProv!="") {
            if (tmp!="") {
              tmp += "*";
            }
            tmp += nQProv;
          }
        }
      }
      if (tmp!="") {
        // tmp = rName+"@n257("+tmp+")";
        tmp = rName+"*("+tmp+")";
        if (res!="") {
          res += "+";
        }
        res += tmp;
      }
    }
  }
  visited.pop_back();
  if (res!="") {
    res = "("+res+")";
  }
  return res;
}


unordered_map<string, int> feedAndExtract(int R, int C, int K, int shift, unordered_map<string, int>& freqs) {
  int totalfreq = 0;
  for (auto it : freqs) {
    totalfreq += it.second;
  }
  int thre = totalfreq/C;
  cout << "total frequence: " << totalfreq << ", threshold to be large flow: " << thre << endl;
  MultiLevelSketch mls (R, C, K, shift);
  for (auto it : freqs) {
    mls.feedFlowKey(it.first, it.second);
  }
  unordered_map<string, int> lfm;
  unordered_map<string, vector<double>> blp;
  mls.modelInference(0.5, lfm, blp);
  return lfm;
}



int main(int argc, char* argv[]) {
  cout << "program start" << endl;
  unordered_map<string, string> args;
  vector<string> argvs;
  for (int i=0; i<argc; i++) {
    argvs.push_back(string(argv[i]));
  }
  for (int i=1; i<argc; i+=2) {
    assert(i+1<argc);
    // row number of prov sketch table
    if (argvs[i]=="-kp") {
      args["kp"] = argvs[i+1];
    }
    // row number of rule sketch table
    else if (argvs[i]=="-kr") {
      args["kr"] = argvs[i+1];
    }
    // column number of prov sketch table
    else if (argvs[i]=="-mp") {
      args["mp"] = argvs[i+1];
    }
    // column number of rule sketch table
    else if (argvs[i]=="-mr") {
      args["mr"] = argvs[i+1];
    }
    // file name of sketch 
    else if (argvs[i]=="-fs") {
      args["fs"] = argvs[i+1];
    }
    // file name of probabilities
    else if (argvs[i]=="-fd") {
      args["fd"] = argvs[i+1];
    }
    // file name of provenance of queried tuple
    else if (argvs[i]=="-fp") {
      args["fp"] = argvs[i+1];
    }
    else if (argvs[i]=="-p") {
      args["p"] = argvs[i+1];
    }
    else if (argvs[i]=="-q") {
      args["q"] = argvs[i+1];
    }
  }

  // pre-process of the data
  unordered_map<string, int> provFreqs;
  unordered_map<string, int> ruleFreqs;
  unordered_map<string, string> idToContent; 
  unordered_map<string, string> contentToId;
  int flownumber = readFromFile_v2(args["fs"], provFreqs, ruleFreqs, idToContent, contentToId);
  map<string, double> probs = setProbs(args["fd"]);

  int Rp, Rr, Cp, Cr;
  if (args.find("p")!=args.end()) {
    Rp = 3;
    Rr = 3;
    Cp = provFreqs.size()*stod(args["p"])/Rp;
    Cr = ruleFreqs.size()*stod(args["p"])/Rr;
  }
  else {
    Rp = stoi(args["kp"]);
    Rr = stoi(args["kr"]);
    Cp = stoi(args["mp"]);
    Cr = stoi(args["mr"]);
  }
  int K = 320;
  int shift = 0;
  double error = 0.01;

  // cout << "compression ratio: " << (Rp*Cp+Rr*Cr)*1.0/(provFreqs.size()+ruleFreqs.size()) << endl;
  cout << "prov sketch table compression ratio: " << (Rp*Cp)*1.0/(provFreqs.size()) << endl;

  vector<string> visited;
  string prov;
  if (args.find("fp")!=args.end()) {
    prov = setProv(args["fp"]);
  }
  else {
    prov = buildProvenance(args["q"], visited, provFreqs, ruleFreqs, idToContent, contentToId);
  }
  // cout << prov << endl;
  clock_t t = clock();
  DNF original(prov, probs);
  Suff suff1(original.getLambda(), error);
  cout << "Sufficient lineage time: " << (clock()-t)*1.0/CLOCKS_PER_SEC << endl;
  vector<map<string, double>> OrigDNF = suff1.getOrigDNF();
  int OrigTerms = 0;
  for (auto it : OrigDNF) {
    OrigTerms += it.size();
  }
  cout << "number of terms: " << OrigTerms << endl;
  double OrigProb = suff1.getOrigProb();
  cout << "Original Probability = " << OrigProb << endl;
  cout << endl;

  clock_t t1 = clock();
  // extract prov table first
  unordered_map<string, int> provs = feedAndExtract(Rp, Cp, K, shift, provFreqs);
  int match = 0;
  for (auto it : provFreqs) {
    if (provs.find(it.first)!=provs.end()) {
      match++;
    }
    else {
      // cout << "failed to extract: " << it.second << endl;
    }
  }
  cout << "recall of provs: " << match*1.0/provFreqs.size() << endl;

  unordered_map<string, int> rules = feedAndExtract(Rr, Cr, K, shift, ruleFreqs);
  match = 0;
  for (auto it : ruleFreqs) {
    if (rules.find(it.first)!=rules.end()) {
      match++;
    }
    else {
      // cout << "failed to extract: " << it.second << endl;
    }
  }
  cout << "recall of rules: " << match*1.0/ruleFreqs.size() << endl;
  clock_t t2 = clock();
  cout << "sketch extraction time: " << (t2-t1)*1.0/CLOCKS_PER_SEC << endl;
  cout << endl;

  string prunedProv = buildProvenance(args["q"], visited, provs, rules, idToContent, contentToId);
  // cout << prunedProv << endl;
  DNF pruned(prunedProv, probs);
  Suff suff2(pruned.getLambda(), error);
  vector<map<string, double>> PruneDNF = suff2.getOrigDNF();
  int PruneTerms = 0;
  for (auto it : PruneDNF) {
    PruneTerms += it.size();
  }
  double PruneProb = suff2.getOrigProb();
  cout << "Pruned Probability = " << PruneProb << endl;

  cout << "delta: " << abs(OrigProb-PruneProb) << endl;
  cout << "ratio of pruned terms: " << abs(PruneTerms-OrigTerms)*1.0/OrigTerms << endl;
  cout << endl;

}





// int main(int argc, char* argv[]) {

//   unordered_map<string, string> args;
//   vector<string> argvs;
//   for (int i=0; i<argc; i++) {
//     argvs.push_back(string(argv[i]));
//   }
//   for (int i=1; i<argc; i+=2) {
//     assert(i+1<argc);
//     if (argvs[i]=="-k") {
//       args["k"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-m") {
//       args["m"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-f") {
//       args["f"] = argvs[i+1];
//     }
//   }

//   // pre-process of the data
//   unordered_map<string, int> freqs;
//   unordered_map<string, int> largeflows;
//   unordered_map<string, int> n1s;
//   int flownumber = readFromFile(args["f"], freqs);
//   int totalnumber = 0;
//   for (auto it : freqs) {
//     totalnumber += it.second;
//   }
//   int thre = totalnumber/stoi(args["m"]);
   

//   // build hierarchy mls
//   int R = stoi(args["k"]);
//   // int C = stoi(args["m"]);
//   // int C = log(totalnumber);
//   int C = stoi(args["m"]);
//   cout << C << endl;
//   int K = 336;
//   MultiLevelSketch mls (R, C, K/2, 0);
//   KHashFunctions kfs = mls.getKHashFunctions();
//   vector<MultiLevelSketch> mlses;
//   for (int i=0; i<R*C; i++) {
//     MultiLevelSketch mlst (R, C, K/2, 0);
//     mlses.push_back(mlst);
//   }
//   for (auto it : freqs) {
//     string f = it.first;
//     int freq = it.second;
//     string n1 = f.substr(0, 42);
//     string n2 = f.substr(42, 84);
//     mls.feedFlowKey(n1, freq);
//     for (int r=0; r<R; r++) {
//       int c = kfs.getHashedValue(n1, r);
//       mlses[r*C+c].feedFlowKey(n2, freq);
//     }
//     // if it is large flow
//     if (freq>thre) {
//       largeflows.insert(it);
//     }
//     n1s[n1] += freq;
//   }

//   cout << "number of distinct flows: " << flownumber << endl;
//   cout << "total frequency: " << totalnumber << endl;
//   cout << "number of n1s: " << n1s.size() << endl;

//   unordered_map<string, int> lfm;
//   unordered_map<string, vector<double>> blp;
//   mls.modelInference(0.5, lfm, blp);
//   int match = 0;
//   for (auto it : n1s) {
//     if (lfm.find(it.first)!=lfm.end()) {
//       match++;
//     }
//   }
//   cout << "recall of n1s: " << 1.0*match/n1s.size() << endl;
//   cout << "precision of n1s: " << 1.0*match/(lfm.size()+1e-10) << endl;

//   match = 0;
//   vector<unordered_map<string, int>> lfms;
//   vector<unordered_map<string, vector<double>>> blps;
//   for (int r=0; r<R; r++) {
//     for (int c=0; c<C; c++) {
//       lfms.push_back(unordered_map<string, int> ());
//       blps.push_back(unordered_map<string, vector<double>> ());
//       mlses[r*C+c].modelInference(0.5, lfms[r*C+c], blps[r*C+c]);
//     }
//   }
//   for (auto it : n1s) {
//     string n1 = it.first;
//     int freq = it.second;
//     unordered_map<string, int> lfmt;
//     int c0 = kfs.getHashedValue(n1, 0);
//     for (auto itt : lfms[c0]) {
//       string n2 = itt.first;
//       bool flag = true;
//       for (int r=1; r<R; r++) {
//         int c = kfs.getHashedValue(n1, r);
//         if (lfms[r*C+c].find(n2)==lfms[r*C+c].end()) {
//           flag = false;
//           break;
//         }
//       }
//       if (flag) {
//         lfmt[n1+itt.first] = itt.second;
//       }
//     }
//     for (auto itt : lfmt) {
//       if (freqs.find(itt.first)!=freqs.end()) {
//         match++;
//       }
//     }
//   }
//   cout << "total recall: " << match*1.0/freqs.size();
// }







// int main(int argc, char* argv[]) {

//   unordered_map<string, string> args;
//   vector<string> argvs;
//   for (int i=0; i<argc; i++) {
//     argvs.push_back(string(argv[i]));
//   }
//   for (int i=1; i<argc; i+=2) {
//     assert(i+1<argc);
//     if (argvs[i]=="-k") {
//       args["k"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-m") {
//       args["m"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-f") {
//       args["f"] = argvs[i+1];
//     }
//   }

//   // pre-process of the data
//   unordered_map<string, int> freqs;
//   int flownumber = readFromFile(args["f"], freqs);
//   int totalnumber = 0;
//   for (auto it : freqs) {
//     totalnumber += it.second;
//   }
//   int thre = totalnumber/stoi(args["m"]);
   

//   // build distributed mls
//   unordered_map<string, MultiLevelSketch> mlses;
//   unordered_map<string, unordered_map<string, int>> freqOfNode;
//   for (auto it : freqs) {
//     string f = it.first;
//     int freq = it.second;
//     string node = f.substr(0, 2);
//     if (mlses.find(node)==mlses.end()) {
//       mlses[node] = MultiLevelSketch(stoi(args["k"]), stoi(args["m"]), 336, 0);
//       freqOfNode[node] = unordered_map<string, int> ();
//     }
//     mlses[node].feedFlowKey(f, freq);
//     freqOfNode[node][f] = freq;
//   }

//   cout << "number of distinct flows: " << flownumber << endl;
//   cout << "total frequency: " << totalnumber << endl;

//   for (auto it : mlses) {
//     int match = 0;
//     int largematch = 0;
//     string node = it.first;
//     cout << "node " << node << endl;
//     cout << "total flow number: " << freqOfNode[node].size() << endl;
//     int totalfreq = 0;
//     for (auto it : freqOfNode[node]) {
//       totalfreq += it.second;
//     }
//     int thre = totalfreq*1.0/stoi(args["m"]);
//     unordered_map<string, int> largeflows;
//     int largeflownumber = 0;
//     for (auto it : freqOfNode[node]) {
//       if (it.second>thre) {
//         largeflownumber++;
//         largeflows.insert(it);
//       }
//     }
//     cout << "threshold: " << thre << ", number of largeflows: " << largeflownumber << endl;

//     unordered_map<string, int> lfm;
//     unordered_map<string, vector<double>> blp;
//     mlses[node].modelInference(0.5, lfm, blp);
//     for (auto it : largeflows) {
//       string f = it.first;
//       int freq = it.second;
//       int diff = abs(freq-freqOfNode[node][f]);
//       if (lfm.find(f)!=lfm.end()&&
//           diff*1.0/freqOfNode[node][f]<1.0
//           ) {
//         largematch++;
//       }
//     }
//     cout << "large flow recall: " << 1.0*largematch/(largeflows.size()+1e-10) << endl;
//     for (auto it : freqOfNode[node]) {
//       string f = it.first;
//       int freq = it.second;
//       if (lfm.find(f)!=lfm.end()
//           // &&abs(lfm[f]-freq)>=1
//           ) {
//         match++;
//       }
//     }
//     cout << "flowkey types: " << freqOfNode[node].size() << ' ' << "extracted: " << match << endl;
//     double recall = match*1.0/freqOfNode[node].size();
//     // if (recall<1) {
//     //   cout << "recall: " << recall << endl;
//     //   for (auto it : freqOfNode[node]) {
//     //     if (abs(it.second-lfm[it.first])>=1) {
//     //       cout << "FN: " << it.first << ' ' << it.second << ' ' << lfm[it.first] << endl;
//     //     }
//     //   }
//     //   mlses[node].printSketchTable();
//     // }
//     for (auto it : lfm) {
//       if (freqOfNode[node].find(it.first)==freqOfNode[node].end()) {
//         cout << "FP: " << it.first << ' ' << it.second << endl;
//       }
//     }
//     cout << endl;
//   }
// }













// int main(int argc, char* argv[]) {
//   cout << "program start" << endl;
//   unordered_map<string, string> args;
//   vector<string> argvs;
//   for (int i=0; i<argc; i++) {
//     argvs.push_back(string(argv[i]));
//   }
//   for (int i=1; i<argc; i+=2) {
//     assert(i+1<argc);
//     if (argvs[i]=="-k") {
//       args["k"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-m") {
//       args["m"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-f") {
//       args["f"] = argvs[i+1];
//     }
//     else if (argvs[i]=="-p") {
//       args["p"] = argvs[i+1];
//     }
//   }

//   if (args.find("f")!=args.end()) {
//     // pre-process of the data
//     unordered_map<string, int> freqs;
//     unordered_map<string, int> largeflows;
//     int flownumber = readFromFile(args["f"], freqs);
//     int totalnumber = 0;
//     for (auto it : freqs) {
//       totalnumber += it.second;
//     }
//     cout << "number of distinct flows: " << flownumber << endl;
//     cout << "total frequency: " << totalnumber << endl; 

//     int R = stoi(args["k"]);
//     // vector<int> fs;
//     // for (auto it : freqs) {
//     //   fs.push_back(it.second);
//     // }
//     // sort(fs.begin(), fs.end());
//     // int accu = 0;
//     // int thre;
//     // for (int i=fs.size()-1; i>=0; i--) {
//     //   accu += fs[i];
//     //   thre = fs[i]-1;
//     //   if (accu*1.0/totalnumber>=stod(args["p"])) {
//     //     break;
//     //   }
//     // }
//     // int C = totalnumber/thre;
//     int C = stoi(args["m"]);
//     int thre = totalnumber/C;
//     cout << "target thre: " << thre << " c: " << C << endl;
//     MultiLevelSketch mls(R, C, 336, 0);
//     KHashFunctions kfs = mls.getKHashFunctions();
//     for (auto it : freqs) {
//       if (it.second>thre) {
//         // cout << "large flow: " << it.first << ' ' << it.second << endl;
//         largeflows.insert(it);
//       }
//       mls.feedFlowKey(it.first, it.second);
//     }
//     cout << "number of large flows: " << largeflows.size() << endl;

//     unordered_map<string, int> lfm;
//     unordered_map<string, vector<double>> blp;
//     mls.modelInference(0.5, lfm, blp);
//     // cout << endl << "extracted large flows: " << endl; 
//     // for (auto it : lfm) {
//     //   if (it.second>thre) {
//     //     cout << it.first << ' ' << it.second << endl;
//     //   }
//     // }
//     // cout << endl;
//     cout << "number of extracted flows: " << lfm.size() << endl;
//     cout << endl;

//     vector<vector<vector<int>>> sketch = mls.getSketch();
//     // for (int i=0; i<sketch.size(); i++) {
//     //   for (int j=0; j<sketch[0].size(); j++) {
//     //     cout << sketch[i][j][0] << ' ';
//     //   }
//     //   cout << endl;
//     // }
//     // cout << endl;
//     vector<vector<vector<int>>> sketchbackup = mls.getSketchBackUp();
//     // for (int i=0; i<sketchbackup.size(); i++) {
//     //   for (int j=0; j<sketchbackup[0].size(); j++) {
//     //     cout << sketchbackup[i][j][0] << ' ';
//     //   }
//     //   cout << endl;
//     // }
//     // cout << endl;

//     int largematch = 0;
//     for (auto it : largeflows) {
//       if (lfm.find(it.first)!=lfm.end()) {
//         largematch++;
//         int diff = abs(it.second-lfm[it.first]);
//         if (1.0*diff/it.second>0.1) {
//           cout << "freq estimate error: " << it.first << ' ' << it.second << ' ' << lfm[it.first] << endl;
//           // string key = it.first;
//           // int c = kfs.getHashedValue(key, 0);
//           // cout << c << endl;
//           // for (int i=0; i<=320; i++) {
//           //   cout << sketch[0][c][i] << ' ';
//           // }
//           // cout << endl;
//         }
//       }
//       else {
//         cout << "failed to extract error: " << it.first << ' ' << it.second << endl; 
//       }
//     }
//     cout << "large flow recall: " << 1.0*largematch/(largeflows.size()+1e-10) << endl;

//     int match = 0;
//     for (auto it : freqs) {
//       if (lfm.find(it.first)!=lfm.end()) {
//         match++;
//         int diff = abs(it.second-lfm[it.first]);
//         if (1.0*diff/it.second>0.1) {
//           // cout << "freq estimate error: " << it.first << ' ' << it.second << ' ' << lfm[it.first] << endl;
//         }
//         cout << it.first << ' ' << it.second << ' ' << lfm[it.first] << endl;
//       }
//       else {
//         cout << "failed to extract error: " << it.first << ' ' << it.second << endl; 
//       }
//     }
//     cout << "total flow recall: " << 1.0*match/(freqs.size()+1e-10) << endl;
//     cout << "total flow precision: " << 1.0*match/(lfm.size()+1e-10) << endl;
//     cout << endl;
//   }
//   else {
//     MultiLevelSketch mls(stoi(args["k"]), stoi(args["m"]), 160, 0);
//     mt19937 rng(current_time_nanoseconds());
//     // generate random flowkeys
//     srand(current_time_nanoseconds());
//     // unordered_map<int, int> smallflows;
//     unordered_map<string, int> smallflows;
//     for (int i=0; i<200; i++) {
//       // int key = rand()%(RAND_MAX);
//       string key = generateRandomHexString(40);
//       int times = rand()%3+1;
//       // int times = 0;
//       if (smallflows.find(key)==smallflows.end()) {
//         for (int i=0; i<times; i++) {
//           mls.feedFlowKey(key);
//         }
//         smallflows[key] = times;
//       }
//     }

//     // unordered_map<int, int> largeflows;
//     unordered_map<string, int> largeflows;
//     for (int i=0; i<3; i++) {
//       // int largeFlow = rand()%(RAND_MAX);
//       string largeFlow = generateRandomHexString(40);
//       int times = 500*(i+1);
//       if (largeflows.find(largeFlow)==largeflows.end()) {    
//         for (int j=0; j<times; j++) {
//           mls.feedFlowKey(largeFlow);
//         }
//         largeflows[largeFlow] = times;
//         cout << largeFlow << ' ' << times << endl;
//       }
//     }

//     int totalnumber = 0;
//     for (auto it=smallflows.begin(); it!=smallflows.end(); it++) {
//       totalnumber += it->second;
//     }
//     for (auto it=largeflows.begin(); it!=largeflows.end(); it++) {
//       totalnumber += it->second;
//     }
//     cout << "total frequency: " << totalnumber << endl; 
//     cout << "minimum frequency to be large flow: " << totalnumber/stoi(args["m"]) << endl;

//     cout << "Print sketch of level 0" << endl;
//     vector<vector<vector<int>>> sketch = mls.getSketch();
//     for (int i=0; i<sketch.size(); i++) {
//       for (int j=0; j<sketch[0].size(); j++) {
//         cout << sketch[i][j][0] << ' ';
//       }
//       cout << endl;
//     }
//     cout << endl;

//     unordered_map<string, int> lfm;
//     unordered_map<string, vector<double>> blp;
//     mls.modelInference(0.5, lfm, blp);
//     cout << endl << "extracted large flows: " << endl; 
//     for (auto it=lfm.begin(); it!=lfm.end(); it++) {
//       cout << it->first << ' ' << it->second << endl;
//     }
//     cout << endl;
//     cout << "number of extracted large flows: " << lfm.size() << endl;
//   }
// }













