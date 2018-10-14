/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#pragma once

#include "../graph.hpp"
#include "../accumulator/tree_accumulator.hpp"
#include "../structure/array.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xindex_view.hpp"

namespace hg {

    /**
     * The area  of a node n of the tree t is equal to the sum of the area of the leaves in the subtree rooted in n.
     *
     * area(n) = sum_{l in leaves(t), l is a  descendant of n} area(n)
     *
     * @tparam tree_t tree type
     * @tparam T xexpression derived type of xleaf_area
     * @param tree input tree
     * @param xleaf_area area of the leaves of the input tree
     * @return an array with the area of each node of the tree
     */
    template<typename tree_t, typename T>
    auto attribute_area(const tree_t &tree, const xt::xexpression<T> &xleaf_area) {
        auto &leaf_area = xleaf_area.derived_cast();
        hg_assert(leaf_area.shape()[0] == num_leaves(tree),
                  "leaf_area size does not match the number of leaves in the tree.");
        return accumulate_sequential(tree, leaf_area, accumulator_sum());
    }

    /**
     * The area  of a node n of the tree t is equal to the number of leaves in the subtree rooted in n.
     *
     * area(n) = |{l in leaves(t), l is a  descendant of n}|
     *
     * @tparam tree_t tree type
     * @param tree input tree
     * @return an array with the area of each node of the tree
     */
    template<typename tree_t>
    auto attribute_area(const tree_t &tree) {
        return attribute_area(tree, xt::ones<long>({num_leaves(tree)}));
    }

    /**
     * The volume of a node n of the tree t is defined recursively as:
     *    volume(n) = abs(altitude(n) - altitude(parent(n)) * area(n) + sum_{c in children(n, t)} volume(c)
     *
     * @tparam tree_t tree type
     * @tparam T1 xexpression derived type of xnode_altitude
     * @tparam T2 xexpression derived type of xnode_area
     * @param tree input tree
     * @param xnode_altitude altitude of the nodes of the input tree
     * @param xnode_area area of the nodes of the input tree
     * @return an array with the volume of each node of the tree
     */
    template<typename tree_t, typename T1, typename T2>
    auto attribute_volume(const tree_t &tree, const xt::xexpression<T1> &xnode_altitude,
                          const xt::xexpression<T2> &xnode_area) {
        auto &node_area = xnode_area.derived_cast();
        auto &node_altitude = xnode_altitude.derived_cast();
        hg_assert(node_area.dimension() == 1, "node_area must be a 1d array");
        hg_assert(node_altitude.dimension() == 1, "node_altitude must be a 1d array");
        hg_assert(node_area.size() == num_vertices(tree),
                  "node_area size does not match the number of nodes in the tree.");
        hg_assert(node_altitude.size() == num_vertices(tree),
                  "node_altitude size does not match the number of nodes in the tree.");

        auto &parent = tree.parents();
        array_1d<double> volume = xt::empty<double>({tree.num_vertices()});
        xt::view(volume, xt::range(0, num_leaves(tree))) = 0;
        for (auto i: leaves_to_root_iterator(tree, leaves_it::exclude)) {
            volume(i) = std::fabs(node_altitude(i) - node_altitude(parent(i))) * node_area(i);
            for (auto c: tree.children(i)) {
                volume(i) += volume(c);
            }
        }
        return volume;
    }

    /**
     * The depth of a node n of the tree t is equal to the number of ancestors of n.
     *
     * @tparam tree_t tree type
     * @param tree input tree
     * @return an array with the depth of each node of the tree
     */
    template<typename tree_t>
    auto attribute_depth(const tree_t &tree) {
        array_1d<long> depth = xt::empty<long>({tree.num_vertices()});
        depth(tree.root()) = 0;
        for (auto i: root_to_leaves_iterator(tree, leaves_it::include, root_it::exclude)) {
            depth(i) = depth(parent(i, tree)) + 1;
        }
        return depth;
    };

