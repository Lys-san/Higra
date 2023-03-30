/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "py_graph_image.hpp"
#include "../py_common.hpp"
#include "higra/image/graph_image.hpp"
#include "xtensor-python/pyarray.hpp"
#include "xtensor-python/pytensor.hpp"
#include "pybind11/functional.h"

template<typename T>
using pyarray = xt::pyarray<T>;

namespace py = pybind11;


struct def_kalhimsky_2_contour {
    template<typename value_t>
    static
    void def(pybind11::module &m, const char *doc) {
        m.def("_khalimsky_2_graph_4_adjacency", [](const pyarray<value_t> &khalimsky,
                                                   const hg::ugraph &graph,
                                                   const std::vector<size_t> &shape,
                                                   bool extra_border) {
                  return hg::khalimsky_2_graph_4_adjacency(khalimsky, graph, hg::embedding_grid_2d(shape), extra_border);
              },
              doc,
              py::arg("khalimsky"),
              py::arg("graph"),
              py::arg("embedding"),
              py::arg("extra_border") = false);
    }
};

struct def_contour2Khalimsky {
    template<typename value_t>
    static
    void def(pybind11::module &m, const char *doc) {
        m.def("_graph_4_adjacency_2_khalimsky", [](const hg::ugraph &graph,
                                                   const std::vector<size_t> &shape,
                                                   const pyarray<value_t> &weights,
                                                   bool add_extra_border) {
                  hg::embedding_grid_2d embedding(shape);
                  return hg::graph_4_adjacency_2_khalimsky(graph, embedding, weights, add_extra_border);
              },
              doc,
              py::arg("graph"),
              py::arg("shape"),
              py::arg("edgeWeights"),
              py::arg("add_extra_border") = false);
    }
};

void py_init_graph_image(pybind11::module &m) {
    xt::import_numpy();

    add_type_overloads<def_contour2Khalimsky, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Create a contour image in the Khalimsky grid from a 4 adjacency edge-weighted graph."
            );


    add_type_overloads<def_kalhimsky_2_contour, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Create a 4 adjacency edge-weighted graph from a contour image in the Khalimsky grid. "
             "Returns a tuple of three elements (graph, embedding, edge_weights)."
            );

}

