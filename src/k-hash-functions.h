#ifndef _KHASHFUNCTIONS_H
#define _KHASHFUNCTIONS_H


#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <random>
#include <vector>

using namespace std;

int current_time_nanoseconds();

int moduloMultiplication(long long a, long long b, long long m);

int moduloAddition(long long a, long long b, long long m);

int moduloHexString(string& s, long long m);

class KHashFunctions {

public:
  KHashFunctions();
  KHashFunctions(int k, int m);

  ~KHashFunctions();

  int getHashedValue(int key, int r);
  int getHashedValue(int key, int r, int shift);
  vector<int> getHashedValues(int key);
  vector<int> getHashedValues(int key, int shift);

  int getHashedValue(string& key, int r);
  int getHashedValue(string& key, int r, int shift);
  vector<int> getHashedValues(string& key);
  vector<int> getHashedValues(string& key, int shift);

private:
  int k;
  int m;
  int p = pow(2, 31)-1;

  vector<vector<int>> params;
};


#endif