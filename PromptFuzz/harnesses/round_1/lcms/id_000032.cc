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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"cmsAppendNamedColor":[[832,66,832,77,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsAppendNamedColor","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsCloseProfile","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsAppendNamedColor","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create profile from input data for cmsFormatterForColorspaceOfProfile
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile) {
        // Use cmsFormatterForColorspaceOfProfile
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(
            hProfile, 4, FALSE);
        
        // 2. Create MLU object for cmsMLUgetTranslation
        cmsMLU* mlu = cmsMLUalloc(ctx, 1);
        if (mlu) {
            // Add some translations
            cmsMLUsetASCII(mlu, "en", "US", "Test Description");
            cmsMLUsetASCII(mlu, "fr", "FR", "Description de test");
            
            // Use cmsMLUgetTranslation
            char obtainedLang[3] = {0}, obtainedCountry[3] = {0};
            cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
            
            cmsMLUfree(mlu);
        }
        
        cmsCloseProfile(hProfile);
    }
    
    // 3. Create tone curve using cmsBuildTabulatedToneCurve16
    // Need at least 512 entries * 2 bytes = 1024 bytes
    if (size >= 1024) {
        cmsUInt32Number nEntries = 256; // Use 256 entries
        cmsUInt16Number* curveValues = (cmsUInt16Number*)data;
        
        // Create tone curve
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, nEntries, curveValues);
        if (toneCurve) {
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // 4. Create IT8 handle for cmsIT8SetPropertyHex and cmsIT8GetDataDbl
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set up IT8 structure
        cmsIT8SetSheetType(hIT8, "CGATS.17");
        
        // Use cmsIT8SetPropertyHex
        cmsUInt32Number hexValue = 0x00FF00; // Green color value
        if (size > 0) {
            hexValue = data[0] << 16 | data[1] << 8 | data[2];
        }
        cmsIT8SetPropertyHex(hIT8, "COLOR_VALUE", hexValue);
        
        // Add some data for cmsIT8GetDataDbl
        cmsIT8SetDataFormat(hIT8, 0, "RGB_R");
        cmsIT8SetDataFormat(hIT8, 1, "RGB_G");
        cmsIT8SetDataFormat(hIT8, 2, "RGB_B");
        
        // Add a patch
        cmsIT8SetData(hIT8, "1", "RGB_R", "0.5");
        cmsIT8SetData(hIT8, "1", "RGB_G", "0.5");
        cmsIT8SetData(hIT8, "1", "RGB_B", "0.5");
        
        // Use cmsIT8GetDataDbl
        double rgbValue = cmsIT8GetDataDbl(hIT8, "1", "RGB_R");
        
        cmsIT8Free(hIT8);
    }
    
    // 5. Create named color list for cmsNamedColorIndex
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
    if (namedColors) {
        // Add some colors
        cmsUInt16Number colorValues[3] = {0, 0, 0};
        
        // Use input data for color values if available
        if (size >= 20) {
            for (int i = 0; i < 3; i++) {
                colorValues[i] = (data[i * 2] << 8) | data[i * 2 + 1];
            }
        }
        
        // Add named colors
        cmsAppendNamedColor(namedColors, "Red", NULL, colorValues);
        cmsAppendNamedColor(namedColors, "Green", NULL, colorValues);
        cmsAppendNamedColor(namedColors, "Blue", NULL, colorValues);
        
        // Use cmsNamedColorIndex
        int index = cmsNamedColorIndex(namedColors, "Green");
        
        cmsFreeNamedColorList(namedColors);
    }
    
    // 6. Try to use the input as a profile again for additional testing
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
        if (hProfile2) {
            // Get some basic info
            cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile2);
            cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile2);
            
            cmsCloseProfile(hProfile2);
        }
        assert_file_closed(&in_file);;
    }
    
    // 7. Create output file if needed for any operations
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Could write some data here if needed
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}