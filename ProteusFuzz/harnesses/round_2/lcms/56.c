// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    
    // Try to use fuzz data to influence context if possible, though context creation is simple
    // We just return the context for use in other functions
    return ctx;
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* BuildSegmentedCurve(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 4) return NULL;
    
    cmsUInt32Number nSegments = *(const cmsUInt32Number*)(Data);
    if (nSegments == 0 || nSegments > 1000) return NULL; // Sanity check
    
    size_t required_size = sizeof(cmsCurveSegment) * nSegments;
    if (Size < 4 + required_size) return NULL;
    
    const cmsCurveSegment *segments = (const cmsCurveSegment*)(Data + 4);
    
    return cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
}

static cmsToneCurve* BuildTabulatedCurve16(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 4) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)(Data);
    if (nEntries == 0 || nEntries > 10000) return NULL; // Sanity check
    
    size_t required_size = sizeof(cmsUInt16Number) * nEntries;
    if (Size < 4 + required_size) return NULL;
    
    const cmsUInt16Number *values = (const cmsUInt16Number*)(Data + 4);
    
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = CreateContext(Data, Size);
    if (!ctx) {
        // Fallback to default context if creation fails
        ctx = NULL; 
    }
    
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *curve4 = NULL;
    cmsToneCurve *curve5 = NULL;
    cmsToneCurve *curve6 = NULL;
    cmsToneCurve *curve7 = NULL;
    
    // Partition input data for different operations
    size_t offset = 0;
    size_t chunk_size = Size / 6;
    
    // 1. Try to build a segmented tone curve
    if (chunk_size > 0) {
        curve1 = BuildSegmentedCurve(ctx, Data + offset, chunk_size);
        offset += chunk_size;
    }
    
    // 2. Try to build a tabulated tone curve
    if (chunk_size > 0 && offset + chunk_size <= Size) {
        curve2 = BuildTabulatedCurve16(ctx, Data + offset, chunk_size);
        offset += chunk_size;
    }
    
    // 3. Duplicate curve1
    if (curve1) {
        curve3 = cmsDupToneCurve(curve1);
    }
    
    // 4. Reverse curve2
    if (curve2) {
        cmsUInt32Number nSamples = 1024;
        if (offset + sizeof(cmsUInt32Number) <= Size) {
            nSamples = *(const cmsUInt32Number*)(Data + offset);
            if (nSamples == 0) nSamples = 1024;
            if (nSamples > 10000) nSamples = 10000;
            offset += sizeof(cmsUInt32Number);
        }
        curve4 = cmsReverseToneCurveEx(nSamples, curve2);
    }
    
    // 5. Join curve3 and curve4
    if (curve3 && curve4) {
        cmsUInt32Number nPoints = 1024;
        if (offset + sizeof(cmsUInt32Number) <= Size) {
            nPoints = *(const cmsUInt32Number*)(Data + offset);
            if (nPoints == 0) nPoints = 1024;
            if (nPoints > 10000) nPoints = 10000;
            offset += sizeof(cmsUInt32Number);
        }
        curve5 = cmsJoinToneCurve(ctx, curve3, curve4, nPoints);
    }
    
    // 6. Duplicate curve5
    if (curve5) {
        curve6 = cmsDupToneCurve(curve5);
    }
    
    // 7. Reverse curve6
    if (curve6) {
        cmsUInt32Number nSamples = 512;
        if (offset + sizeof(cmsUInt32Number) <= Size) {
            nSamples = *(const cmsUInt32Number*)(Data + offset);
            if (nSamples == 0) nSamples = 512;
            if (nSamples > 10000) nSamples = 10000;
        }
        curve7 = cmsReverseToneCurveEx(nSamples, curve6);
    }
    
    // Cleanup: Free all allocated curves
    // Use cmsFreeToneCurveTriple for groups of 3
    cmsToneCurve *triple[3];
    
    // Free curve1, curve2, curve3
    triple[0] = curve1;
    triple[1] = curve2;
    triple[2] = curve3;
    cmsFreeToneCurveTriple(triple);
    
    // Free curve4, curve5, curve6
    triple[0] = curve4;
    triple[1] = curve5;
    triple[2] = curve6;
    cmsFreeToneCurveTriple(triple);
    
    // Free curve7 individually
    if (curve7) {
        cmsFreeToneCurve(curve7);
    }
    
    // Cleanup context
    CleanupContext(ctx);
    
    return 0;
}