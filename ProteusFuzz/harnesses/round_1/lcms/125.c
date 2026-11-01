// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Use data to determine number of entries for curve creation
    // Ensure minimum size for at least one entry
    size_t nEntries = (Size > 0) ? (Size / 2) : 1;
    if (nEntries > 65536) nEntries = 65536; // Safety cap
    if (nEntries == 0) nEntries = 1;

    // Allocate buffer for 16-bit values
    cmsUInt16Number *values = NULL;
    if (Size >= nEntries * 2) {
        values = (cmsUInt16Number *)malloc(nEntries * sizeof(cmsUInt16Number));
        if (values) {
            // Copy data into values, ensuring valid 16-bit range
            for (size_t i = 0; i < nEntries; i++) {
                uint16_t raw = Data[i * 2] | (Data[i * 2 + 1] << 8);
                // Clamp to valid 16-bit unsigned range (0-65535)
                values[i] = (raw & 0xFFFF);
            }
        }
    } else {
        // Fallback if data is too small
        values = (cmsUInt16Number *)malloc(nEntries * sizeof(cmsUInt16Number));
        if (values) {
            for (size_t i = 0; i < nEntries; i++) {
                values[i] = (Data[i % Size] * 256) & 0xFFFF;
            }
        }
    }

    if (!values) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsToneCurve *curve = NULL;
    cmsToneCurve *reversed_curve = NULL;
    cmsHPROFILE profile = NULL;
    cmsToneCurve *curves[4] = {NULL, NULL, NULL, NULL};

    // 1. Build Tabulated Tone Curve
    curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
    
    if (curve) {
        // 2. Reverse Tone Curve
        reversed_curve = cmsReverseToneCurve(curve);

        // 3. Get Estimated Table
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(curve);
        if (table) {
            // Read from table to ensure execution path
            volatile cmsUInt16Number dummy = table[0];
            (void)dummy;
        }

        // 4. Evaluate Tone Curve
        // Ensure we have at least 2 bytes before accessing Data[0] and Data[1]
        cmsUInt16Number eval_input = 0;
        if (Size >= 2) {
            eval_input = (Data[0] << 8) | Data[1];
        }
        cmsUInt16Number eval_output = cmsEvalToneCurve16(curve, eval_input);
        (void)eval_output;

        // 5. Create Linearization DeviceLink
        // We need an array of curves. Let's use the created curve for all channels if valid
        cmsColorSpaceSignature colorSpace = cmsSigRgbData;
        if (nEntries > 0) {
            curves[0] = curve;
            curves[1] = curve;
            curves[2] = curve;
            curves[3] = NULL; // Alpha not used for RGB
            
            profile = cmsCreateLinearizationDeviceLinkTHR(ctx, colorSpace, curves);
        }
    }

    // 6. Endianness adjustment (internal function, but exposed in API list)
    cmsUInt16Number endian_word = 0;
    if (Size >= 2) {
        endian_word = (Data[0] << 8) | Data[1];
    }
    cmsUInt16Number adjusted_word = _cmsAdjustEndianess16(endian_word);
    (void)adjusted_word;

    // Cleanup
    cleanup_curve(reversed_curve);
    cleanup_curve(curve);
    cleanup_profile(profile);
    free(values);
    cmsDeleteContext(ctx);

    return 0;
}