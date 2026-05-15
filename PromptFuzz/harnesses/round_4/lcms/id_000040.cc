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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 40
//<Quality> {"density":13,"unique_branches":{"IsAvailableOnList":[[1293,9,1293,16,0,0,4,1],[1297,13,1297,20,0,0,4,1]],"cmsIT8SaveToFile":[[2025,15,2025,36,0,0,4,1],[2035,13,2035,28,0,0,4,1],[2036,13,2036,34,0,0,4,0],[2036,13,2036,34,0,0,4,1],[2043,9,2043,31,0,0,4,1]],"WriteData":[[1974,12,1974,20,0,0,4,1],[1980,12,1980,35,0,0,4,0],[1982,24,1982,36,0,0,4,0],[1982,24,1982,36,0,0,4,1],[1986,28,1986,43,0,0,4,0],[1986,28,1986,43,0,0,4,1],[1990,24,1990,35,0,0,4,0],[1990,24,1990,35,0,0,4,1],[1994,28,1994,52,0,0,4,0],[1994,28,1994,52,0,0,4,1],[2004,34,2004,58,0,0,4,0],[2004,34,2004,58,0,0,4,1]],"WriteHeader":[[1872,29,1872,40,0,0,4,0],[1874,13,1874,32,0,0,4,1],[1894,13,1894,76,0,0,4,0],[1894,13,1894,76,0,0,4,1],[1906,13,1906,21,0,0,4,0],[1910,13,1910,32,0,0,4,0],[1910,13,1910,32,0,0,4,1],[1914,13,1914,33,0,0,4,0],[1914,13,1914,33,0,0,4,1],[1918,13,1918,35,0,0,4,1],[1922,13,1922,30,0,0,4,1],[1926,13,1926,28,0,0,4,1],[1930,13,1930,20,0,0,4,1]],"WriteDataFormat":[[1948,9,1948,25,0,0,4,1],[1954,12,1954,35,0,0,4,0],[1956,24,1956,36,0,0,4,0],[1956,24,1956,36,0,0,4,1],[1959,30,1959,51,0,0,4,0],[1959,30,1959,51,0,0,4,1]],"WriteStr":[[1819,9,1819,19,0,0,4,0],[1821,13,1821,50,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsWhitePointFromTemp","cmsGetAlarmCodes","cmsIT8LoadFromMem","cmsCreateLab2ProfileTHR","cmsIT8Free","cmsDeleteContext","cmsGetHeaderModel","cmsDetectDestinationBlackPoint","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsWhitePointFromTemp","cmsGetAlarmCodes","cmsIT8LoadFromMem","cmsCreateLab2ProfileTHR","cmsGetHeaderModel","cmsDetectDestinationBlackPoint","cmsIT8SetPropertyStr","cmsIT8GetPropertyMulti","cmsIT8SaveToFile","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
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

    // Initialize context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Create white point from temperature (using first 8 bytes as temperature)
    cmsCIExyY whitePoint;
    cmsFloat64Number temp = 6500.0;  // Default
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(&temp, data, sizeof(cmsFloat64Number));
        if (temp < 2000.0) temp = 2000.0;
        if (temp > 10000.0) temp = 10000.0;
    }
    
    cmsBool whitePointResult = cmsWhitePointFromTemp(&whitePoint, temp);
    
    // 2. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);

    // 3. Create IT8 handle from input data
    cmsHANDLE it8 = nullptr;
    if (size > 100) {
        it8 = cmsIT8LoadFromMem(context, data, size);
    }

    // 4. Create Lab2 profile with computed white point
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, &whitePoint);
    if (!labProfile) {
        if (it8) cmsIT8Free(it8);
        cmsDeleteContext(context);
        return 0;
    }

    // 5. Get header model
    cmsUInt32Number model = cmsGetHeaderModel(labProfile);

    // 6. Detect destination black point
    cmsCIEXYZ blackPoint;
    cmsBool blackPointDetected = cmsDetectDestinationBlackPoint(&blackPoint, labProfile, 
                                                                fuzz_uint32_t_1, fuzz_uint32_t_2);

    // 7. IT8 operations if we have an IT8 handle
    if (it8) {
        // Set property string (using some data for property value)
        const char* propValue = "TestProperty";
        if (size > 50) {
            // Use part of input data as property value
            char buffer[50];
            size_t len = size < 50 ? size : 50;
            memcpy(buffer, data + 10, len - 10);
            buffer[len - 10] = '\0';
            cmsIT8SetPropertyStr(it8, fuzz_str_3, buffer);
        } else {
            cmsIT8SetPropertyStr(it8, fuzz_str_4, fuzz_str_5);
        }

        // Get property multi
        const char* result = cmsIT8GetPropertyMulti(it8, fuzz_str_6, nullptr);

        // Save IT8 to file
        cmsIT8SaveToFile(it8, fuzz_str_7);
    }

    // 8. GBD operations - create a simple Lab point and check it
    cmsCIELab labPoint;
    labPoint.L = 50.0;
    labPoint.a = 0.0;
    labPoint.b = 0.0;
    
    // Note: cmsGDBCheckPoint needs a GBD handle, which we don't have
    // We'll skip this API as we can't create a GBD with available functions
    
    // 9. Cleanup
    if (it8) {
        cmsIT8Free(it8);
    }
    
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    
    // Remove output file if created
    remove("output_it8_file");
    
    return 0;
}