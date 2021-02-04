
#include "k-hash-functions.h"

int current_time_nanoseconds(){
  struct timespec tm;
  clock_gettime(CLOCK_REALTIME, &tm);
  return tm.tv_nsec;
}


int moduloMultiplication(long long a, long long b, long long m) {
  int res = 0;
  a %= m;
  while (b) {
    if (b&1) {
      res = (res+a) % m;
    }
    a = (2*a) % m;
    b >>= 1;
  }
  return res;
} 


int moduloAddition(long long a, long long b, long long m) {
  int res = 0;
  long long tmpa = a % m;
  long long tmpb = b % m;
  res = (tmpa+tmpb) % m;
  return res;
}


KHashFunctions::KHashFunctions() {}


KHashFunctions::KHashFunctions(int k, int m) {
  this->k = k;
  this->m = m;

  mt19937 rng(4868);
  uniform_int_distribution<> uniform_distributor(1,RAND_MAX/2);
  for (int i=0; i<k; i++) {
    int a = uniform_distributor(rng);
    int b = uniform_distributor(rng);
    vector<int> tmp = vector<int> ({a, b});
    this->params.push_back(tmp);
  }
}


int KHashFunctions::getHashedValue(int key, int r) {
  assert(r<this->params.size()&&r>=0);
  int a = this->params[r][0];
  int b = this->params[r][1];
  int tmp = moduloMultiplication(a, key, this->p);
  int res = moduloAddition(tmp, b, this->p) % this->m;
  return res;
}


vector<int> KHashFunctions::getHashedValues(int key) {
  vector<int> res;
  for (int i=0; i<this->k; i++) {
    res.push_back(getHashedValue(key, i));
  }
  return res;
}



KHashFunctions::~KHashFunctions() {};