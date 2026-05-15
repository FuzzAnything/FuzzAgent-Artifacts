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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 2
//<Quality> {"density":32,"unique_branches":{"OptimizeByComputingLinearization":[[1102,21,1102,54,0,0,4,0],[1251,13,1251,21,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfileTHR","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsCreateRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetToneCurveParametricType","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetDeviceClass","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_sRGBProfileTHR","cmsFormatterForColorspaceOfProfile","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsGetHeaderCreator","cmsBuildTabulatedToneCurve16","cmsGetColorSpace","cmsDetectRGBProfileGamma","cmsIT8Alloc","cmsIT8SetSheetType","cmsIT8SetPropertyHex","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetDataFormat","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsCreateRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsGetToneCurveParametricType","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorIndex","cmsFreeNamedColorList","cmsGetDeviceClass","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfileTHR(context);
    }
    
    // 2. Get formatter for colorspace
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(
        hProfile, 4, FALSE);
    
    // 3. Allocate and use MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        char lang[3] = {'e', 'n', '\0'};
        char country[3] = {'U', 'S', '\0'};
        char obtainedLang[3], obtainedCountry[3];
        cmsMLUgetTranslation(mlu, lang, country, 
                           obtainedLang, obtainedCountry);
        cmsMLUfree(mlu);
    }
    
    // 4. Get header creator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    
    // 5. Build tone curve from input data
    const size_t curvePoints = 256;
    cmsUInt16Number curveData[curvePoints];
    for (size_t i = 0; i < curvePoints && i < size/2; i++) {
        curveData[i] = data[i*2] | (data[i*2+1] << 8);
    }
    cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurve16(
        context, curvePoints, curveData);
    
    // 6. Get colorspace
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(hProfile);
    
    // 7. Detect gamma if RGB profile
    if (colorspace == cmsSigRgbData) {
        cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, 0.1);
    }
    
    // 8. Create and use IT8
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        cmsIT8SetSheetType(it8, "CGATS.17");
        cmsIT8SetPropertyHex(it8, "NUMBER_OF_FIELDS", 3);
        cmsIT8SetDataFormat(it8, 0, "SAMPLE_ID");
        cmsIT8SetDataFormat(it8, 1, "RGB_R");
        cmsIT8SetDataFormat(it8, 2, "RGB_G");
        
        // Add some sample data
        for (int i = 0; i < 5 && size > i*10 + 50; i++) {
            char patch[20], val[20];
            snprintf(patch, sizeof(patch), "PATCH_%d", i);
            snprintf(val, sizeof(val), "%d", data[50 + i*10]);
            cmsIT8SetData(it8, patch, "RGB_R", val);
        }
        
        // Get data back
        cmsFloat64Number sampleValue = cmsIT8GetDataDbl(it8, "PATCH_0", "RGB_R");
        cmsIT8Free(it8);
    }
    
    // 9. Create multiprofile transform
    cmsHPROFILE profiles[2];
    profiles[0] = hProfile;
    
    // Create second profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    cmsToneCurve *transferFuncs[3] = {toneCurve, toneCurve, toneCurve};
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(context, &whitePoint, 
                                                   &primaries, transferFuncs);
    profiles[1] = rgbProfile;
    
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, 2, TYPE_RGB_8, TYPE_RGB_8, 
        INTENT_PERCEPTUAL, 0);
    
    // 10. Get tone curve parametric type
    if (toneCurve) {
        cmsInt32Number paramType = cmsGetToneCurveParametricType(toneCurve);
    }
    
    // 11. Create named color list
    cmsNAMEDCOLORLIST *namedColors = cmsAllocNamedColorList(
        context, 10, 3, "Prefix", "Suffix");
    if (namedColors) {
        cmsUInt16Number pcs[3] = {32768, 32768, 32768};
        cmsUInt16Number colorant[16] = {0};
        for (int i = 0; i < 3 && i < 16; i++) {
            colorant[i] = data[i] * 257;
        }
        
        cmsAppendNamedColor(namedColors, "TestColor", pcs, colorant);
        cmsInt32Number index = cmsNamedColorIndex(namedColors, "TestColor");
        cmsFreeNamedColorList(namedColors);
    }
    
    // 12. Get device class
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    
    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (rgbProfile) cmsCloseProfile(rgbProfile);
    if (toneCurve) cmsFreeToneCurve(toneCurve);
    if (hProfile) cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    
    return 0;
}