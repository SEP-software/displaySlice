#ifndef IO_FUNC_H
#define IO_FUNC_H 1
#include <memory>
#include "hypercube.h"
#include "util.h"

namespace SEP {
class io_func {
 public:
  io_func() { hyper = 0; };
  void set_basics(std::shared_ptr<util> pars,
                  std::shared_ptr<SEP::hypercube> h);

  virtual void read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                float *buf);
  virtual void read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                               unsigned char *buf);
  virtual void convert_to_local(std::vector<int> &nwin, std::vector<int> &fwin,
                                std::vector<int> &nwout,
                                std::vector<int> &fwout);
  void return_clips(float *b, float *e) {
    *b = bclip;
    *e = eclip;
  }
  virtual void read_headers(){};
  std::shared_ptr<SEP::hypercube> return_hyper();
  bool not_byte() { return non_byte; }
  bool io_changed() { return changed; }
  void set_clip(float *dat, int inum, long long nelem, int swap = 0) {
    par->set_float_clip(dat, inum, nelem, &bclip, &eclip, swap);
  }

  void convert_to_byte(float *flt, long long foff, unsigned char *tbuf,
                       long long coff, long long n123) {
    par->convert_to_byte(flt, foff, tbuf, coff, n123, bclip, eclip);
  }
  virtual std::vector<std::string> return_header_keys() {
    std::vector<std::string> x;
    return x;
  }
  virtual int return_header_index(const std::string &x) {
    if (x == "") {
      ;
    }
    return -1;
  }
  virtual void get_grid_loc(int *in) {
    if (in == 0) {
      ;
    };
  }
  virtual void get_io_loc(int *in) {
    if (in == 0) {
      ;
    };
  }

  virtual std::vector<std::string> get_header_val(long long inum) {
    if (inum == 0) {
      ;
    };
    std::vector<std::string> send;
    return send;
  }
  virtual float get_header_num(long long inum, int index) {
    if (inum == 0 && index == 0) {
      ;
    }
    return 0.;
  }
  virtual std::string return_key_name(int index) {
    std::string x;
    if (index == 0) {
      ;
    }

    return x;
  }
  std::string get_data_type() { return data_type; }
  virtual void add_pt_action(int *iloc) {
    if (iloc == 0) {
      ;
    };
  }
  virtual void del_pt_action(int *iloc) {
    if (iloc == 0) {
      ;
    };
  }
  bool am_region() { return isregion; }

  std::shared_ptr<SEP::hypercube> hyper;
  std::shared_ptr<util> par;
  float bclip, eclip;
  bool non_byte;
  bool changed;
  bool isregion;
  std::string data_type;
};
}  // namespace SEP
#endif
