// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsXYZEncoded2Float(const uint8_t *data, size_t size) {
    cmsCIEXYZ fxyz;
    cmsUInt16Number XYZ[3];
    
    if (size < sizeof(XYZ)) return;
    
    memcpy(XYZ, data, sizeof(XYZ));
    cmsXYZEncoded2Float(&fxyz, XYZ);
}

static void fuzz_cmsXYZ2Lab(const uint8_t *data, size_t size) {
    cmsCIEXYZ WhitePoint, xyz;
    cmsCIELab Lab;
    
    if (size < sizeof(WhitePoint) + sizeof(xyz)) return;
    
    memcpy(&WhitePoint, data, sizeof(WhitePoint));
    memcpy(&xyz, data + sizeof(WhitePoint), sizeof(xyz));
    
    cmsXYZ2Lab(&WhitePoint, &Lab, &xyz);
    cmsXYZ2Lab(NULL, &Lab, &xyz);
}

static void fuzz_cmsxyY2XYZ(const uint8_t *data, size_t size) {
    cmsCIEXYZ Dest;
    cmsCIExyY Source;
    
    if (size < sizeof(Source)) return;
    
    memcpy(&Source, data, sizeof(Source));
    cmsxyY2XYZ(&Dest, &Source);
}

static void fuzz_cmsFloat2XYZEncoded(const uint8_t *data, size_t size) {
    cmsUInt16Number XYZ[3];
    cmsCIEXYZ fXYZ;
    
    if (size < sizeof(fXYZ)) return;
    
    memcpy(&fXYZ, data, sizeof(fXYZ));
    cmsFloat2XYZEncoded(XYZ, &fXYZ);
}

static void fuzz_cmsLab2XYZ(const uint8_t *data, size_t size) {
    cmsCIEXYZ WhitePoint, xyz;
    cmsCIELab Lab;
    
    if (size < sizeof(WhitePoint) + sizeof(Lab)) return;
    
    memcpy(&WhitePoint, data, sizeof(WhitePoint));
    memcpy(&Lab, data + sizeof(WhitePoint), sizeof(Lab));
    
    cmsLab2XYZ(&WhitePoint, &xyz, &Lab);
    cmsLab2XYZ(NULL, &xyz, &Lab);
}

static void fuzz_cmsXYZ2xyY(const uint8_t *data, size_t size) {
    cmsCIExyY Dest;
    cmsCIEXYZ Source;
    
    if (size < sizeof(Source)) return;
    
    memcpy(&Source, data, sizeof(Source));
    cmsXYZ2xyY(&Dest, &Source);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_cmsXYZEncoded2Float(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_cmsXYZ2Lab(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_cmsxyY2XYZ(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_cmsFloat2XYZEncoded(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_cmsLab2XYZ(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_cmsXYZ2xyY(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}