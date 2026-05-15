// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsCIELab* parseLabFromData(const uint8_t** data, size_t* size) {
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

static double parseDoubleFromData(const uint8_t** data, size_t* size) {
    if (*size < sizeof(double)) {
        return 0.0;
    }
    
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    
    return value;
}

static cmsUInt32Number parseUInt32FromData(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) {
        return 0;
    }
    
    cmsUInt32Number value;
    memcpy(&value, *data, sizeof(cmsUInt32Number));
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    
    return value;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }
    
    const uint8_t* data_ptr = Data;
    size_t remaining_size = Size;
    
    // Parse initial Lab point
    cmsCIELab* lab1 = parseLabFromData(&data_ptr, &remaining_size);
    if (!lab1) {
        return 0;
    }
    
    // Test cmsDesaturateLab
    if (remaining_size >= sizeof(double) * 4) {
        double amax = parseDoubleFromData(&data_ptr, &remaining_size);
        double amin = parseDoubleFromData(&data_ptr, &remaining_size);
        double bmax = parseDoubleFromData(&data_ptr, &remaining_size);
        double bmin = parseDoubleFromData(&data_ptr, &remaining_size);
        
        cmsCIELab lab_copy = *lab1;
        cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);
    }
    
    // Test GBD functions
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);
    if (hGBD) {
        // Test cmsGDBCheckPoint
        cmsGDBCheckPoint(hGBD, lab1);
        
        // Test cmsGDBAddPoint with initial point
        cmsGDBAddPoint(hGBD, lab1);
        
        // Add more points from remaining data
        while (remaining_size >= sizeof(double) * 3) {
            cmsCIELab* lab_point = parseLabFromData(&data_ptr, &remaining_size);
            if (lab_point) {
                cmsGDBAddPoint(hGBD, lab_point);
                cmsGDBCheckPoint(hGBD, lab_point);
                free(lab_point);
            }
        }
        
        // Test cmsGDBCompute with flags from data
        if (remaining_size >= sizeof(cmsUInt32Number)) {
            cmsUInt32Number flags = parseUInt32FromData(&data_ptr, &remaining_size);
            cmsGDBCompute(hGBD, flags);
        } else {
            // Try with some default flags
            cmsGDBCompute(hGBD, 0);
        }
        
        // Clean up
        cmsGBDFree(hGBD);
    }
    
    free(lab1);
    return 0;
}