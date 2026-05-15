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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 2
//<Quality> {"density":35,"unique_branches":{"cmsFreeToneCurveTriple":[[960,9,960,25,0,0,4,1],[961,9,961,25,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsIsTag","cmsIsTag","cmsIsTag","cmsIsTag","cmsReadTag","cmsGetProfileInfo","cmsSetHeaderManufacturer","cmsBuildSegmentedToneCurve","cmsIsToneCurveMonotonic","cmsBuildSegmentedToneCurve","cmsBuildSegmentedToneCurve","cmsIT8LoadFromMem","cmsIT8GetDataRowCol","cmsMLUalloc","cmsMLUsetWide","cmsCreateNULLProfileTHR","cmsCreateTransform","cmsChangeBuffersFormat","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsStageNext","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsPipelineInsertStage","cmsSliceSpaceFloat","cmsPipelineFree","cmsDoTransform","cmsEvalToneCurve16","cmsDeleteTransform","cmsCloseProfile","cmsSaveProfileToFile","cmsCloseProfile","cmsIT8Free","cmsMLUfree","cmsFreeToneCurveTriple","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsIsTag","cmsIsTag","cmsIsTag","cmsIsTag","cmsReadTag","cmsGetProfileInfo","cmsSetHeaderManufacturer","cmsBuildSegmentedToneCurve","cmsIsToneCurveMonotonic","cmsBuildSegmentedToneCurve","cmsBuildSegmentedToneCurve","cmsIT8LoadFromMem","cmsIT8GetDataRowCol","cmsMLUalloc","cmsMLUsetWide","cmsCreateNULLProfileTHR","cmsCreateTransform","cmsChangeBuffersFormat","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsGetStageContextID","cmsStageNext","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageAllocCLutFloat","cmsPipelineInsertStage","cmsSliceSpaceFloat","cmsPipelineFree","cmsDoTransform","cmsEvalToneCurve16","cmsDeleteTransform","cmsCloseProfile","cmsSaveProfileToFile","cmsCloseProfile","cmsIT8Free","cmsMLUfree","cmsFreeToneCurveTriple","cmsDeleteContext"],"visited":0}
/*To create a fuzz driver that comprehensively tests the lcms library using the provided APIs, I'll implement a scenario that processes color profile data, manipulates color transformations, and validates various color management operations. Here's the complete implementation:

*/


