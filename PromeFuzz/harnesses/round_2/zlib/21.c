// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for at least basic parameters
    if (Size < 4 * sizeof(uLong) + sizeof(z_off_t) + sizeof(z_off64_t)) {
        return 0;
    }

    // Extract parameters from fuzzer input
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Read uLong values (assuming uLong is 4 bytes - common on many platforms)
    uLong crc1 = 0, crc2 = 0, adler1 = 0, adler2 = 0, op = 0;
    for (int i = 0; i < 5 && remaining >= sizeof(uLong); i++) {
        uLong val = 0;
        memcpy(&val, ptr, sizeof(uLong));
        ptr += sizeof(uLong);
        remaining -= sizeof(uLong);
        
        switch(i) {
            case 0: crc1 = val; break;
            case 1: crc2 = val; break;
            case 2: adler1 = val; break;
            case 3: adler2 = val; break;
            case 4: op = val; break;
        }
    }

    // Read z_off_t (signed offset type)
    z_off_t len2 = 0;
    if (remaining >= sizeof(z_off_t)) {
        memcpy(&len2, ptr, sizeof(z_off_t));
        ptr += sizeof(z_off_t);
        remaining -= sizeof(z_off_t);
    }

    // Read z_off64_t (signed 64-bit offset type)
    z_off64_t len2_64 = 0;
    if (remaining >= sizeof(z_off64_t)) {
        memcpy(&len2_64, ptr, sizeof(z_off64_t));
        ptr += sizeof(z_off64_t);
        remaining -= sizeof(z_off64_t);
    }

    // Test all target functions with the extracted parameters
    uLong result;
    
    // 1. crc32_combine_gen64
    result = crc32_combine_gen64(len2_64);
    
    // 2. crc32_combine64
    result = crc32_combine64(crc1, crc2, len2_64);
    
    // 3. crc32_combine_gen
    result = crc32_combine_gen(len2);
    
    // 4. adler32_combine
    result = adler32_combine(adler1, adler2, len2);
    
    // 5. crc32_combine_op
    result = crc32_combine_op(crc1, crc2, op);
    
    // 6. crc32_combine
    result = crc32_combine(crc1, crc2, len2);

    // Additional test: use crc32_combine_gen with crc32_combine_op
    if (len2 >= 0) {
        uLong gen_op = crc32_combine_gen(len2);
        result = crc32_combine_op(crc1, crc2, gen_op);
    }

    // Additional test: use crc32_combine_gen64 with crc32_combine_op
    // Note: crc32_combine_op expects uLong op, but crc32_combine_gen64 returns uLong
    // This is fine as both are uLong
    uLong gen64_op = crc32_combine_gen64(len2_64);
    result = crc32_combine_op(crc1, crc2, gen64_op);

    return 0;
}