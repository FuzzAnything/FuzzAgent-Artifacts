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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsCreateProfilePlaceholder","cmsWriteRawTag","cmsGetHeaderCreationDateTime","cmsD50_xyY","cmsCreateLab2Profile","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsCreateProfilePlaceholder","cmsWriteRawTag","cmsGetHeaderCreationDateTime","cmsD50_xyY","cmsCreateLab2Profile","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if we don't have enough data
    if (size < 128) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    
    // Read profile from memory stream
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    // Read the profile data
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "rb");
    
    // Try to create a profile from the data directly
    if (!hProfile) {
        // Create a placeholder profile and write raw data
        hProfile = cmsCreateProfilePlaceholder(ctx);
        if (hProfile) {
            cmsWriteRawTag(hProfile, cmsSigProfileDescriptionTag, data, 
                          size < 1024 ? size : 1024);
        }
    }
    
    if (hProfile) {
        // Get creation date time
        struct tm creationDate;
        memset(&creationDate, 0, sizeof(creationDate));
        cmsBool gotDate = cmsGetHeaderCreationDateTime(hProfile, &creationDate);
        
        // Create Lab2 profile with D50 white point
        const cmsCIExyY* d50 = cmsD50_xyY();
        cmsHPROFILE labProfile = cmsCreateLab2Profile(d50);
        
        if (labProfile) {
            // Create a transform
            cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
                ctx, hProfile, TYPE_Lab_DBL, labProfile, TYPE_Lab_DBL,
                INTENT_PERCEPTUAL, 0);
            
            if (hTransform) {
                // Get the transform pipeline
                const cmsPipeline* pipeline = cmsGetTransformPipeline(hTransform);
                
                // Create a dictionary
                cmsHANDLE hDict = cmsDictAlloc(ctx);
                if (hDict) {
                    // Create MLU objects for dictionary entry
                    cmsMLU* displayName = cmsMLUalloc(ctx, 1);
                    cmsMLU* displayValue = cmsMLUalloc(ctx, 1);
                    
                    if (displayName && displayValue) {
                        // Set some values in MLUs
                        cmsMLUsetWide(displayName, "en", "US", L"Test Display Name");
                        cmsMLUsetWide(displayValue, "en", "US", L"Test Display Value");
                        
                        // Add entry to dictionary
                        cmsDictAddEntry(hDict, L"TestName", L"TestValue", 
                                       displayName, displayValue);
                        
                        // Clean up MLUs
                        cmsMLUfree(displayName);
                        cmsMLUfree(displayValue);
                    }
                    
                    cmsDictFree(hDict);
                }
                
                // Create and smooth a tone curve
                cmsToneCurve* toneCurve = cmsBuildGamma(ctx, 2.2);
                if (toneCurve) {
                    // Smooth the tone curve
                    cmsSmoothToneCurve(toneCurve, 0.5);
                    cmsFreeToneCurve(toneCurve);
                }
                
                cmsDeleteTransform(hTransform);
            }
            
            cmsCloseProfile(labProfile);
        }
        
        cmsCloseProfile(hProfile);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}