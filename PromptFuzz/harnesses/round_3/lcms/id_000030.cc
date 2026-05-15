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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 23
//<Quality> {"density":16,"unique_branches":{"WriteNamedColorCRD":[[1429,9,1429,22,0,0,4,0]],"_MultiplyMatrix":[[212,26,212,44,0,0,4,0]],"_cmsReadDevicelinkLUT":[[721,9,721,61,0,0,4,0],[725,13,725,23,0,0,4,0]],"AddConversion":[[460,17,460,38,0,0,4,0],[461,17,461,128,0,0,4,1]],"OptimizeMatrixShaper":[[1691,45,1691,75,0,0,4,0]],"DefaultICCintents":[[576,70,576,86,0,0,4,0]],"ColorSpaceIsCompatible":[[498,9,498,32,0,0,4,0],[498,36,498,57,0,0,4,1],[499,9,499,30,0,0,4,0],[499,34,499,57,0,0,4,1]],"DefaultEvalParametricFn":[[365,13,365,18,0,0,4,0],[367,17,367,61,0,0,4,0],[367,17,367,61,0,0,4,1],[374,17,374,55,0,0,4,0]],"WriteOutputLUT":[[1307,9,1307,22,0,0,4,0]],"cmsCIE94DeltaE":[[466,9,466,17,0,0,4,1]],"EmitHeader":[[347,9,347,28,0,0,4,1],[348,9,348,26,0,0,4,1]],"GenerateCRD":[[1492,9,1492,61,0,0,4,0],[1494,13,1494,64,0,0,4,0],[1502,13,1502,60,0,0,4,0]]},"library_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLinkTHR","cmsGetProfileContextID","cmsAllocNamedColorList","cmsGetProfileContextID","cmsAllocNamedColorList","cmsGetProfileContextID","cmsNamedColorCount","cmsLab2XYZ","cmsCIE94DeltaE","cmsGetPostScriptCRD","cmsGetProfileContextID","cmsPluginTHR","cmsGetProfileContextID","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLinkTHR","cmsGetProfileContextID","cmsAllocNamedColorList","cmsGetProfileContextID","cmsAllocNamedColorList","cmsNamedColorCount","cmsGetProfileContextID","cmsLab2XYZ","cmsCIE94DeltaE","cmsGetPostScriptCRD","cmsGetProfileContextID","cmsPluginTHR","cmsGetProfileContextID","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // 1. Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file length using cmsfilelength
    long file_len = cmsfilelength(in_file);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    // 3. Create profile from memory data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Get manufacturer from profile header
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    
    // 5. Create ink limiting device link
    cmsHPROFILE hDeviceLink = cmsCreateInkLimitingDeviceLinkTHR(
        cmsGetProfileContextID(hProfile), 
        cmsSigRgbData,  // RGB color space
        1.0  // Limit
    );
    
    // 6. Create a named color list (dummy for demonstration)
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(
        cmsGetProfileContextID(hProfile),
        10,  // 10 colors
        3,   // 3 colorants
        "Prefix",
        "Suffix"
    );
    
    // 7. Duplicate the named color list
    cmsNAMEDCOLORLIST* dupNamedColorList = nullptr;
    if (namedColorList) {
        // Note: cmsDupNamedColorList is not in provided APIs, using cmsAllocNamedColorList
        // as fallback since we need to demonstrate duplication concept
        dupNamedColorList = cmsAllocNamedColorList(
            cmsGetProfileContextID(hProfile),
            cmsNamedColorCount(namedColorList),
            3,
            "DupPrefix",
            "DupSuffix"
        );
    }
    
    // 8. Prepare Lab and XYZ data for conversion
    cmsCIEXYZ whitePoint = {0.9505, 1.0000, 1.0890}; // D65 white point
    cmsCIELab lab = {50.0, 20.0, 30.0}; // Sample Lab values
    cmsCIEXYZ xyz;
    
    // 9. Convert Lab to XYZ
    cmsLab2XYZ(&whitePoint, &xyz, &lab);
    
    // 10. Create another Lab value and compute DeltaE
    cmsCIELab lab2 = {55.0, 25.0, 35.0};
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab, &lab2);
    
    // 11. Get PostScript CRD data
    cmsUInt32Number bufferSize = 4096;
    void* buffer = malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number crdSize = cmsGetPostScriptCRD(
            cmsGetProfileContextID(hProfile),
            hProfile,
            INTENT_PERCEPTUAL,
            0,
            buffer,
            bufferSize
        );
        free(buffer);
    }
    
    // 12. Plugin registration (using NULL plugin for demonstration)
    cmsBool pluginResult = cmsPluginTHR(cmsGetProfileContextID(hProfile), NULL);
    
    // 13. Write output to file (demonstrating file operations)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        fwrite(&manufacturer, sizeof(manufacturer), 1, out_file);
        fwrite(&deltaE, sizeof(deltaE), 1, out_file);
        fwrite(&file_len, sizeof(file_len), 1, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 14. Cleanup all allocated resources
    if (dupNamedColorList) {
        cmsFreeNamedColorList(dupNamedColorList);
    }
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    if (hDeviceLink) {
        cmsCloseProfile(hDeviceLink);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}