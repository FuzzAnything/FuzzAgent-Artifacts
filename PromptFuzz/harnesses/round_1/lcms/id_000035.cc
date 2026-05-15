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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsDetectBlackPoint","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsDetectRGBProfileGamma","cmsCloseProfile","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsDetectBlackPoint","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Open profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile) {
        // Get formatter for colorspace
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profile, 4, false);
        
        // Allocate and use MLU
        cmsMLU *mlu = cmsMLUalloc(context, 1);
        if (mlu) {
            cmsMLUsetASCII(mlu, "en", "US", "Test String");
            char lang[3], country[3];
            cmsMLUgetTranslation(mlu, "en", "US", lang, country);
            cmsMLUfree(mlu);
        }
        
        // Get header creator
        cmsUInt32Number creator = cmsGetHeaderCreator(profile);
        
        // Build and free tone curve
        cmsUInt16Number curveValues[2] = {0, 65535};
        cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurve16(context, 2, curveValues);
        if (toneCurve) {
            cmsFreeToneCurve(toneCurve);
        }
        
        // Detect RGB profile gamma
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(profile, 0.01);
        
        // Close profile
        cmsCloseProfile(profile);
    }
    
    // IT8 operations
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        cmsIT8SetPropertyHex(it8, "PROP_HEX", 0xFF00FF);
        cmsIT8SetDataFormat(it8, 0, "RGB_R");
        cmsIT8SetData(it8, "Patch1", "RGB_R", "255");
        cmsFloat64Number val = cmsIT8GetDataDbl(it8, "Patch1", "RGB_R");
        cmsIT8Free(it8);
    }
    
    // Named color list operations
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsUInt16Number pcs[3] = {32768, 32768, 32768};
        cmsUInt16Number colorant[16] = {0};
        cmsAppendNamedColor(namedColorList, "TestColor", pcs, colorant);
        cmsInt32Number index = cmsNamedColorIndex(namedColorList, "TestColor");
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Open profile from stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        cmsHPROFILE streamProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
        if (streamProfile) {
            // Get color space and device class
            cmsColorSpaceSignature colorspace = cmsGetColorSpace(streamProfile);
            cmsProfileClassSignature deviceClass = cmsGetDeviceClass(streamProfile);
            
            // Detect black point
            cmsCIEXYZ blackPoint;
            cmsDetectBlackPoint(&blackPoint, streamProfile, 0, 0);
            
            cmsCloseProfile(streamProfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // Cleanup
    cmsDeleteContext(context);
    assert_file_closed(&in_file);
	return 0;
}