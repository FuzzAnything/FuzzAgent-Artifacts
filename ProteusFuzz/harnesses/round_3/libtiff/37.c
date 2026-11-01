// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzzer_test_LogL10fromY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    double y = *(const double *)Data;
    int em = (Size > sizeof(double)) ? Data[sizeof(double)] : 0;
    int result = LogL10fromY(y, em);
    (void)result;
}

static void fuzzer_test_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float) * 3) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int em = (Size > sizeof(float) * 3) ? Data[sizeof(float) * 3] : 0;
    uint32_t result = LogLuv32fromXYZ(xyz, em);
    (void)result;
}

static void fuzzer_test_LogL16toY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int)) return;
    int logl = *(const int *)Data;
    double result = LogL16toY(logl);
    (void)result;
}

static void fuzzer_test_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float) * 3) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int em = (Size > sizeof(float) * 3) ? Data[sizeof(float) * 3] : 0;
    uint32_t result = LogLuv24fromXYZ(xyz, em);
    (void)result;
}

static void fuzzer_test_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int)) return;
    int logl = *(const int *)Data;
    double result = LogL10toY(logl);
    (void)result;
}

static void fuzzer_test_LogL16fromY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    double y = *(const double *)Data;
    int em = (Size > sizeof(double)) ? Data[sizeof(double)] : 0;
    int result = LogL16fromY(y, em);
    (void)result;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzzer_test_LogL10fromY(Data, Size);
    fuzzer_test_LogLuv32fromXYZ(Data, Size);
    fuzzer_test_LogL16toY(Data, Size);
    fuzzer_test_LogLuv24fromXYZ(Data, Size);
    fuzzer_test_LogL10toY(Data, Size);
    fuzzer_test_LogL16fromY(Data, Size);

    return 0;
}