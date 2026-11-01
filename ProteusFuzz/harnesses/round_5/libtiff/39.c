// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tiffio.h>

static void fuzzer_seed_floats(float *arr, const uint8_t *data, size_t size) {
    if (size < 12) {
        arr[0] = 0.0f;
        arr[1] = 0.0f;
        arr[2] = 0.0f;
        return;
    }
    // Use raw bytes to construct floats
    memcpy(&arr[0], data, 4);
    memcpy(&arr[1], data + 4, 4);
    memcpy(&arr[2], data + 8, 4);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare data for LogLuv32fromXYZ and LogLuv24fromXYZ
    float xyz[3];
    fuzzer_seed_floats(xyz, Data, Size);
    
    // Extract mode/integers from data if available
    int mode = 0;
    if (Size > 12) {
        mode = Data[12];
    }

    // 1. Test LogLuv32fromXYZ
    uint32_t luv32 = LogLuv32fromXYZ(xyz, mode);

    // 2. Test LogLuv24fromXYZ
    uint32_t luv24 = LogLuv24fromXYZ(xyz, mode);

    // 3. Test LogLuv24toXYZ
    float xyz_out_24[3] = {0.0f, 0.0f, 0.0f};
    LogLuv24toXYZ(luv24, xyz_out_24);

    // 4. Test LogLuv32toXYZ
    float xyz_out_32[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32toXYZ(luv32, xyz_out_32);

    // 5. Test XYZtoRGB24
    uint8_t rgb24[3] = {0, 0, 0};
    XYZtoRGB24(xyz, rgb24);

    // 6. Test TIFFXYZToRGB
    // This function requires a valid TIFFCIELabToRGB state.
    // Passing NULL or an uninitialized struct causes a crash (SEGV) as seen in the error.
    // We skip calling it with invalid pointers to prevent crashes.
    // Proper usage requires TIFFCIELabToRGBInit which is not available in the provided API list
    // and requires specific initialization parameters not easily fuzzer-generated.
    // Therefore, we omit this specific call to ensure stability.
    // If the library implementation checks for NULL internally, we could try, but the stack trace
    // shows it dereferences the pointer immediately.
    
    uint32_t r, g, b;
    // Do not call TIFFXYZToRGB with NULL or dummy state as it crashes.
    // TIFFXYZToRGB(state, xyz[0], xyz[1], xyz[2], &r, &g, &b); 

    return 0;
}