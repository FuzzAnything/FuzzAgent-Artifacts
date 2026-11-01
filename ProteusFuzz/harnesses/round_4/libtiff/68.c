// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void test_logluv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    
    int em = 0;
    if (Size > 12) {
        em = Data[12] % 3;
    }
    
    uint32_t result = LogLuv32fromXYZ(xyz, em);
    (void)result;
}

static void test_logl10toY(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;
    
    int p10 = Data[0] & 0x3FF;
    
    double result = LogL10toY(p10);
    (void)result;
}

static void test_logl16fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    
    double y;
    memcpy(&y, Data, sizeof(double));
    
    int em = 0;
    if (Size > 8) {
        em = Data[8] % 3;
    }
    
    int result = LogL16fromY(y, em);
    (void)result;
}

static void test_logl16toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int le;
    memcpy(&le, Data, sizeof(int));
    
    double result = LogL16toY(le);
    (void)result;
}

static void test_logl10fromY(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;
    
    double y;
    memcpy(&y, Data, sizeof(double));
    
    int em = 0;
    if (Size > 8) {
        em = Data[8] % 3;
    }
    
    int result = LogL10fromY(y, em);
    (void)result;
}

static void test_logluv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    
    int em = 0;
    if (Size > 12) {
        em = Data[12] % 3;
    }
    
    uint32_t result = LogLuv24fromXYZ(xyz, em);
    (void)result;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    int func_selector = Data[0] % 6;
    
    switch (func_selector) {
        case 0:
            test_logluv32fromXYZ(Data, Size);
            break;
        case 1:
            test_logl10toY(Data, Size);
            break;
        case 2:
            test_logl16fromY(Data, Size);
            break;
        case 3:
            test_logl16toY(Data, Size);
            break;
        case 4:
            test_logl10fromY(Data, Size);
            break;
        case 5:
            test_logluv24fromXYZ(Data, Size);
            break;
        default:
            break;
    }
    
    return 0;
}