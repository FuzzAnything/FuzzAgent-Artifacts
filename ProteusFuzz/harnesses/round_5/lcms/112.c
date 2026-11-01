// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lcms2.h"

static void init_cmsCIELab(cmsCIELab* lab, double l, double a, double b) {
    if (!lab) return;
    lab->L = (cmsFloat64Number)l;
    lab->a = (cmsFloat64Number)a;
    lab->b = (cmsFloat64Number)b;
}

static void init_cmsCIEXYZ(cmsCIEXYZ* xyz, double x, double y, double z) {
    if (!xyz) return;
    xyz->X = (cmsFloat64Number)x;
    xyz->Y = (cmsFloat64Number)y;
    xyz->Z = (cmsFloat64Number)z;
}

static void init_cmsCIELCh(cmsCIELCh* lch, double l, double c, double h) {
    if (!lch) return;
    lch->L = (cmsFloat64Number)l;
    lch->C = (cmsFloat64Number)c;
    lch->h = (cmsFloat64Number)h;
}

static int read_uint16(const uint8_t* data, size_t size, size_t offset) {
    if (offset + 2 > size) return -1;
    return (data[offset] << 8) | data[offset + 1];
}

static double read_double(const uint8_t* data, size_t size, size_t offset) {
    if (offset + 8 > size) return 0.0;
    double val;
    memcpy(&val, data + offset, sizeof(double));
    return val;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    cmsCIELab lab_in, lab_out;
    cmsCIELCh lch_out;
    cmsCIEXYZ xyz_out, white_point;
    cmsUInt16Number wLab[3];
    cmsUInt16Number wLab_out[3];

    // Initialize structures with data from fuzz input
    int l_enc = read_uint16(Data, Size, 0);
    int a_enc = read_uint16(Data, Size, 2);
    int b_enc = read_uint16(Data, Size, 4);

    wLab[0] = (cmsUInt16Number)l_enc;
    wLab[1] = (cmsUInt16Number)a_enc;
    wLab[2] = (cmsUInt16Number)b_enc;

    // Test cmsLabEncoded2FloatV2
    cmsLabEncoded2FloatV2(&lab_out, wLab);

    // Test cmsLabEncoded2Float
    cmsLabEncoded2Float(&lab_in, wLab);

    // Test cmsFloat2LabEncoded
    cmsFloat2LabEncoded(wLab_out, &lab_in);

    // Test cmsFloat2LabEncodedV2
    cmsFloat2LabEncodedV2(wLab_out, &lab_in);

    // Test cmsLab2XYZ with custom white point
    if (Size >= 24) {
        double wx = read_double(Data, Size, 6);
        double wy = read_double(Data, Size, 14);
        double wz = read_double(Data, Size, 22);
        init_cmsCIEXYZ(&white_point, wx, wy, wz);
        cmsLab2XYZ(&white_point, &xyz_out, &lab_in);
    } else {
        // Use NULL for D50 default
        cmsLab2XYZ(NULL, &xyz_out, &lab_in);
    }

    // Test cmsLab2LCh
    cmsLab2LCh(&lch_out, &lab_in);

    // Additional state exploration: vary Lab values directly
    if (Size >= 32) {
        double l_val = read_double(Data, Size, 24);
        double a_val = read_double(Data, Size, 32);
        double b_val = read_double(Data, Size, 40);
        if (Size >= 48) {
            init_cmsCIELab(&lab_in, l_val, a_val, b_val);
            cmsLab2LCh(&lch_out, &lab_in);
            cmsLab2XYZ(NULL, &xyz_out, &lab_in);
            cmsFloat2LabEncoded(wLab_out, &lab_in);
            cmsFloat2LabEncodedV2(wLab_out, &lab_in);
        }
    }

    return 0;
}