#include "util.h"
#include "debug.h"

std::shared_ptr<Tree> generate_induced_tree(const std::shared_ptr<Tree> tree,
                                            const missingData *missing_data,
                                            const std::map<std::string, leaf_number> &species_map,
                                            const std::vector<std::string> &id_to_label,
                                            const size_t partition) {
    if (tree == nullptr) {
        return nullptr;
    }

    if (tree->is_leaf()) {
        if (species_map.count(id_to_label[tree->id]) == 1
            && getDataMatrix(missing_data,
                             species_map.at(id_to_label[tree->id]),
                             partition)) {
            auto leaf = std::make_shared<Tree>();
            leaf->id = tree->id;
            leaf->left = nullptr;
            leaf->right = nullptr;
            return tree;
        }
    } else {
        // Inner node case
        assert(tree->left != nullptr);
        auto left = generate_induced_tree(tree->left, missing_data, species_map,
                                          id_to_label, partition);
        assert(tree->right != nullptr);
        auto right = generate_induced_tree(tree->right, missing_data, species_map,
                                           id_to_label, partition);
        //Left and right subtrees are included -> common ancestor is included
        if (left != nullptr && right != nullptr) {
            auto inner_node = std::make_shared<Tree>();
            left->parent = inner_node;
            right->parent = inner_node;
            inner_node->left = left;
            inner_node->right = right;
            return inner_node;
        } else if (left != nullptr) {
            return left;
        } else if (right != nullptr) {
            return right;
        }
    }
    return nullptr;
}

std::vector<std::string> get_root_species(const missingData *missing_data) {
    assert(missing_data != nullptr);

    std::vector<std::string> root_species;
    for (size_t i = 0; i < missing_data->numberOfSpecies; i++) {
        bool contains_all_data = true;
        for (size_t k = 0; k < missing_data->numberOfPartitions; k++) {
            if (getDataMatrix(missing_data, i, k) == 0) {
                contains_all_data = false;
                break;
            }
        }
        if (contains_all_data) {
            root_species.emplace_back(missing_data->speciesNames[i]);
        }
    }
    return root_species;
}

std::shared_ptr<Tree> root_tree(ntree_t *tree,
                                const missingData *missing_data,
                                std::string &root_species_name,
                                std::vector<std::string> &id_to_label) {
    assert(tree != nullptr);
    size_t root_species_number = 0;
    bool root_specied_found = false;
    for (size_t i = 0; i < missing_data->numberOfSpecies; i++) {
        bool contains_all_data = true;
        for (size_t k = 0; k < missing_data->numberOfPartitions; k++) {
            if (getDataMatrix(missing_data, i, k) == 0) {
                contains_all_data = false;
                break;
            }
        }
        if (contains_all_data) {
            root_specied_found = true;
            root_species_number = i;
            break;
        }
    }
    if (root_specied_found) {
        ntree_t *future_root = get_leaf_by_name(tree,
                                                missing_data->speciesNames[root_species_number]);
        root_species_name = missing_data->speciesNames[root_species_number];
        if (future_root == nullptr) {
            dout("In root_tree(...): label "
                         << missing_data->speciesNames[root_species_number]
                         << " not found in tree" << std::endl);
            return nullptr;
        }
        return root_at(future_root, id_to_label);
    } else {
        //tree cannot be rooted consistently
        return nullptr;
    }
}

ntree_t *get_leaf_by_name(ntree_t *tree, const char *label) {
    assert(tree != nullptr);
    assert(label != nullptr);
    if (tree->label != nullptr && strcmp(tree->label, label) == 0) {
        return tree;
    } else {
        for (int i = 0; i < tree->children_count; i++) {
            assert(tree->children[i]->parent != nullptr);
            if (tree != tree->children[i]->parent) {
                if (tree->children[i]->label != nullptr) {
                    dout("parent von " << tree->children[i]->label
                                       << " ist falsch\n");
                } else {
                    dout("parent von " << tree->children[i]
                                       << "ist falsch\n");
                }

            }
            //assert(tree == tree->children[i]->parent);
            ntree_t *temp = get_leaf_by_name(tree->children[i], label);
            if (temp != nullptr) {
                return temp;
            }
        }
    }
    return nullptr; //label not found
}

