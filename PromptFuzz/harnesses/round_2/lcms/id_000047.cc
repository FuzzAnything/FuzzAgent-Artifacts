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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsTagLinkedTo","cmsChangeBuffersFormat","cmsDesaturateLab","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsTagLinkedTo","cmsChangeBuffersFormat","cmsDesaturateLab","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // 1. Create input profile from memory data
    cmsHPROFILE hInProfile = cmsOpenProfileFromMem(data, size);
    if (!hInProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Create a simple sRGB output profile
    cmsHPROFILE hOutProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (!hOutProfile) {
        cmsCloseProfile(hInProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 3. Create transform using the input profile
    cmsUInt32Number inputFormat = TYPE_Lab_16;
    cmsUInt32Number outputFormat = TYPE_RGB_16;
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hInProfile, inputFormat,
        hOutProfile, outputFormat,
        INTENT_PERCEPTUAL, 0);
    
    if (!hTransform) {
        cmsCloseProfile(hOutProfile);
        cmsCloseProfile(hInProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 4. Get the transform pipeline
    cmsPipeline *pipeline = cmsGetTransformPipeline(hTransform);
    if (pipeline && cmsPipelineInputChannels(pipeline) > 0) {
        // 5. If there are stages in the pipeline, try to duplicate one
        // (Note: cmsStageDup is not in provided API list, so we simulate using available APIs)
        // Instead, we'll work with what we have from the event APIs
        // Use cmsTagLinkedTo to check tag relationships
        cmsTagSignature linkedTag = cmsTagLinkedTo(hInProfile, cmsSigAToB0Tag);
        // linkedTag might be 0 (no linked tag) or another tag signature
        (void)linkedTag; // Use variable to avoid unused warning
    }

    // 6. Change buffer format of the transform
    // Change from Lab16 to Lab8 if possible
    cmsBool formatChanged = cmsChangeBuffersFormat(
        hTransform, TYPE_Lab_8, TYPE_RGB_8);
    (void)formatChanged; // Use variable

    // 7. Test desaturation on Lab color
    cmsCIELab labColor;
    // Initialize with some values from input data
    size_t offset = size > 24 ? 24 : 0;
    const uint8_t *labData = data + (offset % (size - 12));
    
    if (size > offset + 12) {
        // Use input bytes to initialize Lab values
        labColor.L = (labData[0] / 255.0) * 100.0;
        labColor.a = (labData[4] / 255.0) * 128.0 - 64.0;
        labColor.b = (labData[8] / 255.0) * 128.0 - 64.0;
        
        // Desaturate the Lab color
        cmsBool desaturated = cmsDesaturateLab(&labColor, 50.0, -50.0, 50.0, -50.0);
        (void)desaturated; // Use variable
    }

    // 8. Process some test data through the transform
    if (size >= 6) {
        std::vector<uint8_t> inputBuffer(6);
        std::vector<uint8_t> outputBuffer(6);
        
        // Use first 6 bytes as input if available
        size_t copySize = size < 6 ? size : 6;
        memcpy(inputBuffer.data(), data, copySize);
        
        // Fill remainder with default values if needed
        for (size_t i = copySize; i < 6; i++) {
            inputBuffer[i] = 128;
        }
        
        cmsDoTransform(hTransform, inputBuffer.data(), outputBuffer.data(), 1);
    }

    // 9. Cleanup
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hOutProfile);
    cmsCloseProfile(hInProfile);
    cmsDeleteContext(ctx);

    return 0;
}