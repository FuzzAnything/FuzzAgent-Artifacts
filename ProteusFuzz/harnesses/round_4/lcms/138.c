// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void fuzz_cmsDetectDestinationBlackPoint(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;

    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    if (!hProfile) return;

    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};
    cmsUInt32Number Intent = Data[0];
    cmsUInt32Number dwFlags = Data[1] | (Data[2] << 8) | (Data[3] << 16);

    cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, Intent, dwFlags);

    cmsCloseProfile(hProfile);
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < 24) return;

    cmsCIEXYZ WhitePoint = {0.0, 0.0, 0.0};
    cmsCIEXYZ xyz = {0.0, 0.0, 0.0};
    cmsCIELab Lab = {0.0, 0.0, 0.0};

    // Parse L, a, b from input (assuming double precision for fuzzing variety)
    // We use a safe cast or direct memory copy if alignment allows, 
    // but for simplicity in fuzzing, we construct from bytes if needed or use doubles.
    // Here we assume the input has enough bytes for 3 doubles (24 bytes).
    Lab.L = *(const double*)(Data);
    Lab.a = *(const double*)(Data + 8);
    Lab.b = *(const double*)(Data + 16);

    // Optionally use custom white point if we have more data, otherwise NULL
    const cmsCIEXYZ *wp = NULL;
    if (Size >= 48) {
        WhitePoint.X = *(const double*)(Data + 24);
        WhitePoint.Y = *(const double*)(Data + 32);
        WhitePoint.Z = *(const double*)(Data + 40);
        wp = &WhitePoint;
    }

    cmsLab2XYZ(wp, &xyz, &Lab);
}

static void fuzz_cmsD50_XYZ(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        // Just access to ensure it's valid and used
        double dummy = d50->X + d50->Y + d50->Z;
        (void)dummy;
    }
}

static void fuzz_cmsxyY2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < 24) return;

    cmsCIExyY Source = {0.0, 0.0, 0.0};
    cmsCIEXYZ Dest = {0.0, 0.0, 0.0};

    Source.x = *(const double*)(Data);
    Source.y = *(const double*)(Data + 8);
    Source.Y = *(const double*)(Data + 16);

    cmsxyY2XYZ(&Dest, &Source);
}

static void fuzz_cmsXYZEncoded2Float(const uint8_t *Data, size_t Size) {
    if (Size < 6) return;

    cmsUInt16Number XYZ[3];
    cmsCIEXYZ fxyz = {0.0, 0.0, 0.0};

    XYZ[0] = Data[0] | (Data[1] << 8);
    XYZ[1] = Data[2] | (Data[3] << 8);
    XYZ[2] = Data[4] | (Data[5] << 8);

    cmsXYZEncoded2Float(&fxyz, XYZ);
}

static void fuzz_cmsFloat2XYZEncoded(const uint8_t *Data, size_t Size) {
    if (Size < 24) return;

    cmsCIEXYZ fXYZ = {0.0, 0.0, 0.0};
    cmsUInt16Number XYZ[3];

    fXYZ.X = *(const double*)(Data);
    fXYZ.Y = *(const double*)(Data + 8);
    fXYZ.Z = *(const double*)(Data + 16);

    cmsFloat2XYZEncoded(XYZ, &fXYZ);
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each target function
    fuzz_cmsDetectDestinationBlackPoint(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    fuzz_cmsD50_XYZ(Data, Size);
    fuzz_cmsxyY2XYZ(Data, Size);
    fuzz_cmsXYZEncoded2Float(Data, Size);
    fuzz_cmsFloat2XYZEncoded(Data, Size);

    return 0;
}