std::shared_ptr<Tree> root_at(ntree_t *leaf, std::vector<std::string> &id_to_label) {
    //if leaf->parent is null, leaf is the root => the tree is not binary, or the node is no leaf
    assert(leaf != nullptr);
    assert(leaf->parent != nullptr);
    assert(leaf->children_count == 0);  //should be a leaf
    ntree_t *neighbour = leaf->parent;
    std::shared_ptr<Tree> root = std::make_shared<Tree>();

    recursive_root(root, neighbour, leaf, id_to_label);
    return root;
}

void recursive_root(std::shared_ptr<Tree> current, ntree_t *current_ntree,
                    ntree_t *parent, std::vector<std::string> &id_to_label) {

    assert(current != nullptr);
    assert(current_ntree != nullptr);

    //when the children_count is 3, we are at the pseudoroot of the unrooted binary tree
    assert(current_ntree->children_count == 2
           || current_ntree->children_count == 0
           || (current_ntree->children_count == 3
               && current_ntree->parent == nullptr));

    if (current_ntree->label != nullptr) {
        current->id = id_to_label.size();
        id_to_label.push_back(std::string(current_ntree->label));
    }

    if (current_ntree->children_count == 0) {   //is leaf
        return;
    }

    current->left = std::make_shared<Tree>();
    current->left->parent = current;
    current->right = std::make_shared<Tree>();
    current->right->parent = current;

    ntree_t *left_ntree;
    ntree_t *right_ntree;

    if (current_ntree->children_count == 3) {   //we are at the pseudoroot
        //set the children, leave out the parent
        if (current_ntree->children[0] == parent) {
            left_ntree = current_ntree->children[1];
            right_ntree = current_ntree->children[2];
        } else if (current_ntree->children[1] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[2];
        } else if (current_ntree->children[2] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[1];
        } else {
            assert(0);
            return;
        }
    } else if (current_ntree->parent == nullptr) { //we are at the regular root
        current->left = nullptr;
        current->right = nullptr;
        if (current_ntree->children[0] == parent) {
            recursive_root(current, current_ntree->children[1], current_ntree, id_to_label);
            return;
        } else {
            recursive_root(current, current_ntree->children[0], current_ntree, id_to_label);
            return;
        }
    } else {
        assert(current_ntree->children_count == 2);
        //set the children, leave out the parent
        if (current_ntree->children[0] == parent) {
            left_ntree = current_ntree->children[1];
            right_ntree = current_ntree->parent;
        } else if (current_ntree->children[1] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->parent;
        } else if (current_ntree->parent == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[1];
        } else {
            assert(0);
            return;
        }
    }

    recursive_root(current->left, left_ntree, current_ntree, id_to_label);
    recursive_root(current->right, right_ntree, current_ntree, id_to_label);
}

bool check_tree(ntree_t *tree) {
    for (int i = 0; i < tree->children_count; i++) {
        if (tree != tree->children[i]->parent) {
            if (tree->children[i]->label != nullptr) {
                std::cout << tree << ": parent of " << tree->children[i]->label
                          << " is wrong: " << tree->children[i]->parent
                          << std::endl;
            } else {
                std::cout << tree << ": parent of " << tree->children[i]
                          << " is wrong: " << tree->children[i]->parent
                          << std::endl;
            }
        }
        check_tree(tree->children[i]);
    }
    return true;
}

void fix_tree(ntree_t *tree) {
    assert(tree != nullptr);
    int x = tree->children_count;
    for (int i = 0; i < x; i++) {
        tree->children[i]->parent = tree;
        fix_tree(tree->children[i]);
    }
}

