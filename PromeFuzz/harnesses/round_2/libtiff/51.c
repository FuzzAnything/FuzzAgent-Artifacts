// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Use the first part of the input to initialize parameters for each function
    size_t offset = 0;
    
    // 1. Test LogLuv24fromXYZ
    if (Size >= 16) { // Need 3 floats (12 bytes) + 1 int (4 bytes)
        float xyz[3];
        int em;
        memcpy(xyz, Data, 12);
        memcpy(&em, Data + 12, 4);
        offset += 16;
        LogLuv24fromXYZ(xyz, em);
    }
    
    // 2. Test LogL16fromY
    if (Size - offset >= 12) { // Need 1 double (8 bytes) + 1 int (4 bytes)
        double y;
        int em;
        memcpy(&y, Data + offset, 8);
        memcpy(&em, Data + offset + 8, 4);
        offset += 12;
        LogL16fromY(y, em);
    }
    
    // 3. Test LogL10fromY
    if (Size - offset >= 12) { // Need 1 double (8 bytes) + 1 int (4 bytes)
        double y;
        int em;
        memcpy(&y, Data + offset, 8);
        memcpy(&em, Data + offset + 8, 4);
        offset += 12;
        LogL10fromY(y, em);
    }
    
    // 4. Test LogLuv24toXYZ
    if (Size - offset >= 4) { // Need 1 uint32_t (4 bytes)
        uint32_t p;
        float xyz_out[3];
        memcpy(&p, Data + offset, 4);
        offset += 4;
        LogLuv24toXYZ(p, xyz_out);
    }
    
    // 5. Test LogLuv32toXYZ
    if (Size - offset >= 4) { // Need 1 uint32_t (4 bytes)
        uint32_t p;
        float xyz_out[3];
        memcpy(&p, Data + offset, 4);
        offset += 4;
        LogLuv32toXYZ(p, xyz_out);
    }
    
    // 6. Test uv_encode
    if (Size - offset >= 20) { // Need 2 doubles (16 bytes) + 1 int (4 bytes)
        double u, v;
        int em;
        memcpy(&u, Data + offset, 8);
        memcpy(&v, Data + offset + 8, 8);
        memcpy(&em, Data + offset + 16, 4);
        offset += 20;
        uv_encode(u, v, em);
    }
    
    return 0;
}