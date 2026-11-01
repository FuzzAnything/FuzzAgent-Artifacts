// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void fuzz_cmsXYZ2xyY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ source;
    cmsCIExyY dest;
    
    memcpy(&source, Data, sizeof(cmsCIEXYZ));
    
    cmsXYZ2xyY(&dest, &source);
}

static void fuzz_cmsD50_XYZ(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        volatile double check = d50->X + d50->Y + d50->Z;
        (void)check;
    }
}

static void fuzz_cmsxyY2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIExyY)) return;
    
    cmsCIExyY source;
    cmsCIEXYZ dest;
    
    memcpy(&source, Data, sizeof(cmsCIExyY));
    
    cmsxyY2XYZ(&dest, &source);
}

static void fuzz_cmsXYZ2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ source;
    cmsCIELab lab;
    cmsCIEXYZ whitePoint;
    
    memcpy(&source, Data, sizeof(cmsCIEXYZ));
    
    if (Size >= sizeof(cmsCIEXYZ) * 2) {
        memcpy(&whitePoint, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
        cmsXYZ2Lab(&whitePoint, &lab, &source);
    } else {
        cmsXYZ2Lab(NULL, &lab, &source);
    }
}

static void fuzz_cmsAdaptToIlluminant(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ) * 3) return;
    
    cmsCIEXYZ result;
    cmsCIEXYZ sourceWhitePt;
    cmsCIEXYZ illuminant;
    cmsCIEXYZ value;
    
    memcpy(&sourceWhitePt, Data, sizeof(cmsCIEXYZ));
    memcpy(&illuminant, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
    memcpy(&value, Data + sizeof(cmsCIEXYZ) * 2, sizeof(cmsCIEXYZ));
    
    cmsBool ret = cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
    (void)ret;
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;
    
    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;
    
    memcpy(&lab, Data, sizeof(cmsCIELab));
    
    if (Size >= sizeof(cmsCIELab) + sizeof(cmsCIEXYZ)) {
        memcpy(&whitePoint, Data + sizeof(cmsCIELab), sizeof(cmsCIEXYZ));
        cmsLab2XYZ(&whitePoint, &xyz, &lab);
    } else {
        cmsLab2XYZ(NULL, &xyz, &lab);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_cmsXYZ2xyY(Data, Size);
    fuzz_cmsD50_XYZ(Data, Size);
    fuzz_cmsxyY2XYZ(Data, Size);
    fuzz_cmsXYZ2Lab(Data, Size);
    fuzz_cmsAdaptToIlluminant(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    
    return 0;
}