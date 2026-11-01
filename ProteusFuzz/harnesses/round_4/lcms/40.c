// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
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

static cmsCIELab* create_lab_from_data(const uint8_t *Data, size_t Size, int index) {
    cmsCIELab *lab = (cmsCIELab*)malloc(sizeof(cmsCIELab));
    if (!lab) return NULL;

    if (Size < 24) {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
        return lab;
    }

    // Extract 8 bytes per component (L, a, b) from the input data
    // Using a simple deterministic extraction based on index and data
    int offset = index * 24;
    if (offset + 24 > Size) {
        offset = 0;
    }

    // Interpret raw bytes as doubles for fuzzing variety
    memcpy(&lab->L, &Data[offset], sizeof(double));
    memcpy(&lab->a, &Data[offset + 8], sizeof(double));
    memcpy(&lab->b, &Data[offset + 16], sizeof(double));

    return lab;
}

static cmsFloat64Number extract_double(const uint8_t *Data, size_t Size, int offset) {
    if (Size < offset + sizeof(double)) {
        return 0.0;
    }
    cmsFloat64Number val;
    memcpy(&val, &Data[offset], sizeof(double));
    return val;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize LCMS if necessary (though these specific functions don't strictly require a profile context)
    // cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create two Lab structures from the input data
    cmsCIELab *Lab1 = create_lab_from_data(Data, Size, 0);
    cmsCIELab *Lab2 = create_lab_from_data(Data, Size, 1);

    if (!Lab1 || !Lab2) {
        free(Lab1);
        free(Lab2);
        return 0;
    }

    // 1. Test cmsDesaturateLab
    // Extract limits from data or use defaults if not enough data
    double amax = extract_double(Data, Size, 48);
    double amin = extract_double(Data, Size, 56);
    double bmax = extract_double(Data, Size, 64);
    double bmin = extract_double(Data, Size, 72);

    // Ensure amin <= amax and bmin <= bmax for valid logic, but fuzzing should test invalid too
    cmsBool res1 = cmsDesaturateLab(Lab1, amax, amin, bmax, bmin);
    (void)res1; // Use result to suppress warnings

    // 2. Test cmsDeltaE
    cmsFloat64Number deltaE = cmsDeltaE(Lab1, Lab2);
    (void)deltaE;

    // 3. Test cmsCMCdeltaE
    cmsFloat64Number l_cmc = extract_double(Data, Size, 80);
    cmsFloat64Number c_cmc = extract_double(Data, Size, 88);
    cmsFloat64Number deltaE_cmc = cmsCMCdeltaE(Lab1, Lab2, l_cmc, c_cmc);
    (void)deltaE_cmc;

    // 4. Test cmsCIE94DeltaE
    cmsFloat64Number deltaE_94 = cmsCIE94DeltaE(Lab1, Lab2);
    (void)deltaE_94;

    // 5. Test cmsCIE2000DeltaE
    cmsFloat64Number Kl = extract_double(Data, Size, 96);
    cmsFloat64Number Kc = extract_double(Data, Size, 104);
    cmsFloat64Number Kh = extract_double(Data, Size, 112);
    cmsFloat64Number deltaE_2000 = cmsCIE2000DeltaE(Lab1, Lab2, Kl, Kc, Kh);
    (void)deltaE_2000;

    // 6. Test cmsBFDdeltaE
    cmsFloat64Number deltaE_bfd = cmsBFDdeltaE(Lab1, Lab2);
    (void)deltaE_bfd;

    // Cleanup
    free(Lab1);
    free(Lab2);

    return 0;
}