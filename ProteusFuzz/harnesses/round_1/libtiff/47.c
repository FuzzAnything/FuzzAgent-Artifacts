// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <tiffio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write input to dummy file if needed (not strictly needed for these math functions, but per instructions)
    dummy_file_write(Data, Size);

    // Extract values from input for fuzzing
    int val_int = Data[0];
    double val_double = 0.0;
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    int em = 0;

    if (Size >= 2) {
        val_int = (Data[0] << 8) | Data[1];
        em = Data[1] % 4; // Ensure em is within reasonable range for error modes
    }

    if (Size >= 3) {
        val_double = (double)Data[2] / 255.0 * 100.0;
        if (Size >= 4) val_double += (double)Data[3] / 255.0;
    }

    if (Size >= 6) {
        xyz[0] = (float)Data[4] / 255.0 * 200.0;
        xyz[1] = (float)Data[5] / 255.0 * 200.0;
        if (Size >= 7) xyz[2] = (float)Data[6] / 255.0 * 200.0;
    }

    // Fuzz LogL10toY
    int p10 = val_int % 1024;
    double y1 = LogL10toY(p10);
    (void)y1; // Use result to avoid unused variable warning

    // Fuzz LogL16fromY
    int l16 = LogL16fromY(val_double, em);
    (void)l16;

    // Fuzz LogL10fromY
    int l10 = LogL10fromY(val_double, em);
    (void)l10;

    // Fuzz uv_encode
    double u = (double)val_int / 1024.0 * 2.0 - 1.0;
    double v = (double)(val_int + 1) / 1024.0 * 2.0 - 1.0;
    int uv_idx = uv_encode(u, v, em);
    (void)uv_idx;

    // Fuzz LogLuv32fromXYZ
    uint32_t luv32 = LogLuv32fromXYZ(xyz, em);
    (void)luv32;

    // Fuzz LogLuv24fromXYZ
    uint32_t luv24 = LogLuv24fromXYZ(xyz, em);
    (void)luv24;

    return 0;
}