#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <lcms2.h>
}
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 217, nr_unique_branch: 56
//<Quality> {"density":31,"unique_branches":{"GenerateCSA":[[1078,16,1078,62,0,0,4,0]],"DecideLUTtypeB2A":[[1858,9,1858,25,0,0,4,0],[1859,13,1859,30,0,0,4,1]],"WriteInputLUT":[[873,9,873,22,0,0,4,0]],"Type_LUT16_Write":[[2413,9,2413,20,0,0,4,0],[2413,24,2413,58,0,0,4,0],[2413,24,2413,58,0,0,4,1],[2416,13,2416,36,0,0,4,1],[2416,40,2416,64,0,0,4,1],[2421,9,2421,20,0,0,4,0],[2421,24,2421,60,0,0,4,0],[2426,9,2426,20,0,0,4,0],[2426,24,2426,56,0,0,4,0],[2431,9,2431,20,0,0,4,0],[2431,24,2431,60,0,0,4,0],[2437,9,2437,20,0,0,4,1],[2445,9,2445,21,0,0,4,1],[2450,21,2450,38,0,0,4,0],[2450,21,2450,38,0,0,4,1],[2451,17,2451,56,0,0,4,1],[2458,9,2458,66,0,0,4,1],[2459,9,2459,67,0,0,4,1],[2460,9,2460,63,0,0,4,1],[2461,9,2461,37,0,0,4,1],[2463,9,2463,23,0,0,4,0],[2463,9,2463,23,0,0,4,1],[2465,21,2465,26,0,0,4,0],[2465,21,2465,26,0,0,4,1],[2467,17,2467,65,0,0,4,1],[2473,13,2473,45,0,0,4,1],[2474,13,2474,45,0,0,4,1],[2475,13,2475,45,0,0,4,1],[2476,13,2476,45,0,0,4,1],[2477,13,2477,45,0,0,4,1],[2478,13,2478,45,0,0,4,1],[2479,13,2479,45,0,0,4,1],[2480,13,2480,45,0,0,4,1],[2481,13,2481,45,0,0,4,1],[2485,9,2485,23,0,0,4,0],[2486,13,2486,90,0,0,4,1],[2491,9,2491,24,0,0,4,0],[2492,13,2492,91,0,0,4,1],[2500,9,2500,23,0,0,4,0],[2501,13,2501,60,0,0,4,1],[2512,9,2512,41,0,0,4,1],[2513,9,2513,21,0,0,4,0],[2515,13,2515,25,0,0,4,0],[2516,17,2516,65,0,0,4,1],[2521,9,2521,24,0,0,4,0],[2522,13,2522,61,0,0,4,1]],"cmsCreateExtendedTransform":[[1201,9,1201,101,0,0,4,0]],"Write16bitTables":[[2292,15,2292,35,0,0,4,0],[2292,15,2292,35,0,0,4,1],[2296,19,2296,31,0,0,4,0],[2296,19,2296,31,0,0,4,1],[2299,17,2299,48,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsCreateTransform","cmsDoTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsFreeNamedColorList","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateBCHSWabstractProfileTHR","cmsDetectTAC","cmsGetColorSpace","cmsChannelsOf","cmsTempFromWhitePoint","cmsTagLinkedTo","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsGetPostScriptCSA","cmsLab2LCh","cmsFloat2LabEncoded","cmsPipelineFree","cmsIT8Free","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfile","cmsBuildTabulatedToneCurveFloat","cmsD50_xyY","cmsCreateGrayProfile","cmsCreateTransform","cmsDoTransform","cmsGetNamedColorList","cmsNamedColorCount","cmsFreeNamedColorList","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8LoadFromMem","cmsIT8Free","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsStageOutputChannels","cmsCreateBCHSWabstractProfileTHR","cmsDetectTAC","cmsGetColorSpace","cmsChannelsOf","cmsTempFromWhitePoint","cmsTagLinkedTo","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsGetPostScriptCSA","cmsLab2LCh","cmsFloat2LabEncoded","cmsPipelineFree","cmsIT8Free","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Looking at these APIs, I can see they cover several key aspects of LCMS: profile creation/loading, tone curve operations, color space conversions, IT8 file handling, and pipeline transformations. I'll create a fuzzer that tests profile loading, transformation creation, and color conversion workflows.

