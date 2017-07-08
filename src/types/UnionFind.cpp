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
		parent[i] = i;
	}
}

size_t UnionFind::find(size_t u) {
	if (parent[u] == u) {
		return u;
	}
	else {
		// recursion and path compression
		parent[u] = find(parent[u]);
		return parent[u];
	}
}

void UnionFind::merge(size_t u, size_t v) {
	size_t set_u = find(u);
	size_t set_v = find(v);
	if (set_u == set_v) return;

	if (rank[set_u] < rank[set_v]) {
		parent[set_u] = set_v;
	}
	else {
		parent[set_v] = set_u;
		if (rank[set_u] == rank[set_v]) {
			rank[set_u]++;
		}
	}
}

