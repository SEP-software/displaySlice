
#ifndef VITERBI_H
#define VITERBI_H 1
#include <my_score.h>
#include <memory>
#include "dynamic.h"
#include "float2DReg.h"
namespace SEP {
class viterbi : public dynamic {
 public:
  viterbi();
  viterbi(std::shared_ptr<SEP::my_score> s,
          std::shared_ptr<SEP::float2DReg> buf, int ns, int flac);
  virtual void calc_score();
  virtual std::vector<path> return_path();

  int nsearch, f;
  virtual ~viterbi(){};
  std::shared_ptr<my_score> sc;
};
}  // namespace SEP

#endif
