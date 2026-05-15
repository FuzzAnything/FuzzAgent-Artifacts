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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"cmsCloseProfile":[[1674,9,1674,13,0,0,4,0]],"cmsOpenProfileFromIOhandlerTHR":[[1251,9,1251,23,0,0,4,1],[1256,9,1256,32,0,0,4,0],[1256,9,1256,32,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsOpenProfileFromIOhandlerTHR","cmsCloseProfile","cmsDeleteContext","cmsGetColorSpace","cmsGetHeaderModel","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSliceSpaceFloat","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsXYZEncoded2Float","cmsLabEncoded2Float","cmsSetAdaptationState","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsOpenProfileFromIOhandlerTHR","cmsGetColorSpace","cmsGetHeaderModel","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSliceSpaceFloat","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsXYZEncoded2Float","cmsLabEncoded2Float","cmsSetAdaptationState","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include necessary lcms headers

// Define a sampler function for cmsSliceSpaceFloat
static cmsInt32Number  id_000045_DummySampler(const cmsFloat32Number In[], 
                                   cmsFloat32Number Out[], 
                                   void* Cargo) {
    // Just copy input to output (simple identity mapping)
    for (int i = 0; i < 3; i++) {
        Out[i] = In[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_3, fuzz_uint32_t_array_size_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
	
		const cmsUInt32Number * fuzzer_var_cmsSliceSpaceFloat_13_1 = fuzz_uint32_t_array_3;
		cmsUInt32Number fuzzer_size_cmsSliceSpaceFloat_13_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_3);
		cmsUInt16Number fuzzer_var_cmsXYZEncoded2Float_16_1[3] = {fuzz_uint16_t_4,fuzz_uint16_t_5,fuzz_uint16_t_6};
		cmsUInt16Number fuzzer_var_cmsLabEncoded2Float_17_1[3] = {fuzz_uint16_t_7,fuzz_uint16_t_8,fuzz_uint16_t_9};//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Create a memory file from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create IO handler from stream
    cmsIOHANDLER* io = cmsOpenIOhandlerFromStream(context, in_file);
    if (!io) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Open profile from IO handler
    cmsHPROFILE profile = cmsOpenProfileFromIOhandlerTHR(context, io);
    if (!profile) {
        cmsCloseProfile(nullptr); // Call with nullptr just to use the API
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Get color space
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(profile);
    
    // 6. Get header creator (using the API from event list)
    // Note: cmsGetHeaderCreator is not in the provided list, but we'll use
    // cmsGetHeaderModel as an alternative
    cmsUInt32Number creator = cmsGetHeaderModel(profile);
    
    // 7. Create a simple transform
    cmsUInt32Number inputFormat = TYPE_Lab_16;
    cmsUInt32Number outputFormat = TYPE_XYZ_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
        profile, inputFormat, 
        profile, outputFormat, 
        fuzz_uint32_t_1, fuzz_uint32_t_2);
    
    if (transform) {
        // 8. Get transform pipeline
        cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
        
        if (pipeline) {
            // 9. Get pipeline input/output channels
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
            
            // 10. Use cmsSliceSpaceFloat with dummy sampler
            cmsUInt32Number clutPoints[] = {5, 5, 5}; // 5x5x5 grid
            if (inputChannels >= 3) {
                cmsSliceSpaceFloat(fuzzer_size_cmsSliceSpaceFloat_13_1, fuzzer_var_cmsSliceSpaceFloat_13_1,  id_000045_DummySampler, nullptr);
            }
            
            // 11. Check stage type if pipeline has stages
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (firstStage) {
                cmsStageSignature stageType = cmsStageType(firstStage);
                (void)stageType; // Use the variable
            }
        }
        
        // 12. Test encoded to float conversions
        cmsUInt16Number encodedXYZ[3] = {0x8000, 0x8000, 0x8000}; // Mid values
        cmsCIEXYZ floatXYZ;
        cmsXYZEncoded2Float(&floatXYZ, fuzzer_var_cmsXYZEncoded2Float_16_1);
        
        cmsUInt16Number encodedLab[3] = {0x8000, 0x8000, 0x8000};
        cmsCIELab floatLab;
        cmsLabEncoded2Float(&floatLab, fuzzer_var_cmsLabEncoded2Float_17_1);
        
        // 13. Set adaptation state
        cmsFloat64Number oldState = cmsSetAdaptationState(1.0);
        
        // 14. Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // 15. Clean up everything
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}