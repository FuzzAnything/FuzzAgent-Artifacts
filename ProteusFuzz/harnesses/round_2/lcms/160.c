// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static cmsCIELab* generate_lab_from_data(const uint8_t *Data, size_t Size, int index) {
    if (Size < 24) return NULL;
    
    cmsCIELab *lab = (cmsCIELab*)malloc(sizeof(cmsCIELab));
    if (!lab) return NULL;
    
    size_t offset = index * 24;
    if (offset + 24 > Size) {
        free(lab);
        return NULL;
    }
    
    memcpy(&lab->L, Data + offset, 8);
    memcpy(&lab->a, Data + offset + 8, 8);
    memcpy(&lab->b, Data + offset + 16, 8);
    
    if (lab->L < 0.0) lab->L = 0.0;
    if (lab->L > 100.0) lab->L = 100.0;
    
    return lab;
}

static cmsCIEXYZ* generate_xyz_from_data(const uint8_t *Data, size_t Size, int index) {
    if (Size < 24) return NULL;
    
    cmsCIEXYZ *xyz = (cmsCIEXYZ*)malloc(sizeof(cmsCIEXYZ));
    if (!xyz) return NULL;
    
    size_t offset = index * 24;
    if (offset + 24 > Size) {
        free(xyz);
        return NULL;
    }
    
    memcpy(&xyz->X, Data + offset, 8);
    memcpy(&xyz->Y, Data + offset + 8, 8);
    memcpy(&xyz->Z, Data + offset + 16, 8);
    
    if (xyz->X < 0.0) xyz->X = 0.0;
    if (xyz->Y < 0.0) xyz->Y = 0.0;
    if (xyz->Z < 0.0) xyz->Z = 0.0;
    
    return xyz;
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 48) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsCIELab *Lab1 = generate_lab_from_data(Data, Size, 0);
    cmsCIELab *Lab2 = generate_lab_from_data(Data, Size, 1);
    cmsCIELab *Lab3 = generate_lab_from_data(Data, Size, 2);
    
    cmsCIEXYZ *XYZ1 = generate_xyz_from_data(Data, Size, 3);
    cmsCIEXYZ *WhitePoint = generate_xyz_from_data(Data, Size, 4);
    
    // Test 1: cmsCIE2000DeltaE
    if (Lab1 && Lab2) {
        cmsFloat64Number Kl = 1.0;
        cmsFloat64Number Kc = 1.0;
        cmsFloat64Number Kh = 1.0;
        
        // Ensure we have enough data for 3 doubles (24 bytes) starting at offset 48
        if (Size >= 48 + 24) {
            memcpy(&Kl, Data + 48, 8);
            memcpy(&Kc, Data + 56, 8);
            memcpy(&Kh, Data + 64, 8);
        }
        
        cmsFloat64Number deltaE = cmsCIE2000DeltaE(Lab1, Lab2, Kl, Kc, Kh);
        (void)deltaE;
    }
    
    // Test 3: cmsDesaturateLab
    if (Lab2) {
        double amax = 100.0, amin = -100.0;
        double bmax = 100.0, bmin = -100.0;
        
        // Ensure we have enough data for 4 doubles (32 bytes) starting at offset 72
        if (Size >= 72 + 32) {
            memcpy(&amax, Data + 72, 8);
            memcpy(&amin, Data + 80, 8);
            memcpy(&bmax, Data + 88, 8);
            memcpy(&bmin, Data + 96, 8);
        }
        
        cmsBool result = cmsDesaturateLab(Lab2, amax, amin, bmax, bmin);
        (void)result;
    }
    
    // Test 4: cmsBFDdeltaE
    if (Lab1 && Lab2) {
        cmsFloat64Number bfdDeltaE = cmsBFDdeltaE(Lab1, Lab2);
        (void)bfdDeltaE;
    }
    
    // Test 6: cmsXYZ2Lab
    if (XYZ1) {
        cmsCIELab LabOut;
        cmsXYZ2Lab(WhitePoint ? WhitePoint : NULL, &LabOut, XYZ1);
        (void)LabOut;
    }
    
    // Cleanup
    cmsDeleteContext(ctx);
    
    if (Lab1) free(Lab1);
    if (Lab2) free(Lab2);
    if (Lab3) free(Lab3);
    if (XYZ1) free(XYZ1);
    if (WhitePoint) free(WhitePoint);
    
    return 0;
}