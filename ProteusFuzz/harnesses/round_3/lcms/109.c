// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static void init_xyz_from_data(const uint8_t *data, size_t size, cmsCIEXYZ *xyz) {
    xyz->X = 0.0;
    xyz->Y = 0.0;
    xyz->Z = 0.0;
    
    if (size >= 24) {
        xyz->X = *(const double *)(data);
        xyz->Y = *(const double *)(data + 8);
        xyz->Z = *(const double *)(data + 16);
    } else if (size >= 3) {
        xyz->X = (double)data[0] / 255.0;
        xyz->Y = (double)data[1] / 255.0;
        xyz->Z = (double)data[2] / 255.0;
    }
}

static void init_lab_from_data(const uint8_t *data, size_t size, cmsCIELab *lab) {
    lab->L = 0.0;
    lab->a = 0.0;
    lab->b = 0.0;
    
    if (size >= 24) {
        lab->L = *(const double *)(data);
        lab->a = *(const double *)(data + 8);
        lab->b = *(const double *)(data + 16);
    } else if (size >= 3) {
        lab->L = (double)data[0] / 255.0 * 100.0;
        lab->a = (double)data[1] / 255.0 * 200.0 - 100.0;
        lab->b = (double)data[2] / 255.0 * 200.0 - 100.0;
    }
}

static void init_white_point_from_data(const uint8_t *data, size_t size, cmsCIEXYZ *wp) {
    wp->X = 0.9642;
    wp->Y = 1.0000;
    wp->Z = 0.8251;
    
    if (size >= 24) {
        wp->X = *(const double *)(data);
        wp->Y = *(const double *)(data + 8);
        wp->Z = *(const double *)(data + 16);
    } else if (size >= 3) {
        wp->X = (double)data[0] / 255.0;
        wp->Y = (double)data[1] / 255.0;
        wp->Z = (double)data[2] / 255.0;
    }
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;
    cmsCIELab lab;
    cmsCIELab lab2;
    cmsFloat64Number Kl = 1.0, Kc = 1.0, Kh = 1.0;
    double amax, amin, bmax, bmin;
    
    // Initialize data structures from input
    init_xyz_from_data(Data, Size, &xyz);
    init_white_point_from_data(Data, Size, &whitePoint);
    init_lab_from_data(Data, Size, &lab);
    
    // Create a second Lab structure for delta E calculations
    init_lab_from_data(Data, Size, &lab2);
    
    // Test cmsXYZ2Lab
    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
    cmsXYZ2Lab(NULL, &lab, &xyz);
    
    // Test cmsDesaturateLab with various bounds
    amax = (Size > 0) ? (double)Data[0] / 255.0 * 200.0 - 100.0 : 0.0;
    amin = (Size > 1) ? (double)Data[1] / 255.0 * 200.0 - 100.0 : 0.0;
    bmax = (Size > 2) ? (double)Data[2] / 255.0 * 200.0 - 100.0 : 0.0;
    bmin = (Size > 3) ? (double)Data[3] / 255.0 * 200.0 - 100.0 : 0.0;
    
    cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
    cmsDesaturateLab(&lab2, amax, amin, bmax, bmin);
    
    // Test cmsCIE2000DeltaE
    Kl = (Size > 4) ? (double)Data[4] / 255.0 * 2.0 : 1.0;
    Kc = (Size > 5) ? (double)Data[5] / 255.0 * 2.0 : 1.0;
    Kh = (Size > 6) ? (double)Data[6] / 255.0 * 2.0 : 1.0;
    
    cmsCIE2000DeltaE(&lab, &lab2, Kl, Kc, Kh);
    
    // Test cmsDeltaE
    cmsDeltaE(&lab, &lab2);
    
    return 0;
}