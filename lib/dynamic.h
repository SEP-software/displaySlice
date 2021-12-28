#ifndef DYNAMIC_H
#define DYNAMIC_H 1
#include <float2DReg.h>
#include <path.h>
#include <memory>
namespace SEP {
class dynamic {
 public:
  dynamic() {
    sim = 0;
    score = 0;
  };
  dynamic(std::shared_ptr<SEP::float2DReg> buf, float jump_pen);
  void add_to_sim(std::shared_ptr<SEP::float2DReg> s);
  virtual void calc_score();
  virtual std::vector<SEP::path> return_path();

  virtual ~dynamic() { delete_ar(); }

  void delete_ar() {}
  std::shared_ptr<SEP::float2DReg> sim, score;
  float jump;
  int maxloc[2];
};
}  // namespace SEP

#endif
