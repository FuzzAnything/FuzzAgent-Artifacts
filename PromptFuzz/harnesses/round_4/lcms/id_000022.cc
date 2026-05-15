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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 240, nr_unique_branch: 26
//<Quality> {"density":20,"unique_branches":{"WriteData":[[1974,12,1974,20,0,0,4,0]],"AddConversion":[[481,5,481,12,0,0,4,0],[482,13,482,28,0,0,4,1]],"cmsGetProfileInfoASCII":[[1032,9,1032,20,0,0,4,1]],"WriteDataFormat":[[1948,9,1948,25,0,0,4,0]],"DefaultICCintents":[[583,18,583,49,0,0,4,1]],"cmsCreateExtendedTransform":[[1185,74,1185,106,0,0,4,0],[1189,13,1189,22,0,0,4,1],[1242,9,1242,69,0,0,4,0],[1245,13,1245,72,0,0,4,1]],"cmsGetNamedColorList":[[973,9,973,18,0,0,4,1],[974,9,974,23,0,0,4,1],[977,9,977,20,0,0,4,0]],"WriteStr":[[1819,9,1819,19,0,0,4,1],[1829,13,1829,21,0,0,4,0],[1829,13,1829,21,0,0,4,1],[1831,17,1831,35,0,0,4,1]],"cmsPipelineCat":[[1619,34,1619,55,0,0,4,0]],"cmsIT8SaveToMem":[[2068,9,2068,16,0,0,4,0],[2068,9,2068,16,0,0,4,1],[2068,20,2068,38,0,0,4,0],[2075,15,2075,36,0,0,4,0],[2075,15,2075,36,0,0,4,1],[2085,9,2085,16,0,0,4,0],[2085,9,2085,16,0,0,4,1]],"_cmsOptimizePipeline":[[1939,9,1939,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsDeleteContext","cmsGetProfileIOhandler","cmsGetHeaderAttributes","cmsCreateTransformTHR","cmsGetNamedColorList","cmsNamedColorCount","cmsDeleteTransform","cmsGetProfileInfoASCII","cmsDesaturateLab","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineFree","cmsFloat2XYZEncoded","cmsIT8Alloc","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetProfileIOhandler","cmsGetHeaderAttributes","cmsCreateTransformTHR","cmsGetNamedColorList","cmsNamedColorCount","cmsDeleteTransform","cmsGetProfileInfoASCII","cmsDesaturateLab","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineFree","cmsFloat2XYZEncoded","cmsIT8Alloc","cmsIT8SaveToMem","cmsIT8SaveToMem","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.cube", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    // Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Create device link profile from cube file
    // First, write input data to a temporary cube file
    FILE *cube_file = fopen("input_file.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    cmsHPROFILE profile = cmsCreateDeviceLinkFromCubeFileTHR(context, fuzz_str_1);
    if (!profile) {
        remove("input_file.cube");
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	assert_file_name_closed("input_file.cube");
	return 0;
    }

    // 2. Get profile IO handler
    cmsIOHANDLER *io_handler = cmsGetProfileIOhandler(profile);
    
    // 3. Get header attributes
    cmsUInt64Number flags;
    cmsGetHeaderAttributes(profile, &flags);
    
    // 4. Get named color count (create a transform first to get named color list)
    cmsUInt32Number input_format = TYPE_RGB_16;
    cmsUInt32Number output_format = TYPE_RGB_16;
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, profile, input_format, 
                                                   profile, output_format, 
                                                   fuzz_uint32_t_2, fuzz_uint32_t_3);
    
    cmsUInt32Number named_color_count = 0;
    if (transform) {
        cmsNAMEDCOLORLIST *named_color_list = cmsGetNamedColorList(transform);
        if (named_color_list) {
            named_color_count = cmsNamedColorCount(named_color_list);
        }
        cmsDeleteTransform(transform);
    }
    
    // 5. Get MLU ASCII string (try to get profile description)
    char buffer[256];
    cmsUInt32Number bytes_read = cmsGetProfileInfoASCII(profile, cmsInfoDescription, 
                                                       fuzz_str_4, fuzz_str_5, buffer, sizeof(buffer));
    
    // 6. Desaturate Lab color
    cmsCIELab lab = {50.0, 20.0, 30.0};  // Example Lab values
    cmsDesaturateLab(&lab, 100.0, 0.0, 100.0, -100.0);
    
    // 7. Pipeline evaluation (create a simple pipeline)
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        cmsUInt16Number in[3] = {0x8000, 0x8000, 0x8000};  // Mid values
        cmsUInt16Number out[3];
        cmsPipelineEval16(in, out, pipeline);
        cmsPipelineFree(pipeline);
    }
    
    // 8. Float to XYZ encoded conversion
    cmsCIEXYZ fxyz = {0.5, 0.5, 0.5};
    cmsUInt16Number encoded_xyz[3];
    cmsFloat2XYZEncoded(encoded_xyz, &fxyz);
    
    // 9. IT8 save to memory (create a simple IT8 handle)
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsUInt32Number bytes_needed = 0;
        cmsIT8SaveToMem(it8, NULL, &bytes_needed);  // Get required buffer size
        
        if (bytes_needed > 0 && bytes_needed < 1024 * 1024) {  // Reasonable size limit
            void *mem_buffer = malloc(bytes_needed);
            if (mem_buffer) {
                cmsIT8SaveToMem(it8, mem_buffer, &bytes_needed);
                free(mem_buffer);
            }
        }
        cmsIT8Free(it8);
    }
    
    // Cleanup
    cmsCloseProfile(profile);
    remove("input_file.cube");
    cmsDeleteContext(context);
    
    assert_file_closed(&cube_file);
	assert_file_name_closed("input_file.cube");
	return 0;
}