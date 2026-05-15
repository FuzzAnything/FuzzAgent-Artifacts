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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":4,"unique_branches":{"cmsCIE94DeltaE":[[466,9,466,17,0,0,4,0]],"cmsDupNamedColorList":[[797,12,797,45,0,0,4,0],[798,13,798,39,0,0,4,1]]},"library_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLinkTHR","cmsGetPostScriptCRD","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsLab2XYZ","cmsCIE94DeltaE","cmsPluginTHR","cmsCloseProfile","cmsCloseProfile"],"critical_calls":["cmsfilelength","cmsOpenProfileFromMem","cmsGetHeaderManufacturer","cmsCreateInkLimitingDeviceLinkTHR","cmsGetPostScriptCRD","cmsAllocNamedColorList","cmsDupNamedColorList","cmsFreeNamedColorList","cmsFreeNamedColorList","cmsLab2XYZ","cmsCIE94DeltaE","cmsPluginTHR","cmsCloseProfile","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file length using cmsfilelength
    long file_len = cmsfilelength(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    
    // Create ink limiting device link profile
    cmsHPROFILE hDeviceLink = cmsCreateInkLimitingDeviceLinkTHR(
        NULL, cmsSigRgbData, 400.0);
    
    // Get PostScript CRD
    cmsUInt32Number buffer_size = 4096;
    void *buffer = malloc(buffer_size);
    if (buffer) {
        cmsUInt32Number crd_size = cmsGetPostScriptCRD(
            NULL, hProfile, INTENT_PERCEPTUAL, 0, buffer, buffer_size);
    }
    free(buffer);
    
    // Create named color list and duplicate it
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(NULL, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsNAMEDCOLORLIST *dupList = cmsDupNamedColorList(namedColorList);
        if (dupList) {
            cmsFreeNamedColorList(dupList);
        }
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Lab to XYZ conversion
    cmsCIEXYZ WhitePoint;
    WhitePoint.X = 0.9505;
    WhitePoint.Y = 1.0000;
    WhitePoint.Z = 1.0890;
    
    cmsCIELab Lab;
    Lab.L = 50.0;
    Lab.a = 0.0;
    Lab.b = 0.0;
    
    cmsCIEXYZ XYZ;
    cmsLab2XYZ(&WhitePoint, &XYZ, &Lab);
    
    // CIE94 Delta E calculation
    cmsCIELab Lab1, Lab2;
    Lab1.L = 50.0; Lab1.a = 0.0; Lab1.b = 0.0;
    Lab2.L = 52.0; Lab2.a = 2.0; Lab2.b = -1.0;
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&Lab1, &Lab2);
    
    // Plugin registration (using NULL plugin for demonstration)
    cmsPluginTHR(NULL, NULL);
    
    // Cleanup
    if (hDeviceLink) cmsCloseProfile(hDeviceLink);
    if (hProfile) cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}