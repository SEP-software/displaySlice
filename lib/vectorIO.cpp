#include "vectorIO.h"
#include "regVector.h"
using namespace SEP;
floatVectorIO::floatVectorIO(std::string tag, std::shared_ptr<floatHyper> vals,
                             std::shared_ptr<dictParams> pars) {

  hyper = vals->getHyper();
  std::shared_ptr<util> x(new util(pars));
  par = x;
  set_basics(x, hyper);
  bclip = vals->min();
  eclip = vals->max();
  _vec = vals;
}

void floatVectorIO::windowIt(std::vector<int> &nw, std::vector<int> &fw,
                             float *buf) {

  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  std::shared_ptr<float1DReg> v1 = std::dynamic_pointer_cast<float1DReg>(_vec);
  if (v1) {
    std::vector<int> n{nw[0]}, f{fw[0]}, j(1, 1);
    std::shared_ptr<float1DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }

  std::shared_ptr<float2DReg> v2 = std::dynamic_pointer_cast<float2DReg>(_vec);
  if (v2) {
    std::vector<int> n{nw[0], nw[1]}, f{fw[0], fw[1]}, j(2, 1);
    std::shared_ptr<float3DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<float3DReg> v3 = std::dynamic_pointer_cast<float3DReg>(_vec);
  if (v3) {
    std::vector<int> n{nw[0], nw[1], nw[2]}, f{fw[0], fw[1], fw[2]}, j(3, 1);
    std::shared_ptr<float3DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<float4DReg> v4 = std::dynamic_pointer_cast<float4DReg>(_vec);

  if (v4) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3]},
        f{fw[0], fw[1], fw[2], fw[3]}, j(4, 1);
    std::shared_ptr<float4DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<float5DReg> v5 = std::dynamic_pointer_cast<float5DReg>(_vec);

  if (v5) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4]}, j(5, 1);
    std::shared_ptr<float5DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }

  std::shared_ptr<float6DReg> v6 = std::dynamic_pointer_cast<float6DReg>(_vec);

  if (v6) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4], nw[5]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4], fw[5]}, j(6, 1);
    std::shared_ptr<float6DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
}

void floatVectorIO::read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                     float *buf) {

  windowIt(nw, fw, buf);
  bclip = 0;
  eclip = 255;
}

void floatVectorIO::read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                                    unsigned char *buf) {
  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  float *buf2 = new float[n123];
  windowIt(nw, fw, buf2);
  par->convert_to_byte(buf2, 0, buf, 0, n123, bclip, eclip);
  delete[] buf2;
}

byteVectorIO::byteVectorIO(std::string tag, std::shared_ptr<byteHyper> vals,
                           std::shared_ptr<dictParams> pars) {

  hyper = vals->getHyper();
  std::shared_ptr<util> x(new util(pars));
  par = x;
  set_basics(x, hyper);
  _vec = vals;
}

void byteVectorIO::read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                                    float *buf) {
  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  unsigned char *buf2 = new unsigned char[n123];
  windowIt(nw, fw, buf2);
  for (size_t i = 0; i < n123; i++)
    buf[i] = (float)buf2[i];
  delete[] buf2;
}

void byteVectorIO::read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                                   unsigned char *buf) {
  windowIt(nw, fw, buf);
}

void byteVectorIO::windowIt(std::vector<int> &nw, std::vector<int> &fw,
                            unsigned char *buf) {

  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  std::shared_ptr<byte1DReg> v1 = std::dynamic_pointer_cast<byte1DReg>(_vec);
  if (v1) {
    std::vector<int> n{nw[0]}, f{fw[0]}, j(1, 1);
    std::shared_ptr<byte1DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }

  std::shared_ptr<byte2DReg> v2 = std::dynamic_pointer_cast<byte2DReg>(_vec);
  if (v2) {
    std::vector<int> n{nw[0], nw[1]}, f{fw[0], fw[1]}, j(2, 1);
    std::shared_ptr<byte2DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }
  std::shared_ptr<byte3DReg> v3 = std::dynamic_pointer_cast<byte3DReg>(_vec);
  if (v3) {
    std::vector<int> n{nw[0], nw[1], nw[2]}, f{fw[0], fw[1], fw[2]}, j(3, 1);
    std::shared_ptr<byte3DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }
  std::shared_ptr<byte4DReg> v4 = std::dynamic_pointer_cast<byte4DReg>(_vec);

  if (v4) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3]},
        f{fw[0], fw[1], fw[2], fw[3]}, j(4, 1);
    std::shared_ptr<byte4DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }
  std::shared_ptr<byte5DReg> v5 = std::dynamic_pointer_cast<byte5DReg>(_vec);

  if (v5) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4]}, j(5, 1);
    std::shared_ptr<byte5DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }

  std::shared_ptr<byte6DReg> v6 = std::dynamic_pointer_cast<byte6DReg>(_vec);

  if (v6) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4], nw[5]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4], fw[5]}, j(6, 1);
    std::shared_ptr<byte6DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(),
           n123 * sizeof(unsigned char));
    return;
  }
}

