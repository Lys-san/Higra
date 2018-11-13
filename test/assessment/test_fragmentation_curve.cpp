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
#include "higra/assessment/fragmentation_curve.hpp"
#include "higra/image/graph_image.hpp"
#include "../test_utils.hpp"

using namespace hg;

BOOST_AUTO_TEST_SUITE(test_fragmentation_curve);

    BOOST_AUTO_TEST_CASE(test_assess_fragmentation_curve_BCE_optimal_cut) {
        tree t(array_1d<index_t>{8, 8, 9, 9, 10, 10, 11, 13, 12, 12, 11, 13, 14, 14, 14});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::BCE);

        BOOST_CHECK(assesser.optimal_number_of_regions() == 3);
        BOOST_CHECK(assesser.number_of_region_ground_truth() == 3);
        BOOST_CHECK(almost_equal(assesser.optimal_score(), (2 + 4.0 / 3 + 2.5) / num_leaves(t)));

        auto res = assesser.fragmentation_curve(false);
        auto &res_scores = res.scores;
        auto &res_k = res.k;

        array_1d<double> ref_scores{2.75, 4.5, 2 + 4.0 / 3 + 2.5, 2 + 4.0 / 3 + 2, 2 + 4.0 / 3 + 4.0 / 3,
                                    2 + 4.0 / 3 + 4.0 / 3, 4, 3};
        array_1d<size_t> ref_k{1, 2, 3, 4, 5, 6, 7, 8};

        BOOST_CHECK(res_scores == (ref_scores / num_leaves(t)));
        BOOST_CHECK(res_k == ref_k);
    }

    BOOST_AUTO_TEST_CASE(test_assess_fragmentation_curve_BCE_optimal_cut_on_rag) {
        array_1d<index_t> vertex_map{0, 0, 1, 1, 2, 2, 3, 4};
        tree t(array_1d<index_t>{6, 6, 5, 5, 7, 7, 8, 8, 8});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::BCE, vertex_map);

        BOOST_CHECK(assesser.optimal_number_of_regions() == 3);
        BOOST_CHECK(assesser.number_of_region_ground_truth() == 3);
        BOOST_CHECK(almost_equal(assesser.optimal_score(), (2 + 4.0 / 3 + 2.5) / num_leaves(t)));

        auto res = assesser.fragmentation_curve(false);
        auto &res_scores = res.scores;
        auto &res_k = res.k;

        std::cout << res_k << std::endl;
        std::cout << res_scores << std::endl;

        array_1d<double> ref_scores{2.75, 4.5, 2 + 4.0 / 3 + 2.5, 2 + 4.0 / 3 + 2, 2 + 4.0 / 3 + 4.0 / 3};
        array_1d<size_t> ref_k{1, 2, 3, 4, 5};

        BOOST_CHECK(res_scores == (ref_scores / num_leaves(t)));
        BOOST_CHECK(res_k == ref_k);
    }

    BOOST_AUTO_TEST_CASE(test_assess_fragmentation_curve_DHaming_optimal_cut) {
        tree t(array_1d<index_t>{8, 8, 9, 9, 10, 10, 11, 13, 12, 12, 11, 13, 14, 14, 14});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::DHamming);

        BOOST_CHECK(assesser.optimal_number_of_regions() == 6);
        BOOST_CHECK(assesser.number_of_region_ground_truth() == 3);
        BOOST_CHECK(almost_equal(assesser.optimal_score(), (8.0) / num_leaves(t)));

        auto res = assesser.fragmentation_curve(true);
        auto &res_scores = res.scores;
        auto &res_k = res.k;

        array_1d<double> ref_scores{3, 5, 7, 7, 7, 8, 8, 8};
        array_1d<double> ref_k{1, 2, 3, 4, 5, 6, 7, 8};

        BOOST_CHECK(res_scores == (ref_scores / num_leaves(t)));
        BOOST_CHECK(res_k == (ref_k / 3));
    }

    BOOST_AUTO_TEST_CASE(test_assess_fragmentation_curve_Covering_optimal_cut) {
        tree t(array_1d<index_t>{8, 8, 9, 9, 10, 10, 11, 13, 12, 12, 11, 13, 14, 14, 14});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::Covering);

        BOOST_CHECK(assesser.optimal_number_of_regions() == 3);
        BOOST_CHECK(assesser.number_of_region_ground_truth() == 3);
        BOOST_CHECK(almost_equal(assesser.optimal_score(), (5 + 4.0 / 3) / num_leaves(t)));

        auto res = assesser.fragmentation_curve(false);
        auto &res_scores = res.scores;
        auto &res_k = res.k;

        array_1d<double> ref_scores{3, 5, 5 + 4.0 / 3, 5 + 2.0 / 3, 4 + 2.0 / 3, 2 + 8.0 / 3, 4, 3};
        array_1d<size_t> ref_k{1, 2, 3, 4, 5, 6, 7, 8};

        BOOST_CHECK(xt::allclose(res_scores, (ref_scores / num_leaves(t))));
        BOOST_CHECK(res_k == ref_k);
    }

    BOOST_AUTO_TEST_CASE(test_assess_optimal_partitions_BCE_optimal_cut) {
        tree t(array_1d<index_t>{8, 8, 9, 9, 10, 10, 11, 13, 12, 12, 11, 13, 14, 14, 14});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::BCE);

        std::vector<array_1d<index_t>> optimal_partitions
                {{0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 1, 1, 1},
                 {0, 0, 1, 1, 2, 2, 2, 2},
                 {0, 0, 1, 1, 2, 2, 2, 3},
                 {0, 0, 1, 1, 2, 2, 3, 4},
                 {0, 0, 1, 1, 2, 3, 4, 5},
                 {0, 0, 1, 2, 3, 4, 5, 6},
                 {0, 1, 2, 3, 4, 5, 6, 7}};

        BOOST_CHECK(is_in_bijection(optimal_partitions[2], assesser.optimal_partition()));

        for (index_t i = 0; i < optimal_partitions.size(); i++) {
            BOOST_CHECK(is_in_bijection(optimal_partitions[i], assesser.optimal_partition(i + 1)));
        }
    }

    BOOST_AUTO_TEST_CASE(test_straight_altitudes_BCE_optimal_cut) {
        tree t(array_1d<index_t>{8, 8, 9, 9, 10, 10, 11, 13, 12, 12, 11, 13, 14, 14, 14});
        array_1d<char> ground_truth{0, 0, 1, 1, 1, 2, 2, 2};

        assesser_optimal_cut assesser(t, ground_truth, optimal_cut_measure::BCE);

        auto altitudes = assesser.straightened_altitudes(false);
        array_1d<double> ref_scores{2.75, 4.5, 2 + 4.0 / 3 + 2.5, 2 + 4.0 / 3 + 2, 2 + 4.0 / 3 + 4.0 / 3,
                                    2 + 4.0 / 3 + 4.0 / 3, 4, 3};

        std::vector<array_1d<index_t>> optimal_partitions
                {{0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 1, 1, 1},
                 {0, 0, 1, 1, 2, 2, 2, 2},
                 {0, 0, 1, 1, 2, 2, 2, 3},
                 {0, 0, 1, 1, 2, 2, 3, 4},
                 {0, 0, 1, 1, 2, 2, 3, 4},
                 {0, 0, 1, 2, 3, 4, 5, 6},
                 {0, 1, 2, 3, 4, 5, 6, 7}};

        auto sorted = xt::sort(altitudes);

        for (index_t i = 0; i < optimal_partitions.size(); i++) {
            auto tmp = labelisation_horizontal_cut_from_threshold(t,
                                                                  altitudes,
                                                                  sorted(root(t) - i));
            BOOST_CHECK(is_in_bijection(optimal_partitions[i], tmp));
        }
    }

BOOST_AUTO_TEST_SUITE_END();