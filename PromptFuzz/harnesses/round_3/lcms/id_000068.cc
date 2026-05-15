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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 240, nr_unique_branch: 11
//<Quality> {"density":30,"unique_branches":{"_cmsICCcolorSpace":[[778,8,778,19,0,0,4,0]],"cmsGetPostScriptCRD":[[1561,9,1561,23,0,0,4,0]],"cmsCreateExtendedTransform":[[1271,13,1271,39,0,0,4,0]],"GenerateCRD":[[1485,9,1485,51,0,0,4,1],[1507,9,1507,51,0,0,4,1]],"AllocEmptyTransform":[[984,21,984,51,0,0,4,0]],"WriteOutputLUT":[[1375,9,1375,51,0,0,4,1]],"TransformOnePixelWithGamutCheck":[[450,9,450,25,0,0,4,0],[450,9,450,25,0,0,4,1],[455,19,455,46,0,0,4,0],[455,19,455,46,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsDeleteContext","cmsDeleteContext","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateProofingTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsGetPostScriptCRD","cmsGetPostScriptCRD","cmsStageFree","cmsStageFree","cmsLinkTag","cmsPluginTHR","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsCreate_sRGBProfileTHR","cmsD50_XYZ","cmsXYZ2xyY","cmsCreateLab2Profile","cmsCreateXYZProfile","cmsOpenProfileFromFileTHR","cmsCreateProofingTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsGetPostScriptCRD","cmsGetPostScriptCRD","cmsStageFree","cmsStageFree","cmsLinkTag","cmsPluginTHR","cmsxyY2XYZ","cmsDeleteTransform","cmsCloseProfile","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Duplicate context
    cmsContext dupContext = cmsDupContext(context, nullptr);
    if (!dupContext) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create profiles
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    if (!srgbProfile) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        return 0;
    }
    
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    cmsCIExyY d50xyY;
    cmsXYZ2xyY(&d50xyY, d50);
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&d50xyY);
    
    cmsHPROFILE xyzProfile = cmsCreateXYZProfile();
    
    // Write input data to file
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (labProfile) cmsCloseProfile(labProfile);
        if (xyzProfile) cmsCloseProfile(xyzProfile);
        cmsCloseProfile(srgbProfile);
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from file
    cmsHPROFILE inputProfile = cmsOpenProfileFromFileTHR(dupContext, "input_file", "r");
    if (inputProfile) {
        // Use the opened profile for transform
        cmsHTRANSFORM transform = cmsCreateProofingTransform(
            inputProfile, TYPE_RGB_8,
            xyzProfile ? xyzProfile : srgbProfile, TYPE_XYZ_16,
            labProfile ? labProfile : srgbProfile,
            INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETRIC,
            cmsFLAGS_GAMUTCHECK | cmsFLAGS_SOFTPROOFING
        );
        
        if (transform) {
            // Get pipeline and first stage
            cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStageSignature stageType = cmsStageType(firstStage);
                    
                    // Allocate CLUT stages
                    cmsUInt32Number gridPoints = 2;
                    std::vector<cmsFloat32Number> table1(gridPoints * gridPoints * gridPoints * 3);
                    for (size_t i = 0; i < table1.size(); ++i) {
                        table1[i] = (i % 256) / 255.0f;
                    }
                    
                    cmsStage* clutStage1 = cmsStageAllocCLutFloat(
                        context, gridPoints, 3, 3, table1.data()
                    );
                    
                    // Allocate granular CLUT
                    cmsUInt32Number clutPoints[] = {2, 3, 2};
                    std::vector<cmsFloat32Number> table2(2 * 3 * 2 * 3);
                    for (size_t i = 0; i < table2.size(); ++i) {
                        table2[i] = ((i + 128) % 256) / 255.0f;
                    }
                    
                    cmsStage* clutStage2 = cmsStageAllocCLutFloatGranular(
                        context, clutPoints, 3, 3, table2.data()
                    );
                    
                    // Get PostScript CRD
                    cmsUInt32Number bufferSize = cmsGetPostScriptCRD(
                        context, srgbProfile, INTENT_PERCEPTUAL,
                        cmsFLAGS_NODEFAULTRESOURCEDEF, nullptr, 0
                    );
                    
                    if (bufferSize > 0) {
                        std::vector<char> buffer(bufferSize);
                        cmsGetPostScriptCRD(
                            context, srgbProfile, INTENT_PERCEPTUAL,
                            cmsFLAGS_NODEFAULTRESOURCEDEF, buffer.data(), bufferSize
                        );
                        
                        // Write to output file
                        FILE* out_file = fopen("output_file", "wb");
                        if (out_file) {
                            fwrite(buffer.data(), 1, buffer.size(), out_file);
                            assert_file_closed(&out_file);;
                        }
                    }
                    
                    // Clean up CLUT stages
                    if (clutStage1) cmsStageFree(clutStage1);
                    if (clutStage2) cmsStageFree(clutStage2);
                }
            }
            
            // Link a tag (example: link mediaWhitePointTag to mediaBlackPointTag)
            cmsLinkTag(srgbProfile, cmsSigMediaWhitePointTag, cmsSigMediaBlackPointTag);
            
            // Plugin (dummy call)
            cmsPluginTHR(context, nullptr);
            
            // Convert xyY to XYZ
            cmsCIExyY source = {0.3457, 0.3585, 1.0};
            cmsCIEXYZ dest;
            cmsxyY2XYZ(&dest, &source);
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(inputProfile);
    }
    
    // Create dummy named color list for cleanup demonstration
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    // Note: In real usage, you'd create this with proper API
    cmsFreeNamedColorList(namedColorList);
    
    // Clean up profiles
    if (labProfile) cmsCloseProfile(labProfile);
    if (xyzProfile) cmsCloseProfile(xyzProfile);
    if (srgbProfile) cmsCloseProfile(srgbProfile);
    
    // Clean up file
    if (in_file) assert_file_closed(&in_file);;
    
    // Clean up contexts
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}