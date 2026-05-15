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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cmsDictGetEntryList":[[1194,9,1194,21,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderFlags","cmsBuildGamma","cmsFreeToneCurve","cmsCloseProfile","cmsCreateLinearizationDeviceLinkTHR","cmsIsToneCurveMultisegment","cmsIT8Alloc","cmsIT8SetPropertyMulti","cmsIT8GetData","cmsReadTag","cmsDictGetEntryList","cmsLabEncoded2FloatV2","cmsDictNextEntry","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetProfileContextID","cmsGetHeaderFlags","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsIsToneCurveMultisegment","cmsIT8Alloc","cmsIT8SetPropertyMulti","cmsIT8GetData","cmsReadTag","cmsDictGetEntryList","cmsLabEncoded2FloatV2","cmsDictNextEntry","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	
		cmsUInt16Number fuzzer_var_cmsLabEncoded2FloatV2_13_1[3] = {fuzz_uint16_t_3,fuzz_uint16_t_4,fuzz_uint16_t_5};//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing
    
    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // 2. Get profile context ID
    cmsContext context = cmsGetProfileContextID(hProfile);
    
    // 3. Get header flags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    
    // 4. Create linearization device link (requires tone curves)
    // Create simple gamma tone curves
    cmsToneCurve* toneCurves[3];
    for (int i = 0; i < 3; i++) {
        toneCurves[i] = cmsBuildGamma(context, 2.2);
        if (!toneCurves[i]) {
            for (int j = 0; j < i; j++) cmsFreeToneCurve(toneCurves[j]);
            cmsCloseProfile(hProfile);
            return 0;
        }
    }
    
    // Create device link using RGB color space
    cmsHPROFILE deviceLink = cmsCreateLinearizationDeviceLinkTHR(
        context, 
        cmsSigRgbData, 
        toneCurves
    );
    
    // 5. Check if tone curves are multisegment
    for (int i = 0; i < 3; i++) {
        cmsIsToneCurveMultisegment(toneCurves[i]);
    }
    
    // 6. Create IT8 handle and set properties
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        // Use parts of input data for IT8 properties
        size_t dataOffset = 0;
        
        // Set property with multi-key
        if (size > 50) {
            char key[32], subkey[32], value[32];
            snprintf(key, sizeof(key), "Key_%02x", data[dataOffset++ % size]);
            snprintf(subkey, sizeof(subkey), "Sub_%02x", data[dataOffset++ % size]);
            snprintf(value, sizeof(value), "Val_%02x", data[dataOffset++ % size]);
            cmsIT8SetPropertyMulti(hIT8, key, subkey, value);
        }
        
        // Get data from IT8 (will return NULL since we didn't add patch data)
        const char* patchData = cmsIT8GetData(hIT8, fuzz_str_1, fuzz_str_2);
        
        // 7. Process dictionary entries if device link exists
        if (deviceLink) {
            // Read dictionary tag from device link
            cmsHANDLE hDict = (cmsHANDLE)cmsReadTag(deviceLink, cmsSigCopyrightTag);
            if (hDict) {
                const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
                while (entry) {
                    // Convert any encoded Lab values if present
                    cmsUInt16Number encodedLab[3] = {0x8000, 0x8000, 0x8000}; // Midpoint
                    cmsCIELab lab;
                    cmsLabEncoded2FloatV2(&lab, fuzzer_var_cmsLabEncoded2FloatV2_13_1);
                    
                    // Move to next entry
                    entry = cmsDictNextEntry(entry);
                }
            }
        }
        
        // 8. Write PostScript color resource to file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(context, "output_file", fuzz_str_6);
            if (io && deviceLink) {
                cmsUInt32Number written = cmsGetPostScriptColorResource(
                    context,
                    cmsPS_RESOURCE_CSA,
                    deviceLink,
                    fuzz_uint32_t_7,
                    fuzz_uint32_t_8,
                    io
                );
                cmsCloseIOhandler(io);
            }
            assert_file_closed(&out_file);;
        }
        
        cmsIT8Free(hIT8);
    }
    
    // 9. Clean up
    for (int i = 0; i < 3; i++) {
        cmsFreeToneCurve(toneCurves[i]);
    }
    
    if (deviceLink) {
        cmsCloseProfile(deviceLink);
    }
    
    cmsCloseProfile(hProfile);
    
    return 0;
}