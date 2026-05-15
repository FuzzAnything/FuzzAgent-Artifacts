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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    // Use default context for simplicity
    cmsContext contextID = NULL;
    
    // Allocate a gamut boundary descriptor
    cmsHANDLE hGBD = cmsGBDAlloc(contextID);
    if (!hGBD) {
        return 0;
    }
    
    // Parse input data to create Lab points
    // We need at least 24 bytes for one Lab point (3 doubles)
    size_t lab_size = sizeof(double) * 3;
    size_t max_points = Size / lab_size;
    
    for (size_t i = 0; i < max_points && i < 10; i++) { // Limit to 10 points
        const uint8_t* lab_data = Data + (i * lab_size);
        cmsCIELab lab;
        
        // Copy data into Lab structure
        memcpy(&lab, lab_data, sizeof(lab));
        
        // Add point to GDB
        cmsGDBAddPoint(hGBD, &lab);
    }
    
    // Try to compute missing data with random flags
    cmsUInt32Number flags = 0;
    if (Size > 0) {
        flags = Data[0] | (Size > 1 ? (Data[1] << 8) : 0) | 
                (Size > 2 ? (Data[2] << 16) : 0) | 
                (Size > 3 ? (Data[3] << 24) : 0);
    }
    cmsGDBCompute(hGBD, flags);
    
    // Check some points if we have data
    if (max_points > 0) {
        for (size_t i = 0; i < max_points && i < 5; i++) { // Check up to 5 points
            const uint8_t* lab_data = Data + (i * lab_size);
            cmsCIELab lab_check;
            
            memcpy(&lab_check, lab_data, sizeof(lab_check));
            cmsGDBCheckPoint(hGBD, &lab_check);
        }
    }
    
    // Test cmsDesaturateLab with random bounds
    if (Size >= lab_size) {
        cmsCIELab lab_desat;
        memcpy(&lab_desat, Data, sizeof(lab_desat));
        
        // Generate bounds from input data
        double amax = 100.0, amin = -100.0;
        double bmax = 100.0, bmin = -100.0;
        
        if (Size >= lab_size + 8) {
            memcpy(&amax, Data + lab_size, sizeof(double));
        }
        if (Size >= lab_size + 16) {
            memcpy(&amin, Data + lab_size + 8, sizeof(double));
        }
        if (Size >= lab_size + 24) {
            memcpy(&bmax, Data + lab_size + 16, sizeof(double));
        }
        if (Size >= lab_size + 32) {
            memcpy(&bmin, Data + lab_size + 24, sizeof(double));
        }
        
        cmsDesaturateLab(&lab_desat, amax, amin, bmax, bmin);
    }
    
    // Clean up
    cmsGBDFree(hGBD);
    
    return 0;
}