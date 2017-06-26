#pragma once

#ifndef IFUGAO_TREE_H
#define IFUGAO_TREE_H

#include "functional.h"

#include <memory>
#include <assert.h>

class Tree {
public:
	Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right,
			std::shared_ptr<Tree> p_parent, std::string p_label) :
			label(p_label), left(p_left), right(p_right), parent(p_parent) {
	}

	Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right,
			std::string p_label) :
			label(p_label), left(p_left), right(p_right), parent(nullptr) {
	}

	Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right) :
			left(p_left), right(p_right), parent(nullptr) {
	}

	Tree(std::shared_ptr<Tree> p_left, std::shared_ptr<Tree> p_right,
			std::shared_ptr<Tree> p_parent) :
			left(p_left), right(p_right), parent(p_parent) {
	}

	Tree(std::string p_label, std::shared_ptr<Tree> p_parent) :
			label(p_label), left(nullptr), right(nullptr), parent(p_parent) {
	}

	Tree(std::string p_label) :
			label(p_label), left(nullptr), right(nullptr), parent(nullptr) {
	}

	Tree() :
			left(nullptr), right(nullptr), parent(nullptr) {
	}

	//TODO getter and setter? //performance vs. code quality?
	std::string label;
	std::shared_ptr<Tree> left;
	std::shared_ptr<Tree> right;
	std::shared_ptr<Tree> parent;

	inline bool is_leaf() const {
		return (left == nullptr && right == nullptr);
	}

	std::string to_newick_string();
	std::string to_newick_string(const std::string &root_label);

    template<typename T>
    using TreeNodeVisitor = T (*)(const Tree*);

    template<typename T>
    T fold (BinaryOperator<T> op, TreeNodeVisitor<T> v) const {
        if(this->is_leaf()) {
            return v(this);
        }

        assert(this->left != nullptr);
        assert(this->right != nullptr);
        return op(this->left->fold(op, v),
                  this->right->fold(op, v));
    }

    size_t count() const;
};

std::ostream& operator<<(std::ostream &strm, const std::shared_ptr<Tree> tree);

std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree);

#endif /* IFUGAO_TREE_H */
