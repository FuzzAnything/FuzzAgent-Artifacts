// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void initialize_structures(const uint8_t *Data, size_t Size, 
                                 cmsJCh *jch, cmsCIEXYZ *xyz, 
                                 cmsCIELab *lab) {
    if (Size >= sizeof(cmsJCh) + sizeof(cmsCIEXYZ) + sizeof(cmsCIELab)) {
        memcpy(jch, Data, sizeof(cmsJCh));
        memcpy(xyz, Data + sizeof(cmsJCh), sizeof(cmsCIEXYZ));
        memcpy(lab, Data + sizeof(cmsJCh) + sizeof(cmsCIEXYZ), sizeof(cmsCIELab));
    } else {
        memset(jch, 0, sizeof(cmsJCh));
        memset(xyz, 0, sizeof(cmsCIEXYZ));
        memset(lab, 0, sizeof(cmsCIELab));
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    cmsJCh jch_in, jch_out;
    cmsCIEXYZ xyz_in, xyz_out, xyz_adapted;
    cmsCIELab lab_in, lab_out;
    
    initialize_structures(Data, Size, &jch_in, &xyz_in, &lab_in);
    
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    
    cmsXYZ2Lab(d50, &lab_out, &xyz_in);
    
    cmsLab2XYZ(d50, &xyz_out, &lab_in);
    
    cmsAdaptToIlluminant(&xyz_adapted, d50, d50, &xyz_in);
    
    cmsViewingConditions vc;
    vc.whitePoint.X = 0.95047;
    vc.whitePoint.Y = 1.0;
    vc.whitePoint.Z = 1.08883;
    vc.Yb = 20.0;
    vc.La = 64.0;
    vc.surround = 1;
    
    cmsHANDLE hModel = cmsCIECAM02Init(NULL, &vc);
    if (hModel) {
        cmsCIECAM02Forward(hModel, &xyz_in, &jch_out);
        cmsCIECAM02Reverse(hModel, &jch_in, &xyz_out);
        cmsCIECAM02Done(hModel);
    }
    
    return 0;
}