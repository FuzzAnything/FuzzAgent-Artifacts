// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void populate_xyz_from_data(cmsCIEXYZ* xyz, const uint8_t* data, size_t size, size_t* offset) {
    if (xyz == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&xyz->X, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Y, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Z, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        xyz->X = xyz->Y = xyz->Z = 0.0;
        if (*offset < size) {
            size_t remaining = size - *offset;
            size_t to_copy = remaining > sizeof(double) * 3 ? sizeof(double) * 3 : remaining;
            memcpy(xyz, data + *offset, to_copy);
            *offset += to_copy;
        }
    }
}

static void populate_lab_from_data(cmsCIELab* lab, const uint8_t* data, size_t size, size_t* offset) {
    if (lab == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&lab->L, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->a, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->b, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        lab->L = lab->a = lab->b = 0.0;
        if (*offset < size) {
            size_t remaining = size - *offset;
            size_t to_copy = remaining > sizeof(double) * 3 ? sizeof(double) * 3 : remaining;
            memcpy(lab, data + *offset, to_copy);
            *offset += to_copy;
        }
    }
}

static void populate_jch_from_data(cmsJCh* jch, const uint8_t* data, size_t size, size_t* offset) {
    if (jch == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&jch->J, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&jch->C, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&jch->h, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        jch->J = jch->C = jch->h = 0.0;
        if (*offset < size) {
            size_t remaining = size - *offset;
            size_t to_copy = remaining > sizeof(double) * 3 ? sizeof(double) * 3 : remaining;
            memcpy(jch, data + *offset, to_copy);
            *offset += to_copy;
        }
    }
}

static void populate_viewing_conditions_from_data(cmsViewingConditions* vc, const uint8_t* data, size_t size, size_t* offset) {
    if (vc == NULL) return;
    
    if (*offset + sizeof(cmsViewingConditions) <= size) {
        memcpy(vc, data + *offset, sizeof(cmsViewingConditions));
        *offset += sizeof(cmsViewingConditions);
    } else {
        memset(vc, 0, sizeof(cmsViewingConditions));
        if (*offset < size) {
            size_t remaining = size - *offset;
            size_t to_copy = remaining > sizeof(cmsViewingConditions) ? sizeof(cmsViewingConditions) : remaining;
            memcpy(vc, data + *offset, to_copy);
            *offset += to_copy;
        }
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    size_t offset = 0;
    
    cmsCIEXYZ result, source_white, illuminant, value;
    cmsCIELab lab;
    cmsJCh jch;
    
    populate_xyz_from_data(&source_white, Data, Size, &offset);
    populate_xyz_from_data(&illuminant, Data, Size, &offset);
    populate_xyz_from_data(&value, Data, Size, &offset);
    
    cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
    
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    
    populate_xyz_from_data(&value, Data, Size, &offset);
    cmsXYZ2Lab(d50, &lab, &value);
    
    populate_lab_from_data(&lab, Data, Size, &offset);
    cmsLab2XYZ(d50, &value, &lab);
    
    cmsViewingConditions vc;
    populate_viewing_conditions_from_data(&vc, Data, Size, &offset);
    cmsHANDLE hModel = cmsCIECAM02Init(NULL, &vc);
    
    if (hModel != NULL) {
        populate_jch_from_data(&jch, Data, Size, &offset);
        cmsCIECAM02Reverse(hModel, &jch, &value);
        
        populate_xyz_from_data(&value, Data, Size, &offset);
        cmsCIECAM02Forward(hModel, &value, &jch);
        
        cmsCIECAM02Done(hModel);
    }
    
    return 0;
}