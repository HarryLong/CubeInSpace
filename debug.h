#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG_ON
#define PRINT_POSITION() std::cout << "FILE: " << __FILE__ << " | LINE: " << __LINE__ << std::endl;
#else
#define PRINT_POSITION()
#endif

#endif DEBUG_H
