// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsCIEXYZ_from_data(cmsCIEXYZ* xyz, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(cmsCIEXYZ)) {
        memcpy(xyz, *data, sizeof(cmsCIEXYZ));
        *data += sizeof(cmsCIEXYZ);
        *size -= sizeof(cmsCIEXYZ);
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

static void fuzz_cmsCIELab_from_data(cmsCIELab* lab, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(cmsCIELab)) {
        memcpy(lab, *data, sizeof(cmsCIELab));
        *data += sizeof(cmsCIELab);
        *size -= sizeof(cmsCIELab);
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void fuzz_cmsJCh_from_data(cmsJCh* jch, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(cmsJCh)) {
        memcpy(jch, *data, sizeof(cmsJCh));
        *data += sizeof(cmsJCh);
        *size -= sizeof(cmsJCh);
    } else {
        jch->J = 0.0;
        jch->C = 0.0;
        jch->h = 0.0;
    }
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    const uint8_t* data_ptr = Data;
    size_t data_size = Size;
    
    cmsCIEXYZ result, sourceWhitePt, illuminant, value, xyz1, xyz2, xyz3, xyz4;
    cmsCIELab lab1, lab2;
    cmsJCh jch1, jch2;
    
    fuzz_cmsCIEXYZ_from_data(&sourceWhitePt, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&illuminant, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&value, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&xyz1, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&xyz2, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&xyz3, &data_ptr, &data_size);
    fuzz_cmsCIEXYZ_from_data(&xyz4, &data_ptr, &data_size);
    fuzz_cmsCIELab_from_data(&lab1, &data_ptr, &data_size);
    fuzz_cmsCIELab_from_data(&lab2, &data_ptr, &data_size);
    fuzz_cmsJCh_from_data(&jch1, &data_ptr, &data_size);
    fuzz_cmsJCh_from_data(&jch2, &data_ptr, &data_size);
    
    cmsBool adapt_result = cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
    (void)adapt_result;
    
    cmsXYZ2Lab(NULL, &lab1, &xyz1);
    cmsXYZ2Lab(cmsD50_XYZ(), &lab2, &xyz2);
    
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    (void)d50;
    
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint = *cmsD50_XYZ();
    vc.Yb = 20.0;
    vc.La = 100.0;
    vc.surround = 1;
    
    cmsHANDLE hModel = cmsCIECAM02Init(NULL, &vc);
    if (hModel) {
        cmsCIECAM02Forward(hModel, &xyz3, &jch1);
        cmsCIECAM02Reverse(hModel, &jch2, &xyz4);
        cmsCIECAM02Done(hModel);
    }
    
    cmsLab2XYZ(NULL, &result, &lab1);
    cmsLab2XYZ(cmsD50_XYZ(), &result, &lab2);
    
    return 0;
}