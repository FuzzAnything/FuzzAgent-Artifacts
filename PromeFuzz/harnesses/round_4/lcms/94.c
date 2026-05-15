// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 128) return 0;
    
    // Test cmsDesaturateLab
    {
        cmsCIELab Lab;
        // Use first 24 bytes for Lab values (3 doubles)
        if (Size >= 24) {
            memcpy(&Lab.L, Data, 8);
            memcpy(&Lab.a, Data + 8, 8);
            memcpy(&Lab.b, Data + 16, 8);
            
            // Use next 32 bytes for bounds (4 doubles)
            double amax, amin, bmax, bmin;
            if (Size >= 56) {
                memcpy(&amax, Data + 24, 8);
                memcpy(&amin, Data + 32, 8);
                memcpy(&bmax, Data + 40, 8);
                memcpy(&bmin, Data + 48, 8);
                
                cmsDesaturateLab(&Lab, amax, amin, bmax, bmin);
            }
        }
    }
    
    // Test GBD functions
    {
        // Allocate GBD
        cmsHANDLE hGBD = cmsGBDAlloc(NULL);
        if (hGBD) {
            // Add points to GBD
            size_t offset = 56; // Start after cmsDesaturateLab data
            for (int i = 0; i < 3 && offset + 24 <= Size; i++) {
                cmsCIELab point;
                memcpy(&point.L, Data + offset, 8);
                memcpy(&point.a, Data + offset + 8, 8);
                memcpy(&point.b, Data + offset + 16, 8);
                offset += 24;
                
                cmsGDBAddPoint(hGBD, &point);
            }
            
            // Compute GDB
            cmsUInt32Number flags = 0;
            if (offset + 4 <= Size) {
                memcpy(&flags, Data + offset, 4);
                offset += 4;
            }
            cmsGDBCompute(hGBD, flags);
            
            // Check a point
            if (offset + 24 <= Size) {
                cmsCIELab checkPoint;
                memcpy(&checkPoint.L, Data + offset, 8);
                memcpy(&checkPoint.a, Data + offset + 8, 8);
                memcpy(&checkPoint.b, Data + offset + 16, 8);
                
                cmsGDBCheckPoint(hGBD, &checkPoint);
            }
            
            // Free GBD
            cmsGBDFree(hGBD);
        }
    }
    
    return 0;
}