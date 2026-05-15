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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsReadRawTag","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsSetAdaptationState","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsLabEncoded2Float","cmsIT8Free","cmsGetTransformGamutCheckPipeline","cmsPipelineStageCount","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsReadRawTag","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsSetAdaptationState","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsLabEncoded2Float","cmsIT8Free","cmsGetTransformGamutCheckPipeline","cmsPipelineStageCount","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
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
	 // Minimal size check

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create a profile from the input data
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!profile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Read raw tag (use copyright tag as example)
    cmsUInt32Number bufferSize = 256;
    char* buffer = (char*)malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number read = cmsReadRawTag(profile, cmsSigCopyrightTag, buffer, bufferSize);
        (void)read; // Consume result
        free(buffer);
    }

    // 3. Create multiprofile transform (using sRGB as second profile)
    cmsHPROFILE sRGB = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profiles[2] = {profile, sRGB};
    
    // Use input data to determine format flags
    cmsUInt32Number inputFormat = size > 0 ? ((data[0] % 8) | TYPE_RGB_8) : TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_Lab_8;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        ctx, profiles, 2, 
        inputFormat, outputFormat,
        INTENT_PERCEPTUAL, 
        cmsFLAGS_NOCACHE
    );

    // 4. Set adaptation state using data-derived value
    if (size > 4) {
        cmsFloat64Number adapt = (cmsFloat64Number)data[1] / 255.0;
        cmsSetAdaptationState(adapt);
    }

    // 5. Create IT8 handle from file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE it8 = cmsIT8LoadFromFile(ctx, "input_file");
        
        if (it8) {
            // 6. Set IT8 property using data
            const char* key = "TestProperty";
            const char* value = "TestValue";
            cmsIT8SetPropertyUncooked(it8, key, value);
            
            // 7. Get IT8 property
            const char* retrieved = cmsIT8GetPropertyMulti(it8, key, NULL);
            (void)retrieved; // Consume result
            
            // 8. Convert Lab encoded to float using data bytes
            if (size >= 6) {
                cmsUInt16Number wLab[3];
                wLab[0] = (data[2] << 8) | data[3];
                wLab[1] = (data[4] << 8) | data[5];
                wLab[2] = size > 6 ? ((data[6] << 8) | data[7]) : 0;
                
                cmsCIELab Lab;
                cmsLabEncoded2Float(&Lab, wLab);
            }
            
            cmsIT8Free(it8);
        }
        assert_file_closed(&in_file);;
    }

    // 9. Get transform gamut check pipeline
    if (transform) {
        cmsPipeline* pipeline = cmsGetTransformGamutCheckPipeline(transform);
        if (pipeline) {
            // Do something minimal with pipeline
            cmsUInt32Number nStages = cmsPipelineStageCount(pipeline);
            (void)nStages; // Consume result
        }
    }

    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (sRGB) cmsCloseProfile(sRGB);
    if (profile) cmsCloseProfile(profile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}