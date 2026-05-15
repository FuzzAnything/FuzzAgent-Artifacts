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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsOpenProfileFromStream","cmsMD5computeID","cmsSetHeaderProfileID","cmsSaveProfileToFile","cmsGetProfileContextID","cmsGetAlarmCodesTHR","cmsLCh2Lab","cmsBuildGamma","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsfilelength","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStream","cmsMD5computeID","cmsSetHeaderProfileID","cmsSaveProfileToFile","cmsGetProfileContextID","cmsGetAlarmCodesTHR","cmsLCh2Lab","cmsBuildGamma","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","cmsfilelength","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStream(in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Compute MD5 and set profile ID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    if (md5Result) {
        // Get the computed ID and set it again (demonstrating the API)
        cmsUInt8Number profileID[16];
        // Note: cmsMD5computeID already sets the ID internally,
        // but we demonstrate cmsSetHeaderProfileID by setting a dummy ID
        memset(profileID, 0xAA, sizeof(profileID));
        cmsSetHeaderProfileID(hProfile, profileID);
    }

    // 2. Save profile to file
    cmsBool saveResult = cmsSaveProfileToFile(hProfile, fuzz_str_2);

    // 3. Get alarm codes
    cmsContext context = cmsGetProfileContextID(hProfile);
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodesTHR(context, alarmCodes);

    // 4. Use cmsLCh2Lab for color conversion
    cmsCIELCh lchColor = {50.0, 30.0, 45.0}; // Example LCh values
    cmsCIELab labColor;
    cmsLCh2Lab(&labColor, &lchColor);

    // 5. Get tone curve estimated table
    // Create a simple tone curve to demonstrate the API
    cmsToneCurve* toneCurve = cmsBuildGamma(context, 2.2);
    if (toneCurve) {
        const cmsUInt16Number* estimatedTable = cmsGetToneCurveEstimatedTable(toneCurve);
        (void)estimatedTable; // Use to avoid unused variable warning
        cmsFreeToneCurve(toneCurve);
    }

    // 6. Use cmsfilelength on the input file
    long fileLen = cmsfilelength(in_file);
    (void)fileLen; // Use to avoid unused variable warning

    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;

    // Also clean up output file if it was created
    if (saveResult) {
        remove("output_profile.icc");
    }

    assert_file_closed(&in_file);
	return 0;
}