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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 15
//<Quality> {"density":17,"unique_branches":{"cmsDetectRGBProfileGamma":[[618,9,618,52,0,0,4,0],[633,9,633,22,0,0,4,0]],"cmsWriteRawTag":[[2139,9,2139,55,0,0,4,1],[2141,9,2141,34,0,0,4,1],[2157,9,2157,32,0,0,4,1]],"IsMyFile":[[2545,8,2545,11,0,0,4,1],[2552,8,2552,23,0,0,4,1]],"freeOneTag":[[1653,13,1653,32,0,0,4,1]],"cmsIT8LoadFromFile":[[2620,10,2620,19,0,0,4,0],[2620,10,2620,19,0,0,4,1],[2624,10,2624,15,0,0,4,1],[2629,10,2629,37,0,0,4,1],[2638,9,2638,31,0,0,4,0],[2638,9,2638,31,0,0,4,1],[2648,9,2648,47,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsIT8LoadFromFile","cmsOpenProfileFromFileTHR","cmsPlugin","cmsGetHeaderModel","cmsDetectRGBProfileGamma","cmsIsTag","cmsReadTag","cmsGetToneCurveEstimatedTableEntries","cmsReverseToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsGetAlarmCodes","cmsCloseProfile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsUnregisterPluginsTHR","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromFile","cmsOpenProfileFromFileTHR","cmsPlugin","cmsGetHeaderModel","cmsDetectRGBProfileGamma","cmsIsTag","cmsReadTag","cmsGetToneCurveEstimatedTableEntries","cmsReverseToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsGetAlarmCodes","cmsCloseProfile","cmsIT8GetPropertyMulti","cmsIT8Free","cmsUnregisterPluginsTHR","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1024) {
	return 0;
	}
	  // Need minimum data for meaningful processing
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create a temporary file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Try to load as IT8 data (for cmsIT8GetPropertyMulti)
    cmsHANDLE it8 = nullptr;
    fseek(in_file, 0, SEEK_SET);
    it8 = cmsIT8LoadFromFile(context, "input_file");
    
    // 3. Try to open as ICC profile
    fseek(in_file, 0, SEEK_SET);
    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // 4. Use cmsPlugin with null plugin (should handle gracefully)
    cmsPlugin(nullptr);
    
    if (profile) {
        // 5. Get header model
        cmsUInt32Number model = cmsGetHeaderModel(profile);
        
        // 6. Detect RGB profile gamma
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(profile, 1.0);
        
        // 7. Get tone curve from profile if possible
        cmsToneCurve* curve = nullptr;
        if (cmsIsTag(profile, cmsSigRedTRCTag)) {
            curve = (cmsToneCurve*)cmsReadTag(profile, cmsSigRedTRCTag);
            if (curve) {
                // 8. Get estimated table entries
                cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve);
                
                // 9. Reverse tone curve
                cmsToneCurve* reversed = cmsReverseToneCurve(curve);
                if (reversed) {
                    cmsFreeToneCurve(reversed);
                }
            }
        }
        
        // 10. Write raw tag (use small portion of input data)
        if (size > 128) {
            cmsWriteRawTag(profile, cmsSigCopyrightTag, data + 64, 64);
        }
        
        // 11. Get alarm codes
        cmsUInt16Number alarmCodes[16];
        cmsGetAlarmCodes(alarmCodes);
        
        cmsCloseProfile(profile);
    }
    
    // 12. Use IT8 APIs if loaded
    if (it8) {
        // Try to get property (keys might not exist, but API should handle)
        const char* prop = cmsIT8GetPropertyMulti(it8, "SAMPLE_ID", "0");
        // Don't use prop as it might be null
        
        cmsIT8Free(it8);
    }
    
    // 13. Unregister plugins
    cmsUnregisterPluginsTHR(context);
    
    // 14. Cleanup
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}