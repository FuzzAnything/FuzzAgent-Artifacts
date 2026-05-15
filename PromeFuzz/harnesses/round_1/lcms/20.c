// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static cmsToneCurve* create_test_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    // Use first 2 bytes for nEntries (limited to reasonable size)
    cmsUInt32Number nEntries = (data[0] << 8) | data[1];
    nEntries = nEntries % 256 + 2; // Limit to 2-257 entries
    
    if (size < 2 + nEntries * 2) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + 2);
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

static cmsStage* create_test_clut_stage(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 12) return NULL;
    
    // Parse parameters from data
    cmsUInt32Number nGridPoints = ((data[0] << 8) | data[1]) % 32 + 2;
    cmsUInt32Number inputChan = (data[2] % 8) + 1;
    cmsUInt32Number outputChan = (data[3] % 8) + 1;
    
    size_t table_size = nGridPoints * inputChan * outputChan;
    if (size < 12 + table_size * 2) return NULL;
    
    const cmsUInt16Number* table = (const cmsUInt16Number*)(data + 4);
    return cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
}

int LLVMFuzzerTestOneInput_20(const uint8_t* data, size_t size) {
    if (size < 4) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Test cmsBuildTabulatedToneCurve16 and cmsEvalToneCurve16
    cmsToneCurve* curve = create_test_tone_curve(ctx, data, size);
    if (curve) {
        // Test evaluation with various input values
        for (int i = 0; i < 4 && i * 2 + 2 < size; i++) {
            cmsUInt16Number v = ((data[i * 2] << 8) | data[i * 2 + 1]) % 65536;
            cmsUInt16Number result = cmsEvalToneCurve16(curve, v);
            (void)result; // Use result to avoid unused variable warning
        }
    }
    
    // Test cmsStageAllocToneCurves
    if (curve) {
        cmsToneCurve* curves[3] = {curve, curve, curve};
        cmsUInt32Number nChannels = (data[0] % 3) + 1;
        cmsStage* tone_stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
        if (tone_stage) {
            cmsStageFree(tone_stage);
        }
    }
    
    // Test cmsStageAllocCLut16bit
    cmsStage* clut_stage = create_test_clut_stage(ctx, data, size);
    
    // Test cmsPipelineEval16 if we have both stages
    if (curve && clut_stage) {
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
        if (pipeline) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, clut_stage);
            
            // Create test input from data
            cmsUInt16Number in[3] = {0, 0, 0};
            cmsUInt16Number out[3] = {0, 0, 0};
            
            for (int i = 0; i < 3 && i * 2 + 4 < size; i++) {
                in[i] = ((data[i * 2 + 4] << 8) | data[i * 2 + 5]) % 65536;
            }
            
            cmsPipelineEval16(in, out, pipeline);
            cmsPipelineFree(pipeline);
        } else {
            cmsStageFree(clut_stage);
        }
    } else if (clut_stage) {
        cmsStageFree(clut_stage);
    }
    
    // Test cmsCreateExtendedTransform
    if (size >= 20) {
        cmsHPROFILE profile = create_dummy_profile(ctx);
        if (profile) {
            cmsHPROFILE profiles[2] = {profile, profile};
            cmsBool bpc[2] = {(data[0] & 1), (data[1] & 1)};
            cmsUInt32Number intents[2] = {data[2] % 4, data[3] % 4};
            cmsFloat64Number adaptation[2] = {(double)(data[4] / 255.0), (double)(data[5] / 255.0)};
            cmsUInt32Number inputFormat = (data[6] % 2) ? TYPE_RGB_16 : TYPE_CMYK_16;
            cmsUInt32Number outputFormat = (data[7] % 2) ? TYPE_RGB_16 : TYPE_CMYK_16;
            cmsUInt32Number flags = data[8];
            
            cmsHTRANSFORM transform = cmsCreateExtendedTransform(
                ctx, 2, profiles, bpc, intents, adaptation,
                NULL, 0, inputFormat, outputFormat, flags
            );
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            cmsCloseProfile(profile);
        }
    }
    
    // Cleanup
    if (curve) {
        cmsFreeToneCurve(curve);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}