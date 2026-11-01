// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

/* 
 * Fuzz driver for lcms color difference and conversion functions.
 * Target APIs: cmsCMCdeltaE, cmsLab2LCh, cmsLCh2Lab, cmsBFDdeltaE, cmsDeltaE, cmsCIE94DeltaE
 */

static void fuzz_cmsCMCdeltaE(const uint8_t *Data, size_t Size) {
    /* 
     * Required size: 2 * sizeof(cmsCIELab) for Lab1 and Lab2 + 2 * sizeof(cmsFloat64Number) for l and c.
     * cmsCIELab typically has 3 doubles (L, a, b). cmsFloat64Number is double.
     * Total: 3*8 + 3*8 + 8 + 8 = 64 bytes.
     */
    size_t required_size = 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number);
    if (Size < required_size) return;

    cmsCIELab Lab1, Lab2;
    cmsFloat64Number l, c;

    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    memcpy(&l, Data + 2 * sizeof(cmsCIELab), sizeof(cmsFloat64Number));
    memcpy(&c, Data + 2 * sizeof(cmsCIELab) + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));

    cmsFloat64Number result = cmsCMCdeltaE(&Lab1, &Lab2, l, c);
    (void)result;
}

static void fuzz_cmsLab2LCh(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;

    cmsCIELab Lab;
    cmsCIELCh LCh;

    memcpy(&Lab, Data, sizeof(cmsCIELab));
    cmsLab2LCh(&LCh, &Lab);
}

static void fuzz_cmsLCh2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELCh)) return;

    cmsCIELCh LCh;
    cmsCIELab Lab;

    memcpy(&LCh, Data, sizeof(cmsCIELCh));
    cmsLCh2Lab(&Lab, &LCh);
}

static void fuzz_cmsBFDdeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab)) return;

    cmsCIELab Lab1, Lab2;

    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));

    cmsFloat64Number result = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)result;
}

static void fuzz_cmsDeltaE(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(cmsCIELab)) return;

    cmsCIELab Lab1, Lab2;

    memcpy(&Lab1, Data, sizeof(cmsCIELab));
    memcpy(&Lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));

    cmsFloat64Number result = cmsDeltaE(&Lab1, &Lab2);
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

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 
     * Use the first byte to select which function to fuzz.
     * This ensures diverse coverage across all target APIs.
     */
    uint8_t selector = Data[0];
    const uint8_t *payload = Data + 1;
    size_t payloadSize = Size - 1;

    switch (selector % 6) {
        case 0:
            fuzz_cmsCMCdeltaE(payload, payloadSize);
            break;
        case 1:
            fuzz_cmsLab2LCh(payload, payloadSize);
            break;
        case 2:
            fuzz_cmsLCh2Lab(payload, payloadSize);
            break;
        case 3:
            fuzz_cmsBFDdeltaE(payload, payloadSize);
            break;
        case 4:
            fuzz_cmsDeltaE(payload, payloadSize);
            break;
        case 5:
            fuzz_cmsCIE94DeltaE(payload, payloadSize);
            break;
    }

    return 0;
}