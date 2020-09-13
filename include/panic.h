#pragma once

#include <assert.h>
#include <stdio.h>

#define PANIC_UNREACHED() (fprintf(stderr, "This code is not supposed to be reached!\n"), assert(0))

#define PANIC_UNIMPLEMENTED() (fprintf(stderr, "This code is not yet implemented!\n"), assert(0))
