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


def compare(t1, t2):
    if len(t1) != len(t2):
        return False
    for i in range(len(t1)):
        if t1[i] != t2[i]:
            return False
    return True


class TestEmbedding(unittest.TestCase):

    def test_create_embedding(self):
        e1 = hg.EmbeddingGrid3d((3, 5, 2))
        e2 = hg.EmbeddingGrid3d([3, 5, 2])
        e3 = hg.EmbeddingGrid3d(np.asarray([3, 5, 2], dtype=np.int64))

        for e in (e1, e2, e3):
            self.assertTrue(e.dimension() == 3)
            self.assertTrue(e.size() == 30)
            self.assertTrue(compare(e.shape(), (3, 5, 2)))

    def test_dynamic_attributes(self):
        e = hg.EmbeddingGrid3d((3, 5, 2))
        e.new_attribute = 42
        self.assertTrue(e.new_attribute == 42)

    def test_contains(self):
        e1 = hg.EmbeddingGrid3d((3, 5, 2))
        self.assertTrue(e1.contains((1, 3, 1)))
        self.assertTrue(e1.contains((0, 0, 0)))
        self.assertTrue(e1.contains((2, 4, 1)))
        self.assertFalse(e1.contains((-1, 4, 1)))
        self.assertFalse(e1.contains((2, 4, 2)))

    def test_containsV(self):
        e1 = hg.EmbeddingGrid2d((5, 10))
        coords = np.asarray((((0, 0), (3, 8), (-1, 2)),
                             ((2, 4), (5, 5), (43, 44))), dtype=np.int64)

        ref = np.asarray(((True, True, False),
                          (True, False, False)))

        res = e1.contains(coords)

        self.assertTrue(np.array_equal(ref, res))

    def test_lin2grid(self):
        e1 = hg.EmbeddingGrid3d((10, 5, 2))
        p = e1.lin2grid(35)
        self.assertTrue(np.all(p == [3, 2, 1]))

    def test_lin2gridV(self):
        e1 = hg.EmbeddingGrid2d((5, 10))

        coord = np.asarray(((0, 1, 2, 3),
                            (22, 42, 43, 44)), dtype=np.uint64)
        ref = np.asarray((((0, 0), (0, 1), (0, 2), (0, 3)),
                          ((2, 2), (4, 2), (4, 3), (4, 4))), dtype=np.int64)

        res = e1.lin2grid(coord)
        self.assertTrue(np.array_equal(ref, res))

    def test_grid2lin(self):
        e1 = hg.EmbeddingGrid3d((10, 5, 2))
        p = e1.grid2lin((3, 2, 1))
        self.assertTrue(p == 35)

    def test_grid2linV(self):
        e1 = hg.EmbeddingGrid2d((5, 10))

        coord = np.asarray((((0, 0), (0, 1), (0, 2), (0, 3)),
                            ((2, 2), (4, 2), (4, 3), (4, 4))), dtype=np.int64)
        ref = np.asarray(((0, 1, 2, 3),
                          (22, 42, 43, 44)), dtype=np.uint64)

        res = e1.grid2lin(coord)
        self.assertTrue(np.array_equal(ref, res))

    def test_pickle(self):
        import pickle

        tests = ((hg.EmbeddingGrid1d, (3,)),
                 (hg.EmbeddingGrid2d, (3, 2)),
                 (hg.EmbeddingGrid3d, (3, 2, 1)),
                 (hg.EmbeddingGrid4d, (3, 2, 1, 2)),
                 (hg.EmbeddingGrid5d, (3, 2, 1, 2, 1)))

        for c, s in tests:
            e = c(s)
            hg.set_attribute(e, "test", (1, 2, 3))
            hg.add_tag(e, "foo")

            data = pickle.dumps(e)
            e2 = pickle.loads(data)

            self.assertTrue(np.all(e.shape() == e2.shape()))

            self.assertTrue(hg.get_attribute(e, "test") == hg.get_attribute(e2, "test"))
            self.assertTrue(e.test == e2.test)
            self.assertTrue(hg.has_tag(e2, "foo"))


if __name__ == '__main__':
    unittest.main()
