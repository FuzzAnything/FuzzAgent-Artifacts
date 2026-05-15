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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"cmsGetNamedColorList":[[973,9,973,18,0,0,4,0]],"cmsMLUgetASCII":[[554,9,554,20,0,0,4,0]]},"library_calls":["cmsCreateDeviceLinkFromCubeFile","cmsGetProfileIOhandler","cmsGetHeaderAttributes","cmsGetNamedColorList","cmsNamedColorCount","cmsMLUgetASCII","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineFree","cmsFloat2XYZEncoded","cmsDesaturateLab","cmsIT8SaveToMem","cmsCloseProfile"],"critical_calls":["cmsCreateDeviceLinkFromCubeFile","cmsGetProfileIOhandler","cmsGetHeaderAttributes","cmsGetNamedColorList","cmsNamedColorCount","cmsMLUgetASCII","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineFree","cmsFloat2XYZEncoded","cmsDesaturateLab","cmsIT8SaveToMem","cmsCloseProfile"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_4, fuzz_uint16_t_array_size_4, fdp);
	
		const cmsUInt16Number * fuzzer_var_cmsPipelineEval16_7_0 = fuzz_uint16_t_array_4;//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for IT8 data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create device link from cube file
    // First write input data to a cube file
    FILE *cube_file = fopen("input.cube", "wb");
    if (cube_file) {
        fwrite(data, 1, size, cube_file);
        assert_file_closed(&cube_file);;
    }
    
    cmsHPROFILE hProfile = cmsCreateDeviceLinkFromCubeFile(fuzz_str_1);
    
    if (hProfile) {
        // 2. Get profile IO handler
        cmsIOHANDLER *io = cmsGetProfileIOhandler(hProfile);
        
        // 3. Get header attributes
        cmsUInt64Number flags = 0;
        cmsGetHeaderAttributes(hProfile, &flags);
        
        // 4. Get named color count (if profile has named colors)
        cmsNAMEDCOLORLIST *namedColors = cmsGetNamedColorList(NULL);
        if (namedColors) {
            cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
            (void)colorCount; // Use to avoid unused variable warning
        }
        
        // 5. Get MLU ASCII string (profile description)
        cmsMLU *mlu = NULL;
        char buffer[256];
        cmsUInt32Number bytesRead = cmsMLUgetASCII(mlu, fuzz_str_2, fuzz_str_3, buffer, sizeof(buffer));
        (void)bytesRead; // Use to avoid unused variable warning
        
        // 6. Create and evaluate pipeline
        // Create a simple pipeline for testing
        cmsPipeline *pipeline = cmsPipelineAlloc(0, 3, 3);
        if (pipeline) {
            cmsUInt16Number inColor[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number outColor[3] = {0};
            cmsPipelineEval16(fuzzer_var_cmsPipelineEval16_7_0, outColor, pipeline);
            cmsPipelineFree(pipeline);
        }
        
        // 7. Float to XYZ encoded conversion
        cmsCIEXYZ fXYZ = {0.5, 0.5, 0.5};
        cmsUInt16Number encodedXYZ[3];
        cmsFloat2XYZEncoded(encodedXYZ, &fXYZ);
        
        // 8. Desaturate Lab color
        cmsCIELab lab = {50.0, 20.0, 30.0};
        cmsBool desaturated = cmsDesaturateLab(&lab, 100.0, 0.0, 100.0, 0.0);
        (void)desaturated; // Use to avoid unused variable warning
        
        // 9. IT8 save to memory
        cmsHANDLE hIT8 = NULL;
        if (hIT8) {
            cmsUInt32Number bytesNeeded = 0;
            cmsBool saved = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
            (void)saved; // Use to avoid unused variable warning
        }
        
        // Cleanup
        if (io) {
            // IO handler cleanup if needed
        }
        cmsCloseProfile(hProfile);
    }
    
    // Clean up temporary files
    remove("input.cube");
    remove("output_file");
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&cube_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}