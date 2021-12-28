#ifndef AUTO_VITERIBI
#define AUTO_VITERIBI 1
#include <memory>
#include "autopick_2d.h"
#include "float2DReg.h"
namespace SEP {
class auto_viteribi : public autopick_2d {
 public:
  auto_viteribi(){};
  auto_viteribi(float peno, float pene, float nl) {
    pen_o = peno;
    pen_e = pene;
    nail = nl;
  }
  std::vector<SEP::path> return_path(std::vector<int> pik,
                                     std::shared_ptr<SEP::float2DReg> vals);

  float pen_o, pen_e, nail;
};
}  // namespace SEP
#endif
