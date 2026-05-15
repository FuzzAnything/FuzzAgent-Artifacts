// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void fuzz_LogL16fromY(const uint8_t *data, size_t size) {
    if (size < sizeof(double) + sizeof(int)) return;
    
    double y;
    int em;
    memcpy(&y, data, sizeof(double));
    memcpy(&em, data + sizeof(double), sizeof(int));
    
    LogL16fromY(y, em);
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(float) + sizeof(int)) return;
    
    float xyz[3];
    int em;
    memcpy(xyz, data, 3 * sizeof(float));
    memcpy(&em, data + 3 * sizeof(float), sizeof(int));
    
    LogLuv24fromXYZ(xyz, em);
}

static void fuzz_uv_encode(const uint8_t *data, size_t size) {
    if (size < 2 * sizeof(double) + sizeof(int)) return;
    
    double u, v;
    int em;
    memcpy(&u, data, sizeof(double));
    memcpy(&v, data + sizeof(double), sizeof(double));
    memcpy(&em, data + 2 * sizeof(double), sizeof(int));
    
    uv_encode(u, v, em);
}

static void fuzz_uv_decode(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int c;
    double up, vp;
    memcpy(&c, data, sizeof(int));
    
    uv_decode(&up, &vp, c);
}

static void fuzz_LogL10fromY(const uint8_t *data, size_t size) {
    if (size < sizeof(double) + sizeof(int)) return;
    
    double y;
    int em;
    memcpy(&y, data, sizeof(double));
    memcpy(&em, data + sizeof(double), sizeof(int));
    
    LogL10fromY(y, em);
}

static void fuzz_LogL10toY(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int l10;
    memcpy(&l10, data, sizeof(int));
    
    LogL10toY(l10);
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use the first byte to decide which function(s) to call
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Call different combinations of functions based on selector bits
    if (selector & 0x01) fuzz_LogL16fromY(remaining_data, remaining_size);
    if (selector & 0x02) fuzz_LogLuv24fromXYZ(remaining_data, remaining_size);
    if (selector & 0x04) fuzz_uv_encode(remaining_data, remaining_size);
    if (selector & 0x08) fuzz_uv_decode(remaining_data, remaining_size);
    if (selector & 0x10) fuzz_LogL10fromY(remaining_data, remaining_size);
    if (selector & 0x20) fuzz_LogL10toY(remaining_data, remaining_size);
    
    return 0;
}