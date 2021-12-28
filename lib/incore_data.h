#pragma once
#include "dataset.h"
namespace SEP {
class incore_data : public dataset {
 public:
  incore_data() { ; }
  ~incore_data(){};

  std::shared_ptr<SEP::hypercube> datas;
  int mmap_it;
};

class incore_data_float : public incore_data {
 public:
  incore_data_float(){};
  incore_data_float(const std::string &title, const std::string &nm,
                    std::shared_ptr<SEP::hypercube> g,
                    std::shared_ptr<io_func> i, std::shared_ptr<paramObj> p,
                    int in, int mmap, int im = 1);
  ~incore_data_float() { clean_bufs(); }
  virtual std::shared_ptr<buffer> create_buffer(
      std::shared_ptr<SEP::orient_cube> pos, int iax1, int iax2) override;
};

class incore_data_byte : public incore_data {
 public:
  incore_data_byte(){};
  incore_data_byte(const std::string &title, const std::string &nm,
                   std::shared_ptr<SEP::hypercube> g,
                   std::shared_ptr<io_func> i, std::shared_ptr<paramObj> p,
                   int in, int im = 1);
  ~incore_data_byte() { clean_bufs(); }
  virtual std::shared_ptr<buffer> create_buffer(
      std::shared_ptr<SEP::orient_cube> pos, int iax1, int iax2) override;
};
}  // namespace SEP
