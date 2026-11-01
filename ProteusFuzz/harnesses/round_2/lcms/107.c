// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_lab_to_xyz(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;

    cmsCIEXYZ WhitePoint;
    cmsCIEXYZ xyz;
    cmsCIELab Lab;

    // Initialize structures with data from input or defaults
    if (Size >= 12) {
        memcpy(&WhitePoint, Data, sizeof(cmsCIEXYZ));
        memcpy(&xyz, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
        memcpy(&Lab, Data + 2 * sizeof(cmsCIEXYZ), sizeof(cmsCIELab));
    } else {
        memset(&WhitePoint, 0, sizeof(WhitePoint));
        memset(&xyz, 0, sizeof(xyz));
        memset(&Lab, 0, sizeof(Lab));
    }

    // Test with provided white point
    cmsLab2XYZ(&WhitePoint, &xyz, &Lab);

    // Test with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &xyz, &Lab);
}

static void fuzz_lab_encoded_float(const uint8_t *Data, size_t Size) {
    if (Size < 6) return;

    cmsCIELab Lab;
    cmsUInt16Number wLab[3];

    // Initialize wLab from input
    if (Size >= 6) {
        memcpy(wLab, Data, sizeof(wLab));
    } else {
        memset(wLab, 0, sizeof(wLab));
    }

    memset(&Lab, 0, sizeof(Lab));

    // Test V1
    cmsLabEncoded2Float(&Lab, wLab);

    // Test V2
    cmsLabEncoded2FloatV2(&Lab, wLab);
}

static void fuzz_float_lab_encoded(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;

    cmsCIELab Lab;
    cmsUInt16Number wLab[3];

    // Initialize Lab from input
    if (Size >= 12) {
        memcpy(&Lab, Data, sizeof(cmsCIELab));
    } else {
        memset(&Lab, 0, sizeof(Lab));
    }

    memset(wLab, 0, sizeof(wLab));

    // Test V1
    cmsFloat2LabEncoded(wLab, &Lab);

    // Test V2
    cmsFloat2LabEncodedV2(wLab, &Lab);
}

static void fuzz_lab_to_lch(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;

    cmsCIELCh LCh;
    cmsCIELab Lab;

    // Initialize Lab from input
    if (Size >= 12) {
        memcpy(&Lab, Data, sizeof(cmsCIELab));
    } else {
        memset(&Lab, 0, sizeof(Lab));
    }

    memset(&LCh, 0, sizeof(LCh));

    cmsLab2LCh(&LCh, &Lab);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run all fuzzing scenarios
    fuzz_lab_to_xyz(Data, Size);
    fuzz_lab_encoded_float(Data, Size);
    fuzz_float_lab_encoded(Data, Size);
    fuzz_lab_to_lch(Data, Size);

    return 0;
}