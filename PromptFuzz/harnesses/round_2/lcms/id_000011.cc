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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"uipow":[[1987,13,1987,30,0,0,4,0]],"Type_LUT16_Write":[[2512,9,2512,41,0,0,4,0]]},"library_calls":["cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsCreateMultiprofileTransformTHR","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsGetHeaderFlags","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsDupContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsCreateMultiprofileTransformTHR","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsGetHeaderFlags","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a context
    cmsContext context = cmsDupContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create output file for writing (as per instruction 4)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	return 0;
    }

    // Create input file from the fuzzer data (as per instruction 4)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Try to load IT8 data from memory using the input
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
    if (it8) {
        // Enumerate data format (first required API)
        char **sampleNames = NULL;
        int numSamples = cmsIT8EnumDataFormat(it8, &sampleNames);
        (void)numSamples; // Avoid unused variable warning
        
        cmsIT8Free(it8);
    }

    // Create an OkLab profile (second required API)
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(context);
    
    // Create a BCHSW abstract profile (third required API)
    // Use some default values for parameters
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfileTHR(
        context, 
        256,        // nLUTPoints
        0.5,        // Bright
        1.0,        // Contrast
        0.0,        // Hue
        1.0,        // Saturation
        5000,       // TempSrc
        6500        // TempDest
    );

    // Array of profiles for multiprofile transform
    cmsHPROFILE profiles[3] = {NULL, NULL, NULL};
    int profileCount = 0;
    
    if (okLabProfile) {
        profiles[profileCount++] = okLabProfile;
    }
    
    if (bchswProfile) {
        profiles[profileCount++] = bchswProfile;
    }

    // Try to load profile from the input data
    // We'll use the in_file FILE* to attempt to read a profile
    fseek(in_file, 0, SEEK_SET);
    cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(context, "input_file", "rb");
    if (io) {
        // Note: cmsReadProfile is not in the provided API list, so we'll skip
        // direct profile loading and use the created profiles instead
        cmsCloseIOhandler(io);
    }

    // Create multiprofile transform if we have at least 2 profiles
    cmsHTRANSFORM transform = NULL;
    if (profileCount >= 2) {
        transform = cmsCreateMultiprofileTransformTHR(
            context,
            profiles,
            profileCount,
            TYPE_RGB_8,     // InputFormat
            TYPE_RGB_8,     // OutputFormat
            INTENT_PERCEPTUAL, // Intent
            cmsFLAGS_NOCACHE   // dwFlags
        );
    }

    // Save BCHSW profile to memory (fourth required API)
    if (bchswProfile) {
        cmsUInt32Number bytesNeeded = 0;
        // First call to get size
        cmsSaveProfileToMem(bchswProfile, NULL, &bytesNeeded);
        
        if (bytesNeeded > 0 && bytesNeeded < 10*1024*1024) { // Sanity check
            void *buffer = malloc(bytesNeeded);
            if (buffer) {
                cmsSaveProfileToMem(bchswProfile, buffer, &bytesNeeded);
                // Write to output file
                fwrite(buffer, 1, bytesNeeded, out_file);
                free(buffer);
            }
        }
    }

    // Get header flags from OkLab profile (fifth required API)
    if (okLabProfile) {
        cmsUInt32Number flags = cmsGetHeaderFlags(okLabProfile);
        (void)flags; // Avoid unused variable warning
    }

    // Cleanup - release all allocated resources
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (okLabProfile) {
        cmsCloseProfile(okLabProfile);
    }
    
    if (bchswProfile) {
        cmsCloseProfile(bchswProfile);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("input_file");
	return 0;
}