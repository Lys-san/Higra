############################################################################
# Copyright ESIEE Paris (2018)                                             #
#                                                                          #
# Contributor(s) : Benjamin Perret                                         #
#                                                                          #
# Distributed under the terms of the CECILL-B License.                     #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

import unittest
import numpy as np
import higra as hg

# needed for reliable access to resource files...

import os
import os.path

# needed for reliable access to resource files...
_my_path = os.path.dirname(os.path.abspath(__file__))
graph_file = os.path.join(_my_path, "..", "resources", "test.graph")

def silent_remove(filename):
    try:
        os.remove(filename)
    except:
        pass


class TestPinkGraphIO(unittest.TestCase):

    def test_graph_read(self):
        global graph_file
        graph, vertex_weights, edge_weights = hg.read_graph_pink(graph_file)

        shape = hg.get_attribute(graph, "shape")

        edges_ref = []
        for i in range(14):
            edges_ref.append((i, i + 1))

        vertex_weights_ref = np.arange(1, 16).reshape(3, 5)
        edges_weights_ref = (3, 0, 0, 1, 3, 0, 1, 0, 2, 0, 1, 0, 3, 0)

        edges = []
        for e in graph.edges():
            edges.append((e[0], e[1]))

        self.assertTrue(shape == [3, 5])
        self.assertTrue(edges == edges_ref)
        self.assertTrue(np.allclose(vertex_weights, vertex_weights_ref))
        self.assertTrue(np.allclose(edge_weights, edges_weights_ref))

    def test_graphWrite(self):
        global graph_file
        filename = "testWriteGraphPink.graph"
        silent_remove(filename)

        vertex_weights = np.arange(1, 16)
        edges_weights = np.asarray((3, 0, 0, 1, 3, 0, 1, 0, 2, 0, 1, 0, 3, 0))
        shape = (3, 5)

        graph = hg.UndirectedGraph(15)
        for i in range(14):
            graph.add_edge(i, i + 1)

        hg.save_graph_pink(filename, graph, vertex_weights, edges_weights, shape)

        self.assertTrue(os.path.exists(filename))

        with open(filename, 'r') as f:
            data = f.read()

        silent_remove(filename)

        with open(graph_file, 'r') as f:
            data_ref = f.read()

        self.assertTrue(data == data_ref)

        # Test default attributes
        hg.save_graph_pink(filename, graph)
        self.assertTrue(os.path.exists(filename))
        silent_remove(filename)


if __name__ == '__main__':
    unittest.main()
