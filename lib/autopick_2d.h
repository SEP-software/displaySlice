#ifndef AUTOPICK_2D_H
#define AUTOPICK_2D_H 1
#include "float2DReg.h"
#include "path.h"
namespace SEP {
class autopick_2d {
 public:
  autopick_2d(){};

  virtual std::vector<path> return_path(std::vector<int> pik,
                                        std::shared_ptr<SEP::float2DReg> array);
  virtual ~autopick_2d(){};
};
}  // namespace SEP
#endif
