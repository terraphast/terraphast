#include "ifugao.h"
#include "debug.h"
#include "terraces.h"

#include <sstream>
#include <iomanip>

std::tuple<std::shared_ptr<LeafSet>,
        std::shared_ptr<LeafSet> > get_nth_partition_tuple(
        const partition_list &partitions,
        const size_t n) {

    assert(partitions.size() > 0);

    auto part_one = partitions[0]->create_empty();
    auto part_two = partitions[0]->create_empty();

    assert(n > 0 && n <= number_partition_tuples(partitions));

    for (size_t i = 0; i < partitions.size(); i++) {
        if (is_bit_set(n, i)) {
            *part_one |= *partitions[i];
        } else {
            *part_two |= *partitions[i];
        }
    }

    return std::make_tuple(part_one, part_two);
}

static std::vector<std::shared_ptr<Tree> > add_leaf_to_tree(
        std::shared_ptr<Tree> current_tree, const leaf_number leaf) {

    std::vector<std::shared_ptr<Tree> > result;

    if (!current_tree->is_leaf()) {
        auto result_left = add_leaf_to_tree(current_tree->left, leaf);
        result.insert(result.end(), result_left.begin(), result_left.end());
        auto result_right = add_leaf_to_tree(current_tree->right, leaf);
        result.insert(result.end(), result_right.begin(), result_right.end());
    }

    auto tree_copy = deep_copy(current_tree);

    auto to_insert = std::make_shared<Tree>(leaf);
    auto new_tree = std::make_shared<Tree>(tree_copy, to_insert);
    new_tree->parent = tree_copy->parent;
    if (tree_copy->parent != nullptr) {
        if (tree_copy->parent->left == tree_copy) {
            tree_copy->parent->left = new_tree;
        } else {
            tree_copy->parent->right = new_tree;
        }
    }
    tree_copy->parent = new_tree;
    to_insert->parent = new_tree;

    result.push_back(root(new_tree));

    return result;
}

std::vector<std::shared_ptr<Tree> > get_all_binary_trees(
        LeafSet &leaves) {

    std::vector<std::shared_ptr<Tree> > result;
    if (leaves.size() == 0) {
        return result;
    }

    auto next_leaf = leaves.pop();

    if (leaves.size() == 0) {
        auto t = std::make_shared<Tree>(next_leaf);
        result.push_back(t);
        return result;
    }

    for (auto t : get_all_binary_trees(leaves)) {
        auto new_trees = add_leaf_to_tree(t, next_leaf);
        result.insert(result.end(), new_trees.begin(), new_trees.end());
    }

    return result;
}

std::vector<std::shared_ptr<Tree> > merge_subtrees(
        const std::vector<std::shared_ptr<Tree> > &left,
        const std::vector<std::shared_ptr<Tree> > &right) {

    std::vector<std::shared_ptr<Tree> > merged_trees;

    for (auto l : left) {
        for (auto r : right) {
            auto new_tree = std::make_shared<Tree>(l, r);
            l->parent = new_tree;
            r->parent = new_tree;
            merged_trees.push_back(new_tree);
        }
    }

    assert(merged_trees.size() == left.size() * right.size());
    return merged_trees;
}

partition_list apply_constraints(const LeafSet &leaves, const std::vector<constraint> &constraints) {

    partition_list sets = leaves.create_partition_list();

    bool found_left_constraint = false;
    bool found_right_constraint = false;
    size_t index_containing_left_constraint = 0;
    size_t index_containing_right_constraint = 0;

    for (constraint cons : constraints) {
         for (size_t i = 0; i < sets.size(); i++) {
            if (sets[i]->contains(cons.smaller_left)) {
                // set contains the left constraint
                found_left_constraint = true;
                index_containing_left_constraint = i;
            }
            if (sets[i]->contains(cons.smaller_right)) {
                // set contains the right constraint
                found_right_constraint = true;
                index_containing_right_constraint = i;
            }
        }
        assert(found_left_constraint
                && index_containing_left_constraint < sets.size());
        assert(
                found_right_constraint
                && index_containing_right_constraint < sets.size());
        if (index_containing_left_constraint != index_containing_right_constraint) {
            // sets need to be merged
            *sets[index_containing_left_constraint] |= *sets[index_containing_right_constraint];
            sets.erase(sets.begin() + static_cast<partition_list::difference_type>(
                                              index_containing_right_constraint));
        }

    }
    return sets;
}

