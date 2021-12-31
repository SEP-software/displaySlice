
#include "dataset.h"

#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace SEP {

void init_dataset(py::module &clsVector) {

  py::class_<dataset, std::shared_ptr<dataset>>(clsVector, "dataset")
      .def("return_io_hyper",
           (std::shared_ptr<SEP::hypercube>(dataset::*)()) &
               dataset::return_io_hyper,
           "Make a copy of the vector")
      .def("getCharData",
           (std::shared_ptr<byteTensor2D> (dataset::*)(std::shared_ptr<orient_cube> pos, int,
                                        int, int, int, int, int)) &
               dataset::getCharData,
           "Get a byte slice of a dataset")

      .def("getFloatData",
           (std::shared_ptr<floatTensor2D> (dataset::*)(std::shared_ptr<orient_cube> pos, int,
                                        int, int, int, int, int)) &
               dataset::getFloatData,
           "Get a float slice of a dataset")
      .def("get_value",
           (float(dataset::*)(std::shared_ptr<orient_cube> pos )) &
               dataset::get_value,
           "Get value at given location");
}

} // namespace SEP
