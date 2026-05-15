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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 107
//<Quality> {"density":16,"unique_branches":{"RootOfLeastSquaresFitQuadraticCurve":[[340,9,340,14,0,0,4,0],[340,9,340,14,0,0,4,1],[342,15,342,20,0,0,4,0],[342,15,342,20,0,0,4,1],[363,9,363,37,0,0,4,1],[370,9,370,26,0,0,4,0],[370,9,370,26,0,0,4,1],[372,13,372,30,0,0,4,0],[372,13,372,30,0,0,4,1],[378,14,378,20,0,0,4,0],[378,14,378,20,0,0,4,1],[385,18,385,35,0,0,4,1],[31,23,31,34,1,0,4,0],[31,23,31,34,1,0,4,1],[31,31,31,32,1,0,4,0],[31,31,31,32,1,0,4,1],[31,44,31,45,1,0,4,0],[31,23,31,34,2,0,4,0],[31,23,31,34,2,0,4,1],[31,31,31,32,2,0,4,0],[31,31,31,32,2,0,4,1],[31,44,31,45,2,0,4,0],[31,44,31,45,2,0,4,1]],"cmsDetectDestinationBlackPoint":[[415,9,415,36,0,0,4,0],[417,9,417,42,0,0,4,0],[433,9,433,57,0,0,4,1],[437,17,437,44,0,0,4,1],[450,9,450,59,0,0,4,0],[450,9,450,59,0,0,4,1],[451,10,451,38,0,0,4,0],[452,10,452,37,0,0,4,0],[452,10,452,37,0,0,4,1],[453,10,453,38,0,0,4,1],[463,9,463,47,0,0,4,1],[489,9,489,27,0,0,4,0],[489,9,489,27,0,0,4,1],[493,15,493,22,0,0,4,0],[493,15,493,22,0,0,4,1],[506,19,506,24,0,0,4,0],[506,19,506,24,0,0,4,1],[511,9,511,38,0,0,4,0],[511,9,511,38,0,0,4,1],[522,9,522,47,0,0,4,1],[546,15,546,22,0,0,4,0],[546,15,546,22,0,0,4,1],[552,9,552,47,0,0,4,1],[565,15,565,22,0,0,4,0],[565,15,565,22,0,0,4,1],[569,13,569,21,0,0,4,0],[569,13,569,21,0,0,4,1],[569,25,569,32,0,0,4,0],[569,25,569,32,0,0,4,1],[578,9,578,14,0,0,4,0],[578,9,578,14,0,0,4,1],[588,9,588,20,0,0,4,1],[30,23,30,34,18,0,4,1],[30,31,30,32,18,0,4,1],[30,44,30,45,18,0,4,1],[30,23,30,34,19,0,4,1],[30,31,30,32,19,0,4,1],[30,44,30,45,19,0,4,1],[30,23,30,34,20,0,4,0],[30,23,30,34,20,0,4,1]],"BlackPointAsDarkerColorant":[[80,9,80,66,0,0,4,0],[116,9,116,22,0,0,4,0]],"_cmsMAT3solve":[[162,9,162,35,0,0,4,1]],"Type_LUT8_Read":[[2023,31,2023,61,0,0,4,0],[2043,33,2043,71,0,0,4,0],[2045,13,2045,116,0,0,4,1],[2096,9,2096,23,0,0,4,1]],"isInkColorspace":[[194,5,194,24,0,0,4,0],[195,5,195,23,0,0,4,1],[196,5,196,24,0,0,4,1],[197,5,197,24,0,0,4,1],[198,5,198,24,0,0,4,1],[199,5,199,24,0,0,4,1],[200,5,200,24,0,0,4,1],[201,5,201,24,0,0,4,1],[202,5,202,24,0,0,4,1],[203,5,203,24,0,0,4,1],[204,5,204,24,0,0,4,1],[205,5,205,24,0,0,4,1],[206,5,206,24,0,0,4,1],[207,5,207,24,0,0,4,1],[208,5,208,24,0,0,4,1],[209,5,209,24,0,0,4,1],[210,5,210,24,0,0,4,1],[211,5,211,26,0,0,4,1],[212,5,212,26,0,0,4,1],[213,5,213,26,0,0,4,1],[214,5,214,26,0,0,4,1],[215,5,215,26,0,0,4,1],[216,5,216,26,0,0,4,1],[217,5,217,26,0,0,4,1],[218,5,218,26,0,0,4,1],[219,5,219,26,0,0,4,1],[220,5,220,27,0,0,4,1],[221,5,221,27,0,0,4,1],[222,5,222,27,0,0,4,1],[223,5,223,27,0,0,4,1],[224,5,224,27,0,0,4,1],[225,5,225,27,0,0,4,1],[227,5,227,12,0,0,4,1]],"Type_LUT16_Read":[[2326,31,2326,61,0,0,4,0],[2355,9,2355,30,0,0,4,0],[2391,9,2391,23,0,0,4,0]],"cmsIsMatrixShaper":[[821,17,821,53,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsGetHeaderModel","cmsWhitePointFromTemp","cmsCreateLab2ProfileTHR","cmsDetectDestinationBlackPoint","cmsGetAlarmCodes","cmsIT8LoadFromMem","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetHeaderModel","cmsWhitePointFromTemp","cmsCreateLab2ProfileTHR","cmsDetectDestinationBlackPoint","cmsGetAlarmCodes","cmsIT8LoadFromMem","cmsIT8SetPropertyStr","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process color profile data:

