// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tiffio.h>

static void fuzz_uv_encode(const uint8_t *Data, size_t Size) {
    if (Size < 24) return;
    
    // Ensure alignment and bounds for double and int
    double u, v;
    int mode;
    
    // Copy data safely to avoid alignment issues
    memcpy(&u, Data, sizeof(double));
    memcpy(&v, Data + 8, sizeof(double));
    memcpy(&mode, Data + 16, sizeof(int));
    
    uv_encode(u, v, mode);
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int mode;
    
    memcpy(xyz, Data, sizeof(float) * 3);
    memcpy(&mode, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    uint32_t pixel;
    float xyz[3];
    
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    LogLuv24toXYZ(pixel, xyz);
}

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int mode;
    
    memcpy(xyz, Data, sizeof(float) * 3);
    memcpy(&mode, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv32fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    uint32_t pixel;
    float xyz[3];
    
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    LogLuv32toXYZ(pixel, xyz);
}

static void fuzz_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int index;
    double u, v;
    
    memcpy(&index, Data, sizeof(int));
    
    uv_decode(&u, &v, index);
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Determine which function to fuzz based on input data
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_uv_encode(Data, Size);
            break;
        case 1:
            fuzz_LogLuv24fromXYZ(Data, Size);
            break;
        case 2:
            fuzz_LogLuv24toXYZ(Data, Size);
            break;
        case 3:
            fuzz_LogLuv32fromXYZ(Data, Size);
            break;
        case 4:
            fuzz_LogLuv32toXYZ(Data, Size);
            break;
        case 5:
            fuzz_uv_decode(Data, Size);
            break;
    }
    
    return 0;
}