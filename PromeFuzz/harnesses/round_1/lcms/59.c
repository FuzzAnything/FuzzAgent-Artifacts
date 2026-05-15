// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void initialize_lab_from_data(cmsCIELab* lab, const uint8_t* data, size_t size, size_t* offset) {
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

static void initialize_xyz_from_data(cmsCIEXYZ* xyz, const uint8_t* data, size_t size, size_t* offset) {
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

static void initialize_encoded_from_data(cmsUInt16Number encoded[3], const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(cmsUInt16Number) * 3 <= size) {
        memcpy(encoded, data + *offset, sizeof(cmsUInt16Number) * 3);
        *offset += sizeof(cmsUInt16Number) * 3;
    } else {
        encoded[0] = encoded[1] = encoded[2] = 0;
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    size_t offset = 0;
    
    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsUInt16Number encoded[3];
    cmsUInt16Number encoded_out[3];
    cmsCIELab lab_out;
    cmsCIEXYZ xyz_out;
    
    initialize_lab_from_data(&lab, Data, Size, &offset);
    initialize_xyz_from_data(&xyz, Data, Size, &offset);
    initialize_encoded_from_data(encoded, Data, Size, &offset);
    
    cmsFloat2LabEncoded(encoded_out, &lab);
    cmsLabEncoded2Float(&lab_out, encoded);
    cmsFloat2XYZEncoded(encoded_out, &xyz);
    cmsFloat2LabEncodedV2(encoded_out, &lab);
    
    cmsCIEXYZ white_point;
    initialize_xyz_from_data(&white_point, Data, Size, &offset);
    cmsLab2XYZ(&white_point, &xyz_out, &lab);
    cmsLab2XYZ(NULL, &xyz_out, &lab);
    
    cmsLabEncoded2FloatV2(&lab_out, encoded);
    
    return 0;
}