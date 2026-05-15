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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 35
//<Quality> {"density":10,"unique_branches":{"cmsSmoothToneCurve":[[1220,9,1220,20,0,0,4,0],[1220,24,1220,49,0,0,4,0],[1224,13,1224,39,0,0,4,0],[1227,17,1227,44,0,0,4,0],[1234,21,1234,30,0,0,4,0],[1234,34,1234,43,0,0,4,0],[1234,47,1234,56,0,0,4,0],[1240,33,1240,43,0,0,4,0],[1240,33,1240,43,0,0,4,1],[1246,25,1246,35,0,0,4,1],[1252,25,1252,91,0,0,4,0],[1257,42,1257,47,0,0,4,0],[1259,33,1259,43,0,0,4,1],[1260,33,1260,47,0,0,4,0],[1260,33,1260,47,0,0,4,1],[1261,33,1261,48,0,0,4,0],[1261,33,1261,48,0,0,4,1],[1269,29,1269,42,0,0,4,1],[1275,29,1275,42,0,0,4,1],[1281,29,1281,42,0,0,4,1],[1302,21,1302,30,0,0,4,0],[1305,21,1305,30,0,0,4,0],[1308,21,1308,30,0,0,4,0]],"smooth2":[[1161,9,1161,14,0,0,4,1],[1161,18,1161,47,0,0,4,1],[1167,9,1167,18,0,0,4,0],[1167,22,1167,31,0,0,4,0],[1167,35,1167,44,0,0,4,0],[1179,17,1179,26,0,0,4,0],[1179,17,1179,26,0,0,4,1],[1198,21,1198,26,0,0,4,0],[1198,21,1198,26,0,0,4,1],[1205,9,1205,18,0,0,4,0],[1206,9,1206,18,0,0,4,0],[1207,9,1207,18,0,0,4,0]]},"library_calls":["cmsOpenProfileFromFile","cmsGetHeaderCreationDateTime","cmsCreateLab4Profile","cmsCloseProfile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromFile","cmsGetHeaderCreationDateTime","cmsCreateLab4Profile","cmsCreateTransform","cmsGetTransformPipeline","cmsDoTransform","cmsDeleteTransform","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictFree","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Write input data to a file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 1. Open profile from the file (using the input data)
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (!hProfile) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Get creation date/time from profile header
    struct tm creationDate;
    cmsBool gotDate = cmsGetHeaderCreationDateTime(hProfile, &creationDate);
    (void)gotDate; // Use variable to avoid unused warning

    // 3. Create a Lab profile (will serve as destination profile)
    cmsHPROFILE labProfile = cmsCreateLab4Profile(nullptr); // Using D50 white point
    if (!labProfile) {
        cmsCloseProfile(hProfile);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Create a transform from input profile to Lab profile
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hProfile, TYPE_Lab_DBL,
        labProfile, TYPE_Lab_DBL,
        INTENT_PERCEPTUAL, 0);
    
    if (hTransform) {
        // 5. Get the transform pipeline (to inspect or modify)
        const cmsPipeline *pipeline = cmsGetTransformPipeline(hTransform);
        (void)pipeline; // Use variable to avoid unused warning
        
        // 6. Process a sample color through the transform
        cmsCIELab labInput = {50.0, 0.0, 0.0}; // Neutral gray
        cmsCIELab labOutput;
        cmsDoTransform(hTransform, &labInput, &labOutput, 1);
        
        cmsDeleteTransform(hTransform);
    }

    // 7. Create a tone curve and smooth it
    cmsToneCurve *curve = cmsBuildGamma(nullptr, 2.2); // Gamma 2.2 curve
    if (curve) {
        // Smooth the tone curve with lambda = 0.5
        cmsBool smoothed = cmsSmoothToneCurve(curve, 0.5);
        (void)smoothed; // Use variable to avoid unused warning
        
        // Free the tone curve
        cmsFreeToneCurve(curve);
    }

    // 8. Create a dictionary and add an entry
    cmsHANDLE hDict = cmsDictAlloc(nullptr);
    if (hDict) {
        // Create MLU objects for display names/values
        cmsMLU *displayName = cmsMLUalloc(nullptr, 1);
        cmsMLU *displayValue = cmsMLUalloc(nullptr, 1);
        
        if (displayName && displayValue) {
            // Set English text for display
            cmsMLUsetWide(displayName, "EN", "US", L"Test Entry");
            cmsMLUsetWide(displayValue, "EN", "US", L"Test Value");
            
            // Add dictionary entry
            cmsBool added = cmsDictAddEntry(hDict, 
                                           L"TestName", 
                                           L"TestValue", 
                                           displayName, 
                                           displayValue);
            (void)added; // Use variable to avoid unused warning
        }
        
        // Clean up MLU objects
        if (displayName) cmsMLUfree(displayName);
        if (displayValue) cmsMLUfree(displayValue);
        
        // Free dictionary
        cmsDictFree(hDict);
    }

    // Clean up profiles
    cmsCloseProfile(labProfile);
    cmsCloseProfile(hProfile);
    
    // Remove temporary file
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}