import unittest

import sys

sys.path.insert(0, "@PYTHON_MODULE_PATH@")

import higra as hg
import numpy as np
import math


class TestUndirectedGraph(unittest.TestCase):

    def test_weighting_graph(self):
        g = hg.get4AdjacencyGraph((2, 2))
        data = np.asarray((0, 1, 2, 3))

        ref = (0.5, 1, 2, 2.5)
        r = hg.weightGraph(g, data, hg.WeightFunction.mean)
        self.assertTrue(np.allclose(ref, r))

        ref = (0, 0, 1, 2)
        r = hg.weightGraph(g, data, hg.WeightFunction.min)
        self.assertTrue(np.allclose(ref, r))

        ref = (1, 2, 3, 3)
        r = hg.weightGraph(g, data, hg.WeightFunction.max)
        self.assertTrue(np.allclose(ref, r))

        ref = (1, 2, 2, 1)
        r = hg.weightGraph(g, data, hg.WeightFunction.L1)
        self.assertTrue(np.allclose(ref, r))

        ref = (math.sqrt(1), 2, 2, math.sqrt(1))
        r = hg.weightGraph(g, data, hg.WeightFunction.L2)
        self.assertTrue(np.allclose(ref, r))

        ref = (1, 2, 2, 1)
        r = hg.weightGraph(g, data, hg.WeightFunction.L_infinity)
        self.assertTrue(np.allclose(ref, r))

        ref = (1, 4, 4, 1)
        r = hg.weightGraph(g, data, hg.WeightFunction.L2_squared)
        self.assertTrue(np.allclose(ref, r))

    def test_weighting_graph_vectorial(self):
        g = hg.get4AdjacencyGraph((2, 2))
        data = np.asarray(((0, 1), (2, 3), (4, 5), (6, 7)))

        ref = (4, 8, 8, 4)
        r = hg.weightGraph(g, data, hg.WeightFunction.L1)
        self.assertTrue(np.allclose(ref, r))

        ref = (math.sqrt(8), math.sqrt(32), math.sqrt(32), math.sqrt(8))
        r = hg.weightGraph(g, data, hg.WeightFunction.L2)
        self.assertTrue(np.allclose(ref, r))

        ref = (2, 4, 4, 2)
        r = hg.weightGraph(g, data, hg.WeightFunction.L_infinity)
        self.assertTrue(np.allclose(ref, r))

        ref = (8, 32, 32, 8)
        r = hg.weightGraph(g, data, hg.WeightFunction.L2_squared)
        self.assertTrue(np.allclose(ref, r))

    def test_weighting_graph_lambda(self):
        g = hg.get4AdjacencyGraph((2, 2))
        data = np.asarray((0, 1, 2, 3))

        ref = (1, 2, 4, 5)
        r = hg.weightGraph(g, lambda i, j: i + j)
        self.assertTrue(np.allclose(ref, r))

        ref = (1, 2, 4, 5)
        r = hg.weightGraph(g, lambda i, j: data[i] + data[j])
        self.assertTrue(np.allclose(ref, r))

    def test_contour_2_khalimsky(self):
        g = hg.get4AdjacencyGraph((2, 3))
        data = np.asarray((1, 0, 2, 1, 1, 1, 2))

        ref = np.asarray(((0, 1, 0, 2, 0),
                          (0, 1, 1, 2, 1),
                          (0, 1, 0, 2, 0)))
        r = hg.contour2Khalimsky(g, hg.EmbeddingGrid2d((2, 3)), data)
        self.assertTrue(np.allclose(ref, r))


if __name__ == '__main__':
    unittest.main()