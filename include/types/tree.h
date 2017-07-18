#pragma once

#include "functional.h"
#include "types.h"

class Tree {
public:
    Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right) :
            left(p_left), right(p_right) {
    }

    Tree(leaf_number p_id) :
            id(p_id), left(nullptr), right(nullptr) {
    }

    Tree() :
            left(nullptr), right(nullptr) {
    }

    leaf_number id;
    std::shared_ptr<Tree> left;
    std::shared_ptr<Tree> right;
    std::weak_ptr<Tree> parent;

    inline bool is_leaf() const {
        return (left == nullptr && right == nullptr);
    }

    std::string to_newick_string(const std::vector<std::string> &id_to_lable) const;

    std::string to_newick_string(const std::vector<std::string> &id_to_lable,
                                 const std::string &root_label) const;

protected:
    void to_newick_string(std::stringstream &ss,
                          const std::vector<std::string> &id_to_lable) const;
};

std::ostream &operator<<(std::ostream &strm, const std::shared_ptr<Tree> tree);

std::shared_ptr<Tree> root(const std::shared_ptr<Tree> tree);

std::tuple<std::shared_ptr<Tree>, std::shared_ptr<Tree>> deep_copy(std::shared_ptr<Tree> tree);

