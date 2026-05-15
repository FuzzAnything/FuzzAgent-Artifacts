// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void fill_viewing_conditions(cmsViewingConditions* vc, const uint8_t* data, size_t size) {
    if (size >= sizeof(cmsViewingConditions)) {
        memcpy(vc, data, sizeof(cmsViewingConditions));
    } else {
        memset(vc, 0, sizeof(cmsViewingConditions));
        if (size > 0) {
            memcpy(vc, data, size);
        }
    }
}

static void fill_xyz(cmsCIEXYZ* xyz, const uint8_t* data, size_t size) {
    if (size >= sizeof(cmsCIEXYZ)) {
        memcpy(xyz, data, sizeof(cmsCIEXYZ));
    } else {
        memset(xyz, 0, sizeof(cmsCIEXYZ));
        if (size > 0) {
            memcpy(xyz, data, size);
        }
    }
}

static void fill_jch(cmsJCh* jch, const uint8_t* data, size_t size) {
    if (size >= sizeof(cmsJCh)) {
        memcpy(jch, data, sizeof(cmsJCh));
    } else {
        memset(jch, 0, sizeof(cmsJCh));
        if (size > 0) {
            memcpy(jch, data, size);
        }
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext context = create_test_context();
    if (!context) return 0;

    cmsViewingConditions vc;
    fill_viewing_conditions(&vc, Data, Size);
    
    cmsHANDLE model = cmsCIECAM02Init(context, &vc);
    
    if (model) {
        cmsCIEXYZ xyz_input;
        cmsJCh jch_output;
        cmsJCh jch_input;
        cmsCIEXYZ xyz_output;
        
        fill_xyz(&xyz_input, Data, Size);
        cmsCIECAM02Forward(model, &xyz_input, &jch_output);
        
        fill_jch(&jch_input, Data, Size);
        cmsCIECAM02Reverse(model, &jch_input, &xyz_output);
        
        cmsCIECAM02Done(model);
    }
    
    cmsCIEXYZ result, source_white, illuminant, value;
    fill_xyz(&source_white, Data, Size);
    fill_xyz(&illuminant, Data + sizeof(cmsCIEXYZ) % Size, Size);
    fill_xyz(&value, Data + (2 * sizeof(cmsCIEXYZ)) % Size, Size);
    
    cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
    
    cmsFloat64Number adaptation_state = (cmsFloat64Number)(Data[0] / 255.0);
    cmsSetAdaptationStateTHR(context, adaptation_state);
    
    cmsDeleteContext(context);
    
    return 0;
}