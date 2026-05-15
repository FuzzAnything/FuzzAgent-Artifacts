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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsDeleteContext","cmsGetAlarmCodesTHR","cmsStageAllocCLutFloatGranular","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsIT8Free","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsMLUalloc","cmsMLUsetUTF8","cmsWriteTag","cmsMLUfree","cmsDoTransformLineStride","cmsDeleteTransform","cmsStageFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetAlarmCodesTHR","cmsStageAllocCLutFloatGranular","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsIT8Free","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsMLUalloc","cmsMLUsetUTF8","cmsWriteTag","cmsMLUfree","cmsDoTransformLineStride","cmsDeleteTransform","cmsStageFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // 1. Create device link profile from cube file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Write input data to temporary cube file
    FILE *cube_file = fopen("input.cube", "wb");
    if (!cube_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFileTHR(context, "input.cube");
    if (!deviceLink) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        remove("input.cube");
        assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodesTHR(context, alarmCodes);

    // 3. Create CLUT stage
    cmsUInt32Number clutPoints[] = {3, 3, 3}; // 3x3x3 grid
    cmsFloat32Number table[81]; // 3*3*3*3 channels
    for (int i = 0; i < 81; i++) {
        table[i] = (i % 10) / 10.0f; // Simple pattern
    }
    cmsStage *clutStage = cmsStageAllocCLutFloatGranular(context, clutPoints, 3, 3, table);
    
    // 4. Create IT8 handle and save to file
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size > 1000 ? 1000 : size);
    if (it8) {
        cmsIT8SaveToFile(it8, "output.it8");
        cmsIT8Free(it8);
    }

    // 5. Create multiprofile transform
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfile(); // Source profile
    profiles[1] = deviceLink; // Device link profile
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(
        profiles, 2, 
        TYPE_RGB_8, // Input format
        TYPE_RGB_8, // Output format
        INTENT_PERCEPTUAL, // Intent
        cmsFLAGS_NOCACHE // Flags
    );

    // 6. Write tag to device link profile
    if (deviceLink && size > 50) {
        // Create simple MLU for description tag
        cmsMLU *mlu = cmsMLUalloc(context, 1);
        if (mlu) {
            cmsMLUsetUTF8(mlu, "en", "US", "Test Profile");
            cmsWriteTag(deviceLink, cmsSigProfileDescriptionTag, mlu);
            cmsMLUfree(mlu);
        }
    }

    // 7. Perform transform with stride
    if (transform) {
        unsigned char inputBuffer[12]; // 4 pixels RGB
        unsigned char outputBuffer[12];
        
        // Fill with some data
        for (int i = 0; i < 12; i++) {
            inputBuffer[i] = (i * 17) % 256;
        }
        
        cmsDoTransformLineStride(
            transform,
            inputBuffer,
            outputBuffer,
            4, // Pixels per line
            1, // Line count
            12, // Bytes per line in (4 pixels * 3 channels)
            12, // Bytes per line out
            0, // Bytes per plane in (interleaved)
            0  // Bytes per plane out
        );
        
        cmsDeleteTransform(transform);
    }

    // 8. Cleanup
    if (clutStage) {
        cmsStageFree(clutStage);
    }
    
    if (profiles[0]) {
        cmsCloseProfile(profiles[0]);
    }
    
    if (deviceLink) {
        cmsCloseProfile(deviceLink);
    }
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove temporary files
    remove("input.cube");
    remove("output.it8");
    
    assert_file_closed(&cube_file);
	assert_file_closed(&in_file);
	return 0;
}