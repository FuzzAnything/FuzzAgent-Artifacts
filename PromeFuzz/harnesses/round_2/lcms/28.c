// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsFloat2XYZEncoded(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ fXYZ;
    cmsUInt16Number XYZ[3];
    
    memcpy(&fXYZ, data, sizeof(cmsCIEXYZ));
    cmsFloat2XYZEncoded(XYZ, &fXYZ);
}

static void fuzz_cmsFloat2LabEncoded(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    
    memcpy(&Lab, data, sizeof(cmsCIELab));
    cmsFloat2LabEncoded(wLab, &Lab);
}

static void fuzz_cmsLabEncoded2Float(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsUInt16Number) * 3) return;
    
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    
    memcpy(wLab, data, sizeof(cmsUInt16Number) * 3);
    cmsLabEncoded2Float(&Lab, wLab);
}

static void fuzz_cmsLab2XYZ(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab;
    cmsCIEXYZ xyz;
    
    memcpy(&Lab, data, sizeof(cmsCIELab));
    cmsLab2XYZ(NULL, &xyz, &Lab);
}

static void fuzz_cmsFloat2LabEncodedV2(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    
    memcpy(&Lab, data, sizeof(cmsCIELab));
    cmsFloat2LabEncodedV2(wLab, &Lab);
}

static void fuzz_cmsLabEncoded2FloatV2(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsUInt16Number) * 3) return;
    
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    
    memcpy(wLab, data, sizeof(cmsUInt16Number) * 3);
    cmsLabEncoded2FloatV2(&Lab, wLab);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_cmsFloat2XYZEncoded(Data, Size);
    fuzz_cmsFloat2LabEncoded(Data, Size);
    fuzz_cmsLabEncoded2Float(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    fuzz_cmsFloat2LabEncodedV2(Data, Size);
    fuzz_cmsLabEncoded2FloatV2(Data, Size);
    
    return 0;
}