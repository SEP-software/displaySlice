#ifndef UTIL_H
#define UTIL_H 1
#include "paramObj.h"
#include <memory>
#include <vector>
namespace SEP {

class clips {
public:
  clips() { ; }
  clips(std::shared_ptr<SEP::paramObj> pars, const int inum);
  void updateClips(const float minv, const float maxv);
  float _clipMin, _clipMax;

private:
  float _pctMin, _pctMax;
  float _minV, _maxV;
  bool _hardClip = false;
};

class util {
public:
  util() { ; }
  util(std::shared_ptr<SEP::paramObj> pars) { par = pars; }
  static std::string string_from_int_array(const int n, const int *ar);
  static std::string string_from_float_array(const int n, const float *ar);
  static int *int_array_from_string(const std::string &str);
  static std::vector<int> int_vector_from_string(const std::string &com);
  static float *float_array_from_string(const std::string &str);

  void swapFloatBytes(const int n, float *buf);
  static std::vector<std::string> split(const std::string &s, const char delim);

  void error(const char *e) { par->error(e); }
  void setFloatClip(float *buf, int inum, long long nelem, float &bc, float &ec,
                    int swap = 0);
  void convert_to_byte(float *fbuf, long long foff, unsigned char *cbuf,
                       long long coff, long long nelem, float bclip,
                       float eclip);
  void convert_to_float(float *fbuf, long long foff, unsigned char *cbuf,
                        long long coff, long long nelem, float bclip,
                        float eclip);

  std::shared_ptr<SEP::paramObj> par;
};
} // namespace SEP
#endif
