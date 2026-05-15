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
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    // Function 1: LogL16fromY
    if (Size >= sizeof(double) + sizeof(int)) {
        double y1;
        int em1;
        memcpy(&y1, Data, sizeof(double));
        memcpy(&em1, Data + sizeof(double), sizeof(int));
        int result1 = LogL16fromY(y1, em1);
        (void)result1;
    }

    // Function 2: LogLuv24toXYZ
    if (Size >= sizeof(uint32_t) + 3 * sizeof(float)) {
        uint32_t p2;
        float xyz[3];
        memcpy(&p2, Data, sizeof(uint32_t));
        LogLuv24toXYZ(p2, xyz);
    }

    // Function 3: LogL16toY
    if (Size >= sizeof(int)) {
        int l3;
        memcpy(&l3, Data, sizeof(int));
        double result3 = LogL16toY(l3);
        (void)result3;
    }

    // Function 4: LogL10toY
    if (Size >= sizeof(int)) {
        int l4;
        memcpy(&l4, Data, sizeof(int));
        double result4 = LogL10toY(l4);
        (void)result4;
    }

    // Function 5: LogLuv32fromXYZ
    if (Size >= 3 * sizeof(float) + sizeof(int)) {
        float xyz5[3];
        int em5;
        memcpy(xyz5, Data, 3 * sizeof(float));
        memcpy(&em5, Data + 3 * sizeof(float), sizeof(int));
        uint32_t result5 = LogLuv32fromXYZ(xyz5, em5);
        (void)result5;
    }

    // Function 6: LogL10fromY
    if (Size >= sizeof(double) + sizeof(int)) {
        double y6;
        int em6;
        memcpy(&y6, Data, sizeof(double));
        memcpy(&em6, Data + sizeof(double), sizeof(int));
        int result6 = LogL10fromY(y6, em6);
        (void)result6;
    }

    return 0;
}