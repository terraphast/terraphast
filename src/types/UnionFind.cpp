/*
 * UnionFind.cpp
 *
 *  Created on: 11.08.2014
 *      Author: Marcel Radermacher
 *      Changed a bit by Henning Meyerhenke to reflect union by rank and path compression
 *        as taught in "Algorithms 1"
 */

#include "types/UnionFind.h"

void UnionFind::allToSingletons() {
    for (size_t i = 0; i < parent.size(); ++i) {
        if (parent.at(i) != std::numeric_limits<size_t>::max()) {
            parent[i] = i;
        }
    }
}

//Johannes: added check if element is valid
size_t UnionFind::find(size_t u) {
    //TODO not sure whether we should just assert this
    if (u >= this->num_elems) {
        return std::numeric_limits<size_t>::max();
    }

    if (parent[u] == std::numeric_limits<size_t>::max()) {
        return parent[u];
    }

    if (parent[u] == u) {
        return u;
    }
    else {
        // recursion and path compression
        parent[u] = find(parent[u]);
        return parent[u];
    }
}

//Johannes: added return value, so that the wrapper class
//knows the new represetnative
size_t UnionFind::merge(size_t u, size_t v) {
    size_t set_u = find(u);
    size_t set_v = find(v);
    if (set_u == set_v) {
        return set_u;
    }

    if (rank[set_u] < rank[set_v]) {
        parent[set_u] = set_v;
        return set_v;
    }
    else {
        parent[set_v] = set_u;
        if (rank[set_u] == rank[set_v]) {
            rank[set_u]++;
        }
        return set_u;
    }
}

