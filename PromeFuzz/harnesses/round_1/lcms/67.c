// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_viewing_conditions(cmsViewingConditions* vc, const double* data) {
    if (vc && data) {
        vc->whitePoint.X = data[0];
        vc->whitePoint.Y = data[1];
        vc->whitePoint.Z = data[2];
        vc->surround = data[3];
        vc->D_value = data[4];
        vc->La = data[5];
        vc->Yb = data[6];
    }
}

static void init_xyz(cmsCIEXYZ* xyz, const double* data) {
    if (xyz && data) {
        xyz->X = data[0];
        xyz->Y = data[1];
        xyz->Z = data[2];
    }
}

static void init_jch(cmsJCh* jch, const double* data) {
    if (jch && data) {
        jch->J = data[0];
        jch->C = data[1];
        jch->h = data[2];
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 11 * sizeof(double)) {
        return 0;
    }

    const double* double_data = (const double*)Data;
    size_t double_count = Size / sizeof(double);
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    cmsViewingConditions vc;
    cmsCIEXYZ xyz_in, xyz_out, source_white, illuminant, value, result;
    cmsJCh jch_in, jch_out;
    
    size_t idx = 0;
    
    if (idx + 7 <= double_count) {
        init_viewing_conditions(&vc, &double_data[idx]);
        idx += 7;
    } else {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsHANDLE hModel = cmsCIECAM02Init(ctx, &vc);
    if (!hModel) {
        cmsDeleteContext(ctx);
        return 0;
    }

    if (idx + 3 <= double_count) {
        init_xyz(&xyz_in, &double_data[idx]);
        idx += 3;
    }

    if (idx + 3 <= double_count) {
        init_jch(&jch_in, &double_data[idx]);
        idx += 3;
    }

    if (hModel) {
        cmsCIECAM02Forward(hModel, &xyz_in, &jch_out);
        cmsCIECAM02Reverse(hModel, &jch_in, &xyz_out);
    }

    if (idx < double_count) {
        double adaptation_state = double_data[idx];
        if (adaptation_state >= 0) {
            cmsSetAdaptationStateTHR(ctx, adaptation_state);
        }
        idx++;
    }

    if (idx + 12 <= double_count) {
        init_xyz(&source_white, &double_data[idx]);
        idx += 3;
        init_xyz(&illuminant, &double_data[idx]);
        idx += 3;
        init_xyz(&value, &double_data[idx]);
        idx += 3;
        init_xyz(&result, &double_data[idx]);
        idx += 3;
        
        cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
    }

    if (hModel) {
        cmsCIECAM02Done(hModel);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}