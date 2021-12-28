#include "float_buffer_buffer.h"
#include <stdlib.h>
#include <iostream>
#include "buffersRegFile.h"
#include "fileIO.h"
#include "float2DReg.h"
#include "simpleMemoryLimit.h"
using namespace SEP;
// Initialize and read in buffer
float_buffer_buffer::float_buffer_buffer(std::shared_ptr<paramObj> p,
                                         std::shared_ptr<hypercube> h,
                                         std::shared_ptr<io_func> i, int in,
                                         std::vector<int> &nwbuf,
                                         std::vector<int> &fwbuf,
                                         const int mem) {
  set_basics(p, h, i, in);
  for (int i = 0; i < 8; i++) hold[i] = true;
  std::shared_ptr<fileIO> fIO = std::dynamic_pointer_cast<fileIO>(i);
  if (!fIO) {
    std::cerr << "Can only specify buffered IO with a buffered file"
              << std::endl;
    assert(fIO);
  }

  std::shared_ptr<genericReg> f = fIO->getFile();
  _file = std::dynamic_pointer_cast<buffersRegFile>(f);
  if (!_file) {
    std::cerr << "Can only specify buffered IO with a buffered file"
              << std::endl;
    assert(_file);
  }

  std::shared_ptr<IO::simpleMemoryLimit> memS(
      new IO::simpleMemoryLimit(mem * 1024));
  _file->setMemoryUsage(memS);
  _ndim = _file->getHyper()->getNdim();
  _clip = clips(_util->par, in);
}

unsigned char *float_buffer_buffer::get_char_data(int n, long long *index) {
  assert(1 == 2);
}

unsigned char *float_buffer_buffer::get_char_data(
    std::shared_ptr<orient_cube> pos, int iax1, int f1, int e1, int iax2,
    int f2, int e2) {
  std::vector<int> jw(_ndim, 1);
  std::vector<int> nw(_ndim, 1);
  std::vector<int> fw(_ndim, 1);
  pos->set_no_rotate();
  int beg2, beg1, delta1, delta2, j1, j2;
  int n1 = abs(e1 - f1), n2 = abs(e2 - f2);

  for (int idim = 0; idim < _ndim; idim++) {
    if (idim == iax1) {
      if (f1 < e1) {
        delta1 = 1;
        beg1 = 0;
        fw[idim] = f1;
        nw[idim] = e1 - f1;
      } else {
        beg1 = n1 - 1;
        delta1 = -1;
        fw[idim] = e1;
        nw[idim] = f1 - e1;
      }

    } else if (idim == iax2) {
      if (f2 < e2) {
        delta2 = 1;
        beg2 = 0;
        fw[idim] = f2;
        nw[idim] = e2 - f2;
      } else {
        delta2 = -1;
        beg2 = n2 - 1;
        fw[idim] = e2;
        nw[idim] = f2 - e2;
      }
    } else {
      fw[idim] = pos->loc[idim];
    }
  }
  if (iax1 > iax2) {
    j1 = n2;
    j2 = 1;
  } else {
    j1 = 1;
    j2 = n1;
  }
  std::shared_ptr<float2DReg> flt(new float2DReg(n1, n2));
  _file->readFloatWindow(nw, fw, jw, (float *)flt->getVoidPtr());
  _clip.updateClips((float)flt->min(), (float)flt->max());
  float *vals = flt->getVals();
  float j = (_clip._clipMax - _clip._clipMin) / 255.;
  unsigned char *out = new unsigned char[n1 * n2];

  int i = 0;
  int l2 = beg2;
  for (int i2 = 0; i2 < n2; i2++) {
    int l1 = beg1;
    for (int i1 = 0; i1 < n1; i1++, i++) {
      out[i] = std::max(
          0,
          std::min(255, (int)((vals[l1 * j1 + l2 * j2] - _clip._clipMin) / j)));
      l1 += delta1;
    }
    l2 += delta2;
  }

  return out;
}

float *float_buffer_buffer::get_float_data(std::shared_ptr<orient_cube> pos,
                                           int iax1, int f1, int e1, int iax2,
                                           int f2, int e2) {
  std::vector<int> jw(_ndim, 1);
  std::vector<int> nw(_ndim, 1);
  std::vector<int> fw(_ndim, 1);
  pos->set_no_rotate();
  int beg2, beg1, delta1, delta2, j1, j2;
  int n1 = abs(e1 - f1), n2 = abs(e2 - f2);

  for (int idim = 0; idim < _ndim; idim++) {
    if (idim == iax1) {
      if (f1 < e1) {
        delta1 = 1;
        beg1 = 0;
        fw[idim] = f1;
        nw[idim] = e1 - f1;
      } else {
        beg1 = n1 - 1;
        delta1 = -1;
        fw[idim] = e1;
        nw[idim] = f1 - e1;
      }

    } else if (idim == iax2) {
      if (f2 < e2) {
        delta2 = 1;
        beg2 = 0;
        fw[idim] = f2;
        nw[idim] = e2 - f2;
      } else {
        delta2 = -1;
        beg2 = n2 - 1;
        fw[idim] = e2;
        nw[idim] = f2 - e2;
      }
    } else {
      fw[idim] = pos->loc[idim];
    }
  }
  if (iax1 > iax2) {
    j1 = n2;
    j2 = 1;
  } else {
    j1 = 1;
    j2 = n1;
  }
  std::shared_ptr<float2DReg> flt(new float2DReg(n1, n2));
  _file->readFloatWindow(nw, fw, jw, (float *)flt->getVoidPtr());
  _clip.updateClips((float)flt->min(), (float)flt->max());
  float *vals = flt->getVals();
  float j = (_clip._clipMax - _clip._clipMin) / 255.;
  float *out = new float[n1 * n2];

  int i = 0;
  int l2 = beg2;
  for (int i2 = 0; i2 < n2; i2++) {
    int l1 = beg1;
    for (int i1 = 0; i1 < n1; i1++, i++) {
      out[i] = std::max(_clip._clipMax,
                        (std::min(_clip._clipMin, vals[l1 * j1 + l2 * j2])));
      l1 += delta1;
    }
    l2 += delta2;
  }

  return out;
}

void float_buffer_buffer::calc_histo() {
  for (int i = 0; i < 256; i++) histo[i] = 0;

  /*
  long long count[256];
  for (int i = 0; i < 256; i++) count[i] = 0;
  io->return_clips(&bclip, &eclip);
  float j = eclip - bclip;
  int b;
  for (long long i = 0; i < n123_view; i++) {
    b = (int)(255 * (fbuf[i] - bclip) / j + .5);
    if (b < 0)
      b = 0;
    else if (b > 255)
      b = 255;
    count[b]++;
  }
  long long mym = 0;
  for (int i = 0; i < 256; i++) {
    if (count[i] > mym) mym = count[i];
  }
  for (int i = 0; i < 256; i++) {
    histo[i] = (float)count[i] / (float)mym;
  }
  */
}

float float_buffer_buffer::get_value(std::shared_ptr<orient_cube> pos) {
  std::vector<int> nw(_ndim, 1), fw(_ndim), jw(_ndim, 1);

  for (int idim = 0; idim < _ndim; idim++) {
    fw[idim] = pos->loc[idim];
  }
  float pt;
  _file->readFloatWindow(nw, fw, jw, &pt);

  return pt;
}
