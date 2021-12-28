#ifndef dataset_H
#define dataset_H 1
#include <assert.h>
#include <iostream>
#include <map>
#include "bar_pt.h"
#include "buffer.h"
#include "io_func.h"
#include "orient_cube.h"
#include "util.h"

namespace SEP {
class dataset {
 public:
  dataset() {}
  void set_basics(const std::string &titl, const std::string &name,
                  std::shared_ptr<SEP::hypercube> g, std::shared_ptr<io_func> i,
                  std::shared_ptr<SEP::paramObj> p, int in, int im = 1);
  virtual ~dataset() { clean_bufs(); }
  void clean_bufs() {
    // for(int i=0; i < (int)buf.size(); i++) delete buf[i];
    buf.clear();
  }
  int check_load_buffer(std::shared_ptr<orient_cube> pos, const int iax1,
                        const int iax2);
  virtual std::shared_ptr<SEP::hypercube> return_io_hyper() {
    return io->return_hyper();
  }
  virtual unsigned char *get_char_data(std::shared_ptr<orient_cube> pos,
                                       int iax1, int f1, int e1, int iax2,
                                       int f2, int e2);
  virtual unsigned char *get_char_data(std::shared_ptr<orient_cube> pos,
                                       int iax1, int iax2, int n,
                                       long long *index);
  virtual float *get_float_data(std::shared_ptr<orient_cube> pos, int iax1,
                                int f1, int e1, int iax2, int f2, int e2);
  virtual float get_value(std::shared_ptr<orient_cube> pos);
  // virtual std::vector<QStringList> get_header(
  //    std::shared_ptr<orient_cube> pos) {
  // long long inum = get_trace_num(pos);
  // return io->get_header(inum);
  //}
  virtual std::vector<std::string> return_header_keys() {
    return io->return_header_keys();
  }
  std::vector<std::string> virtual get_header_vals(
      std::shared_ptr<orient_cube> pos) {
    long long inum = get_trace_num(pos);
    return io->get_header_val(inum);
  }
  std::string return_axis_name(int iax);
  long long get_trace_num(std::shared_ptr<orient_cube> pos);
  void snap_location(float *floc, int single, const std::string &stype);
  int find_buffer(std::shared_ptr<orient_cube> pos);
  virtual void afterInitialization() { ; }
  virtual std::shared_ptr<buffer> create_buffer(
      std::shared_ptr<orient_cube> pos, int iax1, int iax2) = 0;
  SEP::axis get_grid_axis(int iax) { return grid->getAxis(iax); }
  SEP::axis get_io_axis(int iax) { return io->return_hyper()->getAxis(iax); }
  std::string get_bcolor() { return bcolor; }
  int getInum() { return inum; }
  std::string get_type() { return data_type; }
  std::string get_ecolor() { return ecolor; }
  void set_bcolor(const std::string &col) { bcolor = col; }
  void set_ecolor(const std::string &col) { ecolor = col; }
  void set_display_axis(int iax, const std::string &x);

  int return_axes_display(int iax) { return axes_display[iax]; }
  std::vector<float> return_axis_value(int iax,
                                       std::vector<long long> positions);

  std::string data_type;
  std::string return_histogram();
  void build_conv();
  int add_pt(float pct, float clp);
  void del_pt(int ipt);
  std::string return_clip();
  std::string data_updated() { return "none"; }
  std::string get_name() { return name; }
  std::shared_ptr<SEP::hypercube> return_grid() { return grid; }
  virtual float get_minval() { return buf[0]->bclip; }
  virtual float get_maxval() { return buf[0]->eclip; }
  std::string get_values() { return values; }
  std::string get_title_short() { return title_short; }
  virtual void delete_dataset(std::shared_ptr<orient_cube> pos, const int iax1,
                              const int iax2);
  bool valid_display(int iax, int *order, std::vector<int> ns);
  virtual bool match_grid();
  virtual void set_contains();
  std::string validate_view(const std::string &nm,
                            std::shared_ptr<orient_cube> pos);
  void get_pt(int ipt, float *pct, float *clp);
  bool get_data_contain(int iax) { return data_contains[iax]; }
  virtual void add_pt_action(int *iloc) { io->add_pt_action(iloc); }
  virtual void del_pt_action(int *iloc) { io->del_pt_action(iloc); }
  virtual void get_io_loc(int *iloc) { io->get_io_loc(iloc); }
  virtual bool am_region() { return io->am_region(); }
  void buildConv();
  std::string set_pts(float *pcts);
  size_t getBufSize() { return buf.size(); }
  void clear_pts();
  std::shared_ptr<SEP::hypercube> grid;
  std::vector<std::shared_ptr<buffer>> buf;
  bool data_contains[8], display_axis[8];
  std::shared_ptr<io_func> io;
  std::shared_ptr<SEP::paramObj> par;
  int nmax_buf, inum;
  unsigned char conv[256];
  std::vector<bar_pt> clip;
  std::vector<int> axes_display;
  std::string name;
  bool pickable;
  std::string restrict;
  long long n123;
  bool moveable;
  std::string values;
  std::string bcolor, ecolor;
  bool show_picks;
  bool update_pick;
  std::string title, title_short;
  std::vector<int> axes_dispay;
};

}  // namespace SEP

#endif
