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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 6
//<Quality> {"density":14,"unique_branches":{"_cmsDestroyMutex":[[683,9,683,38,0,0,4,1]],"_cmsUnlockMutex":[[702,9,702,37,0,0,4,1]],"_cmsCreateMutex":[[674,9,674,37,0,0,4,0]],"cmsDictDup":[[1171,9,1171,21,0,0,4,1],[1175,12,1175,25,0,0,4,1]],"_cmsLockMutex":[[693,9,693,35,0,0,4,0]]},"library_calls":["cmsfilelength","cmsUnregisterPluginsTHR","cmsWhitePointFromTemp","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsIsIntentSupported","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsCloseProfile","cmsFreeToneCurve"],"critical_calls":["cmsfilelength","cmsUnregisterPluginsTHR","cmsWhitePointFromTemp","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfileTHR","cmsIsIntentSupported","cmsReverseToneCurveEx","cmsFreeToneCurve","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsCloseProfile","cmsFreeToneCurve"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file length using cmsfilelength
    long file_len = cmsfilelength(in_file);
    
    // Create a context for THR functions (using default context)
    cmsContext ctx = nullptr;
    
    // Unregister plugins to start fresh
    cmsUnregisterPluginsTHR(ctx);

    // Parse temperature from first 8 bytes of input (if available)
    cmsFloat64Number tempK = 6500.0; // Default temperature
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(&tempK, data, sizeof(cmsFloat64Number));
        if (tempK < 1000) tempK = 1000;
        if (tempK > 25000) tempK = 25000;
    }

    // Create white point from temperature
    cmsCIExyY whitePoint;
    cmsBool wp_result = cmsWhitePointFromTemp(&whitePoint, tempK);
    if (!wp_result) {
        // Fallback to D50 white point
        whitePoint.x = 0.3457;
        whitePoint.y = 0.3585;
        whitePoint.Y = 1.0;
    }

    // Create a simple tone curve from input data
    const cmsUInt32Number nEntries = 256;
    cmsUInt16Number curveValues[nEntries];
    for (cmsUInt32Number i = 0; i < nEntries; ++i) {
        curveValues[i] = (i < size) ? (data[i] << 8) : (i << 8);
    }
    cmsToneCurve *transferFunc = cmsBuildTabulatedToneCurve16(ctx, nEntries, curveValues);
    if (!transferFunc) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a gray profile using the generated tone curve
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, transferFunc);
    if (!grayProfile) {
        cmsFreeToneCurve(transferFunc);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if perceptual intent is supported
    cmsBool intentSupported = cmsIsIntentSupported(grayProfile, INTENT_PERCEPTUAL, LCMS_USED_AS_INPUT);

    // Create reverse tone curve (using 1024 samples)
    cmsToneCurve *reversedCurve = cmsReverseToneCurveEx(1024, transferFunc);
    if (reversedCurve) {
        // Could create another profile with reversed curve here
        cmsFreeToneCurve(reversedCurve);
    }

    // Duplicate a dictionary (create empty dict first)
    cmsHANDLE hDict = cmsDictAlloc(ctx);
    cmsHANDLE dupDict = nullptr;
    if (hDict) {
        dupDict = cmsDictDup(hDict);
        cmsDictFree(hDict);
        if (dupDict) cmsDictFree(dupDict);
    }

    // Clean up resources
    cmsCloseProfile(grayProfile);
    cmsFreeToneCurve(transferFunc);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}