static std::tuple<leaf_number, leaf_number> extract_constraints_from_tree_rec(
        const std::shared_ptr<Tree> node,
        std::vector<constraint> &constraints) {

    assert(node != nullptr);

    if (node->is_leaf()) {
        return std::make_tuple(node->id, node->id);
    }

    // (l,r) of the left child node
    leaf_number l_left_most;
    leaf_number l_right_most;
    std::tie(l_left_most, l_right_most) = extract_constraints_from_tree_rec(
            node->left, constraints);

    // (l,r) of the right child node
    leaf_number r_left_most;
    leaf_number r_right_most;
    std::tie(r_left_most, r_right_most) = extract_constraints_from_tree_rec(
            node->right, constraints);

    if (l_left_most != l_right_most) {
        constraint c;
        c.smaller_left = l_left_most;
        c.smaller_right = l_right_most;
        c.bigger_left = l_left_most;
        c.bigger_right = r_right_most;
        constraints.push_back(c);
    }

    if (r_left_most != r_right_most) {
        constraint c;
        c.smaller_left = r_left_most;
        c.smaller_right = r_right_most;
        c.bigger_left = l_left_most;
        c.bigger_right = r_right_most;
        constraints.push_back(c);
    }

    return std::make_tuple(l_left_most, r_right_most);
}

std::vector<constraint> extract_constraints_from_tree(
        const std::shared_ptr<Tree> supertree) {
    std::vector<constraint> constraints;

    if(supertree != nullptr) {
        extract_constraints_from_tree_rec(supertree, constraints);
    }

    return constraints;
}

std::vector<constraint> find_constraints(const LeafSet &leaves,
                                         const std::vector<constraint> &constraints) {

    std::vector<constraint> valid_constraints;

    for (constraint cons : constraints) {
        if (cons.smaller_left == cons.bigger_left) {
            if (leaves.contains(cons.smaller_left)
                && leaves.contains(cons.smaller_right)
                && leaves.contains(cons.bigger_right)) {
                // constraint is valid on leaf set
                valid_constraints.push_back(cons);
            }
        } else {
            // smaller_right == bigger_right
            if (leaves.contains(cons.smaller_left)
                && leaves.contains(cons.smaller_right)
                && leaves.contains(cons.bigger_left)) {
                // constraint is valid on leaf set
                valid_constraints.push_back(cons);
            }
        }
    }
    return valid_constraints;
}

std::string missing_data_to_nexus(const missingData* m) {

    std::stringstream ss;
    size_t max_len = 0;
    for (size_t i = 0; i < m->numberOfSpecies; i++) {
        if(strlen(m->speciesNames[i]) > max_len) {
            max_len = strlen(m->speciesNames[i]);
        }
    }

    ss << "#NEXUS\n";
    ss << "Begin data;\n";
    ss << "    Dimensions ntax=" << m->numberOfSpecies << " nchar=" << m->numberOfPartitions << ";\n";
    ss << "    Format datatype=dna gap=-;\n";
    ss << "    Matrix\n";
    for (size_t i = 0; i < m->numberOfSpecies; i++) {
        ss << std::left << std::setw(static_cast<int>(max_len+1)) << m->speciesNames[i];
        for (size_t j = 0; j < m->numberOfPartitions; j++) {
            ss << (getDataMatrix(m, i, j)>0 ? 'A' : '-');
        }
        ss << std::endl;
    }
    ss << "\t;\n";
    ss << "End;\n";
    ss << "\n";

    ss << "BEGIN SETS;\n";
    for (size_t j = 0; j < m->numberOfPartitions; j++) {
        ss << "\tCHARSET  P" << std::to_string(j+1) << " = "
             << std::to_string(j+1) << "-" << std::to_string(j+1) << ";\n";
    }
    ss << "END;\n";
    return ss.str();
}

std::ostream& operator<<(std::ostream &strm,
                         const std::vector<std::shared_ptr<std::set<leaf_number>> >& set) {
    strm << "[";
    bool first = true;
    for(size_t i = 0; i < set.size(); i++) {
        if(first) {
            first = false;
        } else {
            strm << ",";
        }

        strm << "{";
        bool first2 = true;
        for(const auto &e : *set[i]) {
            if(first2) {
                first2 = false;
            } else {
                strm << ",";
            }
            strm << e;
        }
        strm << "}";
    }
    return strm << "]";
}

std::ostream& operator<<(std::ostream &strm, const constraint& c) {
    return strm << "lca(" << c.smaller_left << "," << c.smaller_right
                << ") < lca(" << c.bigger_left << "," << c.bigger_right << ")";
}
