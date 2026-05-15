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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 4
//<Quality> {"density":12,"unique_branches":{"cmsGetTagOffsetAndSize":[[664,9,664,26,0,0,4,1],[665,9,665,27,0,0,4,1],[667,9,667,23,0,0,4,0],[668,9,668,21,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsGetTagOffsetAndSize","cmsCreateInkLimitingDeviceLinkTHR","cmsAllocNamedColorList","cmsNamedColorCount","cmsLab2XYZ","cmsSetAlarmCodes","cmsCIE94DeltaE","cmsGetPostScriptCRD","cmsPluginTHR","cmsSliceSpaceFloat","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderManufacturer","cmsGetTagOffsetAndSize","cmsCreateInkLimitingDeviceLinkTHR","cmsAllocNamedColorList","cmsNamedColorCount","cmsLab2XYZ","cmsSetAlarmCodes","cmsCIE94DeltaE","cmsGetPostScriptCRD","cmsPluginTHR","cmsSliceSpaceFloat","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


// Include necessary headers for lcms

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file length
    fseek(in_file, 0, SEEK_END);
    long file_len = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_len <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Read file into buffer for profile
    uint8_t *profile_data = (uint8_t *)malloc(file_len);
    if (!profile_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_len = fread(profile_data, 1, file_len, in_file);
    assert_file_closed(&in_file);;
    
    if (read_len == 0) {
        free(profile_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data, read_len);
    if (!hProfile) {
        free(profile_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Get profile context ID
    cmsContext context = cmsGetProfileContextID(hProfile);
    
    // 6. Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer; // Use to avoid unused warning
    
    // 7. Get tag offset and size for first tag
    cmsUInt32Number offset = 0, tag_size = 0;
    cmsBool tag_result = cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tag_size);
    (void)tag_result;
    
    // 8. Create ink limiting device link
    cmsHPROFILE hLink = cmsCreateInkLimitingDeviceLinkTHR(context, 
                                                         cmsSigRgbData,  // RGB color space
                                                         200.0);         // Limit value
    if (hLink) {
        // 9. Allocate named color list
        cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(context, 
                                                                  10,       // n colors
                                                                  3,        // colorant count
                                                                  "PREFIX", // prefix
                                                                  "SUFFIX");// suffix
        
        if (namedColorList) {
            // 10. Get named color count
            cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);
            (void)colorCount;
            
            // 11. Lab to XYZ conversion
            cmsCIEXYZ whitePoint = {0.95047, 1.0, 1.08883};
            cmsCIEXYZ xyz;
            cmsCIELab lab = {50.0, 20.0, 30.0};
            cmsLab2XYZ(&whitePoint, &xyz, &lab);
            
            // 12. Set alarm codes
            cmsUInt16Number alarmCodes[16] = {0};
            for (int i = 0; i < 16; i++) {
                alarmCodes[i] = (cmsUInt16Number)(i * 100);
            }
            cmsSetAlarmCodes(alarmCodes);
            
            // 13. Compute CIE94 delta E
            cmsCIELab lab1 = {50.0, 20.0, 30.0};
            cmsCIELab lab2 = {55.0, 25.0, 35.0};
            cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
            (void)deltaE;
            
            // 14. Get PostScript CRD
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                cmsUInt32Number buffer_size = 8192;
                char *buffer = (char *)malloc(buffer_size);
                if (buffer) {
                    cmsUInt32Number crd_size = cmsGetPostScriptCRD(context, 
                                                                   hProfile,
                                                                   INTENT_PERCEPTUAL,
                                                                   0,
                                                                   buffer,
                                                                   buffer_size);
                    if (crd_size > 0) {
                        fwrite(buffer, 1, crd_size, out_file);
                    }
                    free(buffer);
                }
                assert_file_closed(&out_file);;
            }
            
            // 15. Try to plugin (NULL plugin should fail gracefully)
            cmsPluginTHR(context, NULL);
            
            // 16. Slice space float with dummy sampler
            cmsUInt32Number clutPoints[3] = {2, 2, 2};
            cmsSliceSpaceFloat(3, clutPoints, NULL, NULL);
            
            // 17. Free named color list
            cmsFreeNamedColorList(namedColorList);
        }
        
        // Close the device link profile
        if (hLink) {
            cmsCloseProfile(hLink);
        }
    }
    
    // 18. Close the main profile
    cmsCloseProfile(hProfile);
    
    // Clean up
    free(profile_data);
    
    assert_file_closed(&in_file);
	return 0;
}