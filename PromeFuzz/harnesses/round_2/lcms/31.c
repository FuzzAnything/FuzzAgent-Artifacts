// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"

static double readDouble(const uint8_t* data, size_t offset) {
    double value;
    memcpy(&value, data + offset, sizeof(double));
    return value;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 8 * sizeof(double)) {
        return 0;
    }

    cmsCIELab lab1, lab2;
    cmsCIELCh lch1, lch2;

    lab1.L = readDouble(Data, 0);
    lab1.a = readDouble(Data, sizeof(double));
    lab1.b = readDouble(Data, 2 * sizeof(double));

    lab2.L = readDouble(Data, 3 * sizeof(double));
    lab2.a = readDouble(Data, 4 * sizeof(double));
    lab2.b = readDouble(Data, 5 * sizeof(double));

    double l = readDouble(Data, 6 * sizeof(double));
    double c = readDouble(Data, 7 * sizeof(double));

    cmsBFDdeltaE(&lab1, &lab2);
    cmsCMCdeltaE(&lab1, &lab2, l, c);
    cmsCIE94DeltaE(&lab1, &lab2);
    cmsDeltaE(&lab1, &lab2);

    if (Size >= 12 * sizeof(double)) {
        lch1.L = readDouble(Data, 8 * sizeof(double));
        lch1.C = readDouble(Data, 9 * sizeof(double));
        lch1.h = readDouble(Data, 10 * sizeof(double));

        cmsLCh2Lab(&lab1, &lch1);
        cmsLab2LCh(&lch2, &lab2);
    }

    return 0;
}