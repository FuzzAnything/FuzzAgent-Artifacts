// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    // Create a context for the GBD operations
    return cmsCreateContext(NULL, NULL);
}

static void destroy_test_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

static int parse_lab_point(const uint8_t* data, size_t size, size_t offset, cmsCIELab* lab) {
    if (offset + sizeof(double) * 3 > size) {
        return 0;
    }
    
    // Use memcpy to avoid alignment issues
    memcpy(&lab->L, data + offset, sizeof(double));
    memcpy(&lab->a, data + offset + sizeof(double), sizeof(double));
    memcpy(&lab->b, data + offset + sizeof(double) * 2, sizeof(double));
    return 1;
}

static int parse_double(const uint8_t* data, size_t size, size_t offset, double* value) {
    if (offset + sizeof(double) > size) {
        return 0;
    }
    memcpy(value, data + offset, sizeof(double));
    return 1;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }
    
    cmsContext context = create_test_context();
    if (!context) {
        return 0;
    }
    
    // Test cmsGBDAlloc
    cmsHANDLE gbd = cmsGBDAlloc(context);
    if (!gbd) {
        destroy_test_context(context);
        return 0;
    }
    
    // Test cmsGDBAddPoint with multiple Lab points from fuzzer input
    size_t offset = 0;
    cmsCIELab lab;
    int points_added = 0;
    
    while (offset + sizeof(double) * 3 <= Size && points_added < 10) {
        if (parse_lab_point(Data, Size, offset, &lab)) {
            cmsGDBAddPoint(gbd, &lab);
            points_added++;
            offset += sizeof(double) * 3;
        } else {
            break;
        }
    }
    
    // Test cmsGDBCheckPoint with the first Lab point
    if (points_added > 0) {
        parse_lab_point(Data, Size, 0, &lab);
        cmsGDBCheckPoint(gbd, &lab);
    }
    
    // Test cmsGDBCompute with various flags
    cmsUInt32Number flags[] = {0, 1, 2, 4};
    for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
        cmsGDBCompute(gbd, flags[i]);
    }
    
    // Test cmsDesaturateLab if we have enough data
    if (Size >= sizeof(double) * 7) {
        cmsCIELab lab_copy;
        parse_lab_point(Data, Size, 0, &lab_copy);
        
        double amax, amin, bmax, bmin;
        size_t bounds_offset = sizeof(double) * 3;
        
        if (parse_double(Data, Size, bounds_offset, &amax) &&
            parse_double(Data, Size, bounds_offset + sizeof(double), &amin) &&
            parse_double(Data, Size, bounds_offset + sizeof(double) * 2, &bmax) &&
            parse_double(Data, Size, bounds_offset + sizeof(double) * 3, &bmin)) {
            
            cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);
        }
    }
    
    // Clean up
    cmsGBDFree(gbd);
    destroy_test_context(context);
    
    return 0;
}