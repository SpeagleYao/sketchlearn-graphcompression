#ifndef _MULTILEVELSKETCH_H
#define _MULTILEVELSKETCH_H


#include <vector>
#include "k-hash-functions.h"

using namespace std;


vector<int> intToVector(int n);


int vectorToInt(vector<int> v);


double gaussian_prob(double d);


class MultiLevelSketch {
public:
  MultiLevelSketch();
  MultiLevelSketch(int r, int c, int k);

  void initSketch();
  void resetSketch();

  void feedFlowKey(int flowkey);

  vector<vector<vector<int>>> getSketch();

  vector<vector<double>> computeDistribution();
  void extractLargeFlows(double theta, int r, int c, vector<vector<double>>& distributions);

  ~MultiLevelSketch();

private:
  int R;
  int C;
  int K;

  KHashFunctions khfs;
  vector<vector<vector<int>>> sketch;
  unordered_set<int> largeFlows;

};


#endif