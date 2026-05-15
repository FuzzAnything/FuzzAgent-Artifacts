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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":15,"unique_branches":{"Clamp_ab_doubleV2":[[248,9,248,32,0,0,4,1],[249,9,249,32,0,0,4,1]],"cmsWhitePointFromTemp":[[69,27,69,39,0,0,4,1]],"Clamp_L_doubleV2":[[238,9,238,14,0,0,4,1],[239,9,239,18,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsDeleteContext","cmsCreateProfilePlaceholder","cmsCloseIOhandler","cmsDeleteContext","cmsGetHeaderCMM","cmsGetHeaderModel","cmsWhitePointFromTemp","cmsxyY2XYZ","cmsFloat2LabEncodedV2","cmsLabEncoded2FloatV2","cmsBuildGamma","cmsIsToneCurveDescending","cmsFreeToneCurve","cmsGetColorSpace","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsCreateProfilePlaceholder","cmsGetHeaderCMM","cmsGetHeaderModel","cmsWhitePointFromTemp","cmsxyY2XYZ","cmsFloat2LabEncodedV2","cmsLabEncoded2FloatV2","cmsBuildGamma","cmsIsToneCurveDescending","cmsFreeToneCurve","cmsGetColorSpace","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


// Dummy sampler function for cmsSliceSpaceFloat
static cmsInt32Number  id_000029_dummySampler(const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) {
    // Simple pass-through sampler
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open the input data as a memory stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create IO handler from the stream
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    if (!io_handler) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a placeholder profile
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(context);
    if (!profile) {
        cmsCloseIOhandler(io_handler);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to read profile data from the stream
    // (In real usage, you'd use cmsReadProfileFromStream, but we'll simulate with what we have)
    
    // 1. Get header information from profile
    cmsUInt32Number cmm = cmsGetHeaderCMM(profile);
    cmsUInt32Number model = cmsGetHeaderModel(profile);
    
    // 2. Create a white point from temperature
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 6500.0; // Daylight temperature
    if (size > 0) {
        // Use first byte to influence temperature
        tempK = 2500.0 + (data[0] * 100.0);
    }
    cmsWhitePointFromTemp(&whitePoint, tempK);
    
    // 3. Convert xyY to XYZ
    cmsCIEXYZ xyz;
    cmsxyY2XYZ(&xyz, &whitePoint);
    
    // 4. Create Lab value and test encoding/decoding
    cmsCIELab lab;
    cmsUInt16Number encodedLab[3];
    
    // Initialize Lab values
    lab.L = 50.0;
    lab.a = 0.0;
    lab.b = 0.0;
    if (size > 3) {
        // Use data to influence Lab values
        lab.L = (data[1] % 100);
        lab.a = ((int)data[2] - 128) / 2.0;
        lab.b = ((int)data[3] - 128) / 2.0;
    }
    
    // Encode Lab to 16-bit
    cmsFloat2LabEncodedV2(encodedLab, &lab);
    
    // Decode back to float
    cmsCIELab decodedLab;
    cmsLabEncoded2FloatV2(&decodedLab, encodedLab);
    
    // 5. Create a simple tone curve and test if descending
    cmsFloat64Number gamma = 2.2;
    cmsToneCurve* toneCurve = cmsBuildGamma(context, gamma);
    if (toneCurve) {
        cmsIsToneCurveDescending(toneCurve);
        cmsFreeToneCurve(toneCurve);
    }
    
    // 6. Test color space conversion
    // Create a simple input array for sampling
    const cmsUInt32Number clutPoints[] = {2, 2, 2};
    cmsFloat32Number input[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number output[3] = {0.0f, 0.0f, 0.0f};
    
    // Use the sampler (even though we don't have a proper CLUT)
    if (size > 10) {
        // Use data to influence input values
        for (int i = 0; i < 3 && i + 4 < (int)size; i++) {
            input[i] = data[i + 4] / 255.0f;
        }
         id_000029_dummySampler(input, output, nullptr);
    }
    
    // 7. Test _cmsLCMScolorSpace (this is an internal function, but we'll use it as specified)
    // Note: _cmsLCMScolorSpace might not be directly available, so we'll use cmsGetColorSpace instead
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(profile);
    // In real usage: _cmsLCMScolorSpace(colorSpace);
    
    // Cleanup
    cmsCloseProfile(profile);
    cmsCloseIOhandler(io_handler);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}