// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsFloat64Number get_double_from_fuzz(const uint8_t *Data, size_t Size, size_t offset, size_t *new_offset) {
    if (offset + sizeof(cmsFloat64Number) > Size) {
        *new_offset = offset;
        return 1.0;
    }
    
    cmsFloat64Number val;
    memcpy(&val, Data + offset, sizeof(cmsFloat64Number));
    *new_offset = offset + sizeof(cmsFloat64Number);
    return val;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsCIELab lab1 = {0}, lab2 = {0};
    cmsCIEXYZ xyz = {0}, whitePoint = {0.95047, 1.0, 1.08883}; // D50 default
    cmsFloat64Number amax, amin, bmax, bmin, Kl, Kc, Kh;
    cmsHANDLE hGBD = NULL;
    cmsBool res;
    cmsFloat64Number delta;
    
    size_t offset = 0;
    
    // Initialize GBD
    // cmsCreateGBD is not available in standard lcms2 public API in many versions.
    // However, cmsGDBAddPoint and cmsGDBCheckPoint require a valid handle.
    // Since we cannot create one via public API, we skip GBD tests.
    // If a custom build exposes cmsCreateGBD, this logic would need adjustment.
    // For now, we set hGBD to NULL and skip those tests.
    
    // Test cmsDesaturateLab
    if (offset + sizeof(cmsCIELab) <= Size) {
        memcpy(&lab1, Data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
        
        amax = get_double_from_fuzz(Data, Size, offset, &offset);
        amin = get_double_from_fuzz(Data, Size, offset, &offset);
        bmax = get_double_from_fuzz(Data, Size, offset, &offset);
        bmin = get_double_from_fuzz(Data, Size, offset, &offset);
        
        res = cmsDesaturateLab(&lab1, amax, amin, bmax, bmin);
        (void)res;
    }
    
    // Test cmsXYZ2Lab
    if (offset + sizeof(cmsCIEXYZ) <= Size) {
        memcpy(&xyz, Data + offset, sizeof(cmsCIEXYZ));
        offset += sizeof(cmsCIEXYZ);
        
        cmsXYZ2Lab(&whitePoint, &lab1, &xyz);
    }
    
    // Skip cmsGDBAddPoint as we cannot create a valid hGBD
    // Skip cmsGDBCheckPoint as we cannot create a valid hGBD
    
    // Test cmsCIE94DeltaE
    if (offset + 2 * sizeof(cmsCIELab) <= Size) {
        memcpy(&lab1, Data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
        memcpy(&lab2, Data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
        
        delta = cmsCIE94DeltaE(&lab1, &lab2);
        (void)delta;
    }
    
    // Test cmsCIE2000DeltaE
    if (offset + 2 * sizeof(cmsCIELab) + 3 * sizeof(cmsFloat64Number) <= Size) {
        memcpy(&lab1, Data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
        memcpy(&lab2, Data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
        
        Kl = get_double_from_fuzz(Data, Size, offset, &offset);
        Kc = get_double_from_fuzz(Data, Size, offset, &offset);
        Kh = get_double_from_fuzz(Data, Size, offset, &offset);
        
        delta = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
        (void)delta;
    }
    
    // Cleanup
    // No GBD to delete
    
    return 0;
}