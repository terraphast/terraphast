#pragma once

#ifndef IFUGAO_CONSTRAINTS_H
#define IFUGAO_CONSTRAINTS_H

#include "types.h"

//lca(smaller_left, smaller_right) < lca(bigger_left, bigger_right)
struct constraint {
    leaf_number smaller_left;
    leaf_number bigger_left;
    leaf_number smaller_right;
    leaf_number bigger_right;
};

#endif // IFUGAO_CONSTRAINTS_H
