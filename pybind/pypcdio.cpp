
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "pcdio/pcdio.h"
#include <vector>
#include <string>

namespace py = pybind11;

class PyPCD {
public:
    py::array_t<double, py::array::c_style> points_;
    py::array_t<double, py::array::c_style> normals_;
    py::array_t<double, py::array::c_style> colors_;
};

PYBIND11_MODULE(pypcdio, m) {
    py::class_<PyPCD> (m, "ptcl")
            .def(py::init<>())
            .def_readwrite("points", &PyPCD::points_)
            .def_readwrite("normals", &PyPCD::normals_)
            .def_readwrite("colors", &PyPCD::colors_);

    m.def("pcdread", [](std::string filename) -> PyPCD {
    auto points = new std::vector<double>();
    auto normals = new std::vector<double>();
    auto colors = new std::vector<double>();
    ReadXXX(filename, *points, *normals, *colors);
    PyPCD result;
    result.points_ = py::array_t<double, py::array::c_style> (
            std::vector<size_t> {points->size()/3, 3}, points->data());
    result.normals_ = py::array_t<double, py::array::c_style> (
            std::vector<size_t> {normals->size()/3, 3}, normals->data());
    result.colors_ = py::array_t<double, py::array::c_style> (
            std::vector<size_t> {colors->size()/3, 3}, colors->data());
    return result;
//    auto v = new std::vector<int>(create_array(size));
//    auto capsule = py::capsule(v, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
//return py::array(v->size(), v->data(), capsule);
    });

    m.def("pcdwrite", [](PyPCD& pointcloud, std::string& filename) -> void {
        std::vector<double> points;
        if (pointcloud.points_.size() > 0) {
            points.assign(pointcloud.points_.data(),
                                       pointcloud.points_.data() + pointcloud.points_.size() - 1);
        }
        std::vector<double> normals;
        if (pointcloud.normals_.size() > 0) {
            normals.assign(pointcloud.normals_.data(), pointcloud.normals_.data()+pointcloud.normals_.size()-1);
        }
        std::vector<double> colors;
        if (pointcloud.colors_.size() > 0) {
            colors.assign(pointcloud.colors_.data(), pointcloud.colors_.data()+pointcloud.colors_.size()-1);
        }
        WriteXXX(filename, points, normals, colors);
    });
}
