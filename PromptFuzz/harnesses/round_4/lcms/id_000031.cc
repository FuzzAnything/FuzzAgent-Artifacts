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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 55
//<Quality> {"density":1,"unique_branches":{"OutputValueSampler":[[569,9,569,23,0,0,4,1]],"PrelinEval8":[[967,13,967,21,0,0,4,1],[981,33,981,41,0,0,4,1],[988,25,988,33,0,0,4,0],[988,37,988,45,0,0,4,1],[995,29,995,37,0,0,4,0],[995,41,995,49,0,0,4,0]],"WriteNamedColorCSA":[[996,9,996,22,0,0,4,0]],"OptimizeMatrixShaper":[[1691,12,1691,41,0,0,4,0]],"_cmsReadInputLUT":[[317,9,317,61,0,0,4,0],[322,13,322,23,0,0,4,0],[392,9,392,53,0,0,4,0]],"DefaultICCintents":[[576,70,576,86,0,0,4,1]],"AddConversion":[[467,17,467,38,0,0,4,0],[468,21,468,106,0,0,4,1],[469,21,469,132,0,0,4,1],[470,21,470,106,0,0,4,1]],"BuildGrayInputMatrixPipeline":[[163,9,163,24,0,0,4,0]],"OptimizeByResampling":[[663,47,663,82,0,0,4,0]],"Emit1Gamma":[[449,9,449,36,0,0,4,0]],"GammaTableEquals":[[518,9,518,19,0,0,4,0]],"cmsReverseToneCurveEx":[[1118,58,1118,67,0,0,4,1]],"cmsIsToneCurveMonotonic":[[1362,9,1362,20,0,0,4,0],[1366,21,1366,32,0,0,4,0],[1366,21,1366,32,0,0,4,1],[1368,17,1368,42,0,0,4,0],[1368,17,1368,42,0,0,4,1]],"OptimizeByJoiningCurves":[[1410,47,1410,82,0,0,4,0]],"EmitCIEBasedDEF":[[764,5,764,11,0,0,4,1],[771,5,771,11,0,0,4,0],[779,5,779,12,0,0,4,1],[786,9,786,52,0,0,4,1],[795,9,795,48,0,0,4,0]],"WriteInputLUT":[[873,9,873,22,0,0,4,1],[883,5,883,11,0,0,4,1],[890,5,890,11,0,0,4,1],[891,5,891,11,0,0,4,0],[898,17,898,35,0,0,4,1],[908,17,908,20,0,0,4,1],[915,5,915,12,0,0,4,1]],"cmsCreateMultiprofileTransform":[[1323,9,1323,23,0,0,4,1],[1323,27,1323,42,0,0,4,1]],"OptimizeByComputingLinearization":[[1062,47,1062,82,0,0,4,0]],"_cmsReasonableGridpointsByColorspace":[[700,9,700,23,0,0,4,0]],"GenerateCSA":[[1046,9,1046,61,0,0,4,0],[1048,13,1048,55,0,0,4,0],[1058,13,1058,40,0,0,4,1],[1067,13,1067,24,0,0,4,0],[1071,13,1071,118,0,0,4,1],[1078,16,1078,62,0,0,4,1]],"PatchLUT":[[485,9,485,25,0,0,4,0],[497,13,497,29,0,0,4,1],[498,13,498,29,0,0,4,1],[499,13,499,29,0,0,4,1],[500,13,500,29,0,0,4,1]]},"library_calls":["cmsOpenProfileFromMem","cmsOpenProfileFromStreamTHR","cmsCreateContext","cmsCreateProfilePlaceholder","cmsSetColorSpace","cmsSetHeaderModel","cmsGetProfileContextID","cmsGetHeaderFlags","cmsBuildGamma","cmsIsToneCurveMultisegment","cmsCreateLinearizationDeviceLinkTHR","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetTable","cmsIT8SetPropertyMulti","cmsIT8GetData","cmsIT8SetPropertyUncooked","cmsLabEncoded2FloatV2","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCreate_sRGBProfileTHR","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCloseProfile","cmsDeleteTransform","cmsDoTransform","cmsCreateTransform","cmsCreate_sRGBProfileTHR","cmsGetPostScriptColorResource","cmsOpenIOhandlerFromFile","cmsLabEncoded2FloatV2","cmsIT8SetPropertyUncooked","cmsIT8GetData","cmsIT8SetPropertyMulti","cmsIT8SetTable","cmsIT8Alloc","cmsCloseProfile","cmsCreateLinearizationDeviceLinkTHR","cmsIsToneCurveMultisegment","cmsBuildGamma","cmsGetHeaderFlags","cmsGetProfileContextID","cmsSetHeaderModel","cmsSetColorSpace","cmsCreateProfilePlaceholder","cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsOpenProfileFromMem","cmsIT8Free","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


