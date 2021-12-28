#include "auto_brown.h"
#include "fastmarch.h"
#include "my_score.h"
#include "viterbi.h"
using namespace SEP;
std::vector<path> auto_brown::return_path(std::vector<int> locs,
                                          std::shared_ptr<float2DReg> array) {
  int imid = (array->getHyper()->getAxis(1).n - 1) / 2;
  n1 = imid * 2 + 1;
  float mymin = array->min();
  for (int i2 = 0; i2 < array->getHyper()->getAxis(2).n; i2++) {
    for (int i1 = 0; i1 < array->getHyper()->getAxis(1).n; i1++) {
      (*array->_mat)[i2][i1] =
          1. / (.3 + 2.5 * ((*array->_mat)[i2][i1] - mymin));
    }
  }
  n2 = array->getHyper()->getAxis(2).n;
  n3 = 1;

  slow = (float*)array->getVoidPtr();

  axis a1 = array->getHyper()->getAxis(1), a2 = array->getHyper()->getAxis(2);
  std::shared_ptr<float2DReg> tot(new float2DReg(a1, a2));
  std::shared_ptr<float2DReg> times(new float2DReg(a1, a2));
  tot->zero();
  b1 = 1;
  b2 = 1;
  b3 = 1;
  d1 = 1.;
  d2 = 1.;
  d3 = 1.;
  n12 = n1 * n2;
  n = n12;
  order = 1;

  // for(int isrc=0; isrc< (int)locs.size(); isrc++){
  for (int isrc = 0; isrc < (int)locs.size(); isrc++) {
    alloc_local();
    slow0 = slow[imid + n1 * locs[isrc]];

    calc_time(imid, locs[isrc], 0, (float*)times->getVoidPtr());
    int i = 0;
    for (int i2 = 0; i2 < n2; i2++) {
      for (int i1 = 0; i1 < n1; i1++, i++) {
        float dist = sqrtf(1. * ((i2 - locs[isrc]) * (i2 - locs[isrc]) +
                                 (i1 - imid) * (i1 - imid)));
        (*tot->_mat)[i2][i1] += dist / ((*times->_mat)[i2][i1] + .01);
      }
    }

    delete_fastmarch();
  }

  int ilast = imid;
  int imin;
  float val;
  for (int i = 0; i < (int)locs.size(); i++) (*tot->_mat)[locs[i]][imid] = nail;

  std::shared_ptr<my_score> score(new my_score(pen_o, pen_e));
  viterbi myd = viterbi(score, tot, 4, 0);

  myd.calc_score();
  return myd.return_path();
  if (1 == 3) {
    std::vector<path> myp;
    for (int i2 = 1; i2 < n2; i2++) {
      mymin = (*tot->_mat)[i2][ilast];
      imin = ilast;
      if (ilast > 0) {
        val = (*tot->_mat)[i2][ilast - 1];
        if (val < mymin) {
          mymin = val;
          imin = ilast - 1;
        }
      }
      if (ilast < n - 1) {
        val = (*tot->_mat)[i2][ilast + 1];
        if (val < mymin) {
          mymin = val;
          imin = ilast + 1;
        }
        ilast = imin;
        myp.push_back(path(ilast, i2));
      }
    }

    return myp;
  }
}
