#include "multi-level-sketch.h"


vector<int> intToVector(int n) {
  vector<int> res (32, 0);
  for (int i=31; i>=0; i--) {
    if (n!=0) {
      if (n%2) {
        res[i] = 1;
      }
      n /= 2;
    }
  }
  return res;
}


int vectorToInt(vector<int> v) {
  int res = 0;
  for (int i=31; i>=0; i--) {
    res += pow(2, 31-i) * v[i];
  }
  return res;
}


double gaussian_prob(double d) {
  return 1 - erf(d/sqrt(2));
}


MultiLevelSketch::MultiLevelSketch() {};


MultiLevelSketch::MultiLevelSketch(int r, int c, int k) {
  this->R = r;
  this->C = c;
  this->K = k;
  this->khfs = KHashFunctions(r, c);
  initSketch();
};


void MultiLevelSketch::initSketch() {
  vector<vector<vector<int>>> sketchTable;
  for (int i=0; i<this->R; i++) {
    vector<vector<int>> tmp;
    for (int j=0; j<this->C; j++) {
      vector<int> ttmp (this->K+1, 0);
      tmp.push_back(ttmp);
    }
    sketchTable.push_back(tmp);
  }
  this->sketch = sketchTable;
}


void MultiLevelSketch::resetSketch() {
  assert(this->R>0&&this->C>0&&this->K>0);
  for (int i=0; i<this->R; i++) {
    for (int j=0; j<this->C; j++) {
      for (int k=0; j<this->K+1; k++) {
        this->sketch[i][j][k] = 0;
      }
    }
  }
}


void MultiLevelSketch::feedFlowKey(int flowkey) {
  vector<int> hashed = this->khfs.getHashedValues(flowkey);
  vector<int> booleans = intToVector(flowkey);
  for (int r=0; r<this->R; r++) {
    int c = hashed[r];
    this->sketch[r][c][0]++;
    for (int k=0; k<this->K; k++) {
      if (booleans[k]) {
        this->sketch[r][c][k+1]++;
      }
    }
  }
}


vector<vector<vector<int>>> MultiLevelSketch::getSketch() {
  return this->sketch;
}


vector<vector<double>> MultiLevelSketch::computeDistribution() {
  vector<vector<double>> res;
  int n = 0;
  for (int j=0; j<this->C; j++) {
    n += this->sketch[0][j][0];
  }
  for (int k=0; k<this->K; k++) {
    double comuP = 0.0;
    for (int i=0; i<this->R; i++) {
      for (int j=0; j<this->C; j++) {
        comuP += (double)this->sketch[i][j][k+1] / (double)this->sketch[i][j][0];
      }
    }
    double p = comuP / (this->C*this->R);
    double delta = p*(1-p)*this->C/n;
    res.push_back(vector<double>({p, delta}));
  }
  return res;
}


void MultiLevelSketch::extractLargeFlows(double theta, int r, int c, vector<vector<double>>& distributions) {
  assert(r>=0&&r<this->R&&c>=0&&c<this->C);
  // 1st step, estinate bit-level probabilities.
  vector<double> phat;
  for (int k=0; k<this->K; k++) {
    double ratio = this->sketch[r][c][k+1]/this->sketch[r][c][0];
    if (ratio<theta) {
      phat.push_back(0.0);
    }
    else if (ratio>1-theta) {
      phat.push_back(1);
    }
    else {
      double p1 = distributions[k][0];
      double p2 = 1-p1;
      // TO BE IMPLEMENTED
    }
  }
}


MultiLevelSketch::~MultiLevelSketch() {}