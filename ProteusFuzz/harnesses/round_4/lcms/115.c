// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <lcms2.h>

/* 
 * Fuzz driver for lcms color conversion and DeltaE functions.
 * Target APIs: cmsLCh2Lab, cmsLab2LCh, cmsDeltaE, cmsCMCdeltaE, cmsCIE94DeltaE, cmsBFDdeltaE
 */

static void fuzz_cmsLCh2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELCh)) return;
    
    cmsCIELCh LCh;
    cmsCIELab Lab;
    
    /* Copy input data to LCh struct, ensuring valid memory access */
    memcpy(&LCh, Data, sizeof(cmsCIELCh));
    
    /* Initialize Lab to avoid undefined behavior with uninitialized memory */
    memset(&Lab, 0, sizeof(cmsCIELab));
    
    cmsLCh2Lab(&Lab, &LCh);
}

static void fuzz_cmsLab2LCh(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab;
    cmsCIELCh LCh;
    
    /* Copy input data to Lab struct */
    memcpy(&Lab, Data, sizeof(cmsCIELab));
    
    /* Initialize LCh to avoid undefined behavior */
    memset(&LCh, 0, sizeof(cmsCIELCh));
    
    cmsLab2LCh(&LCh, &Lab);
}

static void fuzz_cmsDeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab1, Lab2;
    
    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsDeltaE(&Lab1, &Lab2);
    (void)result; /* Suppress unused variable warning */
}

static void fuzz_cmsCMCdeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number)) return;
    
    cmsCIELab Lab1, Lab2;
    cmsFloat64Number l, c;
    
    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    memcpy(&l, Data + 2 * sizeof(cmsCIELab), sizeof(cmsFloat64Number));
    memcpy(&c, Data + 2 * sizeof(cmsCIELab) + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    
    cmsFloat64Number result = cmsCMCdeltaE(&Lab1, &Lab2, l, c);
    (void)result;
}

static void fuzz_cmsCIE94DeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab1, Lab2;
    
    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)result;
}

static void fuzz_cmsBFDdeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab)) return;
    
    cmsCIELab Lab1, Lab2;
    
    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)result;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    /* 
     * We divide the input buffer into segments to fuzz multiple functions.
     * Each function gets a portion of the data based on its requirements.
     */
    
    size_t total_needed = 0;
    
    /* Estimate minimum size needed for all functions */
    total_needed += sizeof(cmsCIELCh); /* For cmsLCh2Lab */
    total_needed += sizeof(cmsCIELab); /* For cmsLab2LCh */
    total_needed += 2 * sizeof(cmsCIELab); /* For cmsDeltaE */
    total_needed += 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number); /* For cmsCMCdeltaE */
    total_needed += 2 * sizeof(cmsCIELab); /* For cmsCIE94DeltaE */
    total_needed += 2 * sizeof(cmsCIELab); /* For cmsBFDdeltaE */
    
    /* If input is too small, we can still fuzz individual functions with partial data */
    
    /* Fuzz cmsLCh2Lab */
    if (Size >= sizeof(cmsCIELCh)) {
        fuzz_cmsLCh2Lab(Data, Size);
    }
    
    /* Fuzz cmsLab2LCh */
    if (Size >= sizeof(cmsCIELab)) {
        fuzz_cmsLab2LCh(Data, Size);
    }
    
    /* Fuzz cmsDeltaE */
    if (Size >= 2 * sizeof(cmsCIELab)) {
        fuzz_cmsDeltaE(Data, Size);
    }
    
    /* Fuzz cmsCMCdeltaE */
    if (Size >= 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number)) {
        fuzz_cmsCMCdeltaE(Data, Size);
    }
    
    /* Fuzz cmsCIE94DeltaE */
    if (Size >= 2 * sizeof(cmsCIELab)) {
        fuzz_cmsCIE94DeltaE(Data, Size);
    }
    
    /* Fuzz cmsBFDdeltaE */
    if (Size >= 2 * sizeof(cmsCIELab)) {
        fuzz_cmsBFDdeltaE(Data, Size);
    }
    
    /* Additional exploration: try different offsets if data is large enough */
    if (Size > total_needed) {
        size_t offset = Size / 2;
        if (offset + sizeof(cmsCIELCh) <= Size) {
            fuzz_cmsLCh2Lab(Data + offset, Size - offset);
        }
        if (offset + sizeof(cmsCIELab) <= Size) {
            fuzz_cmsLab2LCh(Data + offset, Size - offset);
        }
        if (offset + 2 * sizeof(cmsCIELab) <= Size) {
            fuzz_cmsDeltaE(Data + offset, Size - offset);
            fuzz_cmsCIE94DeltaE(Data + offset, Size - offset);
            fuzz_cmsBFDdeltaE(Data + offset, Size - offset);
        }
        if (offset + 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number) <= Size) {
            fuzz_cmsCMCdeltaE(Data + offset, Size - offset);
        }
    }
    
    return 0;
}