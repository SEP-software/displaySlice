
#include "position.h"

#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace SEP {

void init_position(py::module &clsVector) {
  py::class_<position, std::shared_ptr<position>>(clsVector, "position")
      .def(py::init<std::shared_ptr<hypercube>, int *, int *, int *, int>(),
           "Initalize position")
      .def("update_position",
           (void(position::*)(int, int, int, int)) & position::update_position,
           "update current poisition")
      .def("update_range",
           (void(position::*)(int, int, int, int, int, int)) &
               position::update_range,
           "update range")
      .def("full_range", (void(position::*)()) & position::full_range,
           "reset to full range")
      .def("set_movie_dir", (void(position::*)(int)) & position::set_movie_dir,
           "set the direction of a movie");
}

} // namespace SEP
