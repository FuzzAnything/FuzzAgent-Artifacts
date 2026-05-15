// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
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

static void initialize_lab_struct(cmsCIELab* lab, const uint8_t* data, size_t size, size_t* offset) {
    if (lab == NULL || data == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&lab->L, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->a, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->b, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        lab->L = lab->a = lab->b = 0.0;
    }
}

static void initialize_xyz_struct(cmsCIEXYZ* xyz, const uint8_t* data, size_t size, size_t* offset) {
    if (xyz == NULL || data == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&xyz->X, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Y, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Z, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        xyz->X = xyz->Y = xyz->Z = 0.0;
    }
}

static void initialize_uint16_array(cmsUInt16Number arr[3], const uint8_t* data, size_t size, size_t* offset) {
    if (arr == NULL || data == NULL) return;
    
    for (int i = 0; i < 3; i++) {
        if (*offset + sizeof(cmsUInt16Number) <= size) {
            memcpy(&arr[i], data + *offset, sizeof(cmsUInt16Number));
            *offset += sizeof(cmsUInt16Number);
        } else {
            arr[i] = 0;
        }
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsUInt16Number)) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Test _cmsAdjustEndianess16
    cmsUInt16Number word = 0;
    if (offset + sizeof(cmsUInt16Number) <= Size) {
        memcpy(&word, Data + offset, sizeof(cmsUInt16Number));
        offset += sizeof(cmsUInt16Number);
        cmsUInt16Number result = _cmsAdjustEndianess16(word);
        (void)result;
    }
    
    // Test cmsFloat2LabEncoded
    cmsCIELab lab1;
    cmsUInt16Number wLab1[3] = {0};
    initialize_lab_struct(&lab1, Data, Size, &offset);
    cmsFloat2LabEncoded(wLab1, &lab1);
    
    // Test cmsXYZEncoded2Float
    cmsUInt16Number XYZ_encoded1[3] = {0};
    cmsCIEXYZ fxyz1;
    initialize_uint16_array(XYZ_encoded1, Data, Size, &offset);
    cmsXYZEncoded2Float(&fxyz1, XYZ_encoded1);
    
    // Test cmsLabEncoded2Float
    cmsUInt16Number wLab2[3] = {0};
    cmsCIELab lab2;
    initialize_uint16_array(wLab2, Data, Size, &offset);
    cmsLabEncoded2Float(&lab2, wLab2);
    
    // Test cmsFloat2XYZEncoded
    cmsCIEXYZ fxyz2;
    cmsUInt16Number XYZ_encoded2[3] = {0};
    initialize_xyz_struct(&fxyz2, Data, Size, &offset);
    cmsFloat2XYZEncoded(XYZ_encoded2, &fxyz2);
    
    // Test cmsLabEncoded2FloatV2
    cmsUInt16Number wLab3[3] = {0};
    cmsCIELab lab3;
    initialize_uint16_array(wLab3, Data, Size, &offset);
    cmsLabEncoded2FloatV2(&lab3, wLab3);
    
    return 0;
}