*/


// Include lcms2 headers

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!hProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Get header model using cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    
    // 3. Create white point from temperature (use first 8 bytes as temperature)
    cmsCIExyY WhitePoint;
    cmsFloat64Number tempK = 6500.0; // Default temperature
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(&tempK, data, sizeof(cmsFloat64Number));
        if (tempK < 1000.0) tempK = 1000.0;
        if (tempK > 25000.0) tempK = 25000.0;
    }
    
    cmsBool whitePointResult = cmsWhitePointFromTemp(&WhitePoint, tempK);
    
    // 4. Create Lab2 profile using the white point
    cmsHPROFILE labProfile = NULL;
    if (whitePointResult) {
        labProfile = cmsCreateLab2ProfileTHR(context, &WhitePoint);
    }
    
    // 5. Detect black point from the original profile
    cmsCIEXYZ BlackPoint;
    cmsBool blackPointResult = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, 
                                                              fuzz_uint32_t_1, fuzz_uint32_t_2);
    
    // 6. Get alarm codes using cmsGetAlarmCodes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    
    // 7. Create IT8 handle and use IT8 APIs
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, data, size);
    if (hIT8) {
        // Set a property string (use first 100 bytes as string if available)
        const char* propStr = "TestProperty";
        if (size > 100) {
            // Create a null-terminated string from data
            char* tempStr = (char*)malloc(101);
            if (tempStr) {
                memcpy(tempStr, data, 100);
                tempStr[100] = '\0';
                cmsIT8SetPropertyStr(hIT8, fuzz_str_3, tempStr);
                free(tempStr);
            }
        } else {
            cmsIT8SetPropertyStr(hIT8, fuzz_str_4, fuzz_str_5);
        }
        
        // Get property multi
        const char* propValue = cmsIT8GetPropertyMulti(hIT8, fuzz_str_6, NULL);
        
        // Save IT8 to file
        cmsIT8SaveToFile(hIT8, fuzz_str_7);
        
        // 8. Create GBD handle and check a point
        // Note: cmsGBDAlloc is not in the provided list, so we'll skip GBD operations
        // since we can't create a GBD handle with the given APIs
        
        cmsIT8Free(hIT8);
    }
    
    // 9. Create a test Lab point for GBD check (if we had a GBD handle)
    cmsCIELab testLab = {50.0, 0.0, 0.0};
    
    // Note: We cannot call cmsGDBCheckPoint because we don't have a GBD handle
    // The API cmsGBDAlloc was not provided in the list
    
    // 10. Clean up resources
    if (labProfile) {
        cmsCloseProfile(labProfile);
    }
    
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    
    // 11. Clean up temporary files
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}