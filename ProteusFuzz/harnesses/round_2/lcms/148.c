// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ContextID, cmsHPROFILE Profile, cmsToneCurve* Curves[], int count) {
    if (Profile != NULL) {
        cmsCloseProfile(Profile);
    }
    for (int i = 0; i < count; i++) {
        if (Curves[i] != NULL) {
            cmsFreeToneCurve(Curves[i]);
        }
    }
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Extract data for various operations
    cmsUInt16Number endian_val = 0;
    if (Size >= 2) {
        endian_val = Data[0] | (Data[1] << 8);
    }

    // Test _cmsAdjustEndianess16
    cmsUInt16Number swapped = _cmsAdjustEndianess16(endian_val);
    (void)swapped; // Suppress unused variable warning

    // Prepare for tone curve operations
    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    cmsUInt32Number nEntries = 0;
    cmsUInt16Number *table_data = NULL;

    if (Size >= 4) {
        nEntries = (Data[2] | (Data[3] << 8)) % 4097; // Ensure within reasonable bounds
        if (nEntries > 0 && Size >= 4 + nEntries * 2) {
            table_data = (cmsUInt16Number *)malloc(nEntries * sizeof(cmsUInt16Number));
            if (table_data) {
                for (uint32_t i = 0; i < nEntries; i++) {
                    table_data[i] = Data[4 + i * 2] | (Data[4 + i * 2 + 1] << 8);
                }
            }
        }
    }

    // Test cmsBuildTabulatedToneCurve16
    if (table_data && nEntries > 0) {
        curves[0] = cmsBuildTabulatedToneCurve16(ContextID, nEntries, table_data);
        if (curves[0] == NULL) {
            free(table_data);
            cleanup_resources(ContextID, NULL, NULL, 0);
            return 0;
        }
    }

    // Test cmsReverseToneCurve
    if (curves[0] != NULL) {
        cmsToneCurve *reversed = cmsReverseToneCurve(curves[0]);
        if (reversed != NULL) {
            curves[1] = reversed;
        }
    }

    // Test cmsEvalToneCurve16
    if (curves[0] != NULL) {
        cmsUInt16Number eval_result = cmsEvalToneCurve16(curves[0], endian_val);
        (void)eval_result;
    }
    if (curves[1] != NULL) {
        cmsUInt16Number eval_result = cmsEvalToneCurve16(curves[1], endian_val);
        (void)eval_result;
    }

    // Test cmsGetToneCurveEstimatedTable
    if (curves[0] != NULL) {
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(curves[0]);
        if (table) {
            // Just read a few values to ensure no crash
            (void)table[0];
        }
    }
    if (curves[1] != NULL) {
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(curves[1]);
        if (table) {
            (void)table[0];
        }
    }

    // Test cmsCreateLinearizationDeviceLinkTHR
    cmsColorSpaceSignature colorSpace = cmsSigRgbData;
    cmsHPROFILE profile = cmsCreateLinearizationDeviceLinkTHR(ContextID, colorSpace, curves);

    // Cleanup
    if (table_data) free(table_data);
    cleanup_resources(ContextID, profile, curves, 3);

    return 0;
}