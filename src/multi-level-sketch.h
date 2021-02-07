#ifndef _MULTILEVELSKETCH_H
#define _MULTILEVELSKETCH_H


#include <vector>
#include "k-hash-functions.h"

using namespace std;


vector<int> intToVector(int n);


int vectorToInt(vector<int>& v);


double gaussian_prob(double d);


int median(vector<int>& v);
double mean(vector<double>& v);
double variance(vector<double>& v);


class MultiLevelSketch {
public:
  MultiLevelSketch();
  MultiLevelSketch(int r, int c, int k);

  void initSketch();
  void resetSketch();

  void feedFlowKey(int flowkey);

  vector<vector<vector<int>>> getSketch();

  void modelInference(double theta, unordered_map<int, int>& lfm, unordered_map<int, vector<double>>& blp);
  vector<vector<double>> computeDistribution();
  vector<vector<double>> computeDistribution_v2();
  void extractLargeFlows(double theta, int r, int c, vector<vector<double>>& distributions,
                        unordered_map<int, int>& lfm, unordered_map<int, vector<double>>& blp);
  int estimateFrequence(int r, int c, vector<int>& f, vector<vector<double>>& distributions);

  unordered_set<int> enumerateCandidates(vector<int>& temp);

  ~MultiLevelSketch();

private:
  int R;
  int C;
  int K;

  KHashFunctions khfs;
  vector<vector<vector<int>>> sketch;
  unordered_map<int, int> largeFlows;

  void recursivelyEnumerateCandidates(vector<int>& temp, int pos, vector<int>& tmp, unordered_set<int>& res);
  void removeFlows(unordered_map<int, int>& lfm);
  bool terminate(int denominator, vector<vector<double>>& distributions);
};


#endif