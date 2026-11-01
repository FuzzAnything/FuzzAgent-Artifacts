// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tiffio.h"

static void fuzzer_test_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int em = (Size >= 16) ? (int)(Data[12] | (Data[13] << 8) | (Data[14] << 16) | (Data[15] << 24)) : 0;
    uint32_t result = LogLuv32fromXYZ(xyz, em);
    (void)result;
}

static void fuzzer_test_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int em = (Size >= 16) ? (int)(Data[12] | (Data[13] << 8) | (Data[14] << 16) | (Data[15] << 24)) : 0;
    uint32_t result = LogLuv24fromXYZ(xyz, em);
    (void)result;
}

static void fuzzer_test_LogL16fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    double y;
    memcpy(&y, Data, sizeof(double));
    int em = (Size >= 12) ? (int)(Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24)) : 0;
    int result = LogL16fromY(y, em);
    (void)result;
}

static void fuzzer_test_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int p10 = (int)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    double result = LogL10toY(p10);
    (void)result;
}

static void fuzzer_test_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int c = (int)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    double u, v;
    int result = uv_decode(&u, &v, c);
    (void)result;
}

static void fuzzer_test_LogL10fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    double y;
    memcpy(&y, Data, sizeof(double));
    int em = (Size >= 12) ? (int)(Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24)) : 0;
    int result = LogL10fromY(y, em);
    (void)result;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run all fuzzer tests
    fuzzer_test_LogLuv32fromXYZ(Data, Size);
    fuzzer_test_LogLuv24fromXYZ(Data, Size);
    fuzzer_test_LogL16fromY(Data, Size);
    fuzzer_test_LogL10toY(Data, Size);
    fuzzer_test_uv_decode(Data, Size);
    fuzzer_test_LogL10fromY(Data, Size);

    return 0;
}