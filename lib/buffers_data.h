#pragma once
#include "dataset.h"
namespace SEP {
class buffers_data : public dataset {
 public:
  buffers_data() { ; }
  ~buffers_data(){};
  int _mem;
};

class buffers_data_float : public buffers_data {
 public:
  buffers_data_float(){};
  buffers_data_float(const std::string& title, const std::string& nm,
                     std::shared_ptr<SEP::hypercube> g,
                     std::shared_ptr<SEP::io_func> i,
                     std::shared_ptr<SEP::paramObj> p, const int inum,
                     const int mem);
  std::shared_ptr<buffer> create_buffer(std::shared_ptr<SEP::orient_cube> pos,
                                        int iax1, int iax2);
};

class buffers_data_byte : public buffers_data {
 public:
  buffers_data_byte(){};
  buffers_data_byte(const std::string& title, const std::string& nm,
                    std::shared_ptr<SEP::hypercube> g,
                    std::shared_ptr<SEP::io_func> i,
                    std::shared_ptr<SEP::paramObj> p, const int inum,
                    const int mem);
  std::shared_ptr<buffer> create_buffer(std::shared_ptr<SEP::orient_cube> pos,
                                        int iax1, int iax2) override;
};
}  // namespace SEP
