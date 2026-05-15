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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"cmsAdaptToIlluminant":[[341,9,341,74,0,0,4,1]]},"library_calls":["cmsOpenProfileFromStream","cmsGetHeaderCreator","cmsGetPCS","cmsGetEncodedICCversion","cmsAdaptToIlluminant","cmsIT8LoadFromMem","cmsIT8SetPropertyStr","cmsIT8Free","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsGetHeaderCreator","cmsGetPCS","cmsGetEncodedICCversion","cmsAdaptToIlluminant","cmsIT8LoadFromMem","cmsIT8SetPropertyStr","cmsIT8Free","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open profile from the input data stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Use required APIs with the opened profile
    
    // a) cmsGetHeaderCreator - get profile creator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    
    // b) cmsGetPCS - get Profile Connection Space
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    
    // c) cmsGetEncodedICCversion - get encoded ICC version
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(hProfile);
    
    // d) cmsAdaptToIlluminant - perform chromatic adaptation
    // We need some color values, so create test XYZ values
    cmsCIEXYZ sourceWhitePt, illuminant, value, result;
    
    // Use D50 as source white point (standard ICC PCS)
    sourceWhitePt.X = 0.9642;
    sourceWhitePt.Y = 1.0000;
    sourceWhitePt.Z = 0.8249;
    
    // Use D65 as illuminant
    illuminant.X = 0.95047;
    illuminant.Y = 1.00000;
    illuminant.Z = 1.08883;
    
    // Create a test value from input data (use first 12 bytes if available)
    if (size >= 12) {
        value.X = (double)data[0] / 255.0;
        value.Y = (double)data[1] / 255.0;
        value.Z = (double)data[2] / 255.0;
    } else {
        // Default test value
        value.X = 0.5;
        value.Y = 0.5;
        value.Z = 0.5;
    }
    
    // Perform adaptation
    cmsBool adaptResult = cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
    
    // e) cmsIT8SetPropertyStr - create IT8 handle and set property
    // Create a minimal IT8 data structure from input
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, data, (cmsUInt32Number)(size > 1000 ? 1000 : size));
    if (hIT8) {
        // Set a property string using part of input data as string
        char propStr[256] = {0};
        size_t strLen = size > 255 ? 255 : size;
        for (size_t i = 0; i < strLen; i++) {
            propStr[i] = (char)(data[i] % 128); // Ensure printable-ish ASCII
        }
        propStr[strLen] = '\0';
        
        cmsBool setResult = cmsIT8SetPropertyStr(hIT8, "TEST_PROPERTY", propStr);
        
        // Clean up IT8 handle
        cmsIT8Free(hIT8);
    }
    
    // 4. Create output file for potential writing operations
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // We could write some data here if needed
        assert_file_closed(&out_file);;
    }
    
    // 5. Clean up all resources
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}