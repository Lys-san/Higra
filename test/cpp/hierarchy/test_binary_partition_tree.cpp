/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <boost/test/unit_test.hpp>
#include "../test_utils.hpp"
#include "higra/hierarchy/binary_partition_tree.hpp"
#include "higra/image/graph_image.hpp"
#include "xtensor/xrandom.hpp"
#include "higra/hierarchy/hierarchy_core.hpp"
#include "higra/algo/tree.hpp"


using namespace hg;
using namespace std;

BOOST_AUTO_TEST_SUITE(test_binary_partition_tree);

    BOOST_AUTO_TEST_CASE(test_single_linkage_clustering_simple) {
        auto graph = get_4_adjacency_graph({3, 3});
        array_1d<double> edge_weights({1, 9, 6, 7, 5, 8, 12, 4, 10, 11, 2, 3});
        auto res = hg::binary_partition_tree(graph,
                                             edge_weights,
                                             hg::make_binary_partition_tree_min_linkage(
                                                     edge_weights));
        auto &tree = res.tree;
        auto &levels = res.altitudes;

        array_1d<index_t> expected_parents({9, 9, 13, 15, 12, 12, 10, 10, 11, 14, 11, 16, 13, 14, 15, 16, 16});
        array_1d<double> expected_levels({0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 8, 10});

        BOOST_CHECK(expected_parents == tree.parents());
        BOOST_CHECK(expected_levels == levels);
    }

    BOOST_AUTO_TEST_CASE(test_single_linkage_clustering_hard) {
        long size = 100;
        auto graph = get_4_adjacency_graph({size, size});
        array_1d<double> edge_weights = xt::random::rand<double>({num_edges(graph)});
        auto res = hg::binary_partition_tree(graph,
                                             edge_weights,
                                             hg::make_binary_partition_tree_min_linkage(edge_weights));
        auto &tree = res.tree;


        auto res2 = hg::bpt_canonical(graph, edge_weights);
        auto &tree2 = res2.tree;

        BOOST_CHECK(hg::test_tree_isomorphism(tree, tree2));
    }

    BOOST_AUTO_TEST_CASE(test_complete_linkage_clustering_simple) {
        auto graph = get_4_adjacency_graph({3, 3});
        array_1d<double> edge_weights({1, 8, 2, 10, 15, 3, 11, 4, 12, 13, 5, 6});
        auto res = hg::binary_partition_tree(graph,
                                             edge_weights,
                                             hg::make_binary_partition_tree_complete_linkage(
                                                     edge_weights));
        auto &tree = res.tree;
        auto &levels = res.altitudes;

        array_1d<index_t> expected_parents({9, 9, 10, 11, 11, 12, 13, 13, 14, 10, 16, 12, 15, 14, 15, 16, 16});
        array_1d<double> expected_levels({0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 13, 15});

        BOOST_CHECK(expected_parents == tree.parents());
        BOOST_CHECK(expected_levels == levels);
    }

    BOOST_AUTO_TEST_CASE(test_average_linkage_clustering_simple) {
        auto graph = get_4_adjacency_graph({3, 3});
        array_1d<double> edge_values({1, 7, 2, 10, 16, 3, 11, 4, 12, 14, 5, 6});
        array_1d<double> edge_weights({7, 1, 7, 3, 2, 8, 2, 2, 2, 1, 5, 9});
        auto res = hg::binary_partition_tree(graph,
                                             edge_values,
                                             hg::make_binary_partition_tree_average_linkage(
                                                     edge_values, edge_weights));
        auto &tree = res.tree;
        auto &levels = res.altitudes;

        array_1d<index_t> expected_parents({9, 9, 10, 11, 11, 12, 13, 13, 14, 10, 15, 12, 15, 14, 16, 16, 16});
        array_1d<double> expected_levels({0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 11.5, 12});

        BOOST_CHECK(expected_parents == tree.parents());
        BOOST_CHECK(expected_levels == levels);
    }

    BOOST_AUTO_TEST_CASE(test_average_linkage_clustering) {
        ugraph graph(10);
        array_1d<index_t> sources{0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 7, 7};
        array_1d<index_t> targets{3, 6, 4, 2, 5, 3, 6, 9, 7, 3, 8, 5, 9, 4, 6, 9, 7, 8, 6, 9, 8};
        add_edges(sources, targets, graph);
        array_1d<double> edge_values{0.87580029, 0.60123697, 0.79924759, 0.74221387, 0.75418382, 0.66159356,
                                     1.31856839, 0.76080612, 1.08881471, 0.98557615, 0.61454158, 0.50913424,
                                     0.63556478, 0.64684775, 1.14865302, 0.81741018, 2.1591071, 0.60563004,
                                     2.06636665, 1.35617725, 0.83085949};
        array_1d<double> edge_weights = xt::ones_like(edge_values);
        auto res = hg::binary_partition_tree(graph,
                                             edge_values,
                                             hg::make_binary_partition_tree_average_linkage(edge_values, edge_weights));
        auto &tree = res.tree;
        auto &altitudes = res.altitudes;

        array_1d<index_t> expected_parents{11, 14, 10, 13, 15, 10, 11, 18, 12, 13, 12, 17, 16, 14, 15, 16, 17, 18, 18};
        array_1d<double> expected_altitudes{0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.509134, 0.601237, 0.610086,
                                            0.635565, 0.661594, 0.732129, 0.810695, 1.241727, 1.35874};
        BOOST_CHECK(expected_parents == parents(tree));
        BOOST_CHECK(xt::allclose(expected_altitudes, altitudes));
    }


BOOST_AUTO_TEST_SUITE_END();