Here's my implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hSRGB = NULL;
    cmsHPROFILE hGray = NULL;
    cmsHPROFILE hAbstract = NULL;
    cmsHTRANSFORM hTransform = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsPipeline* pipeline = NULL;
    cmsToneCurve* toneCurve = NULL;
    cmsStage* stage = NULL;
    
    // 1. Create context
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 2. Try to open profile from memory (using fuzzed data)
    // Only try if we have enough data for a minimal ICC profile
    if (size >= 128) {
        hProfile = cmsOpenProfileFromMem(data, size);
    }
    
    // 3. Create sRGB profile
    hSRGB = cmsCreate_sRGBProfile();
    
    // 4. Create a tabulated tone curve from fuzzed data
    // Use first 256 bytes for tone curve values
    size_t toneCurveSize = size > 1024 ? 256 : size / 4;
    if (toneCurveSize > 2) {
        cmsFloat32Number* curveValues = (cmsFloat32Number*)malloc(toneCurveSize * sizeof(cmsFloat32Number));
        if (curveValues) {
            // Convert bytes to float values between 0 and 1
            for (size_t i = 0; i < toneCurveSize; i++) {
                curveValues[i] = (cmsFloat32Number)data[i % size] / 255.0f;
            }
            toneCurve = cmsBuildTabulatedToneCurveFloat(ctx, toneCurveSize, curveValues);
            free(curveValues);
        }
    }
    
    // 5. Get D50 white point
    const cmsCIExyY* d50 = cmsD50_xyY();
    
    // 6. Create gray profile using D50 and tone curve (if created)
    if (d50) {
        hGray = cmsCreateGrayProfile(d50, toneCurve ? toneCurve : NULL);
    }
    
    // 7. Test color space conversions
    if (hSRGB && hGray) {
        // Create transform from sRGB to gray
        hTransform = cmsCreateTransform(hSRGB, TYPE_RGB_8, hGray, TYPE_GRAY_8, 
                                       INTENT_PERCEPTUAL, cmsFLAGS_NOOPTIMIZE);
        
        if (hTransform) {
            // Test with some sample data
            unsigned char input[3] = {128, 64, 192};
            unsigned char output[1];
            cmsDoTransform(hTransform, input, output, 1);
            
            // Get named color list (usually empty for these profiles)
            cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(hTransform);
            if (namedColors) {
                cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
                cmsFreeNamedColorList(namedColors);
            }
        }
    }
    
    // 8. Test IT8 file loading from memory
    if (size > 0) {
        hIT8 = cmsIT8Alloc(ctx);
        if (hIT8) {
            cmsIT8DefineDblFormat(hIT8, "%.6f");
            
            // Try to load IT8 data from memory
            cmsHANDLE hIT8Loaded = cmsIT8LoadFromMem(ctx, data, size);
            if (hIT8Loaded) {
                cmsIT8Free(hIT8Loaded);
            }
        }
    }
    
    // 9. Create and test pipeline operations
    pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline && toneCurve) {
        // Create tone curve stage
        cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
        stage = cmsStageAllocToneCurves(ctx, 3, curves);
        
        if (stage) {
            // Insert stage into pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            
            // Test pipeline evaluation
            cmsUInt16Number in[3] = {0x8000, 0x4000, 0xC000};
            cmsUInt16Number out[3];
            cmsPipelineEval16(in, out, pipeline);
            
            // Check pipeline properties
            cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
            }
        }
    }
    
    // 10. Create abstract profile
    hAbstract = cmsCreateBCHSWabstractProfileTHR(ctx, 17, 1.0, 1.0, 0.0, 1.0, 5000, 6500);
    
    // 11. Test other operations
    if (hProfile) {
        // Detect TAC
        cmsFloat64Number tac = cmsDetectTAC(hProfile);
        
        // Get color space
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
        cmsUInt32Number channels = cmsChannelsOf(colorSpace);
        
        // Test temp from white point
        cmsFloat64Number tempK;
        if (cmsTempFromWhitePoint(&tempK, d50)) {
            // Temperature conversion successful
        }
        
        // Test tag linking
        cmsTagSignature linkedTag = cmsTagLinkedTo(hProfile, cmsSigMediaWhitePointTag);
        
        // Try to save profile to memory
        cmsUInt32Number bytesNeeded = 0;
        cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
        if (bytesNeeded > 0 && bytesNeeded < 10 * 1024 * 1024) {
            void* buffer = malloc(bytesNeeded);
            if (buffer) {
                cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
                free(buffer);
            }
        }
        
        // Test PostScript CSA
        cmsUInt32Number bufferSize = 4096;
        void* psBuffer = malloc(bufferSize);
        if (psBuffer) {
            cmsUInt32Number result = cmsGetPostScriptCSA(ctx, hProfile, INTENT_PERCEPTUAL, 0, psBuffer, bufferSize);
            free(psBuffer);
        }
    }
    
    // 12. Test color conversions
    cmsCIELab lab = {50.0, 20.0, 30.0};
    cmsCIELCh lch;
    cmsLab2LCh(&lch, &lab);
    
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncoded(wLab, &lab);
    
    // 13. Clean up resources
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    if (hSRGB) {
        cmsCloseProfile(hSRGB);
    }
    
    if (hGray) {
        cmsCloseProfile(hGray);
    }
    
    if (hAbstract) {
        cmsCloseProfile(hAbstract);
    }
    
    if (ctx) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}