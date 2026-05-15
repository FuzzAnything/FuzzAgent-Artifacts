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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 1
//<Quality> {"density":26,"unique_branches":{"AllocateToneCurveStruct":[[286,42,286,75,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsGetHeaderManufacturer","cmsReadTag","cmsMLUtranslationsCodes","cmsMLUdup","cmsMLUfree","cmsSetPCS","cmsBuildSegmentedToneCurve","cmsBuildGamma","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsXYZ2xyY","cmsOpenIOhandlerFromNULL","cmsCloseIOhandler","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsGetColorSpace","cmsChannelsOfColorSpace","cmsGetHeaderManufacturer","cmsReadTag","cmsMLUtranslationsCodes","cmsMLUdup","cmsMLUfree","cmsSetPCS","cmsBuildSegmentedToneCurve","cmsBuildGamma","cmsBuildTabulatedToneCurveFloat","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsXYZ2xyY","cmsOpenIOhandlerFromNULL","cmsCloseIOhandler","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Write input data to temporary file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Get color space information
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
    int channels = cmsChannelsOfColorSpace(cs);
    
    // Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    
    // Read copyright tag (common tag)
    cmsMLU* copyrightMLU = (cmsMLU*)cmsReadTag(hProfile, cmsSigCopyrightTag);
    if (copyrightMLU) {
        char lang[3] = {0}, country[3] = {0};
        // Get first translation code
        cmsMLUtranslationsCodes(copyrightMLU, 0, lang, country);
        
        // Duplicate the MLU
        cmsMLU* mluCopy = cmsMLUdup(copyrightMLU);
        if (mluCopy) {
            cmsMLUfree(mluCopy);
        }
    }
    
    // Set PCS to Lab
    cmsSetPCS(hProfile, cmsSigLabData);
    
    // Build various tone curves
    cmsCurveSegment segments[2];
    segments[0].Type = 6;  // Type 6: y = (ax + b)^γ + c
    segments[0].Params[0] = 1.0;
    segments[0].Params[1] = 0.0;
    segments[0].Params[2] = 1.0;
    segments[0].Params[3] = 0.0;
    segments[0].x0 = 0.0;
    segments[0].x1 = 1.0;
    
    segments[1].Type = 0;  // Type 0: linear
    segments[1].Params[0] = 1.0;
    segments[1].Params[1] = 0.0;
    segments[1].x0 = 1.0;
    segments[1].x1 = 2.0;
    
    cmsToneCurve* segmentedCurve = cmsBuildSegmentedToneCurve(ctx, 2, segments);
    
    // Build gamma curve
    cmsToneCurve* gammaCurve = cmsBuildGamma(ctx, 2.2);
    
    // Build tabulated curve
    float tabValues[] = {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f};
    cmsToneCurve* tabCurve = cmsBuildTabulatedToneCurveFloat(ctx, 6, tabValues);
    
    // Create RGB profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};  // D50
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},   // Red
        {0.30, 0.60, 1.0},   // Green
        {0.15, 0.06, 1.0}    // Blue
    };
    cmsToneCurve* transferFunc[3] = {gammaCurve, gammaCurve, gammaCurve};
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transferFunc);
    
    // Allocate named color list
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
    
    // Convert XYZ to xyY
    cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
    cmsCIExyY xyY;
    cmsXYZ2xyY(&xyY, &xyz);
    
    // Create IO handler for output
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create IO handler from file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Use fd if needed
        
        // Close IO handler
        cmsIOHANDLER* io = cmsOpenIOhandlerFromNULL(ctx);
        if (io) {
            cmsCloseIOhandler(io);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (segmentedCurve) cmsFreeToneCurve(segmentedCurve);
    if (gammaCurve) cmsFreeToneCurve(gammaCurve);
    if (tabCurve) cmsFreeToneCurve(tabCurve);
    if (namedColors) cmsFreeNamedColorList(namedColors);
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    
    // Close main profile
    cmsCloseProfile(hProfile);
    
    // Delete context
    cmsDeleteContext(ctx);
    
    // Clean up temporary files
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}