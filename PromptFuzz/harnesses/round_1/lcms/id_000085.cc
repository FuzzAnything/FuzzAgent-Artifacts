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
//<ID> 668
//<Prompt> []
/*<Combination>: [
*/
//<score> 124, nr_unique_branch: 26
//<Quality> {"density":31,"unique_branches":{"cmsStageSampleCLut16bit":[[761,9,761,21,0,0,4,0]],"Eval1Input":[[279,12,279,30,0,0,4,0],[279,12,279,30,0,0,4,1],[279,34,279,53,0,0,4,1],[283,30,283,53,0,0,4,0],[283,30,283,53,0,0,4,1],[296,23,296,42,0,0,4,0],[301,30,301,53,0,0,4,0],[301,30,301,53,0,0,4,1]],"TranslateNonICCIntents":[[673,9,673,47,0,0,4,1],[674,9,674,48,0,0,4,1],[677,9,677,58,0,0,4,0],[678,9,678,59,0,0,4,1],[681,9,681,47,0,0,4,1],[682,9,682,48,0,0,4,1],[685,9,685,16,0,0,4,1]],"_MultiplyMatrix":[[213,29,213,61,0,0,4,0]],"DefaultInterpolatorsFactory":[[1196,20,1196,40,0,0,4,1],[1206,24,1206,31,0,0,4,1]],"is_cmyk_devicelink":[[723,12,723,58,0,0,4,1]],"BlackPreservingKOnlyIntents":[[748,9,748,22,0,0,4,1],[748,26,748,41,0,0,4,1],[751,15,751,28,0,0,4,0],[751,15,751,28,0,0,4,1],[760,12,760,44,0,0,4,1],[772,9,772,57,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsPluginTHR","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsBuildGamma","cmsD50_xyY","cmsCreateGrayProfileTHR","cmsCreateRGBProfile","cmsSetAlarmCodes","cmsSetAlarmCodesTHR","cmsGetHeaderAttributes","cmsGetHeaderRenderingIntent","cmsSetProfileVersion","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsReadTag","cmsDetectDestinationBlackPoint","cmsCreateTransform","cmsDoTransform","cmsStageAllocIdentity","cmsStageSampleCLut16bit","cmsStageFree","cmsDeleteTransform","cmsSaveProfileToFile","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPluginTHR","cmsOpenProfileFromStreamTHR","cmsBuildGamma","cmsD50_xyY","cmsCreateGrayProfileTHR","cmsCreateRGBProfile","cmsSetAlarmCodes","cmsSetAlarmCodesTHR","cmsGetHeaderAttributes","cmsGetHeaderRenderingIntent","cmsSetProfileVersion","cmsMLUalloc","cmsMLUtranslationsCodes","cmsMLUfree","cmsReadTag","cmsDetectDestinationBlackPoint","cmsCreateTransform","cmsDoTransform","cmsStageAllocIdentity","cmsStageSampleCLut16bit","cmsStageFree","cmsDeleteTransform","cmsSaveProfileToFile","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimum size for meaningful processing

    // 1. Create context and plugin
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Try to register a plugin (NULL plugin)
    cmsPluginTHR(ctx, nullptr);

    // 2. Open profile from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create tone curves and profiles
    cmsToneCurve* gammaCurve = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve* rgbCurves[3] = {gammaCurve, gammaCurve, gammaCurve};
    
    // Create gray profile
    const cmsCIExyY* d50 = cmsD50_xyY();
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(ctx, d50, gammaCurve);
    
    // Create RGB profile
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 0.2126},  // sRGB red
        {0.3000, 0.6000, 0.7152},  // sRGB green
        {0.1500, 0.0600, 0.0722}   // sRGB blue
    };
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(d50, &primaries, rgbCurves);
    
    // 4. Set alarm codes (global and thread-specific)
    cmsUInt16Number alarmCodes[16] = {0};
    for (int i = 0; i < 16; i++) alarmCodes[i] = i * 1000;
    cmsSetAlarmCodes(alarmCodes);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    
    // 5. Get header information
    cmsUInt64Number flags = 0;
    cmsGetHeaderAttributes(hProfile, &flags);
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    
    // 6. Set profile version
    cmsSetProfileVersion(hProfile, 4.2);
    
    // 7. Create MLU and get translation codes
    cmsMLU* mlu = cmsMLUalloc(ctx, 10);
    if (mlu) {
        char lang[3] = {'e','n','\0'};
        char country[3] = {'U','S','\0'};
        cmsMLUtranslationsCodes(mlu, 0, lang, country);
        cmsMLUfree(mlu);
    }
    
    // 8. Read tag from profile
    void* tagData = cmsReadTag(hProfile, cmsSigCopyrightTag);
    
    // 9. Detect black point
    cmsCIEXYZ blackPoint;
    cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, 0);
    
    // 10. Create color transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        grayProfile, TYPE_GRAY_8,
        rgbProfile, TYPE_RGB_8,
        intent, 0
    );
    
    // 11. Perform color transformation
    if (transform) {
        uint8_t inputBuffer[256];
        uint8_t outputBuffer[256 * 3];
        
        // Use some data from input
        size_t samples = size < 256 ? size : 256;
        memcpy(inputBuffer, data, samples);
        
        cmsDoTransform(transform, inputBuffer, outputBuffer, samples);
        
        // 12. Try to sample CLUT (requires a stage)
        // Create a simple stage for sampling
        cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
        if (stage) {
            struct SamplerData {
                uint8_t* data;
                size_t size;
            } samplerData = {(uint8_t*)data, size};
            
            auto sampler = [](const cmsUInt16Number* in, cmsUInt16Number* out, void* cargo) {
                auto* sd = static_cast<SamplerData*>(cargo);
                // Simple sampler that uses input data
                for (int i = 0; i < 3 && sd->size > 0; i++) {
                    out[i] = (sd->data[0] << 8) | sd->data[1];
                    sd->data += 2;
                    sd->size -= 2;
                }
                return 1;
            };
            
            cmsStageSampleCLut16bit(stage, sampler, &samplerData, 0);
            cmsStageFree(stage);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // 13. Save modified profile to file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToFile(hProfile, "output_file");
        assert_file_closed(&out_file);;
    }
    
    // 14. Cleanup
    cmsFreeToneCurve(gammaCurve);  // Free the curve we created
    cmsCloseProfile(hProfile);
    cmsCloseProfile(grayProfile);
    cmsCloseProfile(rgbProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}