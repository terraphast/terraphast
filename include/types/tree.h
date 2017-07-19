/*** DO NOT INCLUDE DIRECTLY, INCLUDE types.h INSTEAD ***/
#pragma once

#include <sstream>

#include "functional.h"
#include "types.h"

class Node {
// make classes that inherit be able to use the protected methods
friend class Leaf;
friend class InnerNode;
friend class UnrootedNode;
friend class AllBinaryCombinationsNode;

public: 
    virtual bool is_leaf() const = 0;
    virtual leaf_number get_leaf() const = 0;

    std::string to_newick_string(const label_mapper &id_to_label) const;

    /**
     * Returns a vector containing all constraints infered from this tree.
     *
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

class AllBinaryCombinationsNode : public Node {
private:
    std::vector<leaf_number> leaves;
public:
    AllBinaryCombinationsNode(std::vector<leaf_number> leaves) : leaves(leaves)
    {
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

typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Leaf> LeafPtr;
typedef std::shared_ptr<InnerNode> InnerNodePtr;
typedef std::shared_ptr<UnrootedNode> UnrootedNodePtr;
typedef std::shared_ptr<AllBinaryCombinationsNode> AllBinaryCombinationsNodePtr;
// for legacy purposes
typedef std::shared_ptr<Node> Tree;


/*
class Tree : public std::enable_shared_from_this<Tree> {
public:
    leaf_number id;
    std::shared_ptr<Tree> left;
    std::shared_ptr<Tree> right;
    std::weak_ptr<Tree> parent;

    Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right) :
            left(p_left), right(p_right) {
    }

    Tree(leaf_number p_id) :
            id(p_id), left(nullptr), right(nullptr) {
    }

    Tree() :
            left(nullptr), right(nullptr) {
    }

    virtual ~Tree() {}

    virtual inline bool is_leaf() const {
        return (left == nullptr && right == nullptr);
    }

    virtual std::string to_newick_string(const label_mapper &id_to_label) const;

    virtual std::string to_newick_string_root(
            const label_mapper &id_to_label) const;

    virtual std::tuple<std::shared_ptr<Tree>, std::shared_ptr<Tree>> deep_copy();

    virtual std::shared_ptr<Tree> root();

    virtual void to_newick_string(std::stringstream &ss,
                                  const label_mapper &id_to_label) const;

protected:
    std::shared_ptr<Tree> deep_copy(std::map<std::shared_ptr<Tree>,
            std::shared_ptr<Tree>> &cover_map);
};


class PartitionNode : public Tree {
public:
    PartitionNode() {}

    std::vector<std::shared_ptr<Tree>> partitions;

    inline bool is_leaf() const {
        return false;
    }

    std::string to_newick_string_root(const label_mapper &ids_to_labels) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};

class LeafSetNode : public Tree {
public:
    LeafSetNode() {}

    LeafSetNode(const std::set<leaf_number> &p_leaves) : leaves(p_leaves) {
    }

    std::set<leaf_number> leaves;

    inline bool is_leaf() const {
        return false;
    }

    std::string to_newick_string_root(const label_mapper &ids_to_labels) const;
    void to_newick_string(std::stringstream &ss,
                          const label_mapper &id_to_label) const;
};
 */
