#include "fileIO.h"
using namespace SEP;
fileIO::fileIO(std::string tag, std::shared_ptr<ioModes> modes,
               std::string fileType) {
  _io = modes->getIO(fileType);
  _file = _io->getReg(tag, usageIn);
  _dataType = _file->getDataType();
  _par = _io->getParamObj();
  hyper = _file->getHyper();
  std::shared_ptr<util> x(new util(_par));
  set_basics(x, hyper);
  switch (_dataType) {
    case DATA_BYTE:
      non_byte = false;
      bclip = _file->getFloat("minval", 0);
      eclip = _file->getFloat("maxval", 255);
      break;
    case DATA_FLOAT:
      non_byte = true;
      break;
    default:
      _par->error("only can handle float and byte files for now");
  }
}

void fileIO::read_block_float(std::vector<int> &nw, std::vector<int> &fw,
                              float *buf) {
  std::vector<int> jw(8, 1);
  long long n123 = 1;
  if (_dataType == DATA_FLOAT) {
    _file->readFloatWindow(nw, fw, jw, buf);

  } else if (_dataType == DATA_BYTE) {
    for (auto i = nw.begin(); i != nw.end(); ++i) n123 *= (long long)*i;
    std::vector<unsigned char> buf2(n123);
    _file->readByteWindow(nw, fw, jw, buf2.data());

    par->convert_to_float(buf, 0, buf2.data(), 0, n123, bclip, eclip);

  } else
    _par->error("Only can handle float and byte");
}

void fileIO::read_block_byte(std::vector<int> &nw, std::vector<int> &fw,
                             unsigned char *buf) {
  std::vector<int> jw(8, 1);
  long long n123 = 1;
  if (_dataType == DATA_BYTE) {
    _file->readByteWindow(nw, fw, jw, buf);

  } else if (_dataType == DATA_FLOAT) {
    for (auto i = nw.begin(); i != nw.end(); ++i) n123 *= (long long)*i;
  std::vector<float> buf2(n123);
    _file->readFloatWindow(nw, fw, jw, buf2.data());

    par->convert_to_byte(buf2.data(), 0, buf, 0, n123, bclip, eclip);

  } else
    _par->error("Only can handle float and byte");
}
