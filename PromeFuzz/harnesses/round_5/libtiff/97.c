// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all operations
    if (Size < 44) {
        return 0;
    }

    // Extract parameters from fuzzer input
    double y1 = *(double*)Data;
    int clamp1 = *(int*)(Data + 8);
    
    uint32_t logluv24 = *(uint32_t*)(Data + 12);
    
    int logl16 = *(int*)(Data + 16);
    
    int logl10 = *(int*)(Data + 20);
    
    float xyz[3];
    xyz[0] = *(float*)(Data + 24);
    xyz[1] = *(float*)(Data + 28);
    xyz[2] = *(float*)(Data + 32);
    int clamp2 = *(int*)(Data + 36);
    
    double y2 = *(double*)(Data + 40);
    int clamp3 = *(int*)(Data + 48);

    // Test LogL16fromY
    int result1 = LogL16fromY(y1, clamp1);
    
    // Test LogLuv24toXYZ
    float xyz_out[3];
    LogLuv24toXYZ(logluv24, xyz_out);
    
    // Test LogL16toY
    double result3 = LogL16toY(logl16);
    
    // Test LogL10toY
    double result4 = LogL10toY(logl10);
    
    // Test LogLuv32fromXYZ
    uint32_t result5 = LogLuv32fromXYZ(xyz, clamp2);
    
    // Test LogL10fromY
    int result6 = LogL10fromY(y2, clamp3);

    // Use results to avoid dead code elimination
    static volatile int sink;
    sink = result1;
    sink = (int)result3;
    sink = (int)result4;
    sink = result5;
    sink = result6;
    sink = (int)xyz_out[0];
    sink = (int)xyz_out[1];
    sink = (int)xyz_out[2];

    return 0;
}