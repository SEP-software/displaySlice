#include "viterbi.h"
using namespace SEP;
viterbi::viterbi(std::shared_ptr<my_score> s2, std::shared_ptr<float2DReg> s,
                 int ns, int floc) {
  sc = s2;
  std::shared_ptr<float2DReg> sc = s->clone();
  sim = s;
  std::shared_ptr<float2DReg> sc2 = s->clone();
  score = sc2;
  nsearch = ns;
  score->scale(0.);
  f = floc;
}

void viterbi::calc_score() {
  int n1 = score->getHyper()->getAxis(1).n;
  int n2 = score->getHyper()->getAxis(2).n;

  for (int i = 0; i < n1; i++)
    (*score->_mat)[n2 - 1][i] = (*sim->_mat)[n2 - 1][i];
  int ib, ie;
  float mymax2 = -100., mymax, ss;
  for (int i2 = n2 - 2; i2 >= 0; i2--) {
    for (int i1 = 0; i1 < n1; i1++) {
      ib = i1 - nsearch;
      ie = i1 + nsearch;
      if (ib < 0) ib = 0;
      if (ie > n1 - 1) ie = n1 - 1;
      mymax = -9999;
      //

      for (int i = ib; i <= ie; i++) {
        ss = (*score->_mat)[i2][i + n1] + (*sim->_mat)[i2][i1] +
             sc->penalty(i2 + f, i, i1);
        if (mymax < ss) mymax = ss;
      }
      (*score->_mat)[i2][i1] = mymax;
      if ((*score->_mat)[i2][i1] > mymax2) {
        mymax2 = (*score->_mat)[i2][i1];

        maxloc[0] = i1;
        maxloc[1] = i2;
      }
    }
  }
}
std::vector<path> viterbi::return_path() {
  std::vector<path> myp;

  int n1 = sim->getHyper()->getAxis(1).n;
  int n2 = sim->getHyper()->getAxis(2).n;

  int i1;
  int imax;

  float mymax;
  // for( i2=0; i2 < n2; i2++){
  mymax = (*score->_mat)[0][0];
  imax = 0;

  for (i1 = 1; i1 < n1; i1++) {
    if (mymax < (*score->_mat)[0][i1]) {
      mymax = (*score->_mat)[0][i1];
      imax = i1;
    }
  }

  myp.push_back(path(imax, 0));
  for (int i2 = 1; i2 < n2; i2++) {
    int ibeg = imax - nsearch, iend = imax + nsearch;
    if (ibeg < 0) ibeg = 0;
    if (iend >= n1 - 1) iend = n1 - 1;
    mymax = (*score->_mat)[i2][0];
    imax = 0;
    for (int i1 = ibeg; i1 < iend; i1++) {
      if (mymax < (*score->_mat)[i2][i1]) {
        mymax = (*score->_mat)[i2][i1];
        imax = i1;
      }
    }
    myp.push_back(path(imax, i2));
  }
  return myp;
}
