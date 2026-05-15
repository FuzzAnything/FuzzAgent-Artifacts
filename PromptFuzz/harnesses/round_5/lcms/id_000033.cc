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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 47
//<Quality> {"density":12,"unique_branches":{"cmsStageSampleCLutFloat":[[823,9,823,20,0,0,4,1],[827,9,827,21,0,0,4,1],[833,9,833,21,0,0,4,1],[834,9,834,22,0,0,4,1],[835,9,835,40,0,0,4,1],[836,9,836,39,0,0,4,1],[839,9,839,26,0,0,4,1],[842,17,842,38,0,0,4,0],[842,17,842,38,0,0,4,1],[845,35,845,40,0,0,4,0],[845,35,845,40,0,0,4,1],[854,13,854,38,0,0,4,0],[855,23,855,41,0,0,4,0],[855,23,855,41,0,0,4,1],[859,13,859,37,0,0,4,1],[862,13,862,41,0,0,4,0],[864,17,864,42,0,0,4,0],[865,27,865,45,0,0,4,0],[865,27,865,45,0,0,4,1]],"cmsStageAllocCLutFloat":[[635,15,635,39,0,0,4,0],[635,15,635,39,0,0,4,1]],"LocatePatch":[[2756,15,2756,31,0,0,4,1]],"cmsDesaturateLab":[[522,9,522,21,0,0,4,1],[532,9,532,21,0,0,4,1],[537,9,537,24,0,0,4,0],[537,9,537,24,0,0,4,1],[537,28,537,41,0,0,4,0],[537,28,537,41,0,0,4,1],[538,9,538,24,0,0,4,0],[538,9,538,24,0,0,4,1],[538,28,538,41,0,0,4,0],[538,28,538,41,0,0,4,1],[547,17,547,32,0,0,4,1],[561,18,561,25,0,0,4,0],[561,29,561,36,0,0,4,0],[561,29,561,36,0,0,4,1],[562,18,562,26,0,0,4,0],[562,18,562,26,0,0,4,1],[562,30,562,39,0,0,4,0],[569,21,569,29,0,0,4,0],[569,33,569,41,0,0,4,0],[569,33,569,41,0,0,4,1],[576,25,576,34,0,0,4,0],[576,38,576,46,0,0,4,0],[576,38,576,46,0,0,4,1],[583,29,583,38,0,0,4,0],[583,42,583,50,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPatchByName","cmsIT8SetDataRowColDbl","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsStageSampleCLutFloat","cmsDesaturateLab","cmsPipelineFree","cmsStageFree","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8GetPatchByName","cmsIT8SetDataRowColDbl","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsStageSampleCLutFloat","cmsDesaturateLab","cmsPipelineFree","cmsIT8Free","cmsDeleteContext"],"visited":0}
/*Looking at the APIs provided, I can see they involve IT8 color tables, CLUT sampling, and color desaturation. Here's a fuzz driver that uses all the specified APIs:

*/


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





    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }
    
    // Write input data to a temporary file for IT8 loading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHANDLE it8Handle = nullptr;
    cmsStage *stage = nullptr;
    cmsPipeline *pipeline = nullptr;
    
    // Try to create an IT8 handle from the input data
    // First, save input to a file that can be read by IT8 functions
    FILE *temp_it8_file = fopen("input_file", "wb");
    if (temp_it8_file) {
        fwrite(data, 1, size, temp_it8_file);
        assert_file_closed(&temp_it8_file);;
    }
    
    // Create a simple IT8 handle (in real usage, we'd load from file)
    // For fuzzing, we create a basic handle
    it8Handle = cmsIT8Alloc(context);
    
    if (it8Handle) {
        // 1. Set table by label
        cmsIT8SetTableByLabel(it8Handle, "DATA_FORMAT", "SAMPLE_ID", "STRING");
        
        // 2. Get patch by name (this might fail if patch doesn't exist, but that's OK)
        int patchRow = cmsIT8GetPatchByName(it8Handle, "test_patch");
        
        // 3. Set data at specific row and column if we got a valid row
        if (patchRow >= 0) {
            // Use some data from input for the value
            double val = size > 0 ? (double)data[0] / 255.0 : 0.5;
            cmsIT8SetDataRowColDbl(it8Handle, patchRow, 0, val);
        }
    }
    
    // Create a simple pipeline with a CLUT stage for testing
    // Create a 3x3x3 CLUT for RGB to RGB transformation
    const cmsUInt32Number clutPoints[] = {3, 3, 3};
    const cmsUInt32Number inputChannels = 3;
    const cmsUInt32Number outputChannels = 3;
    
    // Allocate memory for CLUT table
    size_t tableSize = 3 * 3 * 3 * outputChannels;
    cmsFloat32Number *clutTable = (cmsFloat32Number*)calloc(tableSize, sizeof(cmsFloat32Number));
    
    if (clutTable) {
        // Fill CLUT table with some data from input
        for (size_t i = 0; i < tableSize && i < size; i++) {
            clutTable[i] = (cmsFloat32Number)data[i] / 255.0f;
        }
        
        // Create the CLUT stage
        stage = cmsStageAllocCLutFloat(context, 3, inputChannels, outputChannels, clutTable);
        free(clutTable);
    }
    
    if (stage) {
        // Create a simple pipeline
        pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
        if (pipeline) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            
            // 4. Sample the CLUT
            // Create a sampler function
            auto sampler = [](const cmsFloat32Number in[], cmsFloat32Number out[], void* cargo) -> cmsInt32Number {
                // Simple sampler that passes through the input
                for (int i = 0; i < 3; i++) {
                    out[i] = in[i];
                }
                return 1;
            };
            
            // Sample the CLUT
            cmsStageSampleCLutFloat(stage, sampler, nullptr, 0);
        }
    }
    
    // 5. Test desaturation of Lab color
    cmsCIELab labColor;
    if (size >= 24) { // Need enough data for Lab values and bounds
        // Extract Lab values from input data
        labColor.L = ((double)data[0] / 255.0) * 100.0;          // L: 0-100
        labColor.a = ((double)data[1] / 255.0) * 200.0 - 100.0;  // a: -100 to +100
        labColor.b = ((double)data[2] / 255.0) * 200.0 - 100.0;  // b: -100 to +100
        
        // Extract bounds from input data
        double amax = ((double)data[3] / 255.0) * 200.0 - 100.0;
        double amin = ((double)data[4] / 255.0) * 200.0 - 100.0;
        double bmax = ((double)data[5] / 255.0) * 200.0 - 100.0;
        double bmin = ((double)data[6] / 255.0) * 200.0 - 100.0;
        
        // Apply desaturation
        cmsDesaturateLab(&labColor, amax, amin, bmax, bmin);
    }
    
    // Clean up resources
    if (pipeline) {
        cmsPipelineFree(pipeline);
    } else if (stage) {
        cmsStageFree(stage);
    }
    
    if (it8Handle) {
        cmsIT8Free(it8Handle);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_it8_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}