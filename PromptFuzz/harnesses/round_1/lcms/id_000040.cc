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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 3
//<Quality> {"density":22,"unique_branches":{"BuildRGBOutputMatrixShaper":[[479,9,479,19,0,0,4,0],[479,23,479,33,0,0,4,0],[479,37,479,47,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsDeleteContext","cmsCloseProfile","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsDeleteContext","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildTabulatedToneCurveFloat","cmsStageAllocCLutFloatGranular","cmsPlugin","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsGetHeaderFlags","cmsDeleteTransform","cmsStageFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsGetAlarmCodes","cmsBuildTabulatedToneCurveFloat","cmsStageAllocCLutFloatGranular","cmsPlugin","cmsCreateTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsStageOutputChannels","cmsGetHeaderFlags","cmsDeleteTransform","cmsStageFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context first
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Create an sRGB profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
    if (!sRGBProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // Use input data as a profile stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsCloseProfile(sRGBProfile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from stream
    cmsHPROFILE streamProfile = cmsOpenProfileFromStreamTHR(context, in_file, "rb");
    if (!streamProfile) {
        assert_file_closed(&in_file);;
        cmsCloseProfile(sRGBProfile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get profile info ASCII from sRGB profile
    char buffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfoASCII(sRGBProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer));
    (void)infoSize; // Use the variable to avoid unused warning

    // Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);

    // Create tabulated tone curve from data - use first 4 floats if available
    cmsToneCurve *toneCurve = nullptr;
    if (size >= sizeof(float) * 4) {
        cmsFloat32Number values[4];
        memcpy(values, data, sizeof(values));
        toneCurve = cmsBuildTabulatedToneCurveFloat(context, 4, values);
    }

    // Create CLUT with granular control - use 2x2x2 grid
    cmsStage *clutStage = nullptr;
    if (size >= sizeof(float) * 8) {
        cmsUInt32Number clutPoints[] = {2, 2, 2};
        cmsFloat32Number table[2 * 2 * 2 * 3]; // 8 entries * 3 output channels
        size_t bytesToCopy = sizeof(table) < size ? sizeof(table) : size;
        memcpy(table, data, bytesToCopy);
        
        clutStage = cmsStageAllocCLutFloatGranular(context, clutPoints, 3, 3, table);
    }

    // Call plugin API
    cmsBool pluginResult = cmsPlugin(nullptr);
    (void)pluginResult;

    // Create a transform to get gamut check pipeline
    cmsHTRANSFORM transform = nullptr;
    if (streamProfile && sRGBProfile) {
        transform = cmsCreateTransform(sRGBProfile, TYPE_RGB_8, streamProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    }

    // Get gamut check pipeline from transform
    cmsPipeline *pipeline = nullptr;
    if (transform) {
        pipeline = cmsGetTransformGamutCheckPipeline(transform);
    }

    // Get first stage from pipeline
    cmsStage *firstStage = nullptr;
    if (pipeline) {
        firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    }

    // Get input/output channels if stage exists
    if (firstStage) {
        cmsUInt32Number inputChannels = cmsStageInputChannels(firstStage);
        cmsUInt32Number outputChannels = cmsStageOutputChannels(firstStage);
        (void)inputChannels;
        (void)outputChannels;
    }

    // Get header flags from stream profile
    cmsUInt32Number flags = cmsGetHeaderFlags(streamProfile);
    (void)flags;

    // Cleanup - in reverse order of creation
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (clutStage) {
        cmsStageFree(clutStage);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    cmsCloseProfile(streamProfile);
    cmsCloseProfile(sRGBProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}