doubleVectorIO::doubleVectorIO(std::string tag,
                               std::shared_ptr<doubleHyper> vals,
                               std::shared_ptr<dictParams> pars) {

  hyper = vals->getHyper();
  std::shared_ptr<util> x(new util(pars));
  par = x;
  set_basics(x, hyper);

  bclip = (float)vals->min();
  eclip = (float)vals->max();

  _vec = vals;
}

void doubleVectorIO::read_block_float(std::vector<int> &nw,
                                      std::vector<int> &fw, float *buf) {
  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  double *buf2 = new double[n123];
  windowIt(nw,fw,buf2);

  for (size_t i = 0; i < n123; i++)
    buf[i] = (float)buf2[i];
  delete[] buf2;
}

void doubleVectorIO::read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                                     unsigned char *buf) {
  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  double *buf2 = new double[n123];
  float *buf3 = new float[n123];
  windowIt(nw, fw, buf2);
  for (auto i = 0; i < n123; i++)
    buf3[i] = (float)buf2[i];
  delete[] buf2;
  par->convert_to_byte(buf3, 0, buf, 0, n123, bclip, eclip);
  delete[] buf3;
}

void doubleVectorIO::windowIt(std::vector<int> &nw, std::vector<int> &fw,
                              double *buf) {

  long long n123 = 1;
  for (auto i = nw.begin(); i != nw.end(); ++i)
    n123 *= (long long)*i;
  std::shared_ptr<double1DReg> v1 = std::dynamic_pointer_cast<double1DReg>(_vec);
  if (v1) {
    std::vector<int> n{nw[0]}, f{fw[0]}, j(1, 1);
    std::shared_ptr<double1DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }

  std::shared_ptr<double2DReg> v2 =
      std::dynamic_pointer_cast<double2DReg>(_vec);
  if (v2) {
    std::vector<int> n{nw[0], nw[1]}, f{fw[0], fw[1]}, j(2, 1);
    std::shared_ptr<double2DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<double3DReg> v3 =
      std::dynamic_pointer_cast<double3DReg>(_vec);
  if (v3) {
    std::vector<int> n{nw[0], nw[1], nw[2]}, f{fw[0], fw[1], fw[2]}, j(3, 1);
    std::shared_ptr<double3DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<double4DReg> v4 =
      std::dynamic_pointer_cast<double4DReg>(_vec);

  if (v4) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3]},
        f{fw[0], fw[1], fw[2], fw[3]}, j(4, 1);
    std::shared_ptr<double4DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
  std::shared_ptr<double5DReg> v5 =
      std::dynamic_pointer_cast < double5DReg>(_vec);

  if (v5) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4]}, j(5, 1);
    std::shared_ptr<double5DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }

  std::shared_ptr<double6DReg> v6 =
      std::dynamic_pointer_cast < double6DReg>( _vec);

  if (v6) {
    std::vector<int> n{nw[0], nw[1], nw[2], nw[3], nw[4], nw[5]},
        f{fw[0], fw[1], fw[2], fw[3], fw[4], fw[5]}, j(6, 1);
    std::shared_ptr<double6DReg> tmp = tmp->window(n, f, j);
    memcpy((void *)buf, (const void *)tmp->getVals(), n123 * sizeof(float));
    return;
  }
}
