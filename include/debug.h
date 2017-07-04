#pragma once

#include <iostream>

/*
 * for debug statements
 */
#ifndef DEBUG
#define dout(obj) do { \
   std::cerr << "" <<  __FILE__ << ":" << __LINE__ << ": " << obj; \
} while(false)
#else
#define dout(obj) do {} while(false)
#endif
