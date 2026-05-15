// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Use first byte to decide which functions to call
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
    }

    // Call various API functions based on fuzzer input
    switch (selector % 6) {
        case 0: {
            // Test magic_getparam
            size_t param_value = 0;
            int param_type = (Size > 1) ? Data[1] : 0;
            magic_getparam(cookie, param_type, &param_value);
            break;
        }
        case 1: {
            // Test magic_load
            write_dummy_file(Data, Size);
            magic_load(cookie, "./dummy_file");
            break;
        }
        case 2: {
            // Test magic_getflags
            magic_getflags(cookie);
            break;
        }
        case 3: {
            // Test magic_setparam
            size_t set_value = (Size > 1) ? Data[1] : 100;
            int param_type = (Size > 2) ? Data[2] : 0;
            magic_setparam(cookie, param_type, &set_value);
            break;
        }
        case 4: {
            // Test magic_errno
            magic_errno(cookie);
            break;
        }
        case 5: {
            // Test magic_setflags
            int flags = (Size > 1) ? Data[1] : 0;
            magic_setflags(cookie, flags);
            break;
        }
    }

    // Try calling multiple functions in sequence to explore more states
    if (Size > 3) {
        // Try setting and getting flags
        int test_flags = Data[2];
        magic_setflags(cookie, test_flags);
        magic_getflags(cookie);
        
        // Try setting a parameter and then getting it
        if (Size > 10) {
            size_t test_value = (size_t)Data[3] << 24 | (size_t)Data[4] << 16 | 
                               (size_t)Data[5] << 8 | (size_t)Data[6];
            int test_param = Data[7];
            
            magic_setparam(cookie, test_param, &test_value);
            
            size_t retrieved_value = 0;
            magic_getparam(cookie, test_param, &retrieved_value);
        }
        
        // Try loading with different magic files
        if (Size > 20) {
            write_dummy_file(Data + 10, Size - 10);
            magic_load(cookie, "./dummy_file");
            magic_errno(cookie);
        }
    }

    // Clean up
    magic_close(cookie);
    
    // Remove dummy file if it exists
    remove("./dummy_file");
    
    return 0;
}