// lcms headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // Initialize context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Split data for different purposes
    size_t split_point = std::min(size/2, size - 50);
    const uint8_t* profile_data = data;
    size_t profile_size = split_point;
    const uint8_t* it8_data = data + split_point;
    size_t it8_size = size - split_point;
    
    // Variables to track allocations for cleanup
    cmsHPROFILE hProfile = nullptr;
    cmsHANDLE hIT8 = nullptr;
    cmsMLU* mlu = nullptr;
    cmsHTRANSFORM hTransform = nullptr;
    cmsPipeline* pipeline = nullptr;
    cmsToneCurve* curves[3] = {nullptr, nullptr, nullptr};
    cmsStage* firstStage = nullptr;
    
    // 1. Create profile from memory
    hProfile = cmsOpenProfileFromMemTHR(context, profile_data, profile_size);
    
    if (hProfile) {
        // 2. Check for various tags
        cmsBool hasTag = cmsIsTag(hProfile, cmsSigMediaBlackPointTag);
        hasTag = cmsIsTag(hProfile, cmsSigCopyrightTag);
        hasTag = cmsIsTag(hProfile, cmsSigProfileDescriptionTag);
        
        // 3. Read a tag if present
        if (cmsIsTag(hProfile, cmsSigProfileDescriptionTag)) {
            void* tagData = cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
            // tagData could be various types, handle carefully
        }
        
        // 4. Get profile information
        wchar_t buffer[256];
        cmsUInt32Number infoSize = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", 
                                                     buffer, sizeof(buffer)/sizeof(wchar_t));
        
        // 5. Set manufacturer
        cmsSetHeaderManufacturer(hProfile, 0x12345678);
        
        // 6. Create segmented tone curve
        cmsCurveSegment segments[1];
        if (size >= sizeof(cmsCurveSegment) + 100) {
            segments[0].x0 = 0.0f;
            segments[0].x1 = 1.0f;
            segments[0].Type = 1; // Linear segment
            segments[0].Params[0] = 1.0;
            segments[0].Params[1] = 0.0;
            segments[0].nGridPoints = 256;
            segments[0].SampledPoints = nullptr;
            
            curves[0] = cmsBuildSegmentedToneCurve(context, 1, segments);
            if (curves[0]) {
                // 7. Check if tone curve is monotonic
                cmsBool isMonotonic = cmsIsToneCurveMonotonic(curves[0]);
                
                // Fill remaining curves for cleanup
                if (size > sizeof(cmsCurveSegment) * 3 + 200) {
                    curves[1] = cmsBuildSegmentedToneCurve(context, 1, segments);
                    curves[2] = cmsBuildSegmentedToneCurve(context, 1, segments);
                }
            }
        }
        
        // 8. Load IT8 data
        if (it8_size > 50) {
            hIT8 = cmsIT8LoadFromMem(context, it8_data, it8_size);
            
            if (hIT8) {
                // 9. Get data from IT8
                const char* cellData = cmsIT8GetDataRowCol(hIT8, 0, 0);
                
                // 10. Create MLU
                mlu = cmsMLUalloc(context, 10);
                if (mlu) {
                    wchar_t wideStr[] = L"Test String";
                    cmsBool mluSet = cmsMLUsetWide(mlu, "en", "US", wideStr);
                }
            }
        }
        
        // 11. Create NULL profile
        cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
        
        if (nullProfile) {
            // 12. Create transform
            hTransform = cmsCreateTransform(nullProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, 
                                           INTENT_PERCEPTUAL, 0);
            
            if (hTransform) {
                // 13. Change buffer format
                cmsBool formatChanged = cmsChangeBuffersFormat(hTransform, TYPE_RGB_FLT, TYPE_RGB_FLT);
                
                // 14. Get transform pipeline
                pipeline = cmsGetTransformPipeline(hTransform);
                
                if (pipeline) {
                    // 15. Get first stage
                    firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                    
                    if (firstStage) {
                        // 16. Get stage context ID
                        cmsContext stageContext = cmsGetStageContextID(firstStage);
                        
                        // 17. Traverse stages
                        cmsStage* currentStage = firstStage;
                        while (currentStage) {
                            currentStage = cmsStageNext(currentStage);
                            if (!currentStage) break;
                        }
                        
                        // 18. Create and manipulate custom pipeline
                        cmsPipeline* customPipeline = cmsPipelineAlloc(context, 3, 3);
                        if (customPipeline) {
                            // 19. Create identity stage
                            cmsStage* identityStage = cmsStageAllocIdentity(context, 3);
                            
                            if (identityStage) {
                                // 20. Insert stage into pipeline
                                cmsBool inserted = cmsPipelineInsertStage(customPipeline, 
                                                                         cmsAT_BEGIN, 
                                                                         identityStage);
                                
                                // 21. Create CLUT stage
                                std::vector<cmsFloat32Number> clutTable;
                                size_t tableSize = std::min((size_t)27, size/4); // 3^3 grid
                                clutTable.resize(tableSize * 3, 0.5f); // Fill with 0.5
                                
                                cmsStage* clutStage = cmsStageAllocCLutFloat(context, 3, 3, 3, 
                                                                             clutTable.data());
                                if (clutStage && inserted) {
                                    cmsPipelineInsertStage(customPipeline, cmsAT_END, clutStage);
                                }
                            }
                            
                            // 22. Perform slice space operation
                            cmsUInt32Number clutPoints[] = {3, 3, 3};
                            auto sampler = [](const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) {
                                for (int i = 0; i < 3; i++) {
                                    out[i] = in[i] * 0.5f;
                                }
                                return 1;
                            };
                            
                            cmsBool sliced = cmsSliceSpaceFloat(3, clutPoints, sampler, nullptr);
                            
                            // 23. Free custom pipeline
                            cmsPipelineFree(customPipeline);
                        }
                    }
                }
                
                // 24. Perform transformation
                if (size >= 12) { // Need at least 3 RGB pixels
                    std::vector<uint8_t> inputBuffer(12, 128);
                    std::vector<uint8_t> outputBuffer(12, 0);
                    
                    // Copy some data for input
                    size_t copySize = std::min((size_t)12, size);
                    memcpy(inputBuffer.data(), data, copySize);
                    
                    cmsDoTransform(hTransform, inputBuffer.data(), outputBuffer.data(), 1);
                    
                    // 25. Evaluate tone curve
                    if (curves[0]) {
                        cmsUInt16Number evalResult = cmsEvalToneCurve16(curves[0], 0x8000);
                    }
                }
                
                // 26. Delete transform
                cmsDeleteTransform(hTransform);
            }
            
            // 27. Close NULL profile
            cmsCloseProfile(nullProfile);
        }
        
        // 28. Save profile to file
        cmsBool saved = cmsSaveProfileToFile(hProfile, "output_file");
        
        // 29. Close main profile
        cmsCloseProfile(hProfile);
    }
    
    // 30. Clean up IT8
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    // 31. Clean up MLU
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    // 32. Clean up tone curves
    if (curves[0] || curves[1] || curves[2]) {
        cmsFreeToneCurveTriple(curves);
    }
    
    // 33. Delete context
    cmsDeleteContext(context);
    
    return 0;
}