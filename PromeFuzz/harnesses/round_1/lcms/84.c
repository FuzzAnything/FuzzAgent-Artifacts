// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsCIELab* parse_lab_point(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double) * 3) {
        return NULL;
    }
    
    cmsCIELab* lab = (cmsCIELab*)malloc(sizeof(cmsCIELab));
    if (!lab) {
        return NULL;
    }
    
    memcpy(&lab->L, *data, sizeof(double));
    *data += sizeof(double);
    memcpy(&lab->a, *data, sizeof(double));
    *data += sizeof(double);
    memcpy(&lab->b, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double) * 3;
    
    return lab;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsUInt32Number) + sizeof(double) * 3) {
        return 0;
    }
    
    // Parse flags from the beginning
    cmsUInt32Number flags;
    memcpy(&flags, Data, sizeof(cmsUInt32Number));
    Data += sizeof(cmsUInt32Number);
    Size -= sizeof(cmsUInt32Number);
    
    // Allocate GBD handle
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);
    if (!hGBD) {
        return 0;
    }
    
    // Add points to GBD until we run out of data
    while (Size >= sizeof(double) * 3) {
        cmsCIELab* lab = parse_lab_point(&Data, &Size);
        if (lab) {
            cmsGDBAddPoint(hGBD, lab);
            free(lab);
        }
    }
    
    // Compute missing data
    cmsGDBCompute(hGBD, flags);
    
    // Check a point if we have enough data
    if (Size >= sizeof(double) * 3) {
        cmsCIELab* check_lab = parse_lab_point(&Data, &Size);
        if (check_lab) {
            cmsGDBCheckPoint(hGBD, check_lab);
            free(check_lab);
        }
    }
    
    // Test desaturateLab if we have enough data
    if (Size >= sizeof(double) * 7) {
        cmsCIELab desat_lab;
        double amax, amin, bmax, bmin;
        
        memcpy(&desat_lab.L, Data, sizeof(double));
        Data += sizeof(double);
        memcpy(&desat_lab.a, Data, sizeof(double));
        Data += sizeof(double);
        memcpy(&desat_lab.b, Data, sizeof(double));
        Data += sizeof(double);
        
        memcpy(&amax, Data, sizeof(double));
        Data += sizeof(double);
        memcpy(&amin, Data, sizeof(double));
        Data += sizeof(double);
        memcpy(&bmax, Data, sizeof(double));
        Data += sizeof(double);
        memcpy(&bmin, Data, sizeof(double));
        Data += sizeof(double);
        
        cmsDesaturateLab(&desat_lab, amax, amin, bmax, bmin);
    }
    
    // Clean up
    cmsGBDFree(hGBD);
    
    return 0;
}