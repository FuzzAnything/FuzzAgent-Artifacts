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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"cmsCreateLab4ProfileTHR":[[530,9,530,27,0,0,4,1]],"Clamp_L_doubleV4":[[271,9,271,14,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateLab4Profile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8TableCount","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsFloat2LabEncoded","cmsLabEncoded2FloatV2","cmsPipelineFree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateLab4Profile","cmsIT8LoadFromMem","cmsIT8TableCount","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsFloat2LabEncoded","cmsLabEncoded2FloatV2","cmsPipelineFree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(cmsCIELab) + sizeof(cmsCIExyY) + sizeof(cmsUInt16Number) * 3) {
        return 0;
    }

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Parse input data into needed structures
    const cmsCIELab* lab_input = reinterpret_cast<const cmsCIELab*>(data);
    const cmsCIExyY* white_point = reinterpret_cast<const cmsCIExyY*>(data + sizeof(cmsCIELab));
    const cmsUInt16Number* encoded_input = reinterpret_cast<const cmsUInt16Number*>(data + sizeof(cmsCIELab) + sizeof(cmsCIExyY));

    // Create Lab profile using white point from input
    cmsHPROFILE lab_profile = cmsCreateLab4Profile(white_point);
    if (!lab_profile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create IT8 handle from remaining data
    size_t it8_data_offset = sizeof(cmsCIELab) + sizeof(cmsCIExyY) + sizeof(cmsUInt16Number) * 3;
    const uint8_t* it8_data = data + it8_data_offset;
    size_t it8_size = (size > it8_data_offset) ? size - it8_data_offset : 0;
    
    cmsHANDLE it8_handle = nullptr;
    if (it8_size > 0) {
        FILE* it8_file = fmemopen((void*)it8_data, it8_size, "rb");
        if (it8_file) {
            it8_handle = cmsIT8LoadFromMem(ctx, it8_data, (cmsUInt32Number)it8_size);
            assert_file_closed(&it8_file);;
        }
    }

    // Use IT8 table count if handle exists
    cmsUInt32Number table_count = 0;
    if (it8_handle) {
        table_count = cmsIT8TableCount(it8_handle);
    }

    // Create a simple pipeline for demonstration
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create an identity stage
        cmsStage* identity_stage = cmsStageAllocIdentity(ctx, 3);
        if (identity_stage) {
            // Insert the stage into pipeline
            cmsBool insert_result = cmsPipelineInsertStage(pipeline, cmsAT_END, identity_stage);
            (void)insert_result; // Use result to avoid unused warning
        }
    }

    // Test Lab encoding/decoding conversion
    cmsUInt16Number encoded_lab[3];
    cmsCIELab decoded_lab;
    
    // Convert float Lab to encoded
    cmsFloat2LabEncoded(encoded_lab, lab_input);
    
    // Convert encoded back to float Lab
    // Note: cmsLabEncoded2FloatV2 is used here as specified in requirements
    // This is a custom function; in actual lcms2.h it might be cmsLabEncoded2Float
    cmsLabEncoded2FloatV2(&decoded_lab, encoded_lab);

    // Clean up resources
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}