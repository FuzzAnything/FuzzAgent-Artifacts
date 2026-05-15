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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"AllocateToneCurveStruct":[[226,9,226,22,0,0,4,0],[226,26,226,40,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateNULLProfile","cmsDeleteContext","cmsGetHeaderCreationDateTime","cmsGetColorSpace","cmsFormatterForPCSOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTableEntries","cmsFreeToneCurve","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfile","cmsGetHeaderCreationDateTime","cmsGetColorSpace","cmsFormatterForPCSOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTableEntries","cmsFreeToneCurve","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
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
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // Write input data to temporary file for IT8 loading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    assert_file_closed(&in_file);; // Close as we'll use filename for cmsIT8LoadFromFile

    // Create NULL profile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    if (!null_profile) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get profile information
    struct tm creation_time;
    if (cmsGetHeaderCreationDateTime(null_profile, &creation_time)) {
        // Time retrieved successfully
    }

    cmsColorSpaceSignature color_space = cmsGetColorSpace(null_profile);
    
    // Calculate formatter for PCS
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(null_profile, 4, false);

    // Create and populate MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        cmsMLUsetASCII(mlu, "en", "US", "Test Description");
        
        char lang_code[3] = {0};
        char country_code[3] = {0};
        if (cmsMLUtranslationsCodes(mlu, 0, lang_code, country_code)) {
            // Codes retrieved
        }
        cmsMLUfree(mlu);
    }

    // Create tone curve
    const size_t max_entries = 256;
    cmsUInt16Number curve_values[max_entries];
    size_t entries_to_use = (size < max_entries * 2) ? size / 2 : max_entries;
    
    for (size_t i = 0; i < entries_to_use && i * 2 + 1 < size; i++) {
        curve_values[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve *tone_curve = cmsBuildTabulatedToneCurve16(context, 
        entries_to_use, curve_values);
    if (tone_curve) {
        cmsUInt32Number estimated_entries = cmsGetToneCurveEstimatedTableEntries(tone_curve);
        cmsFreeToneCurve(tone_curve);
    }

    // Create identity stage
    cmsStage *identity_stage = cmsStageAllocIdentity(context, 3);
    if (identity_stage) {
        cmsUInt32Number input_channels = cmsStageInputChannels(identity_stage);
        cmsStageFree(identity_stage);
    }

    // Load IT8 from file (using input data as file content)
    // Note: We need to write data to a file first
    FILE *it8_file = fopen("input_file", "wb");
    if (it8_file) {
        fwrite(data, 1, size, it8_file);
        assert_file_closed(&it8_file);;
        
        cmsHANDLE it8_handle = cmsIT8LoadFromFile(context, "input_file");
        if (it8_handle) {
            cmsFloat64Number it8_value = cmsIT8GetDataDbl(it8_handle, "1", "R");
            cmsIT8Free(it8_handle);
        }
        remove("input_file");
    }

    // Cleanup
    cmsCloseProfile(null_profile);
    cmsDeleteContext(context);

    assert_file_closed(&it8_file);
	assert_file_closed(&in_file);
	return 0;
}