// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsFloat2XYZEncoded(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ fXYZ;
    memcpy(&fXYZ, data, sizeof(cmsCIEXYZ));
    
    cmsUInt16Number XYZ[3];
    cmsFloat2XYZEncoded(XYZ, &fXYZ);
}

static void fuzz__cmsAdjustEndianess16(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt16Number)) return;
    
    cmsUInt16Number word;
    memcpy(&word, data, sizeof(cmsUInt16Number));
    
    _cmsAdjustEndianess16(word);
}

static void fuzz_cmsLabEncoded2Float(const uint8_t* data, size_t size) {
    if (size < 3 * sizeof(cmsUInt16Number)) return;
    
    cmsUInt16Number wLab[3];
    memcpy(wLab, data, 3 * sizeof(cmsUInt16Number));
    
    cmsCIELab Lab;
    cmsLabEncoded2Float(&Lab, wLab);
}

static void fuzz_cmsXYZEncoded2Float(const uint8_t* data, size_t size) {
    if (size < 3 * sizeof(cmsUInt16Number)) return;
    
    cmsUInt16Number XYZ[3];
    memcpy(XYZ, data, 3 * sizeof(cmsUInt16Number));
    
    cmsCIEXYZ fxyz;
    cmsXYZEncoded2Float(&fxyz, XYZ);
}

static void fuzz_cmsFloat2LabEncoded(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab;
    memcpy(&Lab, data, sizeof(cmsCIELab));
    
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncoded(wLab, &Lab);
}

static void fuzz_cmsLabEncoded2FloatV2(const uint8_t* data, size_t size) {
    if (size < 3 * sizeof(cmsUInt16Number)) return;
    
    cmsUInt16Number wLab[3];
    memcpy(wLab, data, 3 * sizeof(cmsUInt16Number));
    
    cmsCIELab Lab;
    cmsLabEncoded2FloatV2(&Lab, wLab);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0:
            fuzz_cmsFloat2XYZEncoded(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz__cmsAdjustEndianess16(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_cmsLabEncoded2Float(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_cmsXYZEncoded2Float(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_cmsFloat2LabEncoded(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_cmsLabEncoded2FloatV2(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}