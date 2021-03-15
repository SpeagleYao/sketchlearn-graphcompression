
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
#include <boost/algorithm/string.hpp>
#include "multi-level-sketch.h"

using namespace std;


void readFromFile(string& filepath, unordered_map<string, int>& freqs) {
  ifstream fp(filepath);
  string line;

  while (getline(fp, line)) {
    if (!line.empty()) {
      vector<string> splits;
      boost::split(splits, line, boost::is_any_of(" "));
      if (freqs.find(splits[0])==freqs.end()) {
        freqs[splits[0]] = stoi(splits[1]);
      }
      else {
        cout << "duplicate edges!" << endl;
      }
    }
  }
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


string generateRandomHexString(int length) {
  string s;
  for (int i=0; i<length; i++) {
    int t = rand()%16;
    s += hexTable[t];
  }
  return s;
}

int main(int argc, char* argv[]) {

  unordered_map<string, string> args;
  vector<string> argvs;
  for (int i=0; i<argc; i++) {
    argvs.push_back(string(argv[i]));
  }
  for (int i=1; i<argc; i+=2) {
    assert(i+1<argc);
    if (argvs[i]=="-k") {
      args["k"] = argvs[i+1];
    }
    else if (argvs[i]=="-m") {
      args["m"] = argvs[i+1];
    }
    else if (argvs[i]=="-f") {
      args["f"] = argvs[i+1];
    }
  }

  // vector<int> t = intToVector(7);
  // for (auto it=t.begin(); it!=t.end(); it++) {
  //   cout << *it << ' ';
  // }
  // cout << endl;
  // int nt = vectorToInt(t);
  // cout << nt << endl;

  if (args.find("f")!=args.end()) {
    // pre-process of the data
    unordered_map<string, int> freqs;
    readFromFile(args["f"], freqs);
    int totalnumber = 0;
    for (auto it : freqs) {
      totalnumber += it.second;
    }
    int thre = totalnumber/stoi(args["m"]);
    cout << "total frequency: " << totalnumber << endl; 
    cout << "minimum frequency to be large flow: " << thre << endl;

    MultiLevelSketch mls(stoi(args["k"]), stoi(args["m"]), 320, 160);
    for (auto it : freqs) {
      if (it.second>thre) {
        cout << "large flow: " << it.first << ' ' << it.second << endl;
      }
      for (int i=0; i<it.second; i++) {
        mls.feedFlowKey(it.first);
      }
    }

    // vector<vector<double>> distributions = mls.computeDistribution();
    // cout << distributions.size() << endl;
    // for (auto it : distributions) {
    //   cout << it[0] << ' ' << it[1] << endl;
    // }

    cout << "Print sketch of level 0" << endl;
    vector<vector<vector<int>>> sketch = mls.getSketch();
    for (int i=0; i<sketch.size(); i++) {
      for (int j=0; j<sketch[0].size(); j++) {
        cout << sketch[i][j][0] << ' ';
      }
      cout << endl;
    }
    cout << endl;

    cout << endl << "extract large flow" << endl << endl;
    unordered_map<string, int> lfm;
    unordered_map<string, vector<double>> blp;
    mls.modelInference(0.5, lfm, blp);
    cout << endl << "extracted large flows: " << endl; 
    for (auto it=lfm.begin(); it!=lfm.end(); it++) {
      cout << it->first << ' ' << it->second << endl;
    }
    cout << endl;
  }
  else {
    MultiLevelSketch mls(stoi(args["k"]), stoi(args["m"]), 160, 0);
    mt19937 rng(current_time_nanoseconds());
    // generate random flowkeys
    srand(current_time_nanoseconds());
    // unordered_map<int, int> smallflows;
    unordered_map<string, int> smallflows;
    for (int i=0; i<200; i++) {
      // int key = rand()%(RAND_MAX);
      string key = generateRandomHexString(40);
      int times = rand()%3+1;
      // int times = 0;
      if (smallflows.find(key)==smallflows.end()) {
        for (int i=0; i<times; i++) {
          mls.feedFlowKey(key);
        }
        smallflows[key] = times;
      }
    }

    // unordered_map<int, int> largeflows;
    unordered_map<string, int> largeflows;
    for (int i=0; i<3; i++) {
      // int largeFlow = rand()%(RAND_MAX);
      string largeFlow = generateRandomHexString(40);
      int times = 500*(i+1);
      if (largeflows.find(largeFlow)==largeflows.end()) {    
        for (int j=0; j<times; j++) {
          mls.feedFlowKey(largeFlow);
        }
        largeflows[largeFlow] = times;
        cout << largeFlow << ' ' << times << endl;
      }
    }

    int totalnumber = 0;
    for (auto it=smallflows.begin(); it!=smallflows.end(); it++) {
      totalnumber += it->second;
    }
    for (auto it=largeflows.begin(); it!=largeflows.end(); it++) {
      totalnumber += it->second;
    }
    cout << "total frequency: " << totalnumber << endl; 
    cout << "minimum frequency to be large flow: " << totalnumber/stoi(args["m"]) << endl;

    // int largeFlow = rand()%(RAND_MAX/2);
    // cout << "random large flow is " << largeFlow << endl;
    // for (int i=0; i<100; i++) {
    //   mls.feedFlowKey(largeFlow);
    // }
    // for (auto it=freqs.begin(); it!=freqs.end(); it++) {
    //   cout << *it << ' ';
    // }

    cout << "Print sketch of level 0" << endl;
    vector<vector<vector<int>>> sketch = mls.getSketch();
    for (int i=0; i<sketch.size(); i++) {
      for (int j=0; j<sketch[0].size(); j++) {
        cout << sketch[i][j][0] << ' ';
      }
      cout << endl;
    }
    cout << endl;

    // exit(0);

    // cout << "Print estimated mean and variance for each level" << endl;
    // vector<vector<double>> distributions = mls.computeDistribution();
    // for (int k=0; k<distributions.size(); k++) {
    //   cout << distributions[k][0] << ' ' << distributions[k][1] << endl;
    // }

    cout << endl << "extract large flow" << endl << endl;
    unordered_map<string, int> lfm;
    unordered_map<string, vector<double>> blp;
    mls.modelInference(0.5, lfm, blp);
    cout << endl << "extracted large flows: " << endl; 
    for (auto it=lfm.begin(); it!=lfm.end(); it++) {
      cout << it->first << ' ' << it->second << endl;
    }
    cout << endl;

    // cout << "query small flow" << endl;
    // int count = 0;
    // for (auto it : smallflows) {
    //   int fp = mls.queryFrequence(it.first);
    //   int fd = it.second;
    //   cout << it.first << ' ' << fd << ' ' << fp << endl;
    //   count++;
    //   if (count==10) break;
    // }

    // cout << endl;

    // // test hex string to vector
    // cout << moduloHexString("AB", 16) << endl;
    // for (int i=0; i<10; i++) {
    //   string s = generateRandomHexString(40);
    //   cout << s << endl;
    //   vector<int> v = hexStringToVector(s);
    //   for (auto ele : v) {
    //     cout << ele << ' ';
    //   }
    //   cout << endl;
    //   cout << vectorToHexString(v) << endl;
    // }
  }

  
}
