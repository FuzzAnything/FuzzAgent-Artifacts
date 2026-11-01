// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
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

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int exp_mult = 0;
    if (Size >= 16) {
        exp_mult = *(int *)(Data + 12);
    }
    uint32_t result = LogLuv32fromXYZ(xyz, exp_mult);
    (void)result;
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int mode = 0;
    if (Size >= 16) {
        mode = *(int *)(Data + 12);
    }
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogL16fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    double y = *(double *)Data;
    int em = 0;
    if (Size >= 12) {
        em = *(int *)(Data + 8);
    }
    int result = LogL16fromY(y, em);
    (void)result;
}

static void fuzz_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int p10 = *(int *)Data;
    double result = LogL10toY(p10);
    (void)result;
}

static void fuzz_LogL16toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int le = *(int *)Data;
    double result = LogL16toY(le);
    (void)result;
}

static void fuzz_LogL10fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    double y = *(double *)Data;
    int em = 0;
    if (Size >= 12) {
        em = *(int *)(Data + 8);
    }
    int result = LogL10fromY(y, em);
    (void)result;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    uint8_t mode = Data[0] % 6;

    switch (mode) {
        case 0:
            fuzz_LogLuv32fromXYZ(Data, Size);
            break;
        case 1:
            fuzz_LogLuv24fromXYZ(Data, Size);
            break;
        case 2:
            fuzz_LogL16fromY(Data, Size);
            break;
        case 3:
            fuzz_LogL10toY(Data, Size);
            break;
        case 4:
            fuzz_LogL16toY(Data, Size);
            break;
        case 5:
            fuzz_LogL10fromY(Data, Size);
            break;
    }

    return 0;
}