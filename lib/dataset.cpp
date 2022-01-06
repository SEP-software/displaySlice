#include "dataset.h"
#include <stdlib.h>
#include <xtensor/xview.hpp>

#include "math.h"
using namespace SEP;

int dataset::check_load_buffer(std::shared_ptr<orient_cube> pos, const int iax1,
                               const int iax2) {
  for (int i = 0; i < (int)buf.size(); i++) {
    if (buf[i]->hold_slice(pos, iax1, iax2, data_contains)) {
      return i;
    }
  }
  if ((int)buf.size() == nmax_buf)
    delete_dataset(pos, iax1, iax2);

  buf.push_back(create_buffer(pos, iax1, iax2));
  return (int)buf.size() - 1;
}
void dataset::build_conv() { // Handle
  float bpct = 0., bclip = 1, epct, eclip, f;
  for (int ipt = 0; ipt < (int)clip.size(); ipt++) {
    epct = clip[ipt].pct * 255;
    eclip = clip[ipt].clip * 255;
    for (int i = std::max(0, (int)(bpct + .5));
         i <= std::min(255, (int)(epct + .5)); i++) {
      f = ((float)i - bpct) / (epct - bpct);
      conv[i] = (int)(((1. - f) * bclip + (f)*eclip) + .5);
    }
    bpct = epct;
    bclip = eclip;
  }
  epct = 255;
  eclip = 254;
  for (int i = (int)(bpct + .5); i <= (int)(.5 + epct); i++) {
    f = ((float)i - bpct) / (epct - bpct);
    if (epct - bpct < .001)
      f = 1.;
    conv[i] = (int)(((1. - f) * bclip + (f)*eclip) + .5);
  }
}
void dataset::set_basics(const std::string &titl, const std::string &nm,
                         std::shared_ptr<hypercube> g,
                         std::shared_ptr<io_func> i,
                         std::shared_ptr<paramObj> p, int in, int im) {
  if (titl == "") {
    ;
  }

  data_type = i->get_data_type();
  // name = QFileInfo(QString::fromStdString(nm)).baseName().toStdString();
  name = nm;
  title = name;
  if (title.length() < 2)
    title = nm;
  if (title.length() < 20)
    title_short = title;
  else {
    title_short = title.erase(0, title.length() - 20);
  }
  io = i;

  par = p;
  inum = in;

  values = par->getString(std::string("values") + std::to_string(inum),
                          std::string(" "));
  nmax_buf = im;
  grid = g;
  pickable = true;
  restrict = "NONE";
  moveable = true;
  show_picks = true;
  update_pick = false;

  build_conv();

  for (int i = 0; i < 8; i++) {
    data_contains[i] = true;
    display_axis[i] = true;
  }
  for (int i = 0; i < 5; i++)
    axes_display.push_back(-1);

  set_contains();
  bcolor = "none";
  ecolor = "none";
  if (!match_grid()) {
    fprintf(stderr, "FILE=%s \n", nm.c_str());
    fprintf(stderr, "description does not match grid ");
  }
}

std::vector<float>
dataset::return_axis_value(int iax, std::vector<long long> positions) {
  std::vector<float> out;
  for (int i = 0; i < (int)positions.size(); i++)
    out.push_back(io->get_header_num(positions[i], axes_display[iax]));
  return out;
}

void dataset::set_display_axis(int iax, const std::string &x) {
  if (x != "grid")
    axes_display[iax] = io->return_header_index(x);
}

std::string dataset::return_axis_name(int iax) {
  if (axes_display[iax] < 0) {
    std::string y;
    y = grid->getAxis(iax + 1).label;
    return y;
  }
  return io->return_key_name(axes_display[iax]);
}

bool dataset::match_grid() {
  std::shared_ptr<hypercube> iod = io->return_hyper();

  bool valid = true;
  for (int i = 0; i < 8; i++) {
    if (data_contains[i]) {
      float bg, eg, bd, ed;

      axis ga = grid->getAxis(i + 1);
      axis da = iod->getAxis(i + 1);
      if (ga.n > 1) {
        bg = ga.o;
        eg = ga.o + ga.d * (ga.n - 1);
        bd = da.o;
        ed = da.o + da.d * (da.n - 1);

        if (((bg - bd) / ga.d) > .01) {
          fprintf(stderr,
                  "Axis %d data axis (%f) begins before grid axis (%f) \n",
                  i + 1, bd, bg);
          valid = false;
        }
        if (((ed - eg) / ga.d) > .01) {
          fprintf(stderr, "Axis %d data axis (%f) ends after grid axis (%f) \n",
                  i + 1, ed, eg);
          valid = false;
        }
      }
    }
  }
  return valid;
}
void dataset::set_contains() {
  for (int i = 0; i < 8; i++) {
    data_contains[i] = true;
    display_axis[i] = true;
  }
}

