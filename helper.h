#include "stdio.h"
#include "stdbool.h"

// DEBUG_PRINT macro

#ifdef DEBUG
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__); } while(false)
#else
#define DEBUG_PRINT(...) do{ } while( false )
#endif

#ifdef DEBUG
#define DEBUG_FLUSH() do{ fflush(stdout); } while(false)
#else
#define DEBUG_FLUSH() do{ } while( false )
#endif

// Library Encoding

#define SAILR_CHAR_ENCODING  "UTF-8" 