// Include lcms headers

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<99) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_11, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_12, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_19, fuzz_char_array_size_19, fdp);
	
		cmsUInt16Number fuzzer_var_cmsLabEncoded2FloatV2_17_1[3] = {fuzz_uint16_t_11,fuzz_uint16_t_12,fuzz_uint16_t_13};
		const void * fuzzer_var_cmsDoTransform_22_1 = fuzz_char_array_19;//fuzzer shim end}




    // Early exit if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Variables to track resource cleanup
    cmsHPROFILE hProfile = nullptr;
    cmsHANDLE hIT8 = nullptr;
    cmsToneCurve* curve = nullptr;
    cmsToneCurve* transferFunctions[3] = {nullptr, nullptr, nullptr};
    cmsContext context = nullptr;
    cmsIOHANDLER* ioHandler = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    try {
        // 1. Create a profile from input data
        hProfile = cmsOpenProfileFromMem(data, size);
        if (!hProfile) {
            // Try opening as stream
            in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                hProfile = cmsOpenProfileFromStreamTHR(nullptr, in_file, fuzz_str_1);
            }
        }
        
        if (!hProfile) {
            // Create a placeholder profile if we can't open from input
            context = cmsCreateContext(nullptr, nullptr);
            hProfile = cmsCreateProfilePlaceholder(context);
            
            // Set some basic properties
            cmsSetColorSpace(hProfile, cmsSigRgbData);
            cmsSetHeaderModel(hProfile, fuzz_uint32_t_2);
        }
        
        // 2. Get profile context ID
        context = cmsGetProfileContextID(hProfile);
        
        // 3. Get header flags
        cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
        
        // 4. Create tone curves for linearization device link
        for (int i = 0; i < 3; i++) {
            transferFunctions[i] = cmsBuildGamma(context, 2.2);
        }
        
        // 5. Check if tone curve is multisegment
        bool isMultiSegment = cmsIsToneCurveMultisegment(transferFunctions[0]);
        
        // 6. Create linearization device link
        cmsHPROFILE deviceLink = cmsCreateLinearizationDeviceLinkTHR(
            context, 
            cmsSigRgbData, 
            transferFunctions
        );
        
        if (deviceLink) {
            cmsCloseProfile(deviceLink);
        }
        
        // 7. Create and use IT8 handle
        hIT8 = cmsIT8Alloc(context);
        if (hIT8) {
            // Set up a simple table
            cmsIT8SetTable(hIT8, fuzz_uint32_t_3);
            
            // Set property with subkey
            const char* key = "Sample";
            const char* subKey = "Color";
            const char* value = "RGB";
            cmsIT8SetPropertyMulti(hIT8, fuzz_str_4, fuzz_str_5, fuzz_str_6);
            
            // Get data from IT8
            const char* patch = "1";
            const char* sample = "R";
            const char* retrievedData = cmsIT8GetData(hIT8, fuzz_str_7, fuzz_str_8);
            
            // Set some more properties for demonstration
            cmsIT8SetPropertyUncooked(hIT8, fuzz_str_9, fuzz_str_10);
        }
        
        // 8. Use cmsLabEncoded2FloatV2
        cmsUInt16Number encodedLab[3] = {0x7FFF, 0x0000, 0x0000}; // Neutral gray
        cmsCIELab labFloat;
        cmsLabEncoded2FloatV2(&labFloat, fuzzer_var_cmsLabEncoded2FloatV2_17_1);
        
        // 9. Dictionary operations (simulated since we don't have a dictionary)
        // Create a simple dictionary entry structure for demonstration
        struct SimpleDictEntry {
            const char* name;
            const char* value;
            SimpleDictEntry* next;
        };
        
        // 10. Get PostScript color resource
        out_file = fopen("output_file", "wb");
        if (out_file) {
            ioHandler = cmsOpenIOhandlerFromFile(context, "output_file", fuzz_str_14);
            if (ioHandler) {
                cmsUInt32Number resourceSize = cmsGetPostScriptColorResource(
                    context,
                    cmsPS_RESOURCE_CSA,
                    hProfile,
                    fuzz_uint32_t_15,
                    fuzz_uint32_t_16,
                    ioHandler
                );
            }
        }
        
        // 11. Process some color data through transform if possible
        if (hProfile) {
            // Create a simple transform
            cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
            if (srgbProfile) {
                cmsHTRANSFORM transform = cmsCreateTransform(
                    srgbProfile,
                    TYPE_RGB_8,
                    hProfile,
                    TYPE_RGB_8,
                    fuzz_uint32_t_17,
                    fuzz_uint32_t_18
                );
                
                if (transform) {
                    // Process some sample data
                    unsigned char input[3] = {128, 128, 128};
                    unsigned char output[3];
                    cmsDoTransform(transform, fuzzer_var_cmsDoTransform_22_1, output, 1);
                    cmsDeleteTransform(transform);
                }
                cmsCloseProfile(srgbProfile);
            }
        }
        
    } catch (...) {
        // Catch any exceptions
    }
    
    // Cleanup resources
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    for (int i = 0; i < 3; i++) {
        if (transferFunctions[i]) {
            cmsFreeToneCurve(transferFunctions[i]);
        }
    }
    
    if (curve) {
        cmsFreeToneCurve(curve);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    if (ioHandler) {
        cmsCloseIOhandler(ioHandler);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}