#ifndef __RENDER_H
#define __RENDER_H

#include "hilevel.h"


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <stdlib.h>


// Include functionality relating to the   kernel.

#include "lolevel.h"
#include     "int.h"
#include    "font.h"
#include    "decoder.h"
#include "render.h"

//Include functionality relating to the platform.

#include   "GIC.h"
#include "PL011.h"
#include "SP804.h"
#include "PL050.h"
#include "PL111.h"
#include "SYS.h"


void carriageReturn();

void renderInit();

void drawLetter(char c);

void deleteLetter(int consoleBuffer);

void drawString(char* c, int n);

#endif
