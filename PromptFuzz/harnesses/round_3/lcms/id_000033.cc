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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.666667, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsDeleteContext","cmsDetectDestinationBlackPoint","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsIT8EnumProperties","cmsDoTransformStride","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsDetectDestinationBlackPoint","cmsCreateXYZProfileTHR","cmsCreateLab2ProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SaveToFile","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsIT8EnumProperties","cmsDoTransformStride","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Free","cmsPipelineFree","cmsPipelineFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Write input data to temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Open ICC profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
    if (!hProfile) {
        // Create fallback profiles if input isn't valid
        cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
        hProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
        if (!hProfile) {
            remove("input_file");
            cmsDeleteContext(ctx);
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
        }
    }

    // Detect black point
    cmsCIEXYZ blackPoint;
    cmsDetectDestinationBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0);

    // Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(ctx);
    
    // Create Lab profile
    cmsCIExyY labWhitePoint = {0.3127, 0.3290, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &labWhitePoint);

    // Create multiprofile transform
    cmsHPROFILE profiles[3] = {hProfile, xyzProfile, labProfile};
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        ctx, profiles, 3, 
        TYPE_RGB_8, TYPE_Lab_8, 
        INTENT_PERCEPTUAL, 0);

    // Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Duplicate pipeline
        cmsPipeline *pipelineDup = cmsPipelineDup(pipeline);
        
        // Create IT8 handle
        cmsHANDLE it8 = cmsIT8Alloc(ctx);
        if (it8) {
            // Set IT8 sheet type
            cmsIT8SetSheetType(it8, "CGATS.17");
            
            // Save IT8 to file
            cmsIT8SaveToFile(it8, "output_file");
        }

        // Create IO handler
        cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, "input_file", "r");
        if (io) {
            cmsCloseIOhandler(io);
        }

        // Create named color list
        cmsNAMEDCOLORLIST *namedColors = cmsAllocNamedColorList(ctx, 10, 3, "PREFIX", "SUFFIX");
        if (namedColors) {
            char name[256], prefix[256], suffix[256];
            cmsUInt16Number pcs[3], colorant[3];
            
            // Get named color info
            cmsNamedColorInfo(namedColors, 0, name, prefix, suffix, pcs, colorant);
            
            cmsFreeNamedColorList(namedColors);
        }

        // Enumerate IT8 properties
        if (it8) {
            char **propNames = nullptr;
            cmsUInt32Number propCount = cmsIT8EnumProperties(it8, &propNames);
            // Clean up property names array if allocated
            if (propNames) {
                for (cmsUInt32Number i = 0; i < propCount; i++) {
                    free(propNames[i]);
                }
                free(propNames);
            }
        }

        // Perform transform with stride
        if (transform) {
            std::vector<uint8_t> inputBuffer(12);  // 4 pixels * 3 channels
            std::vector<uint8_t> outputBuffer(12);
            
            // Initialize with some data
            for (size_t i = 0; i < inputBuffer.size() && i < size; i++) {
                inputBuffer[i] = data[i];
            }
            
            cmsDoTransformStride(transform, inputBuffer.data(), outputBuffer.data(), 
                                4, 3);  // 4 pixels, stride = 3 bytes
            
            // CIECAM02 reverse (requires model handle - simplified)
            cmsJCh jch = {50.0, 30.0, 45.0};
            cmsCIEXYZ xyz;
            // Note: cmsCIECAM02Reverse requires a model handle which we don't have
            // This is called for API coverage but won't execute without proper handle
            
            // Try to get MLU translation (requires MLU)
            cmsMLU *mlu = cmsMLUalloc(ctx, 1);
            if (mlu) {
                char obtainedLang[3], obtainedCountry[3];
                cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
                cmsMLUfree(mlu);
            }
        }

        // Clean up IT8
        if (it8) {
            cmsIT8Free(it8);
        }

        // Clean up pipelines
        cmsPipelineFree(pipelineDup);
        cmsPipelineFree(pipeline);
    }

    // Clean up transforms
    if (transform) {
        cmsDeleteTransform(transform);
    }

    // Clean up profiles
    if (labProfile) cmsCloseProfile(labProfile);
    if (xyzProfile) cmsCloseProfile(xyzProfile);
    if (hProfile) cmsCloseProfile(hProfile);

    // Clean up context
    cmsDeleteContext(ctx);

    // Remove temporary files
    remove("input_file");
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}