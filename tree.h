#ifndef TREE_H
#define TREE_H

#include <memory>

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
	//double length;  //TODO do we need the length?
	std::shared_ptr<Tree> left;
	std::shared_ptr<Tree> right;
	std::shared_ptr<Tree> parent;

	// unused from newick structure:
	//unsigned int leaves;
	//char * color;
	//int mark;
	//void * data;

	inline bool is_leaf() {
		return (left == nullptr && right == nullptr);
	}

	std::string to_newick_string();
};

std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree);

#endif /* TREE_H */
