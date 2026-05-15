// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void initialize_float_array(float* arr, size_t count, const uint8_t* data, size_t size, size_t* offset) {
    for (size_t i = 0; i < count && *offset + sizeof(float) <= size; i++) {
        memcpy(&arr[i], data + *offset, sizeof(float));
        *offset += sizeof(float);
    }
}

static void initialize_uint16_array(cmsUInt16Number* arr, size_t count, const uint8_t* data, size_t size, size_t* offset) {
    for (size_t i = 0; i < count && *offset + sizeof(cmsUInt16Number) <= size; i++) {
        memcpy(&arr[i], data + *offset, sizeof(cmsUInt16Number));
        *offset += sizeof(cmsUInt16Number);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test cmsXYZEncoded2Float
    if (Size >= sizeof(cmsUInt16Number) * 3) {
        cmsCIEXYZ fxyz;
        cmsUInt16Number XYZ[3];
        initialize_uint16_array(XYZ, 3, Data, Size, &offset);
        cmsXYZEncoded2Float(&fxyz, XYZ);
    }
    
    // Test cmsFloat2XYZEncoded
    if (Size - offset >= sizeof(float) * 3) {
        cmsUInt16Number XYZ[3];
        cmsCIEXYZ fXYZ;
        initialize_float_array((float*)&fXYZ, 3, Data, Size, &offset);
        cmsFloat2XYZEncoded(XYZ, &fXYZ);
    }
    
    // Reset offset for independent testing
    offset = 0;
    
    // Test cmsFloat2LabEncoded
    if (Size >= sizeof(float) * 3) {
        cmsUInt16Number wLab[3];
        cmsCIELab Lab;
        initialize_float_array((float*)&Lab, 3, Data, Size, &offset);
        cmsFloat2LabEncoded(wLab, &Lab);
    }
    
    // Test cmsLabEncoded2Float
    if (Size - offset >= sizeof(cmsUInt16Number) * 3) {
        cmsCIELab Lab;
        cmsUInt16Number wLab[3];
        initialize_uint16_array(wLab, 3, Data, Size, &offset);
        cmsLabEncoded2Float(&Lab, wLab);
    }
    
    // Test _cmsAdjustEndianess16
    if (Size >= sizeof(cmsUInt16Number)) {
        cmsUInt16Number word;
        memcpy(&word, Data, sizeof(cmsUInt16Number));
        cmsUInt16Number result = _cmsAdjustEndianess16(word);
        (void)result;
    }
    
    // Test cmsLabEncoded2FloatV2
    if (Size >= sizeof(cmsUInt16Number) * 3) {
        cmsCIELab Lab;
        cmsUInt16Number wLab[3];
        size_t v2_offset = 0;
        initialize_uint16_array(wLab, 3, Data, Size, &v2_offset);
        cmsLabEncoded2FloatV2(&Lab, wLab);
    }
    
    return 0;
}