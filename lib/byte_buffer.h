#pragma once
#include "buffer.h"
namespace SEP {
class byte_buffer : public buffer {
public:
  byte_buffer(){};

  byte_buffer(std::shared_ptr<SEP::paramObj> p,
              std::shared_ptr<SEP::hypercube> h,
              std::shared_ptr<SEP::io_func> i, int in, std::vector<int> &nw,
              std::vector<int> &fw);

  virtual ~byte_buffer(){};
  std::shared_ptr<byteTensor2D> getCharData(int n,
                                            std::shared_ptr<longTensor2D> ind);

  virtual std::shared_ptr<floatTensor2D>

  getFloatData(std::shared_ptr<longTensor2D> mp);
  std::shared_ptr<byteTensor2D> getCharData(std::shared_ptr<longTensor2D> mp);
  virtual std::shared_ptr<byteTensor2D>
  getCharData(std::shared_ptr<SEP::orient_cube> pos, int iax1, int f1, int e1,
              int iax2, int f2, int e2) override;
  virtual std::shared_ptr<floatTensor2D>
  getFloatData(std::shared_ptr<SEP::orient_cube> pos, int iax1, int f1, int e1,
               int iax2, int f2, int e2) override;
  void read_buffer(std::vector<int> &nwin, std::vector<int> &fwin,
                   std::vector<int> &nwout, std::vector<int> &fwout, int ndim,
                   std::vector<int> &nloop);
  virtual void calc_histo();
  virtual float get_value(std::shared_ptr<SEP::orient_cube> pos);

private:
  std::vector<unsigned char> cbuf;
};
} // namespace SEP
