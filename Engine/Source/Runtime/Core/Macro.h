#pragma once

#ifdef DEBUG
#define DEBUG_CALL(fn, ...) fn(__VA_ARGS__)
#else
#define DEBUG_CALL(fn, ...) ((void)0)
#endif
