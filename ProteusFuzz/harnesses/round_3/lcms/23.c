// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_curve_from_input(const uint8_t* Data, size_t Size, int type) {
    if (Size < 4) return NULL;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    
    cmsToneCurve* curve = NULL;
    cmsUInt32Number count = 0;
    
    // Parse count safely
    if (Size >= 4) {
        count = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        // Limit to reasonable size to prevent OOM and buffer overflows
        if (count > 1000) count = 1000;
        if (count == 0) count = 1;
    } else {
        count = 1;
    }
    
    if (type == 0) {
        // 16-bit tabulated
        if (Size >= 4 + count * 2) {
            cmsUInt16Number* values = (cmsUInt16Number*)(Data + 4);
            curve = cmsBuildTabulatedToneCurve16(ctx, count, values);
        }
    } else if (type == 1) {
        // Float tabulated
        if (Size >= 4 + count * 4) {
            cmsFloat32Number* values = (cmsFloat32Number*)(Data + 4);
            curve = cmsBuildTabulatedToneCurveFloat(ctx, count, values);
        }
    } else if (type == 2) {
        // Segmented - Strict validation to prevent buffer overflow
        // cmsCurveSegment structure size check
        if (Size >= 4 + count * sizeof(cmsCurveSegment)) {
            cmsCurveSegment* segments = (cmsCurveSegment*)(Data + 4);
            
            // Validate segment data to ensure internal consistency
            // This prevents passing malformed data that causes out-of-bounds reads inside lcms
            int valid = 1;
            for (cmsUInt32Number i = 0; i < count && valid; i++) {
                if (segments[i].x0 < 0.0 || segments[i].x1 > 1.0 || segments[i].x0 > segments[i].x1) {
                    valid = 0;
                }
                // Ensure nGridPoints is reasonable
                if (segments[i].nGridPoints > 1000 || segments[i].nGridPoints == 0) {
                    valid = 0;
                }
            }
            
            if (valid) {
                curve = cmsBuildSegmentedToneCurve(ctx, count, segments);
            }
        }
    }
    
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
    
    return curve;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int type = Data[0] % 3; // 0: 16-bit, 1: float, 2: segmented
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* curve3 = NULL;
    cmsToneCurve* curve4 = NULL;
    cmsToneCurve* curve5 = NULL;
    cmsUInt32Number sample_count = 0;
    cmsUInt32Number join_points = 0;
    
    // Parse sample count and join points from input
    if (Size >= 5) {
        sample_count = (Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4];
        if (sample_count > 1000) sample_count = 1000;
        if (sample_count == 0) sample_count = 1;
        
        if (Size >= 9) {
            join_points = (Data[5] << 24) | (Data[6] << 16) | (Data[7] << 8) | Data[8];
            if (join_points > 1000) join_points = 1000;
            if (join_points == 0) join_points = 1;
        } else {
            join_points = sample_count;
        }
    }
    
    // Create first curve
    if (Size > 1) {
        curve1 = create_curve_from_input(Data + 1, Size - 1, type);
    }
    
    // Create second curve if we have enough data
    if (Size > 100 && curve1 != NULL) {
        int type2 = (Data[100] % 3);
        curve2 = create_curve_from_input(Data + 101, Size - 101, type2);
    }
    
    // Test cmsReverseToneCurveEx
    if (curve1 != NULL) {
        curve3 = cmsReverseToneCurveEx(sample_count, curve1);
        cleanup_curve(curve3);
    }
    
    // Test cmsGetToneCurveEstimatedTableEntries
    if (curve1 != NULL) {
        cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
        (void)entries; // Use the value to avoid unused warning
    }
    
    // Test cmsJoinToneCurve
    if (curve1 != NULL && curve2 != NULL) {
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx != NULL) {
            curve4 = cmsJoinToneCurve(ctx, curve1, curve2, join_points);
            cleanup_curve(curve4);
            cmsDeleteContext(ctx);
        }
    }
    
    // Test cmsBuildTabulatedToneCurve16 directly with varied inputs
    if (Size >= 4) {
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx != NULL) {
            cmsUInt32Number nEntries = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
            if (nEntries > 1000) nEntries = 1000;
            if (nEntries == 0) nEntries = 1;
            
            if (Size >= 4 + nEntries * 2) {
                cmsUInt16Number* values = (cmsUInt16Number*)(Data + 4);
                curve5 = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
                cleanup_curve(curve5);
            }
            
            cmsDeleteContext(ctx);
        }
    }
    
    // Test cmsBuildTabulatedToneCurveFloat directly
    if (Size >= 4) {
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx != NULL) {
            cmsUInt32Number nEntries = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
            if (nEntries > 1000) nEntries = 1000;
            if (nEntries == 0) nEntries = 1;
            
            if (Size >= 4 + nEntries * 4) {
                cmsFloat32Number* values = (cmsFloat32Number*)(Data + 4);
                curve5 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
                cleanup_curve(curve5);
            }
            
            cmsDeleteContext(ctx);
        }
    }
    
    // Test cmsBuildSegmentedToneCurve directly with strict validation
    if (Size >= 4) {
        cmsContext ctx = cmsCreateContext(NULL, NULL);
        if (ctx != NULL) {
            cmsUInt32Number nSegments = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
            if (nSegments > 100) nSegments = 100; // Reduced limit for safety
            if (nSegments == 0) nSegments = 1;
            
            if (Size >= 4 + nSegments * sizeof(cmsCurveSegment)) {
                cmsCurveSegment* segments = (cmsCurveSegment*)(Data + 4);
                
                // Validate segments before passing to lcms
                int valid = 1;
                for (cmsUInt32Number i = 0; i < nSegments && valid; i++) {
                    if (segments[i].x0 < 0.0 || segments[i].x1 > 1.0 || segments[i].x0 > segments[i].x1) {
                        valid = 0;
                    }
                    if (segments[i].nGridPoints > 100 || segments[i].nGridPoints == 0) {
                        valid = 0;
                    }
                }
                
                if (valid) {
                    curve5 = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                    cleanup_curve(curve5);
                }
            }
            
            cmsDeleteContext(ctx);
        }
    }
    
    // Cleanup
    cleanup_curve(curve1);
    cleanup_curve(curve2);
    
    return 0;
}