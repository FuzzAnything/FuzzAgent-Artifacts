// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;  // Minimum size for some operations

    // Test cmsXYZEncoded2Float
    if (Size >= 6) {
        cmsUInt16Number encodedXYZ[3];
        memcpy(encodedXYZ, Data, 6);
        cmsCIEXYZ floatXYZ;
        cmsXYZEncoded2Float(&floatXYZ, encodedXYZ);
    }

    // Test cmsFloat2LabEncoded
    if (Size >= 24) {
        cmsCIELab labFloat;
        memcpy(&labFloat, Data, 24);
        cmsUInt16Number encodedLab[3];
        cmsFloat2LabEncoded(encodedLab, &labFloat);
    }

    // Test cmsLabEncoded2FloatV2
    if (Size >= 6) {
        cmsUInt16Number encodedLab[3];
        memcpy(encodedLab, Data, 6);
        cmsCIELab labFloat;
        cmsLabEncoded2FloatV2(&labFloat, encodedLab);
    }

    // Test _cmsAdjustEndianess16
    if (Size >= 2) {
        cmsUInt16Number word;
        memcpy(&word, Data, 2);
        _cmsAdjustEndianess16(word);
    }

    // Test cmsFloat2XYZEncoded
    if (Size >= 24) {
        cmsCIEXYZ floatXYZ;
        memcpy(&floatXYZ, Data, 24);
        cmsUInt16Number encodedXYZ[3];
        cmsFloat2XYZEncoded(encodedXYZ, &floatXYZ);
    }

    // Test cmsLabEncoded2Float
    if (Size >= 6) {
        cmsUInt16Number encodedLab[3];
        memcpy(encodedLab, Data, 6);
        cmsCIELab labFloat;
        cmsLabEncoded2Float(&labFloat, encodedLab);
    }

    return 0;
}