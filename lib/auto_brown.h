#ifndef AUTO_BROWN
#define AUTO_BROWN 1
#include "autopick_2d.h"
#include "fastmarch.h"
#include "float2DReg.h"
namespace SEP {
class auto_brown : public autopick_2d, fastmarch {
 public:
  auto_brown(float peno, float pene, float nl) {
    pen_o = peno;
    pen_e = pene;
    nail = nl;
  }
  virtual std::vector<path> return_path(std::vector<int> locs,
                                        std::shared_ptr<SEP::float2DReg> array);
  ~auto_brown() {}

  float pen_o, pen_e, nail;
};
}  // namespace SEP
#endif
