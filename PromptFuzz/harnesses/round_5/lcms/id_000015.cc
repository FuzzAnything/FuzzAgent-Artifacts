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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cmsGetProfileIOhandler":[[580,9,580,20,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsCreateNULLProfile","cmsDeleteContext","cmsSetHeaderManufacturer","cmsGetProfileIOhandler","cmsCloseProfile","cmsDeleteContext","cmsDictAlloc","cmsCloseProfile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsIT8Free","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsSetHeaderManufacturer","cmsGetProfileIOhandler","cmsDictAlloc","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsIT8GetPatchName","cmsIT8Free","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsDictFree","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create I/O handler from input file
    int fd = fuzz_fileno(in_file);
    if (fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create profile from I/O handler
    cmsHPROFILE profile = cmsCreateNULLProfile();
    if (!profile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set manufacturer using input data
    if (size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number manufacturer = *(cmsUInt32Number*)data;
        cmsSetHeaderManufacturer(profile, manufacturer);
    }

    // Get profile I/O handler
    cmsIOHANDLER *io_handler = cmsGetProfileIOhandler(profile);
    if (!io_handler) {
        cmsCloseProfile(profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Allocate dictionary
    cmsHANDLE dict = cmsDictAlloc(context);
    if (!dict) {
        cmsCloseProfile(profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Load IT8 data from memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)(size > 1024 ? 1024 : size));
    if (it8) {
        // Find data format
        const char *sample = "R";
        int format = cmsIT8FindDataFormat(it8, sample);
        
        // Get patch name
        char buffer[256];
        const char *patch_name = cmsIT8GetPatchName(it8, 0, buffer);
        
        // Free IT8 handle
        cmsIT8Free(it8);
    }

    // Create CLUT stage with float table
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number inputChan = 3;
    cmsUInt32Number outputChan = 3;
    
    // Create simple CLUT table
    cmsFloat32Number table[2*2*2*3] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };
    
    cmsStage *clut_stage = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, table);
    if (clut_stage) {
        // Create pipeline for evaluation
        cmsPipeline *pipeline = cmsPipelineAlloc(context, inputChan, outputChan);
        if (pipeline) {
            // Insert CLUT stage into pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
            
            // Prepare input and output arrays
            cmsFloat32Number input[3] = {0.5f, 0.5f, 0.5f};
            cmsFloat32Number output[3];
            
            // Evaluate pipeline
            cmsPipelineEvalFloat(input, output, pipeline);
            
            // Free pipeline (this also frees the inserted stage)
            cmsPipelineFree(pipeline);
        }
    }

    // Clean up
    cmsDictFree(dict);
    cmsCloseProfile(profile);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(context);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}