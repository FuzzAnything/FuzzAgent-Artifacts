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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 8
//<Quality> {"density":12,"unique_branches":{"cmsReadTag":[[1749,13,1749,44,0,0,4,0]],"cmsFloat2XYZEncoded":[[383,9,383,19,0,0,4,1],[390,9,390,35,0,0,4,1],[393,9,393,18,0,0,4,1],[396,9,396,35,0,0,4,1],[399,9,399,18,0,0,4,1],[402,9,402,35,0,0,4,1],[405,9,405,18,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8SetDataDbl","cmsIT8GetPatchName","cmsOpenProfileFromFile","cmsWriteRawTag","cmsReadTag","cmsMLUgetUTF8","cmsFloat2XYZEncoded","cmsCloseProfile","cmsPipelineAlloc","cmsStageAllocCLut16bit","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsUnregisterPlugins","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8SetDataDbl","cmsIT8GetPatchName","cmsOpenProfileFromFile","cmsWriteRawTag","cmsReadTag","cmsMLUgetUTF8","cmsFloat2XYZEncoded","cmsCloseProfile","cmsPipelineAlloc","cmsStageAllocCLut16bit","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsUnregisterPlugins","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
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
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create IT8 handle and set data format
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set data format using part of input data as format string
        const char* format = "L*a*b*";
        cmsIT8SetDataFormat(hIT8, 0, format);
        
        // Add a patch and set double data
        cmsIT8SetDataDbl(hIT8, "Patch1", "L*", 50.5);
        
        // Get patch name
        char buffer[256];
        const char* name = cmsIT8GetPatchName(hIT8, 0, buffer);
    }
    
    // 2. Create a profile from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
        if (hProfile) {
            // Write raw tag using input data
            cmsWriteRawTag(hProfile, cmsSigCopyrightTag, data, 
                          size > 1024 ? 1024 : (cmsUInt32Number)size);
            
            // Read MLU tag and get UTF8 string
            cmsMLU* mlu = (cmsMLU*)cmsReadTag(hProfile, cmsSigCopyrightTag);
            if (mlu) {
                char utf8buf[256];
                cmsMLUgetUTF8(mlu, "en", "US", utf8buf, sizeof(utf8buf));
            }
            
            // Convert XYZ to encoded format
            cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
            cmsUInt16Number encodedXYZ[3];
            cmsFloat2XYZEncoded(encodedXYZ, &xyz);
            
            cmsCloseProfile(hProfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // 3. Create pipeline and stage operations
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create and link a CLUT stage
        cmsStage* stage = cmsStageAllocCLut16bit(ctx, 8, 3, 3, NULL);
        if (stage) {
            // Link stage then unlink it
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            cmsStage* unlinked = NULL;
            cmsPipelineUnlinkStage(pipeline, cmsAT_END, &unlinked);
            
            if (unlinked) {
                cmsStageFree(unlinked);
            }
        }
        cmsPipelineFree(pipeline);
    }
    
    // 4. Unregister plugins
    cmsUnregisterPlugins();
    
    // 5. Clean up IT8 handle
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);
	return 0;
}