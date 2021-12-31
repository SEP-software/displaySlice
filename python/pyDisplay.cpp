
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
namespace SEP
{
    void init_dataset(py::module &);
    void init_position(py::module &);

    using namespace SEP;

    PYBIND11_MODULE(pySepVector, clsVector)

    {
        init_dataset(clsVector);
        init_position(clsVector);

    }
}
