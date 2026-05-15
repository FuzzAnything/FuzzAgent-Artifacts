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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 235, nr_unique_branch: 4
//<Quality> {"density":47,"unique_branches":{"ComputeCorrelates":[[201,9,201,22,0,0,4,0]],"cmsReverseToneCurveEx":[[1118,58,1118,67,0,0,4,1]],"InSymbol":[[860,17,860,34,0,0,4,1]],"BinSrchKey":[[628,13,628,21,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsJoinToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsCreateRGBProfile","cmsD50_xyY","cmsCreateNULLProfile","cmsCreateNULLProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsFormatterForColorspaceOfProfile","cmsFormatterForColorspaceOfProfile","cmsCreateProofingTransformTHR","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8Free","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsFreeToneCurveTriple","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsJoinToneCurve","cmsCIECAM02Init","cmsCIECAM02Forward","cmsCIECAM02Done","cmsCreateRGBProfile","cmsD50_xyY","cmsCreateNULLProfile","cmsCreateNULLProfile","cmsFormatterForColorspaceOfProfile","cmsFormatterForColorspaceOfProfile","cmsCreateProofingTransformTHR","cmsIT8LoadFromMem","cmsIT8GetSheetType","cmsIT8Free","cmsTransform2DeviceLink","cmsCloseProfile","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsFreeToneCurveTriple","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Create I/O handler from NULL (simulating output stream)
    cmsIOHANDLER *nullIO = cmsOpenIOhandlerFromNULL(ctx);
    if (!nullIO) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Create tone curves from input data
    cmsUInt16Number curve1_data[256], curve2_data[256];
    size_t curve_data_size = sizeof(curve1_data);
    if (size >= curve_data_size * 2) {
        memcpy(curve1_data, data, curve_data_size);
        memcpy(curve2_data, data + curve_data_size, curve_data_size);
    } else {
        // Use default linear curves if insufficient data
        for (int i = 0; i < 256; i++) {
            curve1_data[i] = curve2_data[i] = (i << 8) | i;
        }
    }

    cmsToneCurve *curve1 = cmsBuildTabulatedToneCurve16(ctx, 256, curve1_data);
    cmsToneCurve *curve2 = cmsBuildTabulatedToneCurve16(ctx, 256, curve2_data);
    if (!curve1 || !curve2) {
        if (curve1) cmsFreeToneCurve(curve1);
        if (curve2) cmsFreeToneCurve(curve2);
        cmsCloseIOhandler(nullIO);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 3. Join tone curves
    cmsToneCurve *joinedCurve = cmsJoinToneCurve(ctx, curve1, curve2, 256);
    if (!joinedCurve) {
        cmsFreeToneCurve(curve1);
        cmsFreeToneCurve(curve2);
        cmsCloseIOhandler(nullIO);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 4. Create CIECAM02 model and perform forward transformation
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = vc.whitePoint.Y = vc.whitePoint.Z = 1.0;
    vc.La = 100;
    vc.Yb = 20;
    vc.surround = 1;
    vc.D_value = 1.0;
    
    cmsHANDLE cam02 = cmsCIECAM02Init(ctx, &vc);
    if (cam02) {
        cmsCIEXYZ xyz_input = {0.5, 0.5, 0.5}; // Sample XYZ input
        cmsJCh jch_output;
        cmsCIECAM02Forward(cam02, &xyz_input, &jch_output);
        cmsCIECAM02Done(cam02);
    }

    // 5. Create profiles for proofing transform
    cmsToneCurve *curves[3] = {joinedCurve, joinedCurve, joinedCurve};
    cmsHPROFILE inputProfile = cmsCreateRGBProfile(cmsD50_xyY(), NULL, curves);
    cmsHPROFILE outputProfile = cmsCreateNULLProfile();
    cmsHPROFILE proofingProfile = cmsCreateNULLProfile();
    
    if (!inputProfile || !outputProfile || !proofingProfile) {
        if (inputProfile) cmsCloseProfile(inputProfile);
        if (outputProfile) cmsCloseProfile(outputProfile);
        if (proofingProfile) cmsCloseProfile(proofingProfile);
        cmsFreeToneCurve(joinedCurve);
        cmsFreeToneCurve(curve1);
        cmsFreeToneCurve(curve2);
        cmsCloseIOhandler(nullIO);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 6. Get formatter for colorspace
    cmsUInt32Number inputFormat = cmsFormatterForColorspaceOfProfile(inputProfile, 8, FALSE);
    cmsUInt32Number outputFormat = cmsFormatterForColorspaceOfProfile(outputProfile, 8, FALSE);

    // 7. Create proofing transform
    cmsHTRANSFORM proofingTransform = cmsCreateProofingTransformTHR(
        ctx,
        inputProfile,
        inputFormat,
        outputProfile,
        outputFormat,
        proofingProfile,
        INTENT_PERCEPTUAL,
        INTENT_RELATIVE_COLORIMETRIC,
        cmsFLAGS_NOCACHE
    );

    // 8. Create IT8 handle from remaining data (simulating color measurement data)
    cmsHANDLE it8 = NULL;
    if (size > curve_data_size * 2) {
        size_t it8_offset = curve_data_size * 2;
        size_t it8_size = size - it8_offset;
        if (it8_size > 0) {
            it8 = cmsIT8LoadFromMem(ctx, data + it8_offset, (cmsUInt32Number)it8_size);
        }
    }
    
    // Get sheet type if IT8 handle exists
    if (it8) {
        const char *sheetType = cmsIT8GetSheetType(it8);
        (void)sheetType; // Use variable to avoid unused warning
        cmsIT8Free(it8);
    }

    // 9. Convert transform to device link
    if (proofingTransform) {
        cmsHPROFILE deviceLink = cmsTransform2DeviceLink(proofingTransform, 4.0, 0);
        if (deviceLink) {
            cmsCloseProfile(deviceLink);
        }
        cmsDeleteTransform(proofingTransform);
    }

    // 10. Create and free tone curve triple
    cmsToneCurve *triple[3] = {curve1, curve2, joinedCurve};
    // Note: cmsFreeToneCurveTriple expects an array of exactly 3 curves
    // We'll create a temporary array to demonstrate
    cmsToneCurve *tempCurves[3];
    for (int i = 0; i < 3; i++) {
        cmsUInt16Number linear[2] = {0, 0xFFFF};
        tempCurves[i] = cmsBuildTabulatedToneCurve16(ctx, 2, linear);
    }
    if (tempCurves[0] && tempCurves[1] && tempCurves[2]) {
        cmsFreeToneCurveTriple(tempCurves);
    }

    // 11. Cleanup
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsCloseProfile(proofingProfile);
    cmsFreeToneCurve(curve1);
    cmsFreeToneCurve(curve2);
    cmsFreeToneCurve(joinedCurve);
    cmsCloseIOhandler(nullIO);
    cmsDeleteContext(ctx);

    return 0;
}