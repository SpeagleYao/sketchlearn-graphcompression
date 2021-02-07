
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
#include "k-hash-functions.h"
#include "multi-level-sketch.h"

using namespace std;


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
  }

  // vector<int> t = intToVector(7);
  // for (auto it=t.begin(); it!=t.end(); it++) {
  //   cout << *it << ' ';
  // }
  // cout << endl;
  // int nt = vectorToInt(t);
  // cout << nt << endl;

  MultiLevelSketch mls(stoi(args["k"]), stoi(args["m"]), 32);
  // KHashFunctions khfs(stoi(args["k"]), stoi(args["m"]));
  // cout << "start" << endl;
  mt19937 rng(current_time_nanoseconds());
  // vector<int> freqs = vector<int> (stoi(args["m"]), 0);
  // generate random flowkeys
  srand(current_time_nanoseconds());
  for (int i=0; i<1000; i++) {
    int key = rand()%(RAND_MAX/2);
    // cout << key << endl;
    // int hashed = khfs.getHashedValue(key, 0);
    // cout << key << ' ' << hashed << endl;
    //freqs[hashed]++;
    mls.feedFlowKey(key);
  }
  for (int i=0; i<5; i++) {
    int largeFlow = rand()%(RAND_MAX/2);
    cout << "random large flow is " << largeFlow << endl;
    for (int j=0; j<500*(i+1); j++) {
      mls.feedFlowKey(largeFlow);
    }
  }
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

  // cout << "Print estimated mean and variance for each level" << endl;
  vector<vector<double>> distributions = mls.computeDistribution_v2();
  for (int k=0; k<distributions.size(); k++) {
    cout << distributions[k][0] << ' ' << distributions[k][1] << endl;
  }

  cout << endl << "extract large flow" << endl;

  unordered_map<int, int> lfm;
  unordered_map<int, vector<double>> blp;
  // for (int r=0; r<stoi(args["k"]); r++) {
  //   for (int c=0; c<stoi(args["m"]); c++) {
  //     mls.extractLargeFlows(0.5, r, c, distributions, lfm, blp);
  //   }
  // }

  mls.modelInference(0.5, lfm, blp);
  cout << endl << "extracted flows: " << endl; 
  for (auto it=lfm.begin(); it!=lfm.end(); it++) {
    cout << it->first << ' ' << it->second << endl;
  }
  
  cout << endl;

}