std::shared_ptr<byteTensor2D>
dataset::getCharData(std::shared_ptr<orient_cube> pos, int iax1, int iax2,
                     int n, long long *index) {
  std::vector<int> nloc = return_io_hyper()->getNs();
  int ibuf = check_load_buffer(pos, iax1, iax2);
  std::shared_ptr<byteTensor2D> cbuf =
      buf[ibuf]->getCharData(pos, iax1, 0, nloc[iax1], iax2, 0, nloc[iax2]);

  std::vector<int> ns = cbuf->getHyper()->getNs();
  auto bufA = xt::view(cbuf->mat, xt::all(), xt::all());

  for (int i2 = 0; i2 < ns[1]; i2++)
    for (int i1 = 0; i1 < ns[0]; i1++)
      bufA(i2, i1) = conv[bufA(i2, i1)];

  return cbuf;
}

std::shared_ptr<byteTensor2D>
dataset::getCharData(std::shared_ptr<orient_cube> pos, int iax1, int f1, int e1,
                     int iax2, int f2, int e2) {
  int ibuf = check_load_buffer(pos, iax1, iax2);

  std::shared_ptr<byteTensor2D> cbuf =
      buf[ibuf]->getCharData(pos, iax1, f1, e1, iax2, f2, e2);
  auto bufA = xt::view(cbuf->mat, xt::all(), xt::all());

  for (int i2 = 0; i2 < abs(e1 - f1); i2++)
    for (int i1 = 0; i1 < abs(e1 - f1); i1++) {
      int tmp = bufA(i2, i1);
      std::cerr<<i1<<" "<<i2<<" "<<tmp<<std::endl;
      bufA(i2, i1) = conv[tmp];
    }

  return cbuf;
}
std::shared_ptr<floatTensor2D>
dataset::getFloatData(std::shared_ptr<orient_cube> pos, int iax1, int f1,
                      int e1, int iax2, int f2, int e2) {
  int ibuf = check_load_buffer(pos, iax1, iax2);
  return buf[ibuf]->getFloatData(pos, iax1, f1, e1, iax2, f2, e2);
}
float dataset::get_value(std::shared_ptr<orient_cube> pos) {
  if ((int)buf.size() == 0)
    return 0.;

  int ibuf = find_buffer(pos);
  return buf[ibuf]->get_value(pos);
}

long long dataset::get_trace_num(std::shared_ptr<orient_cube> pos) {
  long long ret = 0;
  long long block = 1;
  int n[8];
  pos->get_ns(n);
  for (int i = 1; i < 8; i++) {
    if (data_contains[i]) {
      ret = ret + block * (long long)pos->get_loc(i);
      block = block * (long long)n[i];
    }
  }
  return ret;
}
void dataset::snap_location(float *floc, int single, const std::string &stype) {
  if ((int)buf.size() == 0)
    return;
  if (stype == "no")
    return;
  std::shared_ptr<orient_cube> pos(new orient_cube(grid));
  for (int i = 0; i < 8; i++)
    pos->set_pos(i, floc[i]);

  int ibuf = find_buffer(pos);
  int ilocs[8];
  pos->get_locs(ilocs);
  axis ax = grid->getAxis(single + 1);

  int ichoose = 0;
  float val = buf[ibuf]->get_value(pos);
  int iloc = pos->get_loc(single);
  for (int i = 0, j = -5; i < 5; i++, j++) {
    if (iloc + j >= 0 && iloc + j < ax.n) {
      pos->set_loc(single, iloc + j);
      float test = buf[ibuf]->get_value(pos);
      if (stype == "low" && test < val) {
        val = test;
        ichoose = j;
      } else if (stype == "high" && test > val) {
        val = test;
        ichoose = j;
      } else if (stype == "zero" && fabs(test) < fabs(val)) {
        val = test;
        ichoose = j;
      }
    }
  }
  pos->set_loc(single, ichoose + iloc);
  floc[single] = pos->get_pos(single);
}

