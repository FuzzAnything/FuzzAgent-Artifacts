#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *curves[], int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
            curves[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsToneCurve *curves[10] = {0};
    int curve_count = 0;

    // Extract parameters
    cmsUInt32Number nSegments = (Size > 0) ? Data[0] : 0;
    cmsUInt32Number nEntries = (Size > 1) ? Data[1] : 0;
    cmsInt32Number type = (Size > 2) ? Data[2] : 0;
    
    // Limit values to prevent OOM or crashes
    if (nSegments > 100) nSegments = 100;
    if (nEntries > 1000) nEntries = 1000;

    // --- Path 1: cmsBuildTabulatedToneCurve16 ---
    // Targets lines: 226 (nEntries=0, nSegments=0), 252 (nEntries=0), 263 (Values != NULL)
    // Also 233, 257, 299, 303-310
    if (Size >= 2) {
        cmsUInt16Number *values = NULL;
        cmsUInt32Number safe_nEntries = nEntries;
        
        // Special case to hit line 226: nEntries=0
        if (safe_nEntries == 0) {
            cmsToneCurve *c = cmsBuildTabulatedToneCurve16(ContextID, 0, NULL);
            if (c && curve_count < 10) curves[curve_count++] = c;
        } else {
            // Allocate values to hit line 263
            values = (cmsUInt16Number *)malloc(safe_nEntries * sizeof(cmsUInt16Number));
            if (values) {
                for (uint32_t i = 0; i < safe_nEntries && i < Size - 2; i++) {
                    values[i] = (cmsUInt16Number)(Data[2 + i] * 256); // Fill with data
                }
                cmsToneCurve *c = cmsBuildTabulatedToneCurve16(ContextID, safe_nEntries, values);
                if (c && curve_count < 10) curves[curve_count++] = c;
                free(values);
            }
        }
    }

    // --- Path 2: cmsBuildSegmentedToneCurve ---
    // Targets lines: 242, 245, 271, 276, 299, 303-310
    // Note: nEntries passed internally is nGridPoints (>= 4096), so 226, 252, 263 are NOT hit here.
    if (nSegments > 0 && Size >= 3) {
        cmsCurveSegment *segments = (cmsCurveSegment *)calloc(nSegments, sizeof(cmsCurveSegment));
        if (segments) {
            for (uint32_t i = 0; i < nSegments; i++) {
                // Initialize segments with valid data
                segments[i].x0 = 0.0;
                segments[i].x1 = 1.0;
                // Use type from data, default to 1 (Gamma) if invalid
                segments[i].Type = (cmsInt32Number)(Data[3 + (i % (Size - 3))]);
                if (segments[i].Type == 0) {
                    segments[i].Type = 1; // Avoid type 0 if we don't have sampled points
                }
                // Set some params
                segments[i].Params[0] = 1.0 + (Data[4 + (i % (Size - 4))] / 100.0);
            }
            
            cmsToneCurve *c = cmsBuildSegmentedToneCurve(ContextID, nSegments, segments);
            if (c && curve_count < 10) curves[curve_count++] = c;
            
            free(segments);
        }
    }

    // --- Path 3: cmsBuildTabulatedToneCurveFloat ---
    // Calls cmsBuildSegmentedToneCurve internally.
    // Can hit segment logic.
    if (Size >= 4 && nEntries > 0) {
        cmsFloat32Number *float_values = (cmsFloat32Number *)malloc(nEntries * sizeof(cmsFloat32Number));
        if (float_values) {
            for (uint32_t i = 0; i < nEntries && i < Size - 4; i++) {
                float_values[i] = (float)(Data[4 + i]) / 255.0;
            }
            cmsToneCurve *c = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, float_values);
            if (c && curve_count < 10) curves[curve_count++] = c;
            free(float_values);
        }
    }

    // --- Path 4: cmsBuildParametricToneCurve ---
    // Calls cmsBuildSegmentedToneCurve internally.
    if (Size >= 5) {
        cmsFloat64Number params[5] = {0};
        params[0] = 1.0 + (Data[5] / 100.0);
        cmsToneCurve *c = cmsBuildParametricToneCurve(ContextID, type, params);
        if (c && curve_count < 10) curves[curve_count++] = c;
    }

    // --- Path 5: cmsDupToneCurve ---
    // If we have a curve, duplicate it to hit the path via cmsDupToneCurve
    if (curve_count > 0) {
        cmsToneCurve *dup = cmsDupToneCurve(curves[0]);
        if (dup && curve_count < 10) curves[curve_count++] = dup;
    }

    // Cleanup
    cleanup_curves(curves, curve_count);
    cmsDeleteContext(ContextID);

    return 0;
}