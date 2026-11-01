// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4, cmsToneCurve *c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    uint16_t *vals16 = NULL;
    float *valsFloat = NULL;
    cmsToneCurve *curve16 = NULL;
    cmsToneCurve *curveFloat = NULL;
    cmsToneCurve *curveRev = NULL;
    cmsToneCurve *curveJoined = NULL;

    // Extract parameters from input
    uint32_t nEntries16 = (Data[0] % 4097) + 1;
    uint32_t nEntriesFloat = (Data[1] % 4097) + 1;
    uint32_t nResultSamples = (Data[2 % Size] % 4097) + 1;
    uint32_t nJoinPoints = (Data[3 % Size] % 4097) + 1;

    // Allocate buffers if size permits
    size_t maxAlloc = Size - 5;
    if (nEntries16 > maxAlloc / sizeof(uint16_t)) nEntries16 = maxAlloc / sizeof(uint16_t);
    if (nEntriesFloat > maxAlloc / sizeof(float)) nEntriesFloat = maxAlloc / sizeof(float);

    if (nEntries16 > 0) {
        vals16 = (uint16_t *)malloc(nEntries16 * sizeof(uint16_t));
        if (vals16) {
            memset(vals16, 0, nEntries16 * sizeof(uint16_t));
            for (uint32_t i = 0; i < nEntries16 && (5 + i * sizeof(uint16_t)) < Size; i++) {
                vals16[i] = Data[5 + i * sizeof(uint16_t)] * 256 + Data[5 + i * sizeof(uint16_t) + 1];
            }
            curve16 = cmsBuildTabulatedToneCurve16(ctx, nEntries16, vals16);
        }
    }

    if (nEntriesFloat > 0) {
        valsFloat = (float *)malloc(nEntriesFloat * sizeof(float));
        if (valsFloat) {
            memset(valsFloat, 0, nEntriesFloat * sizeof(float));
            for (uint32_t i = 0; i < nEntriesFloat && (5 + i * sizeof(float)) < Size; i++) {
                uint32_t raw = 0;
                memcpy(&raw, Data + 5 + i * sizeof(float), sizeof(float));
                valsFloat[i] = (float)(raw / 4294967296.0); // Normalize to 0-1 roughly
            }
            curveFloat = cmsBuildTabulatedToneCurveFloat(ctx, nEntriesFloat, valsFloat);
        }
    }

    // Test cmsReverseToneCurveEx
    cmsToneCurve *inputForReverse = curve16 ? curve16 : (curveFloat ? curveFloat : NULL);
    if (inputForReverse) {
        curveRev = cmsReverseToneCurveEx(nResultSamples, inputForReverse);
    }

    // Test cmsGetToneCurveEstimatedTableEntries
    cmsToneCurve *checkCurve = curve16 ? curve16 : (curveFloat ? curveFloat : (curveRev ? curveRev : NULL));
    if (checkCurve) {
        cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(checkCurve);
        (void)entries; // Use value to avoid unused warning
    }

    // Test cmsJoinToneCurve
    cmsToneCurve *X = curve16 ? curve16 : curveRev;
    cmsToneCurve *Y = curveFloat ? curveFloat : curve16;
    if (X && Y) {
        curveJoined = cmsJoinToneCurve(ctx, X, Y, nJoinPoints);
    }

    // Cleanup
    cleanup_curves(curve16, curveFloat, curveRev, curveJoined, NULL);
    free(vals16);
    free(valsFloat);
    
    cmsDeleteContext(ctx);

    return 0;
}