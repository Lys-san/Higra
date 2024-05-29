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
import higra as hg
import numpy as np


class TestUndirectedGraph(unittest.TestCase):

    @staticmethod
    def test_graph():
        g = hg.UndirectedGraph(4)
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        g.add_edge(0, 2)
        return g

    def test_add_vertex(self):
        g = hg.UndirectedGraph()
        self.assertTrue(g.num_vertices() == 0)
        self.assertTrue(g.add_vertex() == 0)
        self.assertTrue(g.num_vertices() == 1)
        self.assertTrue(g.add_vertex() == 1)
        self.assertTrue(g.num_vertices() == 2)

        g = hg.UndirectedGraph(3)
        self.assertTrue(g.num_vertices() == 3)

    def test_add_vertices(self):
        g = hg.UndirectedGraph()
        self.assertTrue(g.num_vertices() == 0)
        g.add_vertices(3)
        self.assertTrue(g.num_vertices() == 3)
        g.add_vertices(2)
        self.assertTrue(g.num_vertices() == 5)

    def test_add_edge(self):
        g = hg.UndirectedGraph(3)
        self.assertTrue(g.num_edges() == 0)
        g.add_edge(0, 1)
        self.assertTrue(g.num_edges() == 1)

        # parallel edge allowed
        g.add_edge(0, 1)
        self.assertTrue(g.num_edges() == 2)

        # still parallel edge allowed
        g.add_edge(1, 0)
        self.assertTrue(g.num_edges() == 3)

        g.add_edge(0, 2)
        self.assertTrue(g.num_edges() == 4)

    def test_add_edges(self):
        g = hg.UndirectedGraph(3)
        g.add_edge(0, 1)
        g.add_edge(0, 2)

        g2 = hg.UndirectedGraph(3)
        g2.add_edges((0, 0), (1, 2))

        self.assertTrue(g2.num_edges() == 2)

        for i in range(g2.num_edges()):
            self.assertTrue(g.edge_from_index(i) == g2.edge_from_index(i))

    def test_dynamic_attributes(self):
        g = TestUndirectedGraph.test_graph()
        g.new_attribute = 42
        self.assertTrue(g.new_attribute == 42)

    def test_vertex_iterator(self):
        g = TestUndirectedGraph.test_graph()
        vref = [0, 1, 2, 3];
        vtest = [];

        for v in g.vertices():
            vtest.append(v)

        self.assertTrue(vtest == vref)

    def test_edge_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [(0, 1), (1, 2), (0, 2)]
        test = []

        for e in g.edges():
            test.append((g.source(e), g.target(e)))

        self.assertTrue(test == ref)

    def test_edge_list(self):
        g = TestUndirectedGraph.test_graph()
        ref_sources = (0, 1, 0)
        ref_targets = (1, 2, 2)

        sources = g.sources()
        self.assertTrue(np.all(ref_sources == sources))

        targets = g.targets()
        self.assertTrue(np.all(ref_targets == targets))

        sources, targets = g.edge_list()
        self.assertTrue(np.all(ref_sources == sources))
        self.assertTrue(np.all(ref_targets == targets))

    def test_out_edge_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [[(0, 1), (0, 2)],
               [(1, 0), (1, 2)],
               [(2, 1), (2, 0)],
               []]
        test = []
        for v in g.vertices():
            test.append([])
            for e in g.out_edges(v):
                test[v].append((e[0], e[1]))

        self.assertTrue(test == ref)

    def test_in_edge_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [[(1, 0), (2, 0)],
               [(0, 1), (2, 1)],
               [(1, 2), (0, 2)],
               []]
        test = []
        for v in g.vertices():
            test.append([])
            for e in g.in_edges(v):
                test[v].append((e[0], e[1]))

        self.assertTrue(test == ref)

    def test_adjacent_vertex_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [[1, 2],
               [0, 2],
               [1, 0],
               []]
        test = []
        for v in g.vertices():
            test.append([])
            for av in g.adjacent_vertices(v):
                test[v].append(av)

        self.assertTrue(test == ref)

    def test_degrees(self):
        g = TestUndirectedGraph.test_graph()
        self.assertTrue(g.degree(0) == 2)
        self.assertTrue(g.out_degree(0) == 2)
        self.assertTrue(g.in_degree(0) == 2)

        self.assertTrue(g.degree(1) == 2)
        self.assertTrue(g.out_degree(1) == 2)
        self.assertTrue(g.in_degree(1) == 2)

        self.assertTrue(g.degree(2) == 2)
        self.assertTrue(g.out_degree(2) == 2)
        self.assertTrue(g.in_degree(2) == 2)

        self.assertTrue(g.degree(3) == 0)
        self.assertTrue(g.out_degree(3) == 0)
        self.assertTrue(g.in_degree(3) == 0)

        indices = np.asarray(((0, 3), (1, 2)))
        ref = np.asarray(((2, 0), (2, 2)))
        self.assertTrue(np.allclose(g.degree(indices), ref))
        self.assertTrue(np.allclose(g.in_degree(indices), ref))
        self.assertTrue(np.allclose(g.out_degree(indices), ref))

    def test_edge_index_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [0, 1, 2]
        test = []

        for e in g.edges():
            test.append(g.index(e))

        self.assertTrue(test == ref)

    def test_out_edge_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [[0, 2],
               [0, 1],
               [1, 2],
               []]
        test = []
        for v in g.vertices():
            test.append([])
            for e in g.out_edges(v):
                test[v].append(e[2])

        self.assertTrue(test == ref)

    def test_in_edge_index_iterator(self):
        g = TestUndirectedGraph.test_graph()
        ref = [[0, 2],
               [0, 1],
               [1, 2],
               []]
        test = []
        for v in g.vertices():
            test.append([])
            for e in g.in_edges(v):
                test[v].append(e[2])

        self.assertTrue(test == ref)

    def test_pickle(self):
        import pickle
        g = TestUndirectedGraph.test_graph()
        hg.set_attribute(g, "test", (1, 2, 3))
        hg.add_tag(g, "foo")

        data = pickle.dumps(g)
        g2 = pickle.loads(data)

        self.assertTrue(g.num_vertices() == g2.num_vertices())
        gs, gt = g.edge_list()
        g2s, g2t = g2.edge_list()
        self.assertTrue(np.all(gs == g2s))
        self.assertTrue(np.all(gt == g2t))

        self.assertTrue(hg.get_attribute(g, "test") == hg.get_attribute(g2, "test"))
        self.assertTrue(g.test == g2.test)
        self.assertTrue(hg.has_tag(g2, "foo"))


if __name__ == '__main__':
    unittest.main()
