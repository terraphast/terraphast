/*** DO NOT INCLUDE DIRECTLY, INCLUDE types.h INSTEAD ***/
#pragma once

#include <sstream>
#include <gmpxx.h>

#include "functional.h"
#include "types.h"
/**
 * Abstract representative of any Tree object used internally.
 * Can not be instantiated, use correct sub-class for using trees.
 */
class Node {
// make classes that inherit be able to use the protected methods
friend class Leaf;
friend class InnerNode;
friend class UnrootedNode;
friend class AllLeafCombinationsNode;
friend class AllTreeCombinationsNode;
friend class UnrootedCombinationsNode;

public: 
    /**
     * Whether this node is used a leaf. Iff this returns true then get_leaf()
     * can safely be called.
     * @return Whether this node is used a leaf.
     */
    inline virtual bool is_leaf() const {
        return false;
    }
    /**
     * The leaf ID of this node. ONLY WORKS iff is_leaf() returns true.
     * @return The leaf ID of this node.
     */
    inline virtual leaf_number get_leaf() const {
        // may not be called for general node, use is_leaf() to check the call
        assert(false);
        return 0;
    }
    /**
     * Get a string representation of this node/tree. Recurses of all children,
     * if there are any.
     * @param id_to_label Mapper of leaf IDs to their label.
     * @return A string representation of this node/tree.
     */
    std::string to_newick_string(const label_mapper &id_to_label) const;

    /**
     * Returns a vector containing all constraints infered from this tree.
     * @return All constraints of this tree.
     */
    std::vector<constraint> extract_constraints() const;

    /**
     * Returns the number of actual trees.
     * If the tree denoted by this node is a compressed tree,
     * it will traverse and count all trees induced by that compressed tree.
     * If the tree is not a compressed tree, the result should be always 1.
     * @return
     */
    virtual mpz_class count_trees() const = 0;

protected:
    /** Used for recursion by extract_constraints */
    virtual std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const {
        // may not be called for general nodes
        assert(false);
        return std::make_tuple(0, 0);
    }

    /** Used for memory efficiency by to_newick_string */
    virtual void to_newick_string(std::stringstream &ss,
                                  const label_mapper &id_to_label) const = 0;
};

/**
 * Simple representative of a leaf. Can only be instantiated given the
 * represented leaf ID.
 */
class Leaf : public Node {
public:
    Leaf(leaf_number leaf_id) : leaf_id(leaf_id) {};

    const leaf_number leaf_id;

    bool is_leaf() const { return true; };
    leaf_number get_leaf() const { return leaf_id; }

    mpz_class count_trees() const {
        return 1;
    };

protected:
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Represents an inner node of a binary tree, therefore has exactly two
 * children. These need to be known upon creation.
 */
class InnerNode : public Node {
friend class Node;
public:
    InnerNode(const std::shared_ptr<Node> left,
              const std::shared_ptr<Node> right) :
            left(left), right(right) {
        assert(left != nullptr);
        assert(right != nullptr);
    };

    const std::shared_ptr<Node> left;
    const std::shared_ptr<Node> right;

    mpz_class count_trees() const {
        auto left_count = left->count_trees();
        left_count *= right->count_trees();
        return left_count;
    };

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Represents the "root" of an unrooted binary tree, meaning a node without a
 * parent, which has 3 children. It is created with minimal overhead by
 * "absorbing" a normal InnerNode and reusing it's children.
 */
class UnrootedNode : public Node {
private:
    const std::shared_ptr<InnerNode> inner;
public:
    UnrootedNode(const std::shared_ptr<InnerNode> inner) : inner(inner) {};

    mpz_class count_trees() const {
        return inner->count_trees();
    };

protected:
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Abstract representation of all possible binary trees that could be created
 * by using any combination of a set of leaves.
 * This can not be treated like a leaf, so is_leaf() returns false.
 */
class AllLeafCombinationsNode : public Node {
friend class UnrootedCombinationsNode;
public:
    AllLeafCombinationsNode(std::vector<leaf_number> leaves) :
            leaves(leaves) {
        assert(leaves.size() > 0);
    };

    const std::vector<leaf_number> leaves;

    mpz_class count_trees() const {
        // formula to count all trees is (2n-5)!!
        mpz_class result = 1;
        for(size_t i = 4; i <= (leaves.size() + 1); i++) {
            result *= (2*i-5);
        }
        return result;
    }

protected:
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Abstract representation of all possible binary trees that could be created
 * by using any combination of a se of trees.
 * Trees can be added ad-hoc with add_tree(), so the object can be created
 * before all trees are known.
 */
class AllTreeCombinationsNode : public Node {
public:
    AllTreeCombinationsNode(std::vector<std::shared_ptr<Node>> trees) :
            trees(trees) {
        for(auto tree:trees) {
            assert(tree != nullptr);
        }
    }

    mpz_class count_trees() const {
        mpz_class result = 0;
        for(auto tree:trees) {
            result += tree->count_trees();
        }
        return result;
    }

    std::vector<std::shared_ptr<Node>> trees;

    inline void add_tree(std::shared_ptr<Node> tree) {
        assert(tree != nullptr);
        trees.push_back(tree);
    }

protected:
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Represents the "root" of an unrooted binary tree, meaning a node without a
 * parent, which has 3 children. It is created with minimal overhead by
 * "absorbing" a normal InnerNode and reusing it's children.
 */
class UnrootedCombinationsNode : public Node {
private:
    const std::shared_ptr<AllLeafCombinationsNode> combinations;
public:
    UnrootedCombinationsNode(
            const std::shared_ptr<AllLeafCombinationsNode> combinations) :
            combinations(combinations) {};

    mpz_class count_trees() const {
        return combinations->count_trees();
    }


protected:
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Leaf> LeafPtr;
typedef std::shared_ptr<InnerNode> InnerNodePtr;
typedef std::shared_ptr<UnrootedNode> UnrootedNodePtr;
typedef std::shared_ptr<AllLeafCombinationsNode> AllLeafCombinationsNodePtr;
typedef std::shared_ptr<AllTreeCombinationsNode> AllTreeCombinationsNodePtr;
typedef std::shared_ptr<UnrootedCombinationsNode> UnrootedCombinationsNodePtr;
// for prettier code
typedef std::shared_ptr<Node> Tree;