std::vector<std::shared_ptr<Tree>> get_neighbours(std::shared_ptr<Tree> node) {
    std::vector<std::shared_ptr<Tree>> neighbours;
    if (node->left != nullptr) {
        neighbours.push_back(node->left);
    }
    if (node->right != nullptr) {
        neighbours.push_back(node->right);
    }
    auto p = node->parent.lock();
    if (p != nullptr) {
        neighbours.push_back(p);
    }
    return neighbours;
}


void get_leafs(std::vector<std::shared_ptr<Tree>> &leaf_list, std::shared_ptr<Tree> current_node,
               std::shared_ptr<Tree> parent) {
    std::vector<std::shared_ptr<Tree>> neighbours = get_neighbours(current_node);
    if (neighbours.size() == 1) {   //if we are at a leaf
        leaf_list.push_back(current_node);
        return;
    }
    for (size_t i = 0; i < neighbours.size(); i++) {
        if (neighbours.at(i) != parent) {   //skip the node where we came from
            get_leafs(leaf_list, neighbours.at(i), current_node);
        }
    }
}

static void d_print_tree_rec(std::ostream &strm,
                             const ntree_t *tree,
                             const int depth) {

    for (int j = 0; j < depth * 4; j++) {
        strm << " ";
    }
    strm << "<node addr=\"" << static_cast<const void *>(tree) << '"';
    if (tree->label != nullptr) {
        strm << " label=\"" << tree->label << '"';
    }
    strm << ">";
    if (tree->children_count > 0) {
        strm << std::endl;
    }
    for (int i = 0; i < tree->children_count; i++) {
        d_print_tree_rec(strm, tree->children[i], depth + 1);
    }
    if (tree->children_count > 0) {
        for (int j = 0; j < depth * 4; j++) {
            strm << " ";
        }
    }
    strm << "<node/>" << std::endl;
}

static short isBinaryAndConsistentRec(ntree_t *tree, std::set<std::string> &set) {
    if(tree->children_count == 0) {
      // tree consisting out of one leaf
      if(set.erase(tree->label) != 1) {
          // label is in tree but not in data file
          return -2;
      }
      return 0;
    } else if(tree->children_count == 2) {
      short ret;
      ret = isBinaryAndConsistentRec(tree->children[0], set);
      if(ret != 0) {
         return ret;
      }
      ret = isBinaryAndConsistentRec(tree->children[1], set);
      if(ret != 0) {
         return ret;
      }
      return 0;
    } 
    return -1;
}

short isBinaryAndConsistent(ntree_t *tree, missingData* missing_data) {
    std::set<std::string> set;
    for (size_t i = 0; i < missing_data->numberOfSpecies; i++) {
        set.insert(missing_data->speciesNames[i]);
    }

    if(tree->children_count == 0) {
      // tree consisting out of one leaf
      if(set.erase(tree->label) != 1) {
          // label is in tree but not in data file
          return -2;
      }
    } else if(tree->children_count == 3) {
      // unrooted tree
      short ret;
      ret = isBinaryAndConsistentRec(tree->children[0], set);
      if(ret != 0) {
         return ret;
      }
      ret = isBinaryAndConsistentRec(tree->children[1], set);
      if(ret != 0) {
         return ret;
      }
      ret = isBinaryAndConsistentRec(tree->children[2], set);
      if(ret != 0) {
         return ret;
      }
    } else if(tree->children_count == 2) {
      // rooted tree
      short ret;
      ret = isBinaryAndConsistentRec(tree->children[0], set);
      if(ret != 0) {
         return ret;
      }
      ret = isBinaryAndConsistentRec(tree->children[1], set);
      if(ret != 0) {
         return ret;
      }
    } else {
      return -1;
    }

    if(!set.empty()) {
      return -2;
    } 
    return 0;
}

std::ostream &operator<<(std::ostream &strm, const ntree_t *tree) {
    strm << "Dump ntree_t:" << std::endl;
    d_print_tree_rec(strm, tree, 0);
    return strm;
}