    /**
     * In a tree t, given that the altitudes of the nodes vary monotically from the leaves to the root,
     * the height of a node n of t is equal to the difference between the altitude of n and
     * the altitude of the deepest leave in the subtree of t rooted in n.
     *
     *
     * If increasing_altitude is true, this means that altitudes are increasing from the leaves to the root
     * (ie. for any node n, altitude(n) <= altitude(parent(n)).
     * Else, if increasing_altitude is false, this means that altitudes are decreasing from the leaves to the root
     * (ie. for any node n, altitude(n) >= altitude(parent(n)).
     *
     * PRE-CONDITION: altitudes of the nodes vary monotically from the leaves to the root
     *
     * @tparam tree_t tree type
     * @tparam T xexpression derived type of xnode_altitude
     * @param tree input tree
     * @param xnode_altitude altitude of the nodes of the input tree
     * @param increasing_altitude true if altitude is increasing, false if it is decreasing
     * @return an array with the height of each node of the tree
     */
    template<typename tree_t, typename T>
    auto
    attribute_height(const tree_t &tree, const xt::xexpression<T> &xnode_altitude, bool increasing_altitude = true) {
        auto &node_altitude = xnode_altitude.derived_cast();
        hg_assert(node_altitude.dimension() == 1, "node_altitude must be a 1d array");
        hg_assert(node_altitude.shape()[0] == num_vertices(tree),
                  "node_altitude size does not match the number of nodes in the tree.");

        if (increasing_altitude) {
            auto extrema = accumulate_sequential(tree, xt::view(node_altitude, xt::range(0, num_leaves(tree))),
                                                 accumulator_min());
            return xt::eval(node_altitude - extrema);
        } else {
            auto extrema = accumulate_sequential(tree, xt::view(node_altitude, xt::range(0, num_leaves(tree))),
                                                 accumulator_max());
            return xt::eval(extrema - node_altitude);
        }
    };

    /**
     * Given a node n of the tree t, the dynamics of n is the difference between
     * the altitude of the deepest minima of the subtree rooted in n and the altitude
     * of the closest ancestor of n that has a deeper minima in its subtree. If no such
     * ancestor exists then, the dynamics of n is equal to the difference between the
     * altitude of the highest node of the tree (the root) and the depth of the deepest minima.
     *
     * @tparam tree_t tree type
     * @tparam T xexpression derived type of xaltitude
     * @param tree input tree
     * @param xaltitude altitude of the nodes of the input tree
     * @return an array with the dynamics of each node of the tree
     */
    template<typename tree_t, typename T>
    auto attribute_dynamics(const tree_t &tree, const xt::xexpression<T> &xaltitude) {
        auto &altitude = xaltitude.derived_cast();
        using value_type = typename T::value_type;
        hg_assert(altitude.dimension() == 1, "node_altitude must be a 1d array");
        hg_assert(altitude.shape()[0] == num_vertices(tree),
                  "node_altitude size does not match the number of nodes in the tree.");


        auto min_depth = xt::empty_like(altitude);
        for (auto n: leaves_to_root_iterator(tree, leaves_it::exclude)) {
            value_type min_children = std::numeric_limits<value_type>::max();
            bool flag = false;
            for (auto c: children_iterator(n, tree)) {
                if (!is_leaf(c, tree)) {
                    flag = true;
                    min_children = std::min(min_children, min_depth(c));
                }
            }
            if (flag) {
                min_depth(n) = min_children;
            } else {
                min_depth(n) = altitude(n);
            }
        }

        auto dynamics = xt::empty_like(altitude);
        dynamics(root(tree)) = altitude(root(tree)) - min_depth(root(tree));
        xt::view(dynamics, xt::range(0, num_leaves(tree))) = 0;
        for (auto n: root_to_leaves_iterator(tree, leaves_it::exclude, root_it::exclude)) {
            auto pn = parent(n, tree);
            if (min_depth(n) == min_depth(pn)) {
                dynamics(n) = dynamics(pn);
            } else {
                dynamics(n) = altitude(pn) - min_depth(n);
            }
        }

        return dynamics;
    };
}
