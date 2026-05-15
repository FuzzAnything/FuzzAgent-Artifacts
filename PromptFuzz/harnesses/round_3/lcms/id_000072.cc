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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 121, nr_unique_branch: 53
//<Quality> {"density":11,"unique_branches":{"cmsMLUdup":[[451,9,451,28,0,0,4,0],[462,9,462,33,0,0,4,0],[471,9,471,23,0,0,4,0]],"WriteMatrix":[[2821,17,2821,22,0,0,4,0],[2821,17,2821,22,0,0,4,1],[2823,13,2823,56,0,0,4,1],[2826,9,2826,26,0,0,4,1],[2834,21,2834,44,0,0,4,0],[2834,21,2834,44,0,0,4,1],[2836,17,2836,49,0,0,4,1]],"QuantizeToSector":[[173,9,173,26,0,0,4,0],[175,9,175,26,0,0,4,0]],"Type_LUTB2A_Write":[[3152,9,3152,79,0,0,4,0],[3153,13,3153,142,0,0,4,1],[3164,9,3164,62,0,0,4,1],[3165,9,3165,63,0,0,4,1],[3166,9,3166,38,0,0,4,1],[3170,9,3170,38,0,0,4,1],[3171,9,3171,38,0,0,4,1],[3172,9,3172,38,0,0,4,1],[3173,9,3173,38,0,0,4,1],[3174,9,3174,38,0,0,4,1],[3176,9,3176,18,0,0,4,1],[3182,9,3182,21,0,0,4,1],[3187,9,3187,18,0,0,4,0],[3190,13,3190,70,0,0,4,1],[3193,9,3193,23,0,0,4,0],[3195,13,3195,43,0,0,4,1],[3198,9,3198,18,0,0,4,0],[3201,13,3201,70,0,0,4,1],[3206,9,3206,37,0,0,4,1],[3208,9,3208,44,0,0,4,1],[3209,9,3209,46,0,0,4,1],[3210,9,3210,44,0,0,4,1],[3211,9,3211,44,0,0,4,1],[3212,9,3212,44,0,0,4,1],[3214,9,3214,35,0,0,4,1]],"cmsGDBAddPoint":[[367,9,367,20,0,0,4,0]],"cmsDictFree":[[1113,17,1113,45,0,0,4,1],[1114,17,1114,45,0,0,4,1]],"ToSpherical":[[130,8,130,19,0,0,4,1]],"WriteSetOfCurves":[[2864,13,2864,41,0,0,4,0],[2875,13,2875,33,0,0,4,0],[2876,21,2876,62,0,0,4,1],[2879,13,2879,43,0,0,4,1]],"_cmsAtan2":[[106,9,106,17,0,0,4,0],[106,9,106,17,0,0,4,1],[106,21,106,29,0,0,4,0],[106,21,106,29,0,0,4,1],[110,12,110,17,0,0,4,0],[110,12,110,17,0,0,4,1]],"GetPoint":[[347,9,347,18,0,0,4,0],[347,22,347,31,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsGBDAlloc","cmsGDBAddPoint","cmsMLUalloc","cmsMLUsetUTF8","cmsCreateNULLProfileTHR","cmsSaveProfileToFile","cmsDictAlloc","cmsDictAddEntry","cmsDictFree","cmsMLUfree","cmsGBDFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsGBDAlloc","cmsGDBAddPoint","cmsMLUalloc","cmsMLUsetUTF8","cmsCreateNULLProfileTHR","cmsSaveProfileToFile","cmsDictAlloc","cmsDictAddEntry","cmsDictFree","cmsMLUfree","cmsGBDFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Need minimum data for Lab point and basic operations
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Set error handler
    cmsSetLogErrorHandlerTHR(ctx, [](cmsContext, cmsUInt32Number, const char*) {});
    
    // Create a GBD handle and add point (using first 24 bytes as Lab coordinates)
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (gbd) {
        cmsCIELab lab;
        // Use the first 24 bytes for Lab coordinates (3 doubles)
        memcpy(&lab.L, data, 8);
        memcpy(&lab.a, data + 8, 8);
        memcpy(&lab.b, data + 16, 8);
        cmsGDBAddPoint(gbd, &lab);
    }
    
    // Create MLU and set UTF8 string
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (mlu) {
        // Use next bytes for UTF8 string (up to 100 bytes or remaining size)
        size_t str_len = size > 124 ? 100 : size - 24;
        if (str_len > 0) {
            char* utf8_str = (char*)malloc(str_len + 1);
            if (utf8_str) {
                memcpy(utf8_str, data + 24, str_len);
                utf8_str[str_len] = '\0';
                cmsMLUsetUTF8(mlu, "en", "US", utf8_str);
                free(utf8_str);
            }
        }
    }
    
    // Create profile and save to file
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
    if (profile) {
        cmsSaveProfileToFile(profile, "output_file");
    }
    
    // Create dictionary and add entry
    cmsHANDLE dict = cmsDictAlloc(ctx);
    if (dict && mlu) {
        // Use sample name and value from remaining data
        size_t remaining = size > 124 ? size - 124 : 0;
        if (remaining > 10) {
            wchar_t name[50], value[50];
            // Convert some bytes to wide chars for demonstration
            size_t name_len = remaining/2 < 49 ? remaining/2 : 49;
            size_t value_len = remaining - name_len < 49 ? remaining - name_len : 49;
            
            for (size_t i = 0; i < name_len; i++) {
                name[i] = data[124 + i];
            }
            name[name_len] = 0;
            
            for (size_t i = 0; i < value_len; i++) {
                value[i] = data[124 + name_len + i];
            }
            value[value_len] = 0;
            
            cmsDictAddEntry(dict, name, value, mlu, mlu);
        }
    }
    
    // Cleanup CIECAM02 model if we had one (not in this flow, but for completeness)
    // cmsCIECAM02Done(some_model); // Not created in this flow
    
    // Clean up resources
    if (dict) cmsDictFree(dict);
    if (mlu) cmsMLUfree(mlu);
    if (gbd) cmsGBDFree(gbd);
    if (profile) cmsCloseProfile(profile);
    cmsDeleteContext(ctx);
    
    return 0;
}