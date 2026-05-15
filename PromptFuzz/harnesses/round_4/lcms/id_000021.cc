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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 10
//<Quality> {"density":17,"unique_branches":{"cmsSetAdaptationStateTHR":[[66,9,66,17,0,0,4,0]],"cmsDictGetEntryList":[[1194,9,1194,21,0,0,4,0]],"cmsCIE2000DeltaE":[[620,32,620,64,0,0,4,0],[620,32,620,64,0,0,4,1],[621,29,621,48,0,0,4,0],[621,29,621,48,0,0,4,1],[624,32,624,61,0,0,4,0],[624,32,624,61,0,0,4,1],[625,29,625,49,0,0,4,0],[625,29,625,49,0,0,4,1]]},"library_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsSetAdaptationStateTHR","cmsGetProfileContextID","cmsIT8Alloc","cmsGetProfileContextID","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8EnumDataFormat","cmsIT8Free","cmsIT8Free","cmsCreate_sRGBProfile","cmsDictGetEntryList","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsGetProfileContextID","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsCIE2000DeltaE","cmsSetHeaderFlags","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsGetHeaderProfileID","cmsSetAdaptationStateTHR","cmsGetProfileContextID","cmsIT8Alloc","cmsGetProfileContextID","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8EnumDataFormat","cmsIT8Free","cmsIT8Free","cmsCreate_sRGBProfile","cmsDictGetEntryList","cmsCloseProfile","cmsBuildTabulatedToneCurveFloat","cmsGetProfileContextID","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsCIE2000DeltaE","cmsSetHeaderFlags","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Open profile from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get profile ID using cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);

    // 3. Set adaptation state using cmsSetAdaptationStateTHR
    // Use first 8 bytes as double value for adaptation state
    double adaptationState = 0.0;
    if (size >= 8) {
        memcpy(&adaptationState, data, sizeof(double));
    }
    cmsSetAdaptationStateTHR(cmsGetProfileContextID(hProfile), adaptationState);

    // 4. Create and use IT8 handle with cmsIT8EnumDataFormat
    cmsHANDLE hIT8 = cmsIT8Alloc(cmsGetProfileContextID(hProfile));
    if (hIT8) {
        // Load IT8 data from remaining portion of input
        size_t it8_offset = size > 100 ? 100 : size/2;
        const void* it8_data = data + it8_offset;
        size_t it8_size = size - it8_offset;
        
        cmsHANDLE hIT8Loaded = cmsIT8LoadFromMem(cmsGetProfileContextID(hProfile), 
                                                 it8_data, it8_size);
        if (hIT8Loaded) {
            char** sampleNames = nullptr;
            int dataFormatCount = cmsIT8EnumDataFormat(hIT8Loaded, &sampleNames);
            
            // Clean up sample names if allocated
            if (sampleNames) {
                for (int i = 0; i < dataFormatCount; i++) {
                    free(sampleNames[i]);
                }
                free(sampleNames);
            }
            
            cmsIT8Free(hIT8Loaded);
        }
        cmsIT8Free(hIT8);
    }

    // 5. Get dictionary entry list using cmsDictGetEntryList
    // First create a dummy profile with dictionary tag
    cmsHPROFILE dummyProfile = cmsCreate_sRGBProfile();
    if (dummyProfile) {
        // Create a simple dictionary
        cmsHANDLE hDict = nullptr; // In actual use, would be obtained from profile
        // For fuzzing, we'll create a minimal dictionary scenario
        const cmsDICTentry* entryList = cmsDictGetEntryList(hDict);
        // entryList would be nullptr in this dummy case
        
        cmsCloseProfile(dummyProfile);
    }

    // 6. Build tabulated tone curve using cmsBuildTabulatedToneCurveFloat
    // Use 256 entries from input data
    const int curveEntries = 256;
    if (size >= 50 + curveEntries * sizeof(float)) {
        cmsFloat32Number curveValues[curveEntries];
        size_t curveOffset = 50;  // Start after header info
        for (int i = 0; i < curveEntries && curveOffset + sizeof(float) <= size; i++) {
            memcpy(&curveValues[i], data + curveOffset, sizeof(float));
            curveOffset += sizeof(float);
        }
        
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurveFloat(
            cmsGetProfileContextID(hProfile), 
            curveEntries, 
            curveValues
        );
        
        if (toneCurve) {
            // Evaluate the curve at a few points
            for (int i = 0; i < 10; i++) {
                cmsFloat32Number eval = cmsEvalToneCurveFloat(toneCurve, i / 10.0f);
                (void)eval;  // Prevent unused variable warning
            }
            cmsFreeToneCurve(toneCurve);
        }
    }

    // 7. Compute CIEDE2000 delta E using cmsCIE2000DeltaE
    // Extract Lab values from input data
    cmsCIELab lab1 = {50.0, 0.0, 0.0};  // Default values
    cmsCIELab lab2 = {50.0, 0.0, 0.0};
    
    if (size >= 100) {
        // Use bytes 20-67 for Lab values (6 doubles = 48 bytes)
        size_t labOffset = 20;
        if (labOffset + 6 * sizeof(double) <= size) {
            memcpy(&lab1.L, data + labOffset, sizeof(double));
            memcpy(&lab1.a, data + labOffset + sizeof(double), sizeof(double));
            memcpy(&lab1.b, data + labOffset + 2 * sizeof(double), sizeof(double));
            memcpy(&lab2.L, data + labOffset + 3 * sizeof(double), sizeof(double));
            memcpy(&lab2.a, data + labOffset + 4 * sizeof(double), sizeof(double));
            memcpy(&lab2.b, data + labOffset + 5 * sizeof(double), sizeof(double));
        }
    }
    
    double deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
    (void)deltaE;  // Prevent unused variable warning

    // 8. Set header flags using cmsSetHeaderFlags
    // Extract flags from input data (use first 4 bytes)
    cmsUInt32Number flags = 0;
    if (size >= 4) {
        memcpy(&flags, data, sizeof(cmsUInt32Number));
    }
    cmsSetHeaderFlags(hProfile, fuzz_uint32_t_2);

    // Clean up resources
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}