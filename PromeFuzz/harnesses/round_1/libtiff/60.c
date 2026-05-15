// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare dummy file for any file operations
    write_dummy_file(Data, Size);

    // Test LogLuv24toXYZ
    if (Size >= 4) {
        uint32_t pixel;
        memcpy(&pixel, Data, 4);
        float xyz[3];
        LogLuv24toXYZ(pixel, xyz);
    }

    // Test LogL16fromY
    if (Size >= 12) {
        double y;
        int clamp;
        memcpy(&y, Data, 8);
        memcpy(&clamp, Data + 8, 4);
        LogL16fromY(y, clamp);
    }

    // Test LogL16toY
    if (Size >= 4) {
        int logl;
        memcpy(&logl, Data, 4);
        LogL16toY(logl);
    }

    // Test LogL10toY
    if (Size >= 4) {
        int logl10;
        memcpy(&logl10, Data, 4);
        LogL10toY(logl10);
    }

    // Test LogL10fromY
    if (Size >= 12) {
        double y;
        int clamp;
        memcpy(&y, Data, 8);
        memcpy(&clamp, Data + 8, 4);
        LogL10fromY(y, clamp);
    }

    // Test LogLuv32fromXYZ
    if (Size >= 16) {
        float xyz[3];
        int clamp;
        memcpy(xyz, Data, 12);
        memcpy(&clamp, Data + 12, 4);
        LogLuv32fromXYZ(xyz, clamp);
    }

    // Additional test combinations for more coverage
    if (Size >= 20) {
        // Chain conversions: LogL16toY -> LogL16fromY
        int logl;
        memcpy(&logl, Data, 4);
        double y = LogL16toY(logl);
        LogL16fromY(y, 1);

        // Chain conversions: LogL10toY -> LogL10fromY
        int logl10;
        memcpy(&logl10, Data + 4, 4);
        double y2 = LogL10toY(logl10);
        LogL10fromY(y2, 1);

        // LogLuv roundtrip
        float xyz[3];
        int clamp;
        memcpy(xyz, Data + 8, 12);
        uint32_t encoded = LogLuv32fromXYZ(xyz, 0);
        float decoded[3];
        LogLuv24toXYZ(encoded & 0xFFFFFF, decoded);
    }

    return 0;
}