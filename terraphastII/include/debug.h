#pragma once

#include <iostream>

/*
 * for debug statements
 */
#ifndef NDEBUG
#define dout(obj) do { \
   std::cerr << "" <<  __FILE__ << ":" << __LINE__ << ": " << obj; \
} while(false)
#else
#define dout(obj) do {} while(false)
#endif
