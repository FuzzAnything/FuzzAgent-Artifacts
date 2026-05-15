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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"DefaultEvalParametricFn":[[601,13,601,29,0,0,4,1],[605,17,605,22,0,0,4,1]],"DecideCurveType":[[1443,9,1443,37,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsBuildSegmentedToneCurve","cmsDeleteContext","cmsCreateGrayProfile","cmsFreeToneCurve","cmsDeleteContext","cmsGetColorSpace","cmsCreateNULLProfile","cmsCloseProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildSegmentedToneCurve","cmsCreateGrayProfile","cmsGetColorSpace","cmsCreateNULLProfile","cmsCloseProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUtranslationsCodes","cmsMLUfree","cmsStageAllocIdentity","cmsStageInputChannels","cmsStageFree","cmsIT8LoadFromFile","cmsIT8GetDataDbl","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 32) {
	return 0;
	}
	  // Need minimum data for operations

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Create a white point from input data
    cmsCIExyY whitePoint;
    // Use first 24 bytes for white point (3 doubles = 24 bytes)
    if (size >= 24) {
        memcpy(&whitePoint.x, data, 8);
        memcpy(&whitePoint.y, data + 8, 8);
        memcpy(&whitePoint.Y, data + 16, 8);
    } else {
        // Default D50 white point if not enough data
        whitePoint.x = 0.3457;
        whitePoint.y = 0.3585;
        whitePoint.Y = 1.0;
    }

    // Create tone curve using segmented tone curve (since parametric isn't available)
    cmsCurveSegment segments[1];
    segments[0].x0 = 0.0;
    segments[0].x1 = 1.0;
    segments[0].Type = 6;  // Gamma curve type
    segments[0].Params[0] = 2.2;  // Gamma value
    
    cmsToneCurve* transferFunction = cmsBuildSegmentedToneCurve(ctx, 1, segments);
    if (!transferFunction) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create gray profile
    cmsHPROFILE hGray = cmsCreateGrayProfile(&whitePoint, transferFunction);
    if (!hGray) {
        cmsFreeToneCurve(transferFunction);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get color space of the gray profile
    cmsColorSpaceSignature cs = cmsGetColorSpace(hGray);

    // Create NULL profile
    cmsHPROFILE hNull = cmsCreateNULLProfile();
    if (hNull) {
        // We don't need to do anything with it, just create it
        cmsCloseProfile(hNull);
    }

    // Create MLU and test translations
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (mlu) {
        // Set some test data
        cmsMLUsetASCII(mlu, "en", "US", "Test String");
        
        // Get translation codes
        char languageCode[3], countryCode[3];
        cmsMLUtranslationsCodes(mlu, 0, languageCode, countryCode);
        
        cmsMLUfree(mlu);
    }

    // Create identity stage
    cmsStage* identityStage = cmsStageAllocIdentity(ctx, 3);
    if (identityStage) {
        // Get input channels of the stage
        cmsUInt32Number inputChannels = cmsStageInputChannels(identityStage);
        (void)inputChannels;  // Use variable to avoid unused warning
        
        cmsStageFree(identityStage);
    }

    // Create IT8 handle from input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE hIT8 = cmsIT8LoadFromFile(ctx, "input_file");
        if (hIT8) {
            // Try to get data using a sample patch and sample name
            // Using default values since we don't know the actual structure
            const char* patch = "1";
            const char* sample = "R";
            
            cmsFloat64Number value = cmsIT8GetDataDbl(hIT8, patch, sample);
            (void)value;  // Use variable to avoid unused warning
            
            cmsIT8Free(hIT8);
        }
        assert_file_closed(&in_file);;
    }

    // Cleanup
    cmsFreeToneCurve(transferFunction);
    cmsCloseProfile(hGray);
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	return 0;
}