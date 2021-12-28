#ifndef FILE_IO_H
#define FILE_IO_H 1
#include "ioModes.h"
#include "io_func.h"
namespace SEP {
class fileIO : public io_func {
 public:
  fileIO(std::string fileName, std::shared_ptr<SEP::ioModes> io,
         std::string fileType);

  virtual void read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                float *buf);
  virtual void read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                               unsigned char *buf);
  std::shared_ptr<genericReg> getFile() { return _file; }

 private:
  std::shared_ptr<genericReg> _file;
  std::shared_ptr<genericIO> _io;
  std::shared_ptr<paramObj> _par;
  std::vector<int> _ns;
  SEP::dataType _dataType;
  std::shared_ptr<util> _util;
};
}  // namespace SEP

#endif
