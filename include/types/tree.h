/*** DO NOT INCLUDE DIRECTLY, INCLUDE types.h INSTEAD ***/
#pragma once

#include <sstream>

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

public: 
    /**
     * Whether this node is used a leaf. Iff this returns true then get_leaf()
     * can safely be called.
     * @return Whether this node is used a leaf.
     */
    virtual bool is_leaf() const = 0;
    /**
     * The leaf ID of this node. ONLY WORKS iff is_leaf() returns true.
     * @return The leaf ID of this node.
     */
    virtual leaf_number get_leaf() const = 0;

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

protected:
    /** Used for recursion by extract_constraints */
    virtual std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const = 0;
    /** Used for memory efficiency by to_newick_string */
    virtual void to_newick_string(std::stringstream &ss,
                                  const label_mapper &id_to_label) const = 0;
};

/**
 * Simple representative of a leaf. Can only be instantiated given the
 * represented leaf ID.
 */
class Leaf : public Node {
private:
    leaf_number leaf_id;
public:
    Leaf(leaf_number leaf_id) : leaf_id(leaf_id) {};

    bool is_leaf() const { return true; }
    leaf_number get_leaf() const { return leaf_id; }

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Represents an inner node of a binary tree, therefore has exactly two
 * children. These need to be known upon creation.
 */
class InnerNode : public Node {
friend class Node;
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
public:
    InnerNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) :
            left(left), right(right) {
                assert(left != nullptr);
                assert(right != nullptr);
            };

    bool is_leaf() const { return false; }
    leaf_number get_leaf() const {
        // may not be called, use is_leaf() to prevent calling it
        assert(false);
        return 0;
    }

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Represents the "root" of an unrooted binary tree, meaning a node without a
 * parent, which has 3 children.
 */
class UnrootedNode : public Node {
private:
    std::shared_ptr<Node> elem1;
    std::shared_ptr<Node> elem2;
    std::shared_ptr<Node> elem3;
public:
    UnrootedNode(std::shared_ptr<Node> elem1, std::shared_ptr<Node> elem2,
                 std::shared_ptr<Node> elem3) :
            elem1(elem1), elem2(elem2), elem3(elem3) {};

    bool is_leaf() const { return false; }
    leaf_number get_leaf() const {
        // may not be called, use is_leaf() to prevent calling it
        assert(false);
        return 0;
    }

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Abstract representation of all possible binary trees that could be created
 * by using any combination of a set of leaves.
 * This can not be treated like a leaf, so is_leaf() returns false.
 */
class AllLeafCombinationsNode : public Node {
private:
    std::vector<leaf_number> leaves;
public:
    AllLeafCombinationsNode(std::vector<leaf_number> leaves) : 
            leaves(leaves) {
        assert(leaves.size() > 0);
    };

    bool is_leaf() const { return false; }
    leaf_number get_leaf() const {
        // may not be called, use is_leaf() to prevent calling it
        assert(false);
        return 0;
    }

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

/**
 * Abstract representation of all possible binary trees that could be created
 * by using any combination of a set of .
 * This can not be treated like a leaf, so is_leaf() returns false.
 */
class AllTreeCombinationsNode : public Node {
private:
    std::vector<std::shared_ptr<Node>> trees;
public:
    AllTreeCombinationsNode(std::vector<std::shared_ptr<Node>> trees) :
            trees(trees) {
        for(auto tree:trees) {
            assert(tree != nullptr);
        }
    }

    bool is_leaf() const { return false; }
    leaf_number get_leaf() const {
        // may not be called, use is_leaf() to prevent calling it
        assert(false);
        return 0;
    }

protected:
    std::tuple<leaf_number, leaf_number> get_constraints(
            std::vector<constraint> &constraints) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Leaf> LeafPtr;
typedef std::shared_ptr<InnerNode> InnerNodePtr;
typedef std::shared_ptr<UnrootedNode> UnrootedNodePtr;
typedef std::shared_ptr<AllLeafCombinationsNode> AllLeafCombinationsNodePtr;
typedef std::shared_ptr<AllTreeCombinationsNode> AllTreeCombinationsNodePtr;
// for legacy purposes
typedef std::shared_ptr<Node> Tree;

