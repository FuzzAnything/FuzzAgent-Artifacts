// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_lab_values(cmsCIELab *lab, const uint8_t *data, size_t offset, size_t size) {
    if (size < offset + 12) {
        // Fallback to random-like values if data is insufficient
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
        return;
    }

    // Read 4 bytes for each component, interpreting as float then converting to double
    // Using a simple method to extract floats from raw bytes to ensure variety
    float f_val;
    memcpy(&f_val, data + offset, sizeof(float));
    lab->L = (cmsFloat64Number)f_val;
    
    if (size >= offset + 8) {
        memcpy(&f_val, data + offset + 4, sizeof(float));
        lab->a = (cmsFloat64Number)f_val;
    } else {
        lab->a = 0.0;
    }

    if (size >= offset + 12) {
        memcpy(&f_val, data + offset + 8, sizeof(float));
        lab->b = (cmsFloat64Number)f_val;
    } else {
        lab->b = 0.0;
    }
}

static cmsFloat64Number fuzz_float64(const uint8_t *data, size_t offset, size_t size) {
    if (size < offset + 8) return 1.0;
    cmsFloat64Number val;
    memcpy(&val, data + offset, sizeof(cmsFloat64Number));
    return val;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    cmsCIELab Lab1, Lab2;
    cmsCIELab LabMutable;
    
    // Initialize Lab1 and Lab2 from input data
    fuzz_lab_values(&Lab1, Data, 0, Size);
    fuzz_lab_values(&Lab2, Data, 12, Size);
    
    // Copy Lab1 to mutable for desaturation test
    LabMutable = Lab1;

    // 1. Test cmsCMCdeltaE
    // Extract l and c parameters
    cmsFloat64Number l_ratio = 1.0;
    cmsFloat64Number c_ratio = 1.0;
    if (Size >= 24) {
        l_ratio = fuzz_float64(Data, 24, Size);
        if (Size >= 32) {
            c_ratio = fuzz_float64(Data, 32, Size);
        }
    }
    cmsFloat64Number resCMC = cmsCMCdeltaE(&Lab1, &Lab2, l_ratio, c_ratio);
    (void)resCMC;

    // 2. Test cmsCIE2000DeltaE
    cmsFloat64Number Kl = 1.0, Kc = 1.0, Kh = 1.0;
    if (Size >= 40) Kl = fuzz_float64(Data, 40, Size);
    if (Size >= 48) Kc = fuzz_float64(Data, 48, Size);
    if (Size >= 56) Kh = fuzz_float64(Data, 56, Size);
    cmsFloat64Number resCIE2000 = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)resCIE2000;

    // 3. Test cmsBFDdeltaE
    cmsFloat64Number resBFD = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)resBFD;

    // 4. Test cmsDesaturateLab
    double amax = 100.0, amin = -100.0, bmax = 100.0, bmin = -100.0;
    if (Size >= 64) amax = (double)fuzz_float64(Data, 64, Size);
    if (Size >= 72) amin = (double)fuzz_float64(Data, 72, Size);
    if (Size >= 80) bmax = (double)fuzz_float64(Data, 80, Size);
    if (Size >= 88) bmin = (double)fuzz_float64(Data, 88, Size);
    
    cmsBool resDesat = cmsDesaturateLab(&LabMutable, amax, amin, bmax, bmin);
    (void)resDesat;

    // 5. Test cmsCIE94DeltaE
    cmsFloat64Number resCIE94 = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)resCIE94;

    // 6. Test cmsDeltaE
    cmsFloat64Number resDeltaE = cmsDeltaE(&Lab1, &Lab2);
    (void)resDeltaE;

    return 0;
}