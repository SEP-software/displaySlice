#ifndef PICK_BUCKET_H
#define PICK_BUCKET_H 1
#include <map>
#include <set>
#include <string>
#include <vector>
//#include "my_colors.h"
#include "pick_new.h"
#include "position.h"
namespace SEP {
class pick_bucket {
 public:
  pick_bucket() { setit = 6; }
  pick_bucket(std::shared_ptr<position> pos);
  std::map<int, std::shared_ptr<picks_vec>> return_picks_from_range(
      int ip, std::vector<int> keys, int *iloc);
  std::shared_ptr<pick_new> add_pick(int *iloc, long long p, int te,
                                     const std::string &col, int ex,
                                     const std::string &txt);
  std::shared_ptr<pick_new> add_pick(int *iloc, int te, const std::string &col,
                                     int ex, const std::string &t) {
    long long i = loc_to_index(iloc);
    return add_pick(iloc, i, te, col, ex, t);
  }
  long long loc_to_index(int *iloc) {
    long long ind = 0;
    for (int i = 0; i < 8; i++) {
      if (dig[i]) ind += block[i] * iloc[i];
    }
    return ind;
  }
  void index_to_loc(long long ind, int *iloc) {
    for (int i = 7; i >= 0; i--) {
      if (dig[i]) {
        iloc[i] = ind / block[i];
        ind -= block[i] * iloc[i];
      }
    }
  }
  void print();
  std::shared_ptr<picks_vec> return_pick_plane(int *iloc,
                                               const std::string col = "None");
  void delete_bucket();
  std::shared_ptr<SEP::pick_new> get_pick(long long p, const std::string &col);
  std::shared_ptr<SEP::picks_vec> picks_from_vec(std::vector<int> *bs,
                                                 const std::string &col);
  std::shared_ptr<SEP::picks_vec> get_pick_type(int typ,
                                                const std::string &col);
  void del_pick(long long p, const std::string &col);
  long long find_nearest(long long p, int iax1, int iax2,
                         const std::string &col);
  inline int bucket_num(long long p);
  inline void get_index(long long p, int *index);
  int get_nbuckets() { return buckets.size(); }
  std::shared_ptr<picks_new> get_bucket(int i) { return buckets[i]; }
  inline int bucket_num(int *index);
  void ignore_axis(int iax) { dig[iax] = false; }
  std::map<long long, std::shared_ptr<pick_new>> return_all(
      const std::string &col);

  std::shared_ptr<picks_vec> return_all_picks(const std::string &col);
  ~pick_bucket() { void delete_bucket(); }

 private:
  int delta[8], nb[8];
  int block_b[8];
  int next[8];
  long long block[8];
  std::vector<std::shared_ptr<picks_new>> buckets;
  std::shared_ptr<position> myp;
  // std::shared_ptr<my_colors> myc;
  bool dig[8];
  int setit;
};

}  // namespace SEP
#endif
