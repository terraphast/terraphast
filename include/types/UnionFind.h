/*** DO NOT INCLUDE DIRECTLY, INCLUDE types.h INSTEAD ***/
/*
 * UnionFind.h
 *
 *  Created on: 11.08.2014
 *      Author: Marcel Radermacher
 *      Changed a bit by Henning Meyerhenke to reflect union by rank and path compression
 *        as taught in "Algorithms 1"
 */
#pragma once

#include <vector>
#include <cstdio>
#include <cassert>
#include <limits>

/**
 * @ingroup structures
 * Implements the Union Find data structure to maintain disjoint sets efficiently.
 * Uses path compression and union by rank to achieve running time linear in
 * the number of elements times the inverse Ackermann function.
 */
class UnionFind {
private:
    std::vector<size_t> parent;
    std::vector<unsigned char> rank;    //TODO evtl uint verwenden?
    size_t num_elems;
public:
        
    /**
     * Create a new set representation with not more the \p max_element elements.
     * Initially every element is in its own set.
     * @param max_element maximum number of elements 
     */
    UnionFind(size_t max_element) : parent(max_element), rank(max_element, 0), num_elems(max_element) {
        allToSingletons();
    }

    /**
     * Creates a new union_find datastructure with given parent- and rank-arrays
     * @param p_parent the parent array
     * @param p_rank the rank array
     */
    UnionFind(std::vector<size_t> &p_parent, std::vector<unsigned char> &p_rank) : parent(p_parent), rank(p_rank), num_elems(parent.size()) {
        assert(p_parent.size() == p_rank.size());
    }


    /**
     * Assigns every element to a singleton set.
     * Set id is equal to element id.
     */

    void allToSingletons();

    /**
     * Find the representative to element \p u
     * @param u element
     * @return representative of set containing \p u
     */    
    size_t find(size_t u);
    
    /**
     *  Merge the two sets contain \p u and \p v
     *  @param u element u
     *  @param v element v
     */
    size_t merge(size_t u, size_t v);

    /**
     * @brief size getter for the size of the datastructure
     * @return size getter for the size of the datastructure
     */
    size_t size() const {
        return num_elems;
    }

    /**
     * @brief get_parent getter for the parent array
     * @return get_parent getter for the parent array
     */
    std::vector<size_t>& get_parent() {
        return parent;
    }

    /**
     * @brief get_rank getter for the rank array
     * @return get_rank getter for the rank array
     */
    std::vector<unsigned char>& get_rank() {
        return rank;
    }

};
