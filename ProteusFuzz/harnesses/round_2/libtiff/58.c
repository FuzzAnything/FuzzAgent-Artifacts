// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
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

static void fuzzer_test_LogL16toY(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    int16_t val = *(int16_t *)Data;
    double result = LogL16toY(val);
    (void)result;
}

static void fuzzer_test_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 13) return;
    float xyz[3];
    memcpy(xyz, Data, 12);
    int mode = Data[12];
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzzer_test_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 13) return;
    float xyz[3];
    memcpy(xyz, Data, 12);
    int mode = Data[12];
    uint32_t result = LogLuv32fromXYZ(xyz, mode);
    (void)result;
}

static void fuzzer_test_LogL10fromY(const uint8_t *Data, size_t Size) {
    if (Size < 9) return;
    double y;
    memcpy(&y, Data, 8);
    int mode = Data[8];
    int result = LogL10fromY(y, mode);
    (void)result;
}

static void fuzzer_test_LogL16fromY(const uint8_t *Data, size_t Size) {
    if (Size < 9) return;
    double y;
    memcpy(&y, Data, 8);
    int mode = Data[8];
    int result = LogL16fromY(y, mode);
    (void)result;
}

static void fuzzer_test_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    int16_t val = *(int16_t *)Data;
    double result = LogL10toY(val);
    (void)result;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int test_case = Data[0] % 6;

    switch (test_case) {
        case 0:
            fuzzer_test_LogL16toY(Data + 1, Size - 1);
            break;
        case 1:
            fuzzer_test_LogLuv24fromXYZ(Data + 1, Size - 1);
            break;
        case 2:
            fuzzer_test_LogLuv32fromXYZ(Data + 1, Size - 1);
            break;
        case 3:
            fuzzer_test_LogL10fromY(Data + 1, Size - 1);
            break;
        case 4:
            fuzzer_test_LogL16fromY(Data + 1, Size - 1);
            break;
        case 5:
            fuzzer_test_LogL10toY(Data + 1, Size - 1);
            break;
    }

    return 0;
}