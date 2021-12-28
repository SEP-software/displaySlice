#pragma once
#include "byte2DReg.h"
#include "buffer.h"
#include "buffers.h"
#include "buffersRegFile.h"
#include "simpleMemoryLimit.h"
namespace SEP {
class byte_buffer_buffer : public buffer {
 public:
  byte_buffer_buffer(){};

  byte_buffer_buffer(std::shared_ptr<SEP::paramObj> p,
                     std::shared_ptr<SEP::hypercube> h,
                     std::shared_ptr<io_func> i, int in, std::vector<int> &nw,
                     std::vector<int> &fw, const int mem);

  unsigned char *get_char_data(std::shared_ptr<SEP::orient_cube> pos, int iax1,
                               int f1, int e1, int iax2, int f2, int e2);
  unsigned char *get_char_data(int n, long long *ind);

  float *get_float_data(std::shared_ptr<SEP::orient_cube> pos, int iax1, int f1,
                        int e1, int iax2, int f2, int e2);

  virtual void calc_histo();
  virtual float get_value(std::shared_ptr<SEP::orient_cube> pos);

 private:
  std::shared_ptr<buffersRegFile> _file;
  int _ndim;
  float _minV, _maxV;
  float _delta;
  // float _minV = 1e31, _maxV = -1e31;
};
}  // namespace SEP
