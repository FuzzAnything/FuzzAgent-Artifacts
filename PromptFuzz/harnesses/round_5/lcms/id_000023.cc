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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsDeleteContext","cmsIT8GetProperty","cmsIT8EnumPropertyMulti","cmsIT8GetPropertyMulti","cmsIT8Free","cmsDeleteContext","cmsOpenProfileFromStream","cmsIT8Free","cmsDeleteContext","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsCreateTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsTransform2DeviceLink","cmsReadTag","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageOutputChannels","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8GetProperty","cmsIT8EnumPropertyMulti","cmsIT8GetPropertyMulti","cmsOpenProfileFromStream","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCreateTransform","cmsTransform2DeviceLink","cmsReadTag","cmsPipelineGetPtrToFirstStage","cmsStageType","cmsStageOutputChannels","cmsCloseProfile","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to achieve an event where we process IT8 color data, transform it through a color pipeline, and analyze the transformation stages:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data

    // Split data into two parts: IT8 data and profile data
    size_t it8_size = size / 2;
    size_t profile_size = size - it8_size;
    const uint8_t* it8_data = data;
    const uint8_t* profile_data = data + it8_size;

    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Create IT8 handler from first half of data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, it8_data, it8_size);
    if (!hIT8) {
        cmsDeleteContext(context);
        return 0;
    }

    // Use cmsIT8GetProperty
    const char* prop_value = cmsIT8GetProperty(hIT8, "DESCRIPTOR");
    (void)prop_value; // Use the value to avoid unused variable warning

    // Use cmsIT8EnumPropertyMulti
    const char** subpropertyNames = nullptr;
    cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, "COLORANT", &subpropertyNames);
    
    // Use cmsIT8GetPropertyMulti if we have subproperties
    const char* multi_prop = nullptr;
    if (subprop_count > 0 && subpropertyNames != nullptr && subpropertyNames[0] != nullptr) {
        multi_prop = cmsIT8GetPropertyMulti(hIT8, "COLORANT", subpropertyNames[0]);
        (void)multi_prop;
    }

    // Create a profile from the second half of data
    FILE* in_file = fmemopen((void*)profile_data, profile_size, "rb");
    if (!in_file) {
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, "r");
    assert_file_closed(&in_file);;
    
    if (!hProfile) {
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a simple output profile (sRGB-like)
    cmsToneCurve* gamma = cmsBuildGamma(context, 2.2);
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},  // Red
        {0.30, 0.60, 1.0},  // Green
        {0.15, 0.06, 1.0}   // Blue
    };
    cmsToneCurve* transferFunc[3] = {gamma, gamma, gamma};
    
    cmsHPROFILE hOutput = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunc);
    cmsFreeToneCurve(gamma);
    
    if (!hOutput) {
        cmsCloseProfile(hProfile);
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hProfile,
        TYPE_RGB_8,
        hOutput,
        TYPE_RGB_8,
        INTENT_PERCEPTUAL,
        0
    );

    if (!hTransform) {
        cmsCloseProfile(hOutput);
        cmsCloseProfile(hProfile);
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Use cmsTransform2DeviceLink
    cmsHPROFILE hDeviceLink = cmsTransform2DeviceLink(hTransform, 4.0, 0);
    
    if (hDeviceLink) {
        // Get the pipeline from device link
        cmsPipeline* pipeline = (cmsPipeline*)cmsReadTag(hDeviceLink, cmsSigBToA0Tag);
        
        if (pipeline) {
            // Get the first stage
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            
            if (firstStage) {
                // Use cmsStageType
                cmsStageSignature stageType = cmsStageType(firstStage);
                (void)stageType;
                
                // Use cmsStageOutputChannels
                cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
                (void)outputChannels;
            }
        }
        
        cmsCloseProfile(hDeviceLink);
    }

    // Cleanup
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hOutput);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}