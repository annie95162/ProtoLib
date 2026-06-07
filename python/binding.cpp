#include "protolib/vector_store.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <stdexcept>
#include <vector>

namespace py = pybind11;
using protolib::VectorStore;

static void check_float32_dtype(const py::array& arr) {
    if (!arr.dtype().is(py::dtype::of<float>())) {
        throw std::invalid_argument("expected a NumPy array with dtype float32");
    }
}

static void check_c_contiguous(const py::array& arr) {
    if (!(arr.flags() & py::array::c_style)) {
        throw std::invalid_argument("expected a C-contiguous NumPy array");
    }
}

static void check_2d_input(const py::array& arr) {
    if (arr.ndim() != 2) {
        throw std::invalid_argument("expected a 2D NumPy array");
    }
}

static void check_1d_input(const py::array& arr) {
    if (arr.ndim() != 1) {
        throw std::invalid_argument("expected a 1D NumPy array");
    }
}

PYBIND11_MODULE(protolib, m) {
    py::class_<VectorStore>(m, "VectorStore")
        .def(py::init<std::size_t>(), py::arg("dim"))

        .def("add", [](VectorStore& store, py::array arr) {
            check_float32_dtype(arr);
            check_c_contiguous(arr);
            check_2d_input(arr);

            py::buffer_info info = arr.request();
            std::size_t n = static_cast<std::size_t>(info.shape[0]);
            std::size_t dim = static_cast<std::size_t>(info.shape[1]);

            if (dim != store.dim()) {
                throw std::invalid_argument("input dimension does not match store dimension");
            }

            const float* src = static_cast<const float*>(info.ptr);
            store.add_raw(src, n);
        })

        .def("search_l2", [](const VectorStore& store, py::array arr, std::size_t k) {
            check_float32_dtype(arr);
            check_c_contiguous(arr);
            check_1d_input(arr);

            py::buffer_info info = arr.request();
            std::size_t dim = static_cast<std::size_t>(info.shape[0]);

            if (dim != store.dim()) {
                throw std::invalid_argument("query dimension does not match store dimension");
            }

            const float* src = static_cast<const float*>(info.ptr);
            std::vector<float> query(src, src + dim);

            return store.search_l2(query, k);
        }, py::arg("query"), py::arg("k"))

        .def("search_cosine", [](const VectorStore& store, py::array arr, std::size_t k) {
            check_float32_dtype(arr);
            check_c_contiguous(arr);
            check_1d_input(arr);

            py::buffer_info info = arr.request();
            std::size_t dim = static_cast<std::size_t>(info.shape[0]);

            if (dim != store.dim()) {
                throw std::invalid_argument("query dimension does not match store dimension");
            }

            const float* src = static_cast<const float*>(info.ptr);
            std::vector<float> query(src, src + dim);

            return store.search_cosine(query, k);
        }, py::arg("query"), py::arg("k"))

        .def("size", &VectorStore::size)
        .def("dim", &VectorStore::dim)
        .def("capacity", &VectorStore::capacity)

        .def("data_view", [](VectorStore& store) {
            std::vector<py::ssize_t> shape = {
                static_cast<py::ssize_t>(store.size()),
                static_cast<py::ssize_t>(store.dim())
            };

            std::vector<py::ssize_t> strides = {
                static_cast<py::ssize_t>(store.dim() * sizeof(float)),
                static_cast<py::ssize_t>(sizeof(float))
            };

            py::object owner = py::cast(&store, py::return_value_policy::reference);

            return py::array_t<float>(
                shape,
                strides,
                store.raw_data(),
                owner
            );
        }, py::return_value_policy::reference_internal);
}
