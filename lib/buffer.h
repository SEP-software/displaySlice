#ifndef BUFFER_H
#define BUFFER_H 1
#include "hypercube.h"
#include "io_func.h"
#include "orient_cube.h"
#include "floatTensor.h"
#include "byteTensor.h"
#include "util.h"
namespace SEP {

class buffer {

public:
  buffer() { }
  void set_basics(std::shared_ptr<SEP::paramObj> p,
                  std::shared_ptr<SEP::hypercube> h, std::shared_ptr<io_func> i,
                  int in);
  void clean_up();
  virtual bool hold_slice(std::shared_ptr<orient_cube> pos, int iax1, int iax2,
                          bool *data_contains);
  virtual void read_hyper(int *nw, int *fw) {
    if (fw == 0 || nw == 0) {
      ;
    };
    _par->error("Read hyper unimplemented");
  }
  void buffer_basics(SEP::paramObj *p);
  // long long *form_index_map(std::shared_ptr<orient_cube>pos, int iax1, int
  // f1, int e1, int iax2,
  //  int f2, int e2);
  std::shared_ptr<longTensor2D> formIndexMap(std::shared_ptr<orient_cube> pos, int iax1,
                            int iax2, int f1, int e1, int f2, int e2);
  std::shared_ptr<longTensor2D> gridToIndex(std::shared_ptr < longTensor2D> buf);
  void set_hold(bool *h);
  virtual std::shared_ptr<byteTensor2D> getCharData(std::shared_ptr<orient_cube> pos,
                                       int iax1, int f1, int e1, int iax2,
                                       int f2, int e2) =0;
  virtual std::shared_ptr<floatTensor2D> getFloatData(std::shared_ptr<orient_cube> pos, int iax1,
                                int f1, int e1, int iax2, int f2, int e2) =0;

  void build_axis_map(SEP::axis ain, SEP::axis aout, int iax);
  void calc_read_loop(std::vector<int> &nwin, std::vector<int> &fwin,
                      std::vector<int> &nwout, std::vector<int> &fwout,
                      std::vector<int> &nloop, int &ndim);
  virtual void read_buffer(int *nwin, int *fwin, int *nwout, int *fwout,
                           int ndim, int *nloop) {
    if (nwin == 0 || fwin == 0 || nloop == 0 || ndim == 0 || nwout == 0 ||
        fwout == 0) {
      ;
    };
  }
  int resize_buffer(std::vector<int> &nwin, std::vector<int> &fwin,
                    std::vector<int> &nwout, std::vector<int> &fwout, int ndim,
                    long long off, unsigned char *tbuf, unsigned char *cbuf,
                    int ssz);
  void calc_get_pars(std::shared_ptr<orient_cube> pos, int iax1, int iax2,
                     long long *j1, long long *j2, long long *first);
  void window_to_local(std::vector<int> nbuf, std::vector<int> fbuf);
  virtual void calc_histo() { _par->error("Calc histo must be overriten"); }

  std::vector<float> returnHisto();
  virtual float get_value(std::shared_ptr<orient_cube> pos) {
    if (pos == 0) {
      ;
    }
    return 0.;
  }
  long long point_to_local(std::shared_ptr<position> pos);
  virtual bool hold_point(float *pos, bool *use_axis);
  void return_hold(bool *h) { memcpy(h, hold, 8 * sizeof(bool)); }

  void calc_resize(std::vector<int> &fio, std::vector<int> &nio,
                   std::vector<int> &fbuf, std::vector<int> &nbuf);

  std::shared_ptr<SEP::hypercube> hyper_io, hyper_buf;
  std::shared_ptr<io_func> io;
  std::shared_ptr<util> _util;
  std::shared_ptr<SEP::paramObj> _par;
  bool hold[8];
  bool resamp_1;
  long long skip[8];
  int loc[8];
  int inum;
  std::vector<std::vector<int>> ii;
  bool same;
  float histo[256];
  long long n123_view, n123_buf;
  float bclip, eclip;
  std::vector<SEP::axis> aios, abufs;
};
} // namespace SEP
#endif
