#pragma once

#include "functional.h"
#include "types.h"

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

    virtual std::string to_newick_string(const std::vector<std::string> &id_to_lable) const;

    virtual std::string to_newick_string(const std::vector<std::string> &id_to_lable,
                                         const std::string &root_label) const;

    virtual std::tuple<std::shared_ptr<Tree>, std::shared_ptr<Tree>> deep_copy();

    virtual std::shared_ptr<Tree> root();

    virtual void to_newick_string(std::stringstream &ss,
                                  const std::vector<std::string> &id_to_lable) const;

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

    std::string to_newick_string(const std::vector<std::string> &ids_to_lables,
                                    const std::string &root_label) const;
    void to_newick_string(std::stringstream &ss,
                          const std::vector<std::string> &id_to_lable) const;
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

    std::string to_newick_string(const std::vector<std::string> &ids_to_lables,
                                 const std::string &root_label) const;
    void to_newick_string(std::stringstream &ss,
                          const std::vector<std::string> &id_to_lable) const;
};
