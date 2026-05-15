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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 1, nr_unique_branch: 0
//<Quality> {"density":1,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetData","cmsBuildGamma","cmsGetToneCurveEstimatedTableEntries","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateTransformTHR","cmsGetTransformOutputFormat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineEvalFloat","cmsMLUalloc","cmsMLUsetWide","cmsMLUtranslationsCount","cmsAdaptToIlluminant","cmsOpenProfileFromStream","cmsCloseProfile","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsMLUfree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsDeleteContext","cmsCloseProfile","cmsMLUfree","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteTransform","cmsPipelineFree","cmsCloseProfile","cmsOpenProfileFromStream","cmsAdaptToIlluminant","cmsMLUtranslationsCount","cmsMLUsetWide","cmsMLUalloc","cmsPipelineEvalFloat","cmsPipelineInsertStage","cmsStageAllocToneCurves","cmsPipelineAlloc","cmsGetTransformOutputFormat","cmsCreateTransformTHR","cmsCreateInkLimitingDeviceLinkTHR","cmsGetToneCurveEstimatedTableEntries","cmsBuildGamma","cmsIT8GetData","cmsIT8SetPropertyStr","cmsIT8Alloc","cmsCreateNULLProfile","cmsCreateContext"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use a try-catch block to handle any exceptions
    try {
        // Create a context for lcms operations
        cmsContext ctx = cmsCreateContext(nullptr, nullptr);
        if (!ctx) {
	return 0;
	}
	
        
        // 1. Create a NULL profile for basic operations
        cmsHPROFILE nullProfile = cmsCreateNULLProfile();
        
        // 2. Create an IT8 handle and use IT8-related APIs
        cmsHANDLE it8 = cmsIT8Alloc(ctx);
        if (it8) {
            // Set a property string using the first part of the input data
            if (size > 0) {
                std::string propName = "TestProperty";
                // Use a safe portion of the input data as property value
                size_t propValueSize = size > 100 ? 100 : size;
                std::string propValue(reinterpret_cast<const char*>(data), propValueSize);
                cmsIT8SetPropertyStr(it8, propName.c_str(), propValue.c_str());
                
                // Try to get data from IT8 (even though we just set a property)
                // Use a safe patch and sample name
                const char* patch = "Patch1";
                const char* sample = "Sample1";
                const char* retrievedData = cmsIT8GetData(it8, patch, sample);
                // Note: retrievedData may be nullptr as we didn't set this specific data
            }
        }
        
        // 3. Create a tone curve for testing
        cmsFloat64Number gamma = 2.2;
        cmsToneCurve* toneCurve = cmsBuildGamma(ctx, gamma);
        if (toneCurve) {
            // Get estimated table entries for the tone curve
            cmsUInt32Number tableEntries = cmsGetToneCurveEstimatedTableEntries(toneCurve);
            // Use the value to prevent optimization
            volatile cmsUInt32Number dummy = tableEntries;
        }
        
        // 4. Create an ink limiting device link profile
        // Use a reasonable ink limit derived from input data
        cmsFloat64Number inkLimit = 300.0;
        if (size > 0) {
            inkLimit = 200.0 + (data[0] % 100);  // Limit between 200-299
        }
        cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(
            ctx, cmsSigRgbData, inkLimit);
        
        // 5. Create a transform using the NULL profile and ink limit profile
        cmsHTRANSFORM transform = nullptr;
        if (nullProfile && inkLimitProfile) {
            transform = cmsCreateTransformTHR(ctx, 
                nullProfile, 
                TYPE_RGB_8, 
                inkLimitProfile, 
                TYPE_RGB_8, 
                INTENT_PERCEPTUAL, 
                0);
            
            if (transform) {
                // Get transform output format
                cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
                volatile cmsUInt32Number dummyFormat = outputFormat;
            }
        }
        
        // 6. Create and use a pipeline for evaluation
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
        if (pipeline) {
            // Add a simple gamma curve to the pipeline
            if (toneCurve) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, cmsStageAllocToneCurves(ctx, 1, &toneCurve));
            }
            
            // Prepare input/output for pipeline evaluation
            cmsFloat32Number input[3] = {0.5f, 0.5f, 0.5f};
            cmsFloat32Number output[3] = {0.0f, 0.0f, 0.0f};
            
            // Evaluate the pipeline
            cmsPipelineEvalFloat(input, output, pipeline);
            
            // Use the output to prevent optimization
            volatile float dummyOutput = output[0] + output[1] + output[2];
        }
        
        // 7. Create and use an MLU (Multi Localized Unicode) object
        cmsMLU* mlu = cmsMLUalloc(ctx, 5);  // Allocate for 5 entries
        if (mlu) {
            // Set some text in the MLU
            const wchar_t* wideText = L"Test String";
            cmsMLUsetWide(mlu, "en", "US", wideText);
            
            // Get translations count
            cmsUInt32Number translationsCount = cmsMLUtranslationsCount(mlu);
            volatile cmsUInt32Number dummyCount = translationsCount;
        }
        
        // 8. Use cmsAdaptToIlluminant with test values
        cmsCIEXYZ sourceWhitePt = {0.9505, 1.0000, 1.0890};  // D50
        cmsCIEXYZ illuminant = {0.9642, 1.0000, 0.8251};     // D65
        cmsCIEXYZ value = {0.5, 0.5, 0.5};
        cmsCIEXYZ result;
        
        cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
        
        // Use the result to prevent optimization
        volatile double dummyResult = result.X + result.Y + result.Z;
        
        // 9. Process input data through a file stream if there's enough data
        if (size > 0) {
            FILE* in_file = fmemopen((void*)data, size, "rb");
            if (in_file) {
                // Try to open a profile from the stream
                cmsHPROFILE streamProfile = cmsOpenProfileFromStream(in_file, "r");
                if (streamProfile) {
                    // Just open and close it
                    cmsCloseProfile(streamProfile);
                }
                assert_file_closed(&in_file);;
            }
        }
        
        // 10. Write output to a file if needed
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write something to the file
            if (size > 0) {
                fwrite(data, 1, size > 100 ? 100 : size, out_file);
            }
            assert_file_closed(&out_file);;
        }
        
        // Cleanup all allocated resources
        if (pipeline) cmsPipelineFree(pipeline);
        if (transform) cmsDeleteTransform(transform);
        if (inkLimitProfile) cmsCloseProfile(inkLimitProfile);
        if (toneCurve) cmsFreeToneCurve(toneCurve);
        if (it8) cmsIT8Free(it8);
        if (mlu) cmsMLUfree(mlu);
        if (nullProfile) cmsCloseProfile(nullProfile);
        
        cmsDeleteContext(ctx);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
        return 0;
    }
    
    return 0;
}