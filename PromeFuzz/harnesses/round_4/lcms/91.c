// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsXYZEncoded2Float(const uint8_t *data, size_t size) {
    if (size < 6) return;
    
    cmsUInt16Number encoded[3];
    memcpy(encoded, data, 6);
    
    cmsCIEXYZ fxyz;
    cmsXYZEncoded2Float(&fxyz, encoded);
}

static void fuzz_cmsFloat2LabEncoded(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIELab lab;
    memcpy(&lab, data, sizeof(cmsCIELab));
    
    cmsUInt16Number encoded[3];
    cmsFloat2LabEncoded(encoded, &lab);
}

static void fuzz_cmsFloat2XYZEncoded(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ fxyz;
    memcpy(&fxyz, data, sizeof(cmsCIEXYZ));
    
    cmsUInt16Number encoded[3];
    cmsFloat2XYZEncoded(encoded, &fxyz);
}

static void fuzz_cmsAdjustEndianess16(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cmsUInt16Number word;
    memcpy(&word, data, 2);
    
    _cmsAdjustEndianess16(word);
}

static void fuzz_cmsLabEncoded2FloatV2(const uint8_t *data, size_t size) {
    if (size < 6) return;
    
    cmsUInt16Number encoded[3];
    memcpy(encoded, data, 6);
    
    cmsCIELab lab;
    cmsLabEncoded2FloatV2(&lab, encoded);
}

static void fuzz_cmsLabEncoded2Float(const uint8_t *data, size_t size) {
    if (size < 6) return;
    
    cmsUInt16Number encoded[3];
    memcpy(encoded, data, 6);
    
    cmsCIELab lab;
    cmsLabEncoded2Float(&lab, encoded);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_cmsXYZEncoded2Float(Data, Size);
    fuzz_cmsFloat2LabEncoded(Data, Size);
    fuzz_cmsFloat2XYZEncoded(Data, Size);
    fuzz_cmsAdjustEndianess16(Data, Size);
    fuzz_cmsLabEncoded2FloatV2(Data, Size);
    fuzz_cmsLabEncoded2Float(Data, Size);
    
    return 0;
}