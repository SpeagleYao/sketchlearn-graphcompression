#ifndef _MULTILEVELSKETCH_H
#define _MULTILEVELSKETCH_H

#include "k-hash-functions.h"

using namespace std;

static unordered_map<char, string> hexStringTable = {
  {'0', "0000"}, {'1', "0001"},
  {'2', "0010"}, {'3', "0011"},
  {'4', "0100"}, {'5', "0101"},
  {'6', "0110"}, {'7', "0111"},
  {'8', "1000"}, {'9', "1001"},
  {'a', "1010"}, {'b', "1011"},
  {'c', "1100"}, {'d', "1101"},
  {'e', "1110"}, {'f', "1111"}
};;

static unordered_map<int, char> hexTable = {
  {0, '0'}, {1, '1'},
  {2, '2'}, {3, '3'},
  {4, '4'}, {5, '5'},
  {6, '6'}, {7, '7'},
  {8, '8'}, {9, '9'},
  {10, 'a'}, {11, 'b'},
  {12, 'c'}, {13, 'd'},
  {14, 'e'}, {15, 'f'},
};

vector<int> intToVector(int n);

int vectorToInt(vector<int>& v);

vector<int> hexStringToVector(string& s);

string vectorToHexString(vector<int>& v);

double gaussian_prob(double d);

int median(vector<int>& v);
int median(vector<double>& v);
double mean(vector<double>& v);
double variance(vector<double>& v);


class MultiLevelSketch {
public:
  MultiLevelSketch();
  MultiLevelSketch(int r, int c, int k);
  MultiLevelSketch(int r, int c, int k, int s);
  MultiLevelSketch(int r, int c, int k, int s, bool exhaust);

  void initSketch();
  void resetSketch();

  void feedFlowKey(int flowkey);
  void feedFlowKey(string flowkey);
  void feedFlowKey(string flowkey, int freq);

  void printSketchTable();

  vector<vector<vector<int>>> getSketch();
  vector<vector<vector<int>>> getSketchBackUp();
  KHashFunctions getKHashFunctions();

  void modelInference(double theta, unordered_map<int, int>& lfm, unordered_map<int, vector<double>>& blp);
  void modelInference(double theta, unordered_map<string, int>& lfm, unordered_map<string, vector<double>>& blp);
  vector<vector<double>> computeDistribution();
  vector<vector<double>> computeDistribution_v2();
  bool extractLargeFlows(double theta, int r, int c, vector<vector<double>>& distributions,
                        unordered_map<int, int>& lfm, unordered_map<int, vector<double>>& blp);
  bool extractLargeFlows(double theta, int r, int c, vector<vector<double>>& distributions,
                        unordered_map<string, int>& lfm, unordered_map<string, vector<double>>& blp);
  int estimateFrequence(int r, int c, vector<int>& f, vector<vector<double>>& distributions);
  int estimateFrequence(int r, int c, int s, int e, vector<int>& f, vector<vector<double>>& distributions);
  unordered_map<string, int> queryFrequence(string key);
  // int queryFrequence_v2(int key);

  unordered_set<int> enumerateCandidates(vector<int>& temp);
  unordered_set<string> enumerateHexStringCandidates(vector<int>& temp);

  ~MultiLevelSketch();

private:
  int R;
  int C;
  int K;
  int shift;
  bool exhaust=false;

  KHashFunctions khfs;
  vector<vector<vector<int>>> sketch;
  vector<vector<vector<int>>> sketchbackup;
  unordered_map<int, int> largeFlows;

  vector<vector<vector<int>>> initialEmptySketch();

  void recursivelyEnumerateCandidates(vector<int>& temp, int pos, vector<int>& tmp, unordered_set<int>& res);
  void recursivelyEnumerateCandidates(vector<int>& temp, int pos, vector<int>& tmp, unordered_set<string>& res);
  void removeFlows(unordered_map<int, int>& lfm);
  void removeFlows(unordered_map<string, int>& lfm);
  bool terminate(int denominator, vector<vector<double>>& distributions);
  bool terminate();
};


#endif