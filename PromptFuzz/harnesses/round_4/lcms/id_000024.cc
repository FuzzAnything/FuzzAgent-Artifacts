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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 95, nr_unique_branch: 9
//<Quality> {"density":19,"unique_branches":{"cmsOpenIOhandlerFromFile":[[467,13,467,23,0,0,4,0]],"cmsGetPostScriptCSA":[[1589,9,1589,23,0,0,4,1],[1594,9,1594,13,0,0,4,1]],"cmsTempFromWhitePoint":[[163,15,163,23,0,0,4,0],[172,13,172,21,0,0,4,0],[172,13,172,21,0,0,4,1],[172,25,172,38,0,0,4,0],[172,25,172,38,0,0,4,1]],"cmsAdaptToIlluminant":[[341,9,341,74,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFile","cmsDeleteContext","cmsDeleteContext","cmsD50_XYZ","cmsXYZ2xyY","cmsTempFromWhitePoint","cmsCreateInkLimitingDeviceLinkTHR","cmsCreate_sRGBProfile","cmsCreateExtendedTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageDup","cmsStageFree","cmsAdaptToIlluminant","cmsDeltaE","cmsGetPostScriptCSA","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsOpenProfileFromFile","cmsD50_XYZ","cmsXYZ2xyY","cmsTempFromWhitePoint","cmsCreateInkLimitingDeviceLinkTHR","cmsCreate_sRGBProfile","cmsCreateExtendedTransform","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageDup","cmsStageFree","cmsAdaptToIlluminant","cmsDeltaE","cmsGetPostScriptCSA","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing

    // 1. Create initial context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Duplicate context (first required API)
    cmsContext dupContext = cmsDupContext(context, nullptr);
    if (!dupContext) {
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Write input data to file for profile loading
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 4. Open profile from the written file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", fuzz_str_1);
    if (!hProfile) {
        remove("input_file");
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 5. Get white point and calculate temperature (second required API)
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 0.0;
    
    // Get profile's white point (simplified - in reality would need proper tag reading)
    cmsCIEXYZ D50 = *cmsD50_XYZ();
    cmsXYZ2xyY(&whitePoint, &D50);
    
    cmsTempFromWhitePoint(&tempK, &whitePoint);

    // 6. Create ink limiting device link (third required API)
    cmsHPROFILE hInkLimit = cmsCreateInkLimitingDeviceLinkTHR(
        context, 
        cmsSigRgbData,  // RGB color space
        0.8              // Limit factor
    );

    // 7. Set up profiles array for extended transform
    std::vector<cmsHPROFILE> profiles;
    profiles.push_back(hProfile);
    if (hInkLimit) profiles.push_back(hInkLimit);
    
    // Add sRGB profile for transformation
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB) profiles.push_back(hSRGB);

    if (profiles.size() >= 2) {
        // 8. Prepare arrays for extended transform
        std::vector<cmsBool> bpc(profiles.size(), FALSE);
        std::vector<cmsUInt32Number> intents(profiles.size(), INTENT_PERCEPTUAL);
        std::vector<cmsFloat64Number> adaptationStates(profiles.size(), 1.0);

        // 9. Create extended transform (fourth required API)
        cmsHTRANSFORM hTransform = cmsCreateExtendedTransform(
            context,
            profiles.size(),
            profiles.data(),
            bpc.data(),
            intents.data(),
            adaptationStates.data(),
            nullptr,  // No gamut profile
            fuzz_uint32_t_2,        // No gamut PCS position
            TYPE_RGB_8,
            TYPE_RGB_8,
            fuzz_uint32_t_3
        );

        if (hTransform) {
            // 10. Get transform pipeline and duplicate a stage (fifth required API)
            cmsPipeline* pipeline = cmsGetTransformPipeline(hTransform);
            if (pipeline) {
                // Get first stage if available
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    cmsStage* dupStage = cmsStageDup(firstStage);
                    if (dupStage) {
                        cmsStageFree(dupStage);
                    }
                }
            }

            // 11. Test color adaptation (sixth required API)
            cmsCIEXYZ sourceColor = {0.5, 0.5, 0.5};
            cmsCIEXYZ adaptedColor;
            cmsCIEXYZ illuminant = {0.95, 1.0, 1.09}; // D65
            
            cmsAdaptToIlluminant(&adaptedColor, &D50, &illuminant, &sourceColor);

            // 12. Calculate Delta E (seventh required API)
            cmsCIELab lab1, lab2;
            cmsFloat64Number deltaE = 0.0;
            
            // Convert XYZ to Lab (simplified - would need proper conversion)
            // For demonstration, use some sample values
            lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
            lab2.L = 55.0; lab2.a = 5.0; lab2.b = 5.0;
            
            deltaE = cmsDeltaE(&lab1, &lab2);

            // 13. Get PostScript CSA (eighth required API)
            std::vector<char> csaBuffer(4096);
            cmsUInt32Number csaSize = cmsGetPostScriptCSA(
                context,
                hProfile,
                fuzz_uint32_t_4,
                fuzz_uint32_t_5,
                csaBuffer.data(),
                csaBuffer.size()
            );

            cmsDeleteTransform(hTransform);
        }
    }

    // 14. Cleanup
    if (hSRGB) cmsCloseProfile(hSRGB);
    if (hInkLimit) cmsCloseProfile(hInkLimit);
    if (hProfile) cmsCloseProfile(hProfile);
    
    remove("input_file");
    
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}