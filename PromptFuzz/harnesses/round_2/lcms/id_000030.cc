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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 48
//<Quality> {"density":18,"unique_branches":{"DataSection":[[2152,36,2152,51,0,0,4,1],[2154,13,2154,36,0,0,4,1],[2166,13,2166,24,0,0,4,0],[2167,21,2167,68,0,0,4,0],[2167,21,2167,68,0,0,4,1],[2171,13,2171,25,0,0,4,0],[2172,21,2172,69,0,0,4,1],[2176,13,2176,20,0,0,4,1],[2181,17,2181,52,0,0,4,0]],"FastIdentity16":[[1389,15,1389,38,0,0,4,0],[1389,15,1389,38,0,0,4,1]],"Skip":[[1110,9,1110,22,0,0,4,0],[1110,26,1110,41,0,0,4,0],[1110,45,1110,65,0,0,4,0]],"DataFormatSection":[[2105,12,2105,39,0,0,4,0],[2105,12,2105,39,0,0,4,1],[2106,9,2106,25,0,0,4,0],[2107,9,2107,24,0,0,4,0],[2108,9,2108,29,0,0,4,0],[2110,17,2110,34,0,0,4,1],[2115,17,2115,64,0,0,4,0],[2115,17,2115,64,0,0,4,1],[2126,12,2126,34,0,0,4,1]],"SetDataFormat":[[1681,9,1681,23,0,0,4,1],[1683,13,1683,37,0,0,4,1],[1687,9,1687,27,0,0,4,1],[1692,9,1692,22,0,0,4,0],[1694,13,1694,37,0,0,4,1]],"cmsReadRawTag":[[2030,9,2030,34,0,0,4,1],[2057,9,2057,30,0,0,4,1],[2080,9,2080,55,0,0,4,1],[2082,9,2082,23,0,0,4,1],[2086,9,2086,21,0,0,4,1],[2091,9,2091,22,0,0,4,1],[2096,9,2096,30,0,0,4,1],[2101,9,2101,28,0,0,4,1],[2108,9,2108,59,0,0,4,1],[2113,9,2113,96,0,0,4,1]],"ParseIT8":[[2355,13,2355,36,0,0,4,0],[2356,25,2356,48,0,0,4,0],[2356,25,2356,48,0,0,4,1]],"HeaderSection":[[2216,16,2216,45,0,0,4,1],[2222,9,2222,22,0,0,4,0],[2224,21,2224,71,0,0,4,1],[2225,21,2225,71,0,0,4,1]],"AllocateDataFormat":[[1647,9,1647,25,0,0,4,1],[1647,29,1647,49,0,0,4,1],[1656,9,1656,30,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateMultiprofileTransformTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsGetTransformGamutCheckPipeline","cmsSetAdaptationState","cmsReadRawTag","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsIT8Free","cmsLabEncoded2Float","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreate_sRGBProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetTransformGamutCheckPipeline","cmsSetAdaptationState","cmsReadRawTag","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsIT8GetPropertyMulti","cmsIT8Free","cmsLabEncoded2Float","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*To create a fuzz driver that uses the specified lcms APIs, I'll implement a scenario where we process ICC profile data from the input, create a multi-profile transform, extract and manipulate gamut check information, and handle IT8 color data.

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create profiles from input data
    // Use first half of data as ICC profile
    size_t profile_size = size / 2;
    const uint8_t* profile_data = data;
    
    cmsHPROFILE profiles[2] = {NULL, NULL};
    
    // First profile from memory
    profiles[0] = cmsOpenProfileFromMemTHR(context, profile_data, profile_size);
    if (!profiles[0]) {
        profiles[0] = cmsCreate_sRGBProfileTHR(context);
    }
    
    // Second profile (sRGB as fallback)
    profiles[1] = cmsCreate_sRGBProfileTHR(context);
    
    if (!profiles[0] || !profiles[1]) {
        if (profiles[0]) cmsCloseProfile(profiles[0]);
        if (profiles[1]) cmsCloseProfile(profiles[1]);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Create multi-profile transform
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number dwFlags = cmsFLAGS_BLACKPOINTCOMPENSATION;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, 2, inputFormat, outputFormat, intent, dwFlags);
    
    if (!transform) {
        cmsCloseProfile(profiles[0]);
        cmsCloseProfile(profiles[1]);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Get gamut check pipeline
    cmsPipeline* gamutPipeline = cmsGetTransformGamutCheckPipeline(transform);
    
    // 4. Set adaptation state (use data-dependent value)
    float adapt_val = 0.0;
    if (size > 0) {
        adapt_val = (data[0] / 255.0) * 2.0; // Range 0-2
    }
    cmsSetAdaptationState(adapt_val);
    
    // 5. Read raw tag from profile
    cmsTagSignature sig = cmsSigMediaWhitePointTag;
    cmsCIEXYZ whitePoint;
    cmsUInt32Number bytesRead = cmsReadRawTag(
        profiles[0], sig, &whitePoint, sizeof(cmsCIEXYZ));
    
    // 6. Create and manipulate IT8 handle
    // Write data to file for IT8 loading
    FILE* it8_file = fmemopen((void*)(data + profile_size), size - profile_size, "rb");
    if (it8_file) {
        cmsHANDLE it8 = NULL;
        
        // Try to load IT8 from file
        fseek(it8_file, 0, SEEK_SET);
        char* buffer = (char*)malloc(size - profile_size + 1);
        if (buffer) {
            fread(buffer, 1, size - profile_size, it8_file);
            buffer[size - profile_size] = '\0';
            
            // Create temporary file for IT8 loading
            FILE* temp_file = fopen("it8_temp.txt", "wb");
            if (temp_file) {
                fwrite(buffer, 1, size - profile_size, temp_file);
                assert_file_closed(&temp_file);;
                
                // Load IT8 from file
                it8 = cmsIT8LoadFromFile(context, "it8_temp.txt");
                
                if (it8) {
                    // 7. Set IT8 property
                    const char* key = "SAMPLE_FIELD";
                    const char* value = "TestValue";
                    cmsIT8SetPropertyUncooked(it8, key, value);
                    
                    // 8. Get IT8 property
                    const char* retrieved = cmsIT8GetPropertyMulti(it8, key, "0");
                    
                    // Clean up temp file
                    remove("it8_temp.txt");
                    
                    // Free IT8 handle
                    cmsIT8Free(it8);
                }
            }
            free(buffer);
        }
        assert_file_closed(&it8_file);;
    }
    
    // 9. Convert encoded Lab to float
    cmsUInt16Number encodedLab[3] = {0x8000, 0x8000, 0x8000}; // Midpoint values
    if (size > 10) {
        // Use some data to influence Lab values
        encodedLab[0] = (data[1] << 8) | data[2];
        encodedLab[1] = (data[3] << 8) | data[4];
        encodedLab[2] = (data[5] << 8) | data[6];
    }
    
    cmsCIELab labFloat;
    cmsLabEncoded2Float(&labFloat, encodedLab);
    
    // 10. Test transform with some pixel data
    unsigned char inputPixel[3] = {128, 128, 128};
    unsigned char outputPixel[3];
    
    if (transform) {
        cmsDoTransform(transform, inputPixel, outputPixel, 1);
    }
    
    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    cmsDeleteContext(context);
    
    assert_file_closed(&it8_file);
	return 0;
}