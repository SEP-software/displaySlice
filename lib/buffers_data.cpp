#include "buffers_data.h"
#include "byte_buffer.h"
#include "byte_buffer_buffer.h"
#include "float_buffer_buffer.h"
using namespace SEP;
buffers_data_float::buffers_data_float(const std::string& title,
                                       const std::string& nm,
                                       std::shared_ptr<SEP::hypercube> g,
                                       std::shared_ptr<SEP::io_func> i,
                                       std::shared_ptr<SEP::paramObj> p,
                                       const int inum, const int mem) {
  set_basics(title, nm, g, i, p, inum, 1);
  _mem = mem;

  // datas = io->return_hyper();
}

std::shared_ptr<buffer> buffers_data_float::create_buffer(
    std::shared_ptr<SEP::orient_cube> pos, int iax1, int iax2) {
  std::vector<int> nw(8, 1), fw(8, 1);
  std::vector<axis> axes = grid->getAxes(8);
  for (int i = 0; i < 8; i++) {
    nw[i] = axes[i].n;
    fw[i] = 0;
  }

  if (iax1 == 0 && iax2 == 0 && pos == 0) {
    ;
  }

  std::shared_ptr<float_buffer_buffer> b(
      new float_buffer_buffer(par, grid, io, inum, nw, fw, _mem));
  return b;
}

buffers_data_byte::buffers_data_byte(const std::string& title,
                                     const std::string& nm,
                                     std::shared_ptr<SEP::hypercube> g,
                                     std::shared_ptr<SEP::io_func> i,
                                     std::shared_ptr<SEP::paramObj> p,
                                     const int inum, const int mem) {
  set_basics(title, nm, g, i, p, inum, 1);
  _mem = mem;
  // datas = io->return_hyper();
}
std::shared_ptr<buffer> buffers_data_byte::create_buffer(
    std::shared_ptr<orient_cube> pos, int iax1, int iax2) {
  std::vector<int> nw(8, 1), fw(8, 1);
  std::vector<axis> axes = grid->getAxes(8);
  for (int i = 0; i < 8; i++) {
    nw[i] = axes[i].n;
    fw[i] = 0;
  }

  if (iax1 == 0 && iax2 == 0 && pos == 0) {
    ;
  }

  std::shared_ptr<byte_buffer_buffer> b(
      new byte_buffer_buffer(par, grid, io, inum, nw, fw, _mem));
  return b;
}