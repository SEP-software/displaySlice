#include <string>
#include <vector>
#ifndef PAIR_NEW_H
#define PAIR_NEW_H 1
namespace SEP {
class pair_new {
 public:
  pair_new(int ix, int iy);
  pair_new(int ix, int iy, const std::string &t);

  int x, y;
  int type, extra;
  std::string txt;
};

class pairs_new {
 public:
  pairs_new() {}
  void add_point(int x, int y) { vals.push_back(pair_new(x, y)); }
  void add_point(int x, int y, const std::string &t) {
    vals.push_back(pair_new(x, y, t));
  }

  void sort1();
  void sort2();
  void clear() { vals.clear(); }
  void info(char *str, int n);
  int size() { return (int)vals.size(); }
  void build_int_line(int f, int n, float *ar);
  std::vector<pair_new> vals;
};
}  // namespace SEP
bool comp1(SEP::pair_new x, SEP::pair_new y);
bool comp2(SEP::pair_new x, SEP::pair_new y);

#endif
