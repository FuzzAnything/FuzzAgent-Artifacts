// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void parse_float64(const uint8_t *data, size_t offset, size_t size, cmsFloat64Number *val) {
    if (offset + sizeof(cmsFloat64Number) > size) {
        *val = 0.0;
        return;
    }
    memcpy(val, data + offset, sizeof(cmsFloat64Number));
}

static cmsCIELab parse_lab(const uint8_t *data, size_t offset, size_t size) {
    cmsCIELab lab = {0.0, 0.0, 0.0};
    if (offset + sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset, size, &lab.L);
    }
    if (offset + 2 * sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset + sizeof(cmsFloat64Number), size, &lab.a);
    }
    if (offset + 3 * sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset + 2 * sizeof(cmsFloat64Number), size, &lab.b);
    }
    return lab;
}

static cmsCIELCh parse_lch(const uint8_t *data, size_t offset, size_t size) {
    cmsCIELCh lch = {0.0, 0.0, 0.0};
    if (offset + sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset, size, &lch.L);
    }
    if (offset + 2 * sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset + sizeof(cmsFloat64Number), size, &lch.C);
    }
    if (offset + 3 * sizeof(cmsFloat64Number) <= size) {
        parse_float64(data, offset + 2 * sizeof(cmsFloat64Number), size, &lch.h);
    }
    return lch;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 6 * sizeof(cmsFloat64Number)) {
        return 0;
    }

    cmsCIELab lab1 = parse_lab(Data, 0, Size);
    cmsCIELab lab2 = parse_lab(Data, 3 * sizeof(cmsFloat64Number), Size);
    cmsCIELCh lch1 = parse_lch(Data, 6 * sizeof(cmsFloat64Number), Size);
    cmsCIELCh lch2 = parse_lch(Data, 9 * sizeof(cmsFloat64Number), Size);
    cmsCIELab lab_out1, lab_out2;
    cmsCIELCh lch_out1, lch_out2;

    cmsFloat64Number l_param, c_param;
    if (Size >= 12 * sizeof(cmsFloat64Number)) {
        parse_float64(Data, 12 * sizeof(cmsFloat64Number), Size, &l_param);
        parse_float64(Data, 13 * sizeof(cmsFloat64Number), Size, &c_param);
    } else {
        l_param = 1.0;
        c_param = 1.0;
    }

    // Test cmsLab2LCh
    cmsLab2LCh(&lch_out1, &lab1);
    cmsLab2LCh(&lch_out2, &lab2);

    // Test cmsLCh2Lab
    cmsLCh2Lab(&lab_out1, &lch1);
    cmsLCh2Lab(&lab_out2, &lch2);

    // Test cmsDeltaE (CIE76)
    cmsFloat64Number delta76 = cmsDeltaE(&lab1, &lab2);
    (void)delta76;

    // Test cmsCIE94DeltaE
    cmsFloat64Number delta94 = cmsCIE94DeltaE(&lab1, &lab2);
    (void)delta94;

    // Test cmsBFDdeltaE
    cmsFloat64Number deltaBFD = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaBFD;

    // Test cmsCMCdeltaE
    cmsFloat64Number deltaCMC = cmsCMCdeltaE(&lab1, &lab2, l_param, c_param);
    (void)deltaCMC;

    return 0;
}