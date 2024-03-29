#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Include functionality relating to the platform.

#include   "GIC.h"
#include "PL011.h"
#include "PL050.h"
#include "PL111.h"
#include   "SYS.h"

// Include functionality relating to the   kernel.

#include "lolevel.h"
#include     "int.h"

#endif
