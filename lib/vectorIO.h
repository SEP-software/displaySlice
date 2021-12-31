#ifndef VECTOR_IO_H
#define VECTOR_IO_H 1
#include "dictParams.h"
#include "floatHyper.h"
#include "ioModes.h"
#include "io_func.h"
namespace SEP {
class floatVectorIO : public io_func {
public:
  floatVectorIO(std::string fileName, std::shared_ptr<floatHyper> vals,
                std::shared_ptr<dictParams> pars);
  void windowIt(std::vector<int> &nw, std::vector<int> &fw, float *buf);

  virtual void read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                float *buf);
  virtual void read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                               unsigned char *buf);
  std::shared_ptr<genericReg> getFile() { return nullptr; }

private:
  std::vector<int> _ns;
  std::shared_ptr<floatHyper> _vec;
};

class byteVectorIO : public io_func {
public:
  byteVectorIO(std::string fileName, std::shared_ptr<byteHyper> vals,
               std::shared_ptr<dictParams> pars);
  void windowIt(std::vector<int> &nw, std::vector<int> &fw, unsigned  char *buf);

  virtual void read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                float *buf);
  virtual void read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                               unsigned char *buf);
  std::shared_ptr<genericReg> getFile() { return nullptr; }

private:
  std::vector<int> _ns;
  std::shared_ptr<byteHyper> _vec;
};
class doubleVectorIO : public io_func {
public:
  doubleVectorIO(std::string fileName, std::shared_ptr<doubleHyper> vals,
                 std::shared_ptr<dictParams> pars);
  void windowIt(std::vector<int> &nw, std::vector<int> &fw, double *buf);

  virtual void read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                float *buf);
  virtual void read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                               unsigned char *buf);
  std::shared_ptr<genericReg> getFile() { return nullptr; }

private:
  std::vector<int> _ns;
  std::shared_ptr<doubleHyper> _vec;
};

} // namespace SEP

#endif
