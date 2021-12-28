#include <math.h>
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <util.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "percentile.h"

using namespace SEP;
std::string util::string_from_int_array(const int n, const int *ar) {
  std::string st = std::to_string(ar[0]);
  for (int i = 1; i < n; i++) st = st + ":" + std::to_string(ar[i]);
  return st;
}
std::string util::string_from_float_array(const int n, const float *ar) {
  std::string st = std::to_string(ar[0]);
  for (int i = 1; i < n; i++) st = st + ":" + std::to_string(ar[i]);
  return st;
}
int *util::int_array_from_string(const std::string &str) {
  std::vector<std::string> ar = split(str, ':');
  size_t nelem = ar.size();
  int *buf = new int[nelem];
  for (int i = 0; i < nelem; i++) buf[i] = atoi(ar[i].c_str());
  return buf;
}
std::vector<int> util::int_vector_from_string(const std::string &str) {
  std::vector<std::string> ar = split(str, ':');

  size_t nelem = ar.size();
  std::vector<int> buf(nelem);
  for (int i = 0; i < nelem; i++) buf[i] = atoi(ar[i].c_str());
  return buf;
}
float *util::float_array_from_string(const std::string &str) {
  std::vector<std::string> ar = split(str, ':');
  size_t nelem = ar.size();
  float *buf = new float[nelem];
  for (int i = 0; i < nelem; i++) buf[i] = atof(ar[i].c_str());
  return buf;
}

void util::set_float_clip(float *buf, int inum, long long nelem, float *bc,
                          float *ec, int swap) {
  float bclip, eclip, clip, bpclip, epclip, pclip, minv, maxv;

  bclip = par->getFloat(std::string("bclip") + std::to_string(inum), -98765.);
  eclip = par->getFloat(std::string("eclip") + std::to_string(inum), -98765.);
  clip = par->getFloat(std::string("clip") + std::to_string(inum), -98765.);
  bpclip = par->getFloat(std::string("bpclip") + std::to_string(inum), -1.);
  epclip = par->getFloat(std::string("epclip") + std::to_string(inum), -1.);
  pclip = par->getFloat(std::string("pclip") + std::to_string(inum), -1.);
  if (bclip != -98765) {
    if (eclip == -98765)
      par->error("If you provide bclip you must also provide eclip");
    minv = bclip;
    maxv = eclip;
  } else if (clip != -98765) {
    minv = -clip;
    maxv = clip;
  } else {
    percentile p = percentile();
    int j = 1;
    int nsmall;
    int sub = 1;
    j = sub;
    nsmall = nelem / sub;
    while (nsmall > 10000000) {
      sub = sub + 2;
      j = sub * j;
      nsmall = nsmall / sub;
    }

    float *tempf = new float[nsmall];
    int ii = 0, i = 0;
    for (i = 0; i < nelem && ii < nsmall; i += j, ii++) tempf[ii] = buf[i];
    //    memcpy((void*)tempf,(const void*) buf,sizeof(float)*nelem);

    if (swap == 1) swap_float_bytes(nsmall, tempf);
    if (bpclip != -1 || epclip != -1) {
      if (bpclip == -1) bpclip = 0;
      if (epclip == -1) epclip = 100;
      minv = p.find(tempf, nelem, ((int)(nsmall * (float)(bpclip / 100.))));
      maxv = p.find(tempf, nelem, ((int)(nsmall * (float)(epclip / 100.))));
    } else {
      if (pclip == -1.) pclip = 99;
      int pos = (int)nsmall * (float)(pclip / 100.);
      if (pos >= nsmall) pos = nsmall - 1;
      maxv = p.find_abs(tempf, nsmall, pos);
      minv = -maxv;
    }
    delete[] tempf;
  }
  *bc = minv;
  *ec = maxv;
}

void util::convert_to_byte(float *fbuf, long long foff, unsigned char *cbuf,
                           long long coff, long long nelem, float bclip,
                           float eclip) {
  int j;
  float range = eclip - bclip;
  for (long long i = 0; i < nelem; i++) {
    j = (int)(255 * (fbuf[i + foff] - bclip) / range);

    if (j < 0) j = 0;
    if (j > 255) j = 255;
    cbuf[i + coff] = j;
  }
}
void util::swap_float_bytes(const int n, float *buf) {
  // int *tni4;
  for (int i = 0; i < n; i++) {
    float *tnf4 = &buf[i];
    int *tni4 = (int *)tnf4;
    *tni4 = (((*tni4 >> 24) & 0xff) | ((*tni4 & 0xff) << 24) |
             ((*tni4 >> 8) & 0xff00) | ((*tni4 & 0xff00) << 8));
  }
}
void util::convert_to_float(float *fbuf, long long foff, unsigned char *cbuf,
                            long long coff, long long nelem, float bclip,
                            float eclip) {
  float o = bclip;
  float d = (eclip - bclip) / 256;

  for (long long i = 0; i < nelem; i++) {
    fbuf[i + foff] = cbuf[i + coff] * d + o;
  }
}
std::vector<std::string> util::split(const std::string &s, const char delim) {
  auto i = 0;
  auto pos = s.find(delim);
  std::vector<std::string> v;
  while (pos != std::string::npos) {
    v.push_back(s.substr(i, pos - i));
    i = ++pos;
    pos = s.find(delim, pos);

    if (pos == std::string::npos) v.push_back(s.substr(i, s.length()));
  }
  return v;
}

clips::clips(std::shared_ptr<SEP::paramObj> pars, const int inum) {
  float bclip =
      pars->getFloat(std::string("bclip") + std::to_string(inum), -98765.);
  float eclip =
      pars->getFloat(std::string("eclip") + std::to_string(inum), -98765.);
  float clip =
      pars->getFloat(std::string("clip") + std::to_string(inum), -98765.);
  float bpclip =
      pars->getFloat(std::string("bpclip") + std::to_string(inum), -1.);
  float epclip =
      pars->getFloat(std::string("epclip") + std::to_string(inum), -1.);
  float pclip =
      pars->getFloat(std::string("pclip") + std::to_string(inum), -1.);

  if (bclip != -98765) {
    if (eclip == -98765)
      pars->error("If you provide bclip you must also provide eclip");
    _clipMin = bclip;
    _clipMax = eclip;
    _hardClip = true;
  } else if (clip != -98765) {
    _clipMin = -clip;
    _clipMax = clip;
    _hardClip = true;
  }
  _pctMin = _pctMax = 100;

  bool set = false;
  if (fabsf(epclip + 1) > .1) {
    _pctMax = epclip;
    set = true;
  }
  if (fabsf(bpclip + 1) > .1) {
    _pctMin = 100 - _pctMin;
    set = true;
  }
  if (fabsf(pclip - 1.) < .1) pclip = 99.;
  _pctMin = _pctMax = pclip;
}
void clips::updateClips(const float minv, const float maxv) {
  _minV = std::min(minv, _minV);
  _maxV = std::max(maxv, _maxV);
  if (!_hardClip) {
    float delta = _maxV - _minV;
    _clipMin = _minV + delta * _pctMin / 100.;
    _clipMax = _maxV - delta * _pctMin / 100.;
  }
}