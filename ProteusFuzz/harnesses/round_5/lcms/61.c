// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
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

static void parse_float64(const uint8_t *data, size_t size, cmsFloat64Number *out) {
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(out, data, sizeof(cmsFloat64Number));
    } else {
        *out = 0.0;
    }
}

static void parse_lab(const uint8_t *data, size_t size, cmsCIELab *lab) {
    // Initialize to zero
    lab->L = 0.0;
    lab->a = 0.0;
    lab->b = 0.0;

    if (size >= sizeof(cmsFloat64Number)) {
        parse_float64(data, size, &lab->L);
    }
    if (size >= 2 * sizeof(cmsFloat64Number)) {
        parse_float64(data + sizeof(cmsFloat64Number), size - sizeof(cmsFloat64Number), &lab->a);
    }
    if (size >= 3 * sizeof(cmsFloat64Number)) {
        parse_float64(data + 2 * sizeof(cmsFloat64Number), size - 2 * sizeof(cmsFloat64Number), &lab->b);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    cmsCIELab Lab1, Lab2;
    cmsFloat64Number Kl, Kc, Kh, l, c;
    double amax, amin, bmax, bmin;
    cmsBool result;
    cmsFloat64Number delta;

    // Ensure we have enough data to parse at least two Lab structs (6 doubles)
    // If not, we can still test with zeros or minimal data, but robustness is key.
    if (Size == 0) {
        return 0;
    }

    // Parse Lab1
    parse_lab(Data, Size, &Lab1);

    // Parse Lab2 from the next available bytes, or reuse Data if short
    size_t offset1 = 3 * sizeof(cmsFloat64Number);
    if (Size > offset1) {
        parse_lab(Data + offset1, Size - offset1, &Lab2);
    } else {
        // Fallback: use Lab1 for Lab2 if data is insufficient
        Lab2 = Lab1;
    }

    // Parse weighting factors if available
    size_t offset2 = 6 * sizeof(cmsFloat64Number);
    if (Size > offset2) {
        parse_float64(Data + offset2, Size - offset2, &Kl);
        if (Size > offset2 + sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset2 + sizeof(cmsFloat64Number), Size - offset2 - sizeof(cmsFloat64Number), &Kc);
        } else {
            Kc = 1.0;
        }
        if (Size > offset2 + 2 * sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset2 + 2 * sizeof(cmsFloat64Number), Size - offset2 - 2 * sizeof(cmsFloat64Number), &Kh);
        } else {
            Kh = 1.0;
        }
        if (Size > offset2 + 3 * sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset2 + 3 * sizeof(cmsFloat64Number), Size - offset2 - 3 * sizeof(cmsFloat64Number), &l);
        } else {
            l = 2.0;
        }
        if (Size > offset2 + 4 * sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset2 + 4 * sizeof(cmsFloat64Number), Size - offset2 - 4 * sizeof(cmsFloat64Number), &c);
        } else {
            c = 1.0;
        }
    } else {
        Kl = 1.0;
        Kc = 1.0;
        Kh = 1.0;
        l = 2.0;
        c = 1.0;
    }

    // Parse gamut limits for DesaturateLab if available
    size_t offset3 = 10 * sizeof(cmsFloat64Number);
    if (Size > offset3) {
        parse_float64(Data + offset3, Size - offset3, &amax);
        if (Size > offset3 + sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset3 + sizeof(cmsFloat64Number), Size - offset3 - sizeof(cmsFloat64Number), &amin);
        } else {
            amin = -128.0;
        }
        if (Size > offset3 + 2 * sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset3 + 2 * sizeof(cmsFloat64Number), Size - offset3 - 2 * sizeof(cmsFloat64Number), &bmax);
        } else {
            bmax = 127.0;
        }
        if (Size > offset3 + 3 * sizeof(cmsFloat64Number)) {
            parse_float64(Data + offset3 + 3 * sizeof(cmsFloat64Number), Size - offset3 - 3 * sizeof(cmsFloat64Number), &bmin);
        } else {
            bmin = -128.0;
        }
    } else {
        amax = 127.0;
        amin = -128.0;
        bmax = 127.0;
        bmin = -128.0;
    }

    // 1. cmsBFDdeltaE
    delta = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)delta; // Suppress unused variable warning

    // 2. cmsCIE2000DeltaE
    delta = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)delta;

    // 3. cmsDesaturateLab
    // Make a copy to modify
    cmsCIELab LabCopy = Lab1;
    result = cmsDesaturateLab(&LabCopy, amax, amin, bmax, bmin);
    (void)result;

    // 4. cmsCIE94DeltaE
    delta = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)delta;

    // 5. cmsDeltaE
    delta = cmsDeltaE(&Lab1, &Lab2);
    (void)delta;

    // 6. cmsCMCdeltaE
    delta = cmsCMCdeltaE(&Lab1, &Lab2, l, c);
    (void)delta;

    return 0;
}