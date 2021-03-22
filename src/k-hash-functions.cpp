
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


int moduloHexString(string& s, long long m) {
  long long t = 0;
  for (int i=0; i<s.size(); i++) {
    t *= 16;
    t += s[i]>='0'&s[i]<='9' ? s[i]-'0' : s[i]-'A'+10;
    if (t>=m) {
      t = t%m;
    }
  }
  return t;
}


KHashFunctions::KHashFunctions() {}


KHashFunctions::KHashFunctions(int k, int m) {
  this->k = k;
  this->m = m;

  mt19937 rng(current_time_nanoseconds());
  // mt19937 rng(1234);
  uniform_int_distribution<> uniform_distributor(1,RAND_MAX/2);
  for (int i=0; i<k; i++) {
    int a = uniform_distributor(rng);
    int b = uniform_distributor(rng);
    // cout << a << ' ' << b << endl;
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


int KHashFunctions::getHashedValue(int key, int r, int shift) {
  assert(r<this->params.size()&&r>=0);
  int a = this->params[r][0];
  int b = this->params[r][1];
  key = key >> shift;
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


vector<int> KHashFunctions::getHashedValues(int key, int shift) {
  vector<int> res;
  for (int i=0; i<this->k; i++) {
    res.push_back(getHashedValue(key, i, shift));
  }
  return res;
}


int KHashFunctions::getHashedValue(string& key, int r) {
  assert(r<this->params.size()&&r>=0);
  int a = this->params[r][0];
  int b = this->params[r][1];
  int intkey = moduloHexString(key, this->p);
  int tmp = moduloMultiplication(a, intkey, this->p);
  // a = a%this->p;
  // int tmp = (a*intkey)%this->p;
  int res = moduloAddition(tmp, b, this->p) % this->m;
  // cout << key << ' ' << intkey << ' ' << res << endl;
  // return res;
  return res;
}


vector<int> KHashFunctions::getHashedValues(string& key) {
  vector<int> res;
  for (int i=0; i<this->k; i++) {
    res.push_back(getHashedValue(key, i));
  }
  return res;
}


KHashFunctions::~KHashFunctions() {};