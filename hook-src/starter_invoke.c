/**
 * This hook accepts any Invoke transaction coming through it
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved) {

    TRACESTR("starter_invoke.c Called.");
    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}