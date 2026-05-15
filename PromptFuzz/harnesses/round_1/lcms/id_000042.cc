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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 4
//<Quality> {"density":19,"unique_branches":{"cmsWriteTag":[[1900,9,1900,21,0,0,4,0],[1904,14,1904,20,0,0,4,1]],"cmsCreateRGBProfileTHR":[[183,13,183,79,0,0,4,0],[212,9,212,13,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetDeviceClass","cmsCreateMultiprofileTransformTHR","cmsBuildTabulatedToneCurve16","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8SetData","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsCreateRGBProfileTHR","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetDeviceClass","cmsCreateMultiprofileTransformTHR","cmsBuildTabulatedToneCurve16","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // 2. Open profile from memory (using input data)
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(context, data, size);
    
    // 3. Get formatter for color space of profile
    if (hProfile) {
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(
            hProfile, 4, FALSE);
    }

    // 4. Allocate MLU
    cmsMLU* mlu = cmsMLUalloc(context, 10);
    
    // 5. Get MLU translation (use dummy codes)
    if (mlu) {
        char obtainedLang[3], obtainedCountry[3];
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
    }

    // 6. Free MLU
    if (mlu) {
        cmsMLUfree(mlu);
    }

    // 7. Get header creator
    if (hProfile) {
        cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    }

    // 8. Build tabulated tone curve (use some sample data)
    if (context) {
        cmsUInt16Number toneCurveValues[] = {0, 1000, 2000, 3000, 4000, 5000, 
                                           6000, 7000, 8000, 9000, 10000};
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(
            context, sizeof(toneCurveValues)/sizeof(toneCurveValues[0]), 
            toneCurveValues);
        
        // 9. Free tone curve
        if (toneCurve) {
            cmsFreeToneCurve(toneCurve);
        }
    }

    // 10. Get color space of profile
    if (hProfile) {
        cmsColorSpaceSignature colorspace = cmsGetColorSpace(hProfile);
    }

    // 11. Detect RGB profile gamma (if profile exists)
    if (hProfile) {
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, 0.001);
    }

    // 12. Allocate IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    
    // 13. Set IT8 sheet type
    if (it8Handle) {
        cmsIT8SetSheetType(it8Handle, "CGATS.17");
    }
    
    // 14. Set IT8 property as hex
    if (it8Handle) {
        cmsIT8SetPropertyHex(it8Handle, "NUMBER_OF_FIELDS", 3);
    }
    
    // 15. Set IT8 data format
    if (it8Handle) {
        const char* samples[] = {"R", "G", "B"};
        for (int i = 0; i < 3; i++) {
            cmsIT8SetDataFormat(it8Handle, i, samples[i]);
        }
    }
    
    // 16. Set IT8 data
    if (it8Handle) {
        cmsIT8SetData(it8Handle, "Patch1", "R", "0.1");
        cmsIT8SetData(it8Handle, "Patch1", "G", "0.2");
        cmsIT8SetData(it8Handle, "Patch1", "B", "0.3");
    }
    
    // 17. Get IT8 data as double
    if (it8Handle) {
        cmsFloat64Number value = cmsIT8GetDataDbl(it8Handle, "Patch1", "R");
    }
    
    // 18. Free IT8 handle
    if (it8Handle) {
        cmsIT8Free(it8Handle);
    }

    // 19. Create RGB profile (using default D65 white point and sRGB primaries)
    cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
    cmsCIExyY whitePoint;
    cmsCIExyYTRIPLE primaries;
    
    // Set white point to D65
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    
    // Set primaries to sRGB
    primaries.Red.x = 0.6400;   primaries.Red.y = 0.3300;   primaries.Red.Y = 1.0;
    primaries.Green.x = 0.3000; primaries.Green.y = 0.6000; primaries.Green.Y = 1.0;
    primaries.Blue.x = 0.1500;  primaries.Blue.y = 0.0600;  primaries.Blue.Y = 1.0;
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(context, &whitePoint, 
                                                    &primaries, transferFunction);
    
    // 20. Allocate named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, 10, 3, 
                                                                "Prefix", "Suffix");
    
    // 21. Append named color
    if (namedColorList) {
        cmsUInt16Number pcs[3] = {32768, 32768, 32768};
        cmsUInt16Number colorant[16] = {65535, 0, 0};
        cmsAppendNamedColor(namedColorList, "Red", pcs, colorant);
    }
    
    // 22. Get named color index
    if (namedColorList) {
        cmsInt32Number index = cmsNamedColorIndex(namedColorList, "Red");
    }
    
    // 23. Free named color list
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }

    // 24. Get device class
    if (hProfile) {
        cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile);
    }

    // 25. Create multiprofile transform (need at least 2 profiles)
    cmsHTRANSFORM transform = NULL;
    if (hProfile && rgbProfile) {
        cmsHPROFILE profiles[2] = {hProfile, rgbProfile};
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = cmsFLAGS_NOCACHE;
        
        transform = cmsCreateMultiprofileTransformTHR(context, profiles, 2, 
                                                      TYPE_RGB_8, TYPE_RGB_8, 
                                                      intent, flags);
    }

    // 26. Get tone curve parametric type (need a valid tone curve)
    cmsToneCurve* testCurve = cmsBuildTabulatedToneCurve16(context, 2, 
                                                          (cmsUInt16Number[]){0, 65535});
    if (testCurve) {
        cmsInt32Number paramType = cmsGetToneCurveParametricType(testCurve);
        cmsFreeToneCurve(testCurve);
    }

    // Cleanup in reverse order
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(context);

    return 0;
}