void dataset::delete_dataset(std::shared_ptr<orient_cube> pos, const int iax1,
                             const int iax2) {
  if (pos == 0 || iax1 == 0 || iax2 == 0) {
    ;
  }
  buf.erase(buf.begin());
}
int dataset::find_buffer(std::shared_ptr<orient_cube> pos) {
  float loc[8];
  pos->get_poss(loc);

  for (int i = 0; i < (int)buf.size(); i++) {
    if (buf[i]->hold_point(loc, data_contains)) {
      return i;
    }
  }
  par->error("Internal error finding point in buffer\n");
  return 0;
}
std::string dataset::return_clip() {
  std::string buf;
  if (clip.size() == 0)
    return "";
  buf = std::to_string(clip[0].pct) + ":" + std::to_string(clip[0].clip);
  for (int i = 1; i < (int)clip.size(); i++) {
    buf = buf + ":" + std::to_string(clip[i].pct) + ":" +
          std::to_string(clip[i].clip);
  }
  return buf;
}
std::string dataset::return_histogram() {
  std::string b;
  std::vector<float> histo;
  if ((int)buf.size() == 0) {
    histo = std::vector<float>(256);
    for (int i = 0; i < 256; i++)
      histo[i] = 0.;
  } else
    histo = buf[0]->returnHisto();
  b = std::to_string(histo[0]);
  for (int i = 1; i < 256; i++) {
    b = b + ":" + std::to_string(histo[i]);
  }
  return b;
}
void dataset::get_pt(int ipt, float *pct, float *clp) {
  *pct = clip[ipt].pct;
  *clp = clip[ipt].clip;
}
void dataset::del_pt(int ipt) {
  if (clip.size() > 1) {
    for (int i = ipt; i < (int)clip.size() - 1; i++)
      clip[i] = clip[i + 1];
    clip.pop_back();
  } else
    clip.clear();
  build_conv();
}
void dataset::clear_pts() {
  clip.clear();
  build_conv();
}
int dataset::add_pt(float pct, float clp) {
  int iin = 0;

  if (clip.size() == 0)
    clip.push_back(bar_pt(pct, clp));
  else {
    if (clip[0].pct > pct)
      iin = 0;
    else if (clip[clip.size() - 1].pct <= pct) {
      iin = clip.size();

    } else {
      for (int i = 0; i < (int)clip.size() - 1; i++) {
        if (pct > clip[i].pct && pct <= clip[i + 1].pct)
          iin = i + 1;
      }
    }

    clip.push_back(bar_pt(0., 0.));

    for (int i = clip.size() - 2; i != iin - 1; i--) {
      clip[i + 1].pct = clip[i].pct;
      clip[i + 1].clip = clip[i].clip;
    }

    clip[iin].pct = pct;
    clip[iin].clip = clp;
  }
  build_conv();
  return iin;
}
std::string dataset::set_pts(float *pcts) {
  std::string cur = return_clip();
  clip.clear();
  for (int i = 0; i < 256; i++)
    clip.push_back(bar_pt((float)i / 255., pcts[i]));
  build_conv();
  return cur;
}

std::string dataset::validate_view(const std::string &nm,
                                   std::shared_ptr<orient_cube> pos) {
  // pos->get_axes(as);
  std::vector<int> n = return_io_hyper()->getNs();

  int order[8];
  pos->get_orders(order);
  bool valid[8];
  for (int i = 0; i < 8; i++) {
    valid[i] = valid_display(i, order, n);
  }

  // See if requested view works
  if (nm == "CUBE" || nm == "CUT" || nm == "THREE" || nm == "MULTI") {
    if (valid[0] && valid[1] && valid[2])
      return nm;
  } else {
    if (nm == "FRONT" && valid[0] && valid[1])
      return nm;
    else if (nm == "SIDE" && valid[0] && valid[2])
      return nm;
    else if (nm == "TOP" && valid[1] && valid[2])
      return nm;
  }
  // Now choose another view
  if (valid[0]) {
    if (valid[1])
      return "FRONT";
    else if (valid[2])
      return "SIDE";
  }
  if (valid[1] && valid[2])
    return "TOP";
  // At this stage just find the first two valid axes
  int iv1 = -1, iv2 = -1;
  for (int i = 0; i < 8; i++) {
    if (valid[i]) {
      if (iv1 < 0)
        iv1 = i;
      else if (iv2 < 0)
        iv2 = i;
    }
  }

  // Time to mess with the order
  int iold = order[0];
  order[0] = iv1;
  order[iv1] = iold;
  iold = order[1];
  order[1] = iv2;
  order[iv2] = iold;
  for (int i = 0; i < 8; i++)
    order[i] = order[i] + 1;

  pos->set_orders(order);

  return std::string("FRONT");
}

bool dataset::valid_display(int iax, int *order, std::vector<int> ns) {
  if (ns[order[iax]] > 1 && display_axis[order[iax]])
    return true;
  return false;
}
