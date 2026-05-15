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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsMLUgetTranslation","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsIT8Alloc","cmsIT8SetPropertyHex","cmsIT8SetDataRowColDbl","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetColorSpace","cmsFormatterForColorspaceOfProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsMLUalloc","cmsMLUsetWide","cmsMLUsetWide","cmsMLUgetTranslation","cmsMLUfree","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsIT8Alloc","cmsIT8SetPropertyHex","cmsIT8SetDataRowColDbl","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetColorSpace","cmsFormatterForColorspaceOfProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create profile from input data
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (!profile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create MLU for testing cmsMLUgetTranslation
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        // Add some translations
        cmsMLUsetWide(mlu, "en", "US", L"English Text");
        cmsMLUsetWide(mlu, "fr", "FR", L"French Text");
        
        // Test cmsMLUgetTranslation
        char obtainedLang[3] = {0};
        char obtainedCountry[3] = {0};
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
        
        cmsMLUfree(mlu);
    }
    
    // Create tone curve using input data for values
    cmsUInt32Number curveEntries = 256;
    if (size >= curveEntries * sizeof(cmsUInt16Number)) {
        const cmsUInt16Number* curveData = reinterpret_cast<const cmsUInt16Number*>(data);
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(context, curveEntries, curveData);
        if (toneCurve) {
            // Use the tone curve
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // Create IT8 handle and test properties
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file");
        if (!it8) {
            // Create new IT8 if loading fails
            it8 = cmsIT8Alloc(context);
        }
        
        if (it8) {
            // Test cmsIT8SetPropertyHex
            cmsUInt32Number hexValue = static_cast<cmsUInt32Number>(size);
            cmsIT8SetPropertyHex(it8, "TestProp", hexValue);
            
            // Test cmsIT8GetDataDbl - try to get some data
            // First set some data to ensure we can retrieve it
            cmsIT8SetDataRowColDbl(it8, 0, 0, 1.0);
            cmsFloat64Number dblValue = cmsIT8GetDataDbl(it8, "0", "0");
            
            cmsIT8Free(it8);
        }
        assert_file_closed(&in_file);;
    }
    
    // Create named color list
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    if (namedColors) {
        // Add some colors (simplified - real code would populate properly)
        
        // Test cmsNamedColorIndex
        int colorIndex = cmsNamedColorIndex(namedColors, "TestColor");
        
        cmsFreeNamedColorList(namedColors);
    }
    
    // Test cmsFormatterForColorspaceOfProfile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(profile);
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profile, 4, false);
    
    // Clean up
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}