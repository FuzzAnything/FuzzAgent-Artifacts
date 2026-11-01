// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
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
#include <lcms2.h>

static void generate_random_lab(cmsCIELab *lab, const uint8_t *data, size_t offset) {
    if (offset + 24 > sizeof(cmsCIELab) * 2) {
        // Fallback to random values if data is insufficient
        lab->L = (double)(rand() % 200) - 100.0; // Allow negative L for edge cases
        lab->a = (double)(rand() % 200) - 100.0;
        lab->b = (double)(rand() % 200) - 100.0;
        return;
    }

    // Read 8 bytes per component (double) from input data
    memcpy(&lab->L, data + offset, sizeof(double));
    memcpy(&lab->a, data + offset + 8, sizeof(double));
    memcpy(&lab->b, data + offset + 16, sizeof(double));
}

static void generate_random_double(cmsFloat64Number *val, const uint8_t *data, size_t offset) {
    if (offset + 8 > sizeof(cmsFloat64Number)) {
        *val = (double)(rand() % 100) / 10.0;
        return;
    }
    memcpy(val, data + offset, sizeof(cmsFloat64Number));
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 48) {
        return 0;
    }

    cmsCIELab Lab1, Lab2;
    cmsFloat64Number Kl, Kc, Kh, l, c;
    cmsFloat64Number amax, amin, bmax, bmin;

    // Initialize Lab1
    generate_random_lab(&Lab1, Data, 0);

    // Initialize Lab2
    generate_random_lab(&Lab2, Data, 24);

    // Initialize weighting factors and limits from data
    generate_random_double(&Kl, Data, 48);
    generate_random_double(&Kc, Data, 56);
    generate_random_double(&Kh, Data, 64);
    generate_random_double(&l, Data, 72);
    generate_random_double(&c, Data, 80);
    generate_random_double(&amax, Data, 88);
    generate_random_double(&amin, Data, 96);
    generate_random_double(&bmax, Data, 104);
    generate_random_double(&bmin, Data, 112);

    // Ensure amax >= amin and bmax >= bmin for valid range checks if needed, 
    // but fuzzing should test invalid ranges too.

    // 1. Test cmsCIE94DeltaE
    cmsFloat64Number res1 = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)res1; // Use result to avoid unused variable warning

    // 2. Test cmsCIE2000DeltaE
    cmsFloat64Number res2 = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)res2;

    // 3. Test cmsCMCdeltaE
    cmsFloat64Number res3 = cmsCMCdeltaE(&Lab1, &Lab2, l, c);
    (void)res3;

    // 4. Test cmsDesaturateLab
    cmsCIELab LabCopy = Lab1;
    cmsBool res4 = cmsDesaturateLab(&LabCopy, amax, amin, bmax, bmin);
    (void)res4;

    // 5. Test cmsDeltaE
    cmsFloat64Number res5 = cmsDeltaE(&Lab1, &Lab2);
    (void)res5;

    // 6. Test cmsBFDdeltaE
    cmsFloat64Number res6 = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)res6;

    return 0;
}