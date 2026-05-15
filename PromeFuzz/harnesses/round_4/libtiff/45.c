// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // For functions requiring double parameters
    if (Size >= sizeof(double) + sizeof(int)) {
        double y;
        int em;
        memcpy(&y, Data, sizeof(double));
        memcpy(&em, Data + sizeof(double), sizeof(int));
        
        // Test LogL10fromY
        LogL10fromY(y, em);
        
        // Test LogL16fromY
        LogL16fromY(y, em);
    }
    
    // For uv_encode function
    if (Size >= 2 * sizeof(double) + sizeof(int)) {
        double u, v;
        int em;
        memcpy(&u, Data, sizeof(double));
        memcpy(&v, Data + sizeof(double), sizeof(double));
        memcpy(&em, Data + 2 * sizeof(double), sizeof(int));
        
        uv_encode(u, v, em);
    }
    
    // For LogLuv32toXYZ function
    if (Size >= sizeof(uint32_t)) {
        uint32_t pixel;
        float xyz[3];
        memcpy(&pixel, Data, sizeof(uint32_t));
        
        LogLuv32toXYZ(pixel, xyz);
    }
    
    // For LogLuv24toXYZ function
    if (Size >= sizeof(uint32_t)) {
        uint32_t pixel;
        float xyz[3];
        memcpy(&pixel, Data, sizeof(uint32_t));
        
        LogLuv24toXYZ(pixel, xyz);
    }
    
    // For LogLuv24fromXYZ function
    if (Size >= 3 * sizeof(float) + sizeof(int)) {
        float xyz[3];
        int em;
        memcpy(xyz, Data, 3 * sizeof(float));
        memcpy(&em, Data + 3 * sizeof(float), sizeof(int));
        
        LogLuv24fromXYZ(xyz, em);
    }
    
    return 0;
}