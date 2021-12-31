#ifndef SLICER_H
#define SLICER_H
#include "hypercube.h"
#include "position.h"
namespace SEP{

template <class T> 

class slicer{
  public:

  slicer(std::vector<int> n, T *buf){
      _skip.push_back(1);
      for(int i=0; i<n.size(); i++) _skip.push_back((long long)n[i]*_skip[i]);
    _buf=buf;

  }



  private:
  std::vector<int> _n;
  std::vector<long long> _skip;
  T *_buf;


};
}