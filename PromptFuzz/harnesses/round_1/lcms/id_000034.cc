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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 16
//<Quality> {"density":27,"unique_branches":{"isInkColorspace":[[196,5,196,24,0,0,4,0],[197,5,197,24,0,0,4,0],[198,5,198,24,0,0,4,0],[199,5,199,24,0,0,4,0],[200,5,200,24,0,0,4,0],[201,5,201,24,0,0,4,0],[202,5,202,24,0,0,4,0],[203,5,203,24,0,0,4,0],[204,5,204,24,0,0,4,0],[205,5,205,24,0,0,4,0],[206,5,206,24,0,0,4,0],[207,5,207,24,0,0,4,0],[208,5,208,24,0,0,4,0],[209,5,209,24,0,0,4,0],[210,5,210,24,0,0,4,0],[222,5,222,27,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsDetectBlackPoint","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsDetectBlackPoint","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Minimum size for meaningful processing

    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 1. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Get formatter for profile colorspace
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(hProfile, 4, FALSE);

    // 3. Create and manipulate MLU
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);
    if (mlu) {
        cmsMLUsetASCII(mlu, "en", "US", "Test Description");
        char obtainedLang[3] = {0}, obtainedCountry[3] = {0};
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
        cmsMLUfree(mlu);
    }

    // 4. Get profile creator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);

    // 5. Create tone curve
    cmsUInt16Number curveValues[10] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 65535};
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 10, curveValues);
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }

    // 6. Detect RGB profile gamma
    if (cmsGetColorSpace(hProfile) == cmsSigRgbData) {
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, 0.01);
    }

    // 7. Create IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        cmsIT8SetPropertyHex(it8, "NUMBER_OF_FIELDS", 3);
        cmsIT8SetDataFormat(it8, 0, "SAMPLE_ID");
        cmsIT8SetDataFormat(it8, 1, "RGB_R");
        cmsIT8SetDataFormat(it8, 2, "RGB_G");
        cmsIT8SetData(it8, "A1", "RGB_R", "0.1");
        cmsFloat64Number value = cmsIT8GetDataDbl(it8, "A1", "RGB_R");
        cmsIT8Free(it8);
    }

    // 8. Create named color list
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(ctx, 5, 3, "Prefix", "Suffix");
    if (namedColors) {
        cmsUInt16Number pcs[3] = {32768, 32768, 32768};
        cmsUInt16Number colorant[16] = {0};
        cmsAppendNamedColor(namedColors, "Red", pcs, colorant);
        cmsInt32Number idx = cmsNamedColorIndex(namedColors, "Red");
        cmsFreeNamedColorList(namedColors);
    }

    // 9. Open profile from stream (using FILE*)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsHPROFILE streamProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
        if (streamProfile) {
            // Get color space and device class
            cmsColorSpaceSignature colorspace = cmsGetColorSpace(streamProfile);
            cmsProfileClassSignature devclass = cmsGetDeviceClass(streamProfile);
            
            // Detect black point
            cmsCIEXYZ blackPoint;
            cmsDetectBlackPoint(&blackPoint, streamProfile, INTENT_RELATIVE_COLORIMETRIC, 0);
            
            cmsCloseProfile(streamProfile);
        }
        assert_file_closed(&in_file);;
    }

    // 10. Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}