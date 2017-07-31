#include "util.h"
#include "debug.h"

Tree generate_induced_tree(const NodePtr node,
                           const missingData *missing_data,
                           const std::map<std::string, leaf_number> &species_map,
                           const label_mapper &id_to_label,
                           const size_t partition) {
    assert(node != nullptr);

    if (node->is_leaf()) {
        const LeafPtr leaf = std::static_pointer_cast<Leaf>(node);
        size_t leaf_id = leaf->leaf_id;
        
        // TODO ifs into asserts?
        if (species_map.count(id_to_label[leaf_id]) == 1
            && getDataMatrix(missing_data,
                             species_map.at(id_to_label[leaf_id]),
                             partition)) {
            return node;
        } else {
            return nullptr;
        }
    } else {
        // Inner node case
        //TODO some kind of assert that is InnerNode (with dynamic_cast?)
        const InnerNodePtr inner = std::static_pointer_cast<InnerNode>(node);
        auto left = generate_induced_tree(inner->left, missing_data,
                species_map, id_to_label, partition);
        auto right = generate_induced_tree(inner->right, missing_data,
                species_map, id_to_label, partition);
        if (left != nullptr && right != nullptr) {
            // left & right subtrees are included -> common ancestor is included
            NodePtr new_inner = std::make_shared<InnerNode>(left, right);
            return new_inner;
        } else if (left != nullptr) {
            return left;
        } else {
            return right; // might be nullptr, but that's ok
        }
    }
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

Tree root_tree(ntree_t *nwk_tree,
               const missingData *missing_data,
               label_mapper &id_to_label) {
    assert(nwk_tree != nullptr);
    
    size_t root_species_id = 0;
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
            root_species_id = i;
            break;
        }
    }
    if (root_specied_found) {
        ntree_t *future_root = get_leaf_by_name(nwk_tree,
                missing_data->speciesNames[root_species_id]);
        id_to_label.root_label = missing_data->speciesNames[root_species_id];
        if (future_root == nullptr) {
            dout("In root_tree(...): label "
                         << missing_data->speciesNames[root_species_id]
                         << " not found in tree" << std::endl);
            return nullptr;
        }
        return root_at(future_root, id_to_label);
    } else {
        //nwk_tree cannot be rooted consistently
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
                    dout("parent of " << tree->children[i]->label
                                       << " is wrong\n");
                } else {
                    dout("parent of " << tree->children[i]
                                       << "is wrong\n");
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

Tree root_at(ntree_t *leaf, label_mapper &id_to_label) {
    // if leaf->parent is null, leaf is the root
    // => the tree is not binary, or the node is no leaf
    assert(leaf != nullptr);
    assert(leaf->parent != nullptr);
    assert(leaf->children_count == 0);  //should be a leaf

    return root_recursive(leaf->parent, leaf, id_to_label);
}

Tree root_recursive(ntree_t *current_ntree, ntree_t *previous,
                    label_mapper &id_to_label) {
    assert(current_ntree != nullptr);
    assert(previous != nullptr);

    // When the children_count is 3, we are at the pseudoroot of the unrooted
    // binary tree, so there is no parent node.
    assert(current_ntree->children_count == 0
           || current_ntree->children_count == 2
           || (current_ntree->children_count == 3
               && current_ntree->parent == nullptr));

    if(current_ntree->children_count == 0) {
        // Leaf case
        assert(current_ntree->label != nullptr); // leaves need to have labels
        
        size_t leaf_id = id_to_label.size();
        
        // handle label_mapper business
        id_to_label.labels.push_back(current_ntree->label);
        
        return std::make_shared<Leaf>(leaf_id);
    } else if (current_ntree->children_count == 2) {
        // InnerNode case
        assert(current_ntree->children[0] != nullptr);
        assert(current_ntree->children[1] != nullptr);
        
        if(current_ntree->parent == nullptr) {
            // root of a rooted tree, simply ignore this node further on
            if(previous == current_ntree->children[0]) {
                return root_recursive (current_ntree->children[1],
                                       current_ntree, id_to_label);
            } else if(previous == current_ntree->children[1]) {
                return root_recursive (current_ntree->children[0],
                                       current_ntree, id_to_label);
            } else {
                assert(0);
            }
        } else {
            // normal inner node
            ntree_t *left; 
            ntree_t *right; 
            if(previous == current_ntree->parent) {
                left = current_ntree->children[0];
                right = current_ntree->children[1];
            } else if(previous == current_ntree->children[0]) {
                left = current_ntree->children[1];
                right = current_ntree->parent;
            } else if(previous == current_ntree->children[1]) {
                left = current_ntree->parent;
                right = current_ntree->children[0];
            } else {
                assert(0);
            }
            auto left_tree = root_recursive(left, current_ntree, id_to_label);
            auto right_tree = root_recursive(right, current_ntree, id_to_label);
            
            return std::make_shared<InnerNode>(left_tree, right_tree);
        }
    } else {
        // UnrootedNode case
        assert(current_ntree->children_count == 3);
        assert(current_ntree->children[0] != nullptr);
        assert(current_ntree->children[1] != nullptr);
        assert(current_ntree->children[2] != nullptr);
        assert(current_ntree->parent == nullptr); // root may not have parent
        
        //set the children, leave out the parent
        ntree_t *left; 
        ntree_t *right; 
        if (current_ntree->children[0] == previous) {
            left = current_ntree->children[1];
            right = current_ntree->children[2];
        } else if (current_ntree->children[1] == previous) {
            left = current_ntree->children[0];
            right = current_ntree->children[2];
        } else if (current_ntree->children[2] == previous) {
            left = current_ntree->children[0];
            right = current_ntree->children[1];
        } else {
            assert(0);
            return nullptr;
        }
        auto left_tree = root_recursive(left, current_ntree, id_to_label);
        auto right_tree = root_recursive(right, current_ntree, id_to_label);
            
        return std::make_shared<InnerNode>(left_tree, right_tree);
    }
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

/* dead code
std::vector<Tree> get_neighbours(Tree node) {
    std::vector<Tree> neighbours;
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
*/
/*
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
}*/

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
