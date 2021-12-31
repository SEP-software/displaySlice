#pragma once

#include <string.h>
#include "hypercube.h"
#include "io_func.h"
#include "util.h"

namespace SEP {
class trace_io : public io_func {
 public:
  trace_io() { }
  ~trace_io() {
  }

  void read_block(int *nw, int *fw, unsigned char *buf, int nbytes);
  void set_trace_basics(const std::string &fl,
                        std::shared_ptr<SEP::hypercube> d, int reel_h, int head,
                        bool sw, std::shared_ptr<SEP::util> pars, int in);
  void read_block_float(int *nw, int *fw, float *buf);
  void read_block_byte(int *nw, int *fw, unsigned char *buf);
  char file[1024];
  bool swap;
  int reel_head, header, inum;
  FILE *fd;
  std::shared_ptr<SEP::hypercube> des;
  std::shared_ptr<SEP::util> pars;
  std::vector<char> header_buf;
  std::vector<std::string> headers;
  std::vector<int> hoff;
  bool loaded;
};
}  // namespace SEP
