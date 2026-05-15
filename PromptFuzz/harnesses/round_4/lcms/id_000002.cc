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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsOpenProfileFromStream","cmsfilelength","cmsGetProfileContextID","cmsGetAlarmCodesTHR","cmsMD5computeID","cmsSetHeaderProfileID","cmsLCh2Lab","cmsBuildGamma","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsSaveProfileToFile","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsfilelength","cmsGetProfileContextID","cmsGetAlarmCodesTHR","cmsMD5computeID","cmsSetHeaderProfileID","cmsLCh2Lab","cmsBuildGamma","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsSaveProfileToFile","cmsCloseProfile"],"visited":0}
/**/


// Include necessary LCMS headers

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // 1. Create input FILE* from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Open profile from input file
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file length using cmsfilelength
    long file_len = cmsfilelength(in_file);
    (void)file_len; // Use to avoid unused variable warning

    // 4. Get alarm codes using cmsGetAlarmCodesTHR
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodesTHR(ctx, alarmCodes);

    // 5. Compute MD5 ID for profile
    cmsBool md5_result = cmsMD5computeID(hProfile);
    (void)md5_result; // Use to avoid unused variable warning

    // 6. Set header profile ID (using first 16 bytes of data as dummy ID)
    cmsUInt8Number profileID[16];
    if (size >= 16) {
        memcpy(profileID, data, 16);
    } else {
        memset(profileID, 0, 16);
    }
    cmsSetHeaderProfileID(hProfile, profileID);

    // 7. Convert LCh to Lab using cmsLCh2Lab
    cmsCIELCh lch = {50.0, 60.0, 30.0}; // Sample LCh values
    cmsCIELab lab;
    cmsLCh2Lab(&lab, &lch);

    // 8. Create a tone curve and get its estimated table
    cmsToneCurve* curve = cmsBuildGamma(ctx, 2.2);
    if (curve) {
        const cmsUInt16Number* estimatedTable = cmsGetToneCurveEstimatedTable(curve);
        (void)estimatedTable; // Use to avoid unused variable warning
        cmsFreeToneCurve(curve);
    }

    // 9. Save profile to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToFile(hProfile, fuzz_str_2);
        // Get output file length
        fseek(out_file, 0, SEEK_END);
        long out_len = ftell(out_file);
        (void)out_len; // Use to avoid unused variable warning
        assert_file_closed(&out_file);;
    }

    // 10. Clean up resources
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;

    // Remove output file if created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}