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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsCreateContext","cmsBuildParametricToneCurve","cmsDeleteContext","cmsCreateLinearizationDeviceLinkTHR","cmsGetHeaderAttributes","cmsCreateLab2ProfileTHR","cmsCreateTransformTHR","cmsDoTransformStride","cmsGetAlarmCodes","cmsLCh2Lab","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsBuildParametricToneCurve","cmsCreateLinearizationDeviceLinkTHR","cmsGetHeaderAttributes","cmsCreateLab2ProfileTHR","cmsCreateTransformTHR","cmsDoTransformStride","cmsGetAlarmCodes","cmsLCh2Lab","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	  // Minimum data for basic operations

    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Create linearization device link profile
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; ++i) {
        // Simple gamma curve using first 3 bytes as gamma values
        double gamma = data[i] / 100.0 + 0.5;  // Range 0.5-3.05
        double params[4] = {gamma, 0.0, 0.0, 0.0};
        curves[i] = cmsBuildParametricToneCurve(context, 1, params);
        if (!curves[i]) {
            cmsDeleteContext(context);
            return 0;
        }
    }
    
    cmsHPROFILE deviceLink = cmsCreateLinearizationDeviceLinkTHR(
        context, cmsSigRgbData, curves);
    
    // 2. Get header attributes
    cmsUInt64Number flags = 0;
    if (deviceLink) {
        cmsGetHeaderAttributes(deviceLink, &flags);
    }
    
    // 3. Create transform for color conversion
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, nullptr);
    cmsHTRANSFORM transform = nullptr;
    
    if (deviceLink && labProfile) {
        transform = cmsCreateTransformTHR(context,
            deviceLink, TYPE_RGB_FLT,
            labProfile, TYPE_Lab_FLT,
            INTENT_PERCEPTUAL, 0);
    }
    
    // 4. Perform color transformation with stride
    if (transform) {
        const size_t pixelCount = 4;
        const size_t stride = 16;  // 4 floats with 4-byte alignment
        float input[pixelCount * stride / sizeof(float)];
        float output[pixelCount * stride / sizeof(float)];
        
        // Use data bytes for input colors
        size_t dataUsed = 0;
        for (size_t i = 0; i < sizeof(input)/sizeof(float) && dataUsed + 4 <= size; ++i) {
            input[i] = *reinterpret_cast<const float*>(data + dataUsed);
            dataUsed += 4;
        }
        
        cmsDoTransformStride(transform, input, output, pixelCount, stride);
    }
    
    // 5. Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    
    // 6. LCh to Lab conversion
    cmsCIELCh lch = {50.0, 30.0, 45.0};  // Default values
    if (size >= sizeof(lch)) {
        // Use data for LCh values if available
        lch = *reinterpret_cast<const cmsCIELCh*>(data);
    }
    
    cmsCIELab lab;
    cmsLCh2Lab(&lab, &lch);
    
    // 7. Add point to gamut boundary descriptor
    cmsHANDLE gbd = cmsGBDAlloc(context);
    if (gbd) {
        cmsGDBAddPoint(gbd, &lab);
        cmsGBDFree(gbd);
    }
    
    // 8. File operations example
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Read and process some data from the file
        uint8_t buffer[32];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    // 9. Output file example
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        if (transform) {
            // Write alarm codes to output
            fwrite(alarmCodes, sizeof(alarmCodes), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (labProfile) cmsCloseProfile(labProfile);
    if (deviceLink) cmsCloseProfile(deviceLink);
    for (int i = 0; i < 3; ++i) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}