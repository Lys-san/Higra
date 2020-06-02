############################################################################
# Copyright ESIEE Paris (2018)                                             #
#                                                                          #
# Contributor(s) : Benjamin Perret                                         #
#                                                                          #
# Distributed under the terms of the CECILL-B License.                     #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

import higra as hg
import numpy as np


@hg.argument_helper(hg.CptGridGraph)
def graph_4_adjacency_2_khalimsky(graph, edge_weights, shape, add_extra_border=False):
    """
    Create a contour image in the Khalimsky grid from a 4 adjacency edge-weighted graph.

    :param graph: must be a 4 adjacency 2d graph (Concept :class:`~higra.CptGridGraph`)
    :param edge_weights: edge weights of the graph
    :param shape: shape of the graph (deduced from :class:`~higra.CptGridGraph`)
    :param add_extra_border: if False result size is 2 * shape - 1 and 2 * shape + 1 otherwise
    :return: a 2d array
    """
    shape = hg.normalize_shape(shape)
    return hg.cpp._graph_4_adjacency_2_khalimsky(graph, shape, edge_weights, add_extra_border)


def khalimsky_2_graph_4_adjacency(khalimsky, extra_border=False):
    """
    Create a 4 adjacency edge-weighted graph from a contour image in the Khalimsky grid.

    :param khalimsky: a 2d array
    :param extra_border: if False the shape of the Khalimsky image  is 2 * shape - 1 and 2 * shape + 1 otherwise, where shape is the shape of the resulting grid graph
    :return: a graph (Concept :class:`~higra.CptGridGraph`) and its edge weights
    """

    graph, embedding, edge_weights = hg.cpp._khalimsky_2_graph_4_adjacency(khalimsky, extra_border)

    hg.CptGridGraph.link(graph, hg.normalize_shape(embedding.shape()))
    hg.set_attribute(graph, "no_border_vertex_out_degree", 4)

    return graph, edge_weights


def mask_2_neighbours(mask, center=None):
    """
    Converts as a neighbouring mask as a neighbour list. A neighbouring :attr:`mask` is a :math:`n`-d matrix where
    each strictly positive value represent a neighbour of the :attr:`center` point. The neighbour list is obtained by
    offsetting the coordinates of those positive values by the coordinates of the center.

    The default center is the center of the :attr:`mask` matrix: i.e. ``mask.shape // 2``.

    :Example:

    >>> mask = [[0, 1, 0], [1, 0, 1], [0, 1, 0]]
    >>> hg.mask_2_neighbours(mask)
    array([[-1, 0], [0, -1], [0, 1], [1, 0]])
    
    >>> mask = [[0, 1, 0], [1, 0, 1], [0, 1, 0]]
    >>> center = [2, 1]
    >>> hg.mask_2_neighbours(mask)
    array([[-2, 0], [-1, -1], [-1, 1], [0, 0]])
    
    >>> mask = [[[0, 0, 0], [0, 1, 0], [0, 0, 0]],
    >>>         [[0, 1, 0], [1, 0, 1], [0, 1, 0]],
    >>>         [[0, 0, 0], [0, 1, 0], [0, 0, 0]]]
    >>> hg.mask_2_neighbours(mask)
    array([[-1, 0, 0], [1, 0, 0], [0, -1, 0], [0, 1, 0], [0, 0, -1], [0, 0, 1]])
       
    :param mask:  a :math:`n`-d matrix
    :param center: a 1d array of size :math:`n` (optional)
    :return: a list of point coordinates
    """
    mask = np.asarray(mask)

    if center is None:
        center = np.floor_divide(np.asarray(mask.shape, dtype=np.int64), 2)
    else:
        center = np.asarray(center)
        if len(center) != mask.ndim:
            raise ValueError("'center' size does not match 'mask' dimension.")

    neighbours = np.argwhere(mask > 0)
    neighbours -= center

    return neighbours


def get_4_adjacency_graph(shape):
    """
    Create an explicit undirected 4 adjacency graph of the given shape.

    :param shape: a pair (height, width)
    :return: a graph (Concept :class:`~higra.CptGridGraph`)
    """
    shape = hg.normalize_shape(shape)
    graph = hg.cpp._get_4_adjacency_graph(shape)
    hg.CptGridGraph.link(graph, shape)
    hg.set_attribute(graph, "no_border_vertex_out_degree", 4)

    return graph


def get_8_adjacency_graph(shape):
    """
    Create an explicit undirected 8 adjacency graph of the given shape.

    :param shape: a pair (height, width)
    :return: a graph (Concept :class:`~higra.CptGridGraph`)
    """
    shape = hg.normalize_shape(shape)
    graph = hg.cpp._get_8_adjacency_graph(shape)
    hg.CptGridGraph.link(graph, shape)
    hg.set_attribute(graph, "no_border_vertex_out_degree", 8)

    return graph


def get_4_adjacency_implicit_graph(shape):
    """
    Create an implicit undirected 4 adjacency graph of the given shape (edges are not stored).

    :param shape: a pair (height, width)
    :return: a graph (Concept :class:`~higra.CptGridGraph`)
    """
    shape = hg.normalize_shape(shape)
    graph = hg.cpp._get_4_adjacency_implicit_graph(shape)
    hg.CptGridGraph.link(graph, shape)
    hg.set_attribute(graph, "no_border_vertex_out_degree", 4)

    return graph


def get_8_adjacency_implicit_graph(shape):
    """
    Create an implicit undirected 8 adjacency graph of the given shape (edges are not stored).

    :param shape: a pair (height, width)
    :return: a graph (Concept :class:`~higra.CptGridGraph`)
    """
    shape = hg.normalize_shape(shape)
    graph = hg.cpp._get_8_adjacency_implicit_graph(shape)
    hg.CptGridGraph.link(graph, shape)
    hg.set_attribute(graph, "no_border_vertex_out_degree", 8)

    return graph
