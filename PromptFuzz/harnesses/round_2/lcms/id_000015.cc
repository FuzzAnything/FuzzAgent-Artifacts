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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"cmsIT8SetIndexColumn":[[3040,8,3040,17,0,0,4,0]]},"library_calls":["cmsCreateBCHSWabstractProfile","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsDeleteTransform","cmsCloseProfile","cmsXYZ2xyY","cmsIT8LoadFromMem","cmsIT8SetIndexColumn","cmsIT8Free","cmsCloseProfile"],"critical_calls":["cmsCreateBCHSWabstractProfile","cmsBuildGamma","cmsEvalToneCurve16","cmsFreeToneCurve","cmsCreate_sRGBProfile","cmsCreateTransform","cmsChangeBuffersFormat","cmsDeleteTransform","cmsCloseProfile","cmsXYZ2xyY","cmsIT8LoadFromMem","cmsIT8SetIndexColumn","cmsIT8Free","cmsCloseProfile"],"visited":0}
/*Here's a step-by-step C++ fuzz driver that uses the specified lcms APIs together:

*/


// Include lcms headers (simplified for the fuzzer)

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Minimum input size check
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Create BCHSW abstract profile using input parameters
    // Extract parameters from input data
    size_t offset = 0;
    
    // Use first 4 bytes for nLUTPoints
    cmsUInt32Number nLUTPoints = 2; // default
    if (size - offset >= sizeof(cmsUInt32Number)) {
        memcpy(&nLUTPoints, data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
        nLUTPoints = nLUTPoints % 100 + 2; // Keep in reasonable range
    }
    
    // Use next 8 bytes for Bright
    cmsFloat64Number Bright = 50.0; // default
    if (size - offset >= sizeof(cmsFloat64Number)) {
        memcpy(&Bright, data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    // Use next 8 bytes for Contrast
    cmsFloat64Number Contrast = 50.0; // default
    if (size - offset >= sizeof(cmsFloat64Number)) {
        memcpy(&Contrast, data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    // Use next 8 bytes for Hue
    cmsFloat64Number Hue = 0.0; // default
    if (size - offset >= sizeof(cmsFloat64Number)) {
        memcpy(&Hue, data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    // Use next 8 bytes for Saturation
    cmsFloat64Number Saturation = 50.0; // default
    if (size - offset >= sizeof(cmsFloat64Number)) {
        memcpy(&Saturation, data + offset, sizeof(cmsFloat64Number));
        offset += sizeof(cmsFloat64Number);
    }
    
    // Use next 4 bytes for TempSrc
    cmsUInt32Number TempSrc = 5000; // default
    if (size - offset >= sizeof(cmsUInt32Number)) {
        memcpy(&TempSrc, data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
        TempSrc = TempSrc % 15000 + 2000; // Keep in reasonable range
    }
    
    // Use next 4 bytes for TempDest
    cmsUInt32Number TempDest = 6500; // default
    if (size - offset >= sizeof(cmsUInt32Number)) {
        memcpy(&TempDest, data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
        TempDest = TempDest % 15000 + 2000; // Keep in reasonable range
    }
    
    // Create BCHSW abstract profile
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfile(
        nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    
    if (!bchswProfile) {
        return 0;
    }
    
    // 2. Create a tone curve and evaluate it
    // Extract some values for tone curve from input
    cmsUInt16Number toneCurveValues[256];
    size_t valuesToRead = size - offset < sizeof(toneCurveValues) ? 
                         (size - offset) / sizeof(cmsUInt16Number) : 
                         256;
    
    if (valuesToRead > 0) {
        // Create a simple tone curve (gamma curve)
        cmsToneCurve* toneCurve = cmsBuildGamma(NULL, 2.2); // Using default context
        if (toneCurve) {
            // Evaluate the tone curve at a point from input data
            cmsUInt16Number inputValue = 128; // default
            if (size - offset >= sizeof(cmsUInt16Number)) {
                memcpy(&inputValue, data + offset, sizeof(cmsUInt16Number));
                offset += sizeof(cmsUInt16Number);
            }
            
            cmsUInt16Number evaluatedValue = cmsEvalToneCurve16(toneCurve, inputValue);
            (void)evaluatedValue; // Use the value to avoid compiler warning
            
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // 3. Create transform and change buffer format
    // Create sRGB profile for transform
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfile();
    if (srgbProfile) {
        // Create transform between profiles
        cmsHTRANSFORM transform = cmsCreateTransform(
            bchswProfile, TYPE_RGB_8,
            srgbProfile, TYPE_RGB_8,
            INTENT_PERCEPTUAL, 0);
        
        if (transform) {
            // Change buffer format using remaining input data
            cmsUInt32Number inputFormat = TYPE_RGB_8; // default
            cmsUInt32Number outputFormat = TYPE_CMYK_8; // default
            
            if (size - offset >= sizeof(cmsUInt32Number)) {
                memcpy(&inputFormat, data + offset, sizeof(cmsUInt32Number));
                offset += sizeof(cmsUInt32Number);
                inputFormat = inputFormat % 20; // Keep in valid range
            }
            
            if (size - offset >= sizeof(cmsUInt32Number)) {
                memcpy(&outputFormat, data + offset, sizeof(cmsUInt32Number));
                offset += sizeof(cmsUInt32Number);
                outputFormat = outputFormat % 20; // Keep in valid range
            }
            
            // Change buffer format
            cmsChangeBuffersFormat(transform, inputFormat, outputFormat);
            
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(srgbProfile);
    }
    
    // 4. Convert XYZ to xyY
    // Create XYZ color from input data
    cmsCIEXYZ xyzColor;
    xyzColor.X = 0.4124; // default
    xyzColor.Y = 0.2126; // default
    xyzColor.Z = 0.0193; // default
    
    if (size - offset >= sizeof(cmsCIEXYZ)) {
        memcpy(&xyzColor, data + offset, sizeof(cmsCIEXYZ));
        offset += sizeof(cmsCIEXYZ);
    }
    
    cmsCIExyY xyYColor;
    cmsXYZ2xyY(&xyYColor, &xyzColor);
    
    // 5. Load IT8 from memory and set index column
    // Use remaining data for IT8
    size_t it8DataSize = size - offset;
    if (it8DataSize > 0) {
        // Load IT8 from memory using the remaining input data
        cmsHANDLE it8Handle = cmsIT8LoadFromMem(NULL, data + offset, it8DataSize);
        
        if (it8Handle) {
            // Try to set index column using a sample name from input
            const char* sampleName = "R"; // default
            
            // Use a byte from input as character for sample name if available
            if (it8DataSize > 1) {
                char customSample[2] = {static_cast<char>(data[offset]), '\0'};
                if (isalnum(customSample[0])) {
                    sampleName = customSample;
                }
            }
            
            cmsIT8SetIndexColumn(it8Handle, sampleName);
            
            // Free IT8 handle
            cmsIT8Free(it8Handle);
        }
    }
    
    // 6. Clean up
    cmsCloseProfile(bchswProfile);
    
    return 0;
}