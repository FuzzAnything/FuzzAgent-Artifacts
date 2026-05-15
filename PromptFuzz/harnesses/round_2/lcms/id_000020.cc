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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 6
//<Quality> {"density":18,"unique_branches":{"cmsGetProfileInfo":[[1021,9,1021,20,0,0,4,0],[1021,9,1021,20,0,0,4,1]],"cmsDictGetEntryList":[[1194,9,1194,21,0,0,4,1]],"cmsStageSampleCLut16bit":[[769,9,769,39,0,0,4,0]],"cmsMLUgetWide":[[643,9,643,21,0,0,4,0],[652,9,652,46,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromStream","cmsCreate_sRGBProfileTHR","cmsSetPCS","cmsSetHeaderProfileID","cmsGetProfileInfo","cmsDictAlloc","cmsMLUalloc","cmsMLUsetASCII","cmsDictGetEntryList","cmsMLUgetWide","cmsMLUfree","cmsDictFree","cmsGetColorSpace","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsStageSampleCLut16bit","cmsPipelineEvalFloat","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStream","cmsCreate_sRGBProfileTHR","cmsSetPCS","cmsSetHeaderProfileID","cmsGetProfileInfo","cmsDictAlloc","cmsMLUalloc","cmsMLUsetASCII","cmsDictGetEntryList","cmsMLUgetWide","cmsMLUfree","cmsDictFree","cmsGetColorSpace","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsStageSampleCLut16bit","cmsPipelineEvalFloat","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input data as a FILE* for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create context for lcms operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create profile from input stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    if (!hProfile) {
        // If cannot open from stream, create a basic profile instead
        hProfile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    if (hProfile) {
        // 2. Set PCS (Profile Connection Space)
        cmsSetPCS(hProfile, cmsSigLabData);
        
        // 3. Set header profile ID using first 16 bytes of input data
        cmsUInt8Number profileID[16];
        size_t idSize = size < 16 ? size : 16;
        if (idSize > 0) {
            memcpy(profileID, data, idSize);
            // Fill remaining bytes if needed
            for (size_t i = idSize; i < 16; i++) {
                profileID[i] = 0;
            }
            cmsSetHeaderProfileID(hProfile, profileID);
        }
        
        // 4. Get profile info (description)
        wchar_t infoBuffer[256];
        cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, 
                                                     "en", "US", infoBuffer, 256);
        
        // 5. Create a dictionary
        cmsHANDLE hDict = cmsDictAlloc(ctx);
        if (hDict) {
            // Create MLU for display name
            cmsMLU *displayName = cmsMLUalloc(ctx, 1);
            if (displayName) {
                // Use part of input data for MLU string (safe version)
                char asciiStr[128];
                size_t strLen = size < 127 ? size : 127;
                if (strLen > 0) {
                    memcpy(asciiStr, data, strLen);
                    asciiStr[strLen] = '\0';
                    cmsMLUsetASCII(displayName, "en", "US", asciiStr);
                }
                
                // 6. Get dictionary entry list
                const cmsDICTentry *entryList = cmsDictGetEntryList(hDict);
                (void)entryList; // Use to avoid unused variable warning
                
                // 7. Get MLU wide string
                wchar_t wideBuffer[256];
                cmsUInt32Number wideSize = cmsMLUgetWide(displayName, "en", "US", 
                                                         wideBuffer, 256);
                
                // Clean up MLU
                cmsMLUfree(displayName);
            }
            
            // 8. Free dictionary
            cmsDictFree(hDict);
        }
        
        // 9. Get color space of profile
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
        (void)colorSpace; // Use to avoid unused variable warning
        
        // 10. Create a simple pipeline for demonstration
        cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
        if (pipeline) {
            // Create a simple tone curve
            cmsToneCurve *toneCurve = cmsBuildGamma(ctx, 2.2);
            if (toneCurve) {
                cmsToneCurve *toneCurves[3] = {toneCurve, toneCurve, toneCurve};
                
                // Add tone curve stage to pipeline
                cmsStage *stage = cmsStageAllocToneCurves(ctx, 3, toneCurves);
                if (stage) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
                    
                    // 11. Sample CLUT 16-bit (using a simple sampler)
                    cmsSAMPLER16 sampler = [](const cmsUInt16Number *in, 
                                             cmsUInt16Number *out, 
                                             void *cargo) -> cmsInt32Number {
                        // Simple pass-through
                        out[0] = in[0];
                        out[1] = in[1];
                        out[2] = in[2];
                        return 1;
                    };
                    
                    cmsUInt32Number clutPoints[3] = {2, 2, 2};
                    cmsStageSampleCLut16bit(stage, sampler, NULL, 0);
                    
                    // 12. Evaluate pipeline with float data
                    cmsFloat32Number input[3] = {0.5f, 0.5f, 0.5f};
                    cmsFloat32Number output[3];
                    cmsPipelineEvalFloat(input, output, pipeline);
                }
                
                // Free tone curve
                cmsFreeToneCurve(toneCurve);
            }
            
            // Free pipeline
            cmsPipelineFree(pipeline);
        }
        
        // Clean up profile
        cmsCloseProfile(hProfile);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}