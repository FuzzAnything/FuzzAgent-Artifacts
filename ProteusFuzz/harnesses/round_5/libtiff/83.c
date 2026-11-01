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
#include <math.h>
#include <tiffio.h>

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int exp_mult;
    
    xyz[0] = *(float*)&Data[0];
    xyz[1] = *(float*)&Data[4];
    xyz[2] = *(float*)&Data[8];
    
    exp_mult = *(int*)&Data[12];
    
    uint32_t result = LogLuv32fromXYZ(xyz, exp_mult);
    (void)result;
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int mode;
    
    xyz[0] = *(float*)&Data[0];
    xyz[1] = *(float*)&Data[4];
    xyz[2] = *(float*)&Data[8];
    
    mode = *(int*)&Data[12];
    
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogL16fromY(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    double y;
    int em;
    
    y = *(double*)&Data[0];
    em = *(int*)&Data[8];
    
    int result = LogL16fromY(y, em);
    (void)result;
}

static void fuzz_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int p10;
    
    p10 = *(int*)&Data[0];
    
    double result = LogL10toY(p10);
    (void)result;
}

static void fuzz_LogL16toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int le;
    
    le = *(int*)&Data[0];
    
    double result = LogL16toY(le);
    (void)result;
}

static void fuzz_LogL10fromY(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    double y;
    int em;
    
    y = *(double*)&Data[0];
    em = *(int*)&Data[8];
    
    int result = LogL10fromY(y, em);
    (void)result;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    int func_id = *(int*)&Data[0];
    
    switch (func_id % 6) {
        case 0:
            if (Size >= 20) {
                fuzz_LogLuv32fromXYZ(Data + 4, Size - 4);
            }
            break;
        case 1:
            if (Size >= 20) {
                fuzz_LogLuv24fromXYZ(Data + 4, Size - 4);
            }
            break;
        case 2:
            if (Size >= 16) {
                fuzz_LogL16fromY(Data + 4, Size - 4);
            }
            break;
        case 3:
            if (Size >= 8) {
                fuzz_LogL10toY(Data + 4, Size - 4);
            }
            break;
        case 4:
            if (Size >= 8) {
                fuzz_LogL16toY(Data + 4, Size - 4);
            }
            break;
        case 5:
            if (Size >= 16) {
                fuzz_LogL10fromY(Data + 4, Size - 4);
            }
            break;
    }
    
    return 0;
}