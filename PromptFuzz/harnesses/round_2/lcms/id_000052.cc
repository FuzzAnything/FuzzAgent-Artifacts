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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"Type_Text_Description_Read":[[1110,9,1110,44,0,0,4,0]],"_cmsICCcolorSpace":[[777,8,777,19,0,0,4,0],[805,8,805,15,0,0,4,0]],"convert_utf16_to_utf32":[[167,42,167,63,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineGetPtrToFirstStage","cmsFreeToneCurve","cmsPipelineFree","_cmsICCcolorSpace","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsGetNamedColorList","cmsNamedColorCount","cmsNamedColorInfo","cmsFreeNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsPipelineGetPtrToFirstStage","cmsFreeToneCurve","cmsPipelineFree","_cmsICCcolorSpace","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Insufficient data for meaningful processing

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Create FILE* from input data
    FILE *icc_file = fmemopen((void *)data, size, "rb");
    if (!icc_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&icc_file);
	return 0;
    }

    // Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, icc_file, "r");
    if (!hProfile) {
        assert_file_closed(&icc_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&icc_file);
	return 0;
    }

    // Get profile info (ASCII)
    char buffer[256];
    cmsUInt32Number bytes = cmsGetProfileInfoASCII(
        hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer)
    );

    // Set header flags (using bits 0-31 from data)
    if (size >= 4) {
        cmsUInt32Number flags = *(cmsUInt32Number*)data;
        cmsSetHeaderFlags(hProfile, flags);
    }

    // Set alarm codes (using first 32 bytes as 16 uint16 values)
    if (size >= 32) {
        cmsUInt16Number alarmCodes[16];
        memcpy(alarmCodes, data, 32);
        cmsSetAlarmCodes(alarmCodes);
    }

    // IT8 handling
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        const char** subProps = nullptr;
        cmsUInt32Number propCount = cmsIT8EnumPropertyMulti(hIT8, "Sample", &subProps);
        cmsIT8Free(hIT8);
    }

    // CIE94 Delta E calculation (using Lab values from bytes 40-71 if available)
    if (size >= 72) {
        cmsCIELab lab1, lab2;
        memcpy(&lab1, data + 40, sizeof(lab1));
        memcpy(&lab2, data + 40 + sizeof(lab1), sizeof(lab2));
        cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    }

    // Read tag (try copyright tag)
    void* tagData = cmsReadTag(hProfile, cmsSigCopyrightTag);

    // Named color list handling
    cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(nullptr);  // Null transform
    if (namedColorList) {
        cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);
        if (colorCount > 0) {
            char name[256], prefix[256], suffix[256];
            cmsUInt16Number pcs[3], colorant[16];
            cmsBool success = cmsNamedColorInfo(
                namedColorList, 0, name, prefix, suffix, pcs, colorant
            );
        }
        cmsFreeNamedColorList(namedColorList);
    }

    // Pipeline operations
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create gamma curve using data-derived value
        cmsFloat64Number gamma = size > 0 ? (data[0] / 255.0 * 4.0 + 0.5) : 2.2;
        cmsToneCurve* toneCurve = cmsBuildGamma(ctx, gamma);
        
        if (toneCurve) {
            cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
            cmsFreeToneCurve(toneCurve);
        }
        cmsPipelineFree(pipeline);
    }

    // Color space conversion (using internal function)
    int notation = size > 10 ? data[10] % 10 : 0;
    cmsColorSpaceSignature cs = _cmsICCcolorSpace(notation);

    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&icc_file);;
    cmsDeleteContext(ctx);

    assert_file_closed(&icc_file);
	return 0;
}