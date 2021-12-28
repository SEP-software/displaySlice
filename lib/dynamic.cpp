
#include "dynamic.h"
using namespace SEP;
dynamic::dynamic(std::shared_ptr<float2DReg> s, float jump_pen) {
  std::shared_ptr<float2DReg> sc = s->clone();
  sim = sc;
  std::shared_ptr<float2DReg> sc2 = s->clone();
  score = sc2;
  jump = jump_pen;
  score->scale(0.);
}
void dynamic::add_to_sim(std::shared_ptr<float2DReg> s) {
  sim->scaleAdd(s, 1., 1.);
}
void dynamic::calc_score() {
  int n1 = sim->getHyper()->getAxis(1).n;
  int n2 = sim->getHyper()->getAxis(2).n;
  // First fill the bottom edges
  for (int i = 0; i < n2; i++)
    (*score->_mat)[i][n1 - 1 + i] = (*sim->_mat)[i][n1 - 1];  //
  for (int i = 0; i < n1; i++)
    (*score->_mat)[n2 - 1][i] = (*sim->_mat)[n2 - 1][i];
  float mymax2 = -100., mymax, sc;
  for (int i2 = n2 - 2; i2 >= 0; i2--) {
    for (int i1 = n1 - 2; i1 >= 0; i1--) {
      mymax = -9;
      for (int i = i1 + 1; i < n1 - 1; i++) {
        sc = (*score->_mat)[i2 + 1][i] - jump * (i - i1);
        if (sc > mymax) sc = mymax;
      }
      for (int j = i2 + 1; j < n2 - 1; j++) {
        sc = (*score->_mat)[j][i1 + 1] - jump * (j - i2);
        if (sc > mymax) sc = mymax;
      }
      (*score->_mat)[i2][i1] = mymax + (*sim->_mat)[i2][i1];
      if ((*score->_mat)[i2][i1] > mymax2) {
        mymax2 = (*score->_mat)[i2][i1];
        maxloc[0] = i1;
        maxloc[1] = i2;
      }
    }
  }
}
std::vector<path> dynamic::return_path() {
  std::vector<path> myp;

  int i1 = maxloc[0], i2 = maxloc[1];
  int n1 = sim->getHyper()->getAxis(1).n;
  int n2 = sim->getHyper()->getAxis(2).n;
  float down, right, diagonal;
  myp.push_back(path(i1, i2));
  while (i1 != n1 - 1 && i2 != n2 - 1) {
    down = (*score->_mat)[i2][i1 + 1];
    right = (*score->_mat)[i2 + 1][i1];
    diagonal = (*score->_mat)[i2 + 1][i1 + 1];

    if (down > right && down > diagonal)
      i1++;
    else if (right > diagonal)
      i2++;
    else {
      i1++;
      i2++;
    }
    myp.push_back(path(i1, i2));
  }
  return myp;
}
