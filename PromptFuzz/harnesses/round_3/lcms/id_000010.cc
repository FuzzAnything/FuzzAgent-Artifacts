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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 53.333332, nr_unique_branch: 46
//<Quality> {"density":16,"unique_branches":{"AllCurvesAreLinear":[[457,9,457,23,0,0,4,1],[461,15,461,20,0,0,4,0],[461,15,461,20,0,0,4,1],[462,13,462,45,0,0,4,0],[462,13,462,45,0,0,4,1]],"_MultiplyMatrix":[[228,26,228,54,0,0,4,1]],"CurvesFree":[[1273,16,1273,35,0,0,4,0],[1273,16,1273,35,0,0,4,1]],"CurvesAlloc":[[1307,9,1307,20,0,0,4,1],[1313,9,1313,28,0,0,4,1],[1318,15,1318,26,0,0,4,0],[1318,15,1318,26,0,0,4,1],[1322,13,1322,35,0,0,4,1],[1332,13,1332,30,0,0,4,0],[1334,23,1334,36,0,0,4,0],[1334,23,1334,36,0,0,4,1]],"FastIdentity16":[[1389,15,1389,38,0,0,4,0],[1389,15,1389,38,0,0,4,1]],"isFloatMatrixIdentity":[[180,20,180,25,0,0,4,1],[181,27,181,32,0,0,4,1],[182,26,182,77,0,0,4,1]],"OptimizeByJoiningCurves":[[1414,10,1414,21,0,0,4,1],[1421,9,1421,21,0,0,4,1],[1425,9,1425,28,0,0,4,1],[1427,15,1427,38,0,0,4,0],[1427,15,1427,38,0,0,4,1],[1429,13,1429,35,0,0,4,1],[1433,15,1433,42,0,0,4,0],[1433,15,1433,42,0,0,4,1],[1435,19,1435,42,0,0,4,0],[1435,19,1435,42,0,0,4,1],[1440,19,1440,42,0,0,4,0],[1440,19,1440,42,0,0,4,1],[1445,9,1445,31,0,0,4,1],[1447,15,1447,38,0,0,4,0],[1447,15,1447,38,0,0,4,1],[1452,9,1452,28,0,0,4,0],[1458,9,1458,44,0,0,4,0],[1458,9,1458,44,0,0,4,1],[1461,13,1461,71,0,0,4,1],[1467,13,1467,46,0,0,4,0],[1470,18,1470,29,0,0,4,1],[1489,13,1489,117,0,0,4,1]],"FastEvaluateCurves8":[[1359,15,1359,33,0,0,4,0],[1359,15,1359,33,0,0,4,1]],"cmsGetNamedColorList":[[973,9,973,18,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsPipelineFree","cmsPipelineFree","cmsIT8Alloc","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsDoTransformStride","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCloseIOhandler","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsPipelineFree","cmsPipelineFree","cmsIT8Alloc","cmsIT8SaveToFile","cmsIT8EnumProperties","cmsIT8Free","cmsOpenIOhandlerFromFile","cmsDoTransformStride","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsCloseIOhandler","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Open profile from file using cmsOpenProfileFromFileTHR
    // Note: cmsOpenProfileFromFileTHR is actually cmsOpenProfileFromFileTHR in lcms2
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!hProfile) {
        // Create a fallback profile if file open fails
        hProfile = cmsCreate_sRGBProfileTHR(context);
    }
    
    // 2. Create multiprofile transform
    cmsHPROFILE profiles[] = {hProfile, hProfile};
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, 
        profiles, 
        2, 
        TYPE_RGB_8, 
        TYPE_RGB_8, 
        intent, 
        flags
    );
    
    // 3. Duplicate pipeline if transform exists
    if (transform) {
        // Get internal pipeline (simulated - actual implementation would need to extract it)
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            cmsPipeline* dupPipeline = cmsPipelineDup(pipeline);
            if (dupPipeline) {
                cmsPipelineFree(dupPipeline);
            }
            cmsPipelineFree(pipeline);
        }
    }
    
    // 4. Create IT8 handle and save to file
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        cmsIT8SaveToFile(hIT8, "output_file");
        
        // 8. Enumerate IT8 properties
        char** propertyNames = NULL;
        cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propertyNames);
        if (propertyNames && propCount > 0) {
            free(propertyNames);
        }
        
        cmsIT8Free(hIT8);
    }
    
    // 5. Open IO handler from file
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromFile(context, "input_file", "r");
    
    // 6. Perform color transformation if we have a valid transform
    if (transform && size >= 12) {
        // Prepare input and output buffers
        uint8_t inputBuffer[12];
        uint8_t outputBuffer[12];
        
        // Use first 12 bytes as input
        memcpy(inputBuffer, data, 12);
        
        // Perform transformation with stride
        cmsDoTransformStride(transform, inputBuffer, outputBuffer, 1, 12);
        
        // Write output to file
        if (out_file) {
            fwrite(outputBuffer, 1, 12, out_file);
        }
    }
    
    // 7. Get named color info if we have a profile
    if (hProfile) {
        cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(transform);
        if (namedColorList && cmsNamedColorCount(namedColorList) > 0) {
            char name[256];
            char prefix[256];
            char suffix[256];
            cmsUInt16Number pcs[3];
            cmsUInt16Number colorant[16];
            
            cmsNamedColorInfo(namedColorList, 0, name, prefix, suffix, pcs, colorant);
        }
    }
    
    // Cleanup
    if (ioHandler) {
        cmsCloseIOhandler(ioHandler);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}