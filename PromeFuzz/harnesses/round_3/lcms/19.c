// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void init_xyz_from_data(cmsCIEXYZ* xyz, const uint8_t* data, size_t size, size_t* offset) {
    if (size - *offset >= sizeof(float) * 3) {
        memcpy(&xyz->X, data + *offset, sizeof(float));
        memcpy(&xyz->Y, data + *offset + sizeof(float), sizeof(float));
        memcpy(&xyz->Z, data + *offset + sizeof(float) * 2, sizeof(float));
        *offset += sizeof(float) * 3;
    } else {
        xyz->X = xyz->Y = xyz->Z = 0.0f;
    }
}

static void init_lab_from_data(cmsCIELab* lab, const uint8_t* data, size_t size, size_t* offset) {
    if (size - *offset >= sizeof(float) * 3) {
        memcpy(&lab->L, data + *offset, sizeof(float));
        memcpy(&lab->a, data + *offset + sizeof(float), sizeof(float));
        memcpy(&lab->b, data + *offset + sizeof(float) * 2, sizeof(float));
        *offset += sizeof(float) * 3;
    } else {
        lab->L = lab->a = lab->b = 0.0f;
    }
}

static void init_encoded_from_data(cmsUInt16Number encoded[3], const uint8_t* data, size_t size, size_t* offset) {
    if (size - *offset >= sizeof(cmsUInt16Number) * 3) {
        memcpy(encoded, data + *offset, sizeof(cmsUInt16Number) * 3);
        *offset += sizeof(cmsUInt16Number) * 3;
    } else {
        encoded[0] = encoded[1] = encoded[2] = 0;
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    uint8_t selector = Data[0] % 6;
    offset++;
    
    switch (selector) {
        case 0: {
            cmsCIEXYZ fXYZ;
            cmsUInt16Number XYZ[3];
            init_xyz_from_data(&fXYZ, Data, Size, &offset);
            cmsFloat2XYZEncoded(XYZ, &fXYZ);
            break;
        }
        case 1: {
            cmsCIELab Lab;
            cmsUInt16Number wLab[3];
            init_lab_from_data(&Lab, Data, Size, &offset);
            cmsFloat2LabEncodedV2(wLab, &Lab);
            break;
        }
        case 2: {
            cmsCIELab Lab;
            cmsUInt16Number wLab[3];
            init_encoded_from_data(wLab, Data, Size, &offset);
            cmsLabEncoded2Float(&Lab, wLab);
            break;
        }
        case 3: {
            cmsCIEXYZ WhitePoint, xyz;
            cmsCIELab Lab;
            init_xyz_from_data(&WhitePoint, Data, Size, &offset);
            init_lab_from_data(&Lab, Data, Size, &offset);
            cmsLab2XYZ(&WhitePoint, &xyz, &Lab);
            break;
        }
        case 4: {
            cmsCIELab Lab;
            cmsUInt16Number wLab[3];
            init_lab_from_data(&Lab, Data, Size, &offset);
            cmsFloat2LabEncoded(wLab, &Lab);
            break;
        }
        case 5: {
            cmsCIELab Lab;
            cmsUInt16Number wLab[3];
            init_encoded_from_data(wLab, Data, Size, &offset);
            cmsLabEncoded2FloatV2(&Lab, wLab);
            break;
        }
    }
    
    return 0;
}