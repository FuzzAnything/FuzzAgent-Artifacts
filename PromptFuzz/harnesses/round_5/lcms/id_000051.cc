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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 8
//<Quality> {"density":28,"unique_branches":{"OptimizeByResampling":[[680,50,680,79,0,0,4,0],[680,83,680,111,0,0,4,0]],"OptimizeByComputingLinearization":[[1065,9,1065,46,0,0,4,0]],"cmsTempFromWhitePoint":[[163,15,163,23,0,0,4,0],[172,13,172,21,0,0,4,0],[172,13,172,21,0,0,4,1],[172,25,172,38,0,0,4,0],[172,25,172,38,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsCreateNULLProfileTHR","cmsDetectDestinationBlackPoint","cmsXYZ2xyY","cmsD50_XYZ","cmsTempFromWhitePoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsGetTagSignature","cmsMLUdup","cmsBuildTabulatedToneCurve16","cmsDupToneCurve","cmsDictNextEntry","cmsUnregisterPlugins","cmsMLUgetWide","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsGetTransformContextID","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsCreateNULLProfileTHR","cmsDetectDestinationBlackPoint","cmsXYZ2xyY","cmsD50_XYZ","cmsTempFromWhitePoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsGetTagSignature","cmsMLUdup","cmsBuildTabulatedToneCurve16","cmsDupToneCurve","cmsDictNextEntry","cmsUnregisterPlugins","cmsMLUgetWide","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsGetTransformContextID","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"],"visited":1}
/**/


// Include lcms2 headers

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Create temporary file for profile data
    FILE *temp_file = fmemopen((void *)data, size, "rb");
    if (!temp_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&temp_file);
	return 0;
    }

    // Create a second temporary file for stream operations
    FILE *temp_file2 = fmemopen((void *)data, size, "rb");
    if (!temp_file2) {
        assert_file_closed(&temp_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&temp_file2);
	assert_file_closed(&temp_file);
	return 0;
    }

    // Variables to track resources
    cmsHPROFILE hProfileFile = NULL;
    cmsHPROFILE hProfileStream = NULL;
    cmsHPROFILE hNullProfile = NULL;
    cmsHPROFILE hLabProfile = NULL;
    cmsHPROFILE hXYZProfile = NULL;
    cmsPipeline *pipeline = NULL;
    cmsStage *matrixStage = NULL;
    cmsMLU *mlu = NULL;
    cmsMLU *mlu2 = NULL;
    cmsToneCurve *toneCurve = NULL;
    cmsToneCurve *dupToneCurve = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsHANDLE it8 = NULL;
    
    // 1. Open profile from file
    hProfileFile = cmsOpenProfileFromFileTHR(ctx, "input_file", "r");
    
    // 2. Open profile from stream
    hProfileStream = cmsOpenProfileFromStreamTHR(ctx, temp_file, "r");
    
    // 3. Create NULL profile
    hNullProfile = cmsCreateNULLProfileTHR(ctx);
    
    // 4. Detect destination black point (use the first profile if available)
    cmsCIEXYZ blackPoint;
    if (hProfileFile) {
        cmsDetectDestinationBlackPoint(&blackPoint, hProfileFile, 0, 0);
    }
    
    // 5. Convert white point to temperature (use D50)
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 0.0;
    cmsXYZ2xyY(&whitePoint, cmsD50_XYZ());
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // 6. Allocate pipeline
    pipeline = cmsPipelineAlloc(ctx, 3, 3);
    
    // 7. Allocate matrix stage (identity matrix)
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    cmsFloat64Number offset[3] = {0.0, 0.0, 0.0};
    matrixStage = cmsStageAllocMatrix(ctx, 3, 3, matrix, offset);
    
    // 8. Free matrix stage (we'll allocate a new one for the pipeline if needed)
    cmsStageFree(matrixStage);
    
    // 9. Set header rendering intent and encoded ICC version
    if (hNullProfile) {
        cmsSetHeaderRenderingIntent(hNullProfile, 0); // Perceptual
        cmsSetEncodedICCversion(hNullProfile, 0x02100000); // Version 2.1
    }
    
    // 10. Allocate MLU
    mlu = cmsMLUalloc(ctx, 10);
    
    // 11. Get tag signature
    if (hProfileFile) {
        cmsTagSignature sig = cmsGetTagSignature(hProfileFile, 0);
        (void)sig; // Use variable to avoid unused warning
    }
    
    // 12. Duplicate MLU
    if (mlu) {
        mlu2 = cmsMLUdup(mlu);
    }
    
    // 13. Create and duplicate tone curve
    if (size >= 8) {
        // Use first 8 bytes to create a simple tone curve
        cmsUInt16Number values[2] = {0, 65535};
        if (size >= 4) {
            values[0] = (data[0] << 8) | data[1];
            values[1] = (data[2] << 8) | data[3];
        }
        toneCurve = cmsBuildTabulatedToneCurve16(ctx, 2, values);
        if (toneCurve) {
            dupToneCurve = cmsDupToneCurve(toneCurve);
        }
    }
    
    // 14. Dictionary iteration (simulated - no actual dictionary available)
    const cmsDICTentry *dictEntry = NULL;
    dictEntry = cmsDictNextEntry(dictEntry); // Returns NULL
    
    // 15. Unregister plugins
    cmsUnregisterPlugins();
    
    // 16. Get wide string from MLU
    if (mlu) {
        wchar_t buffer[256];
        cmsMLUgetWide(mlu, "en", "US", buffer, 256);
    }
    
    // 17. Create Lab4 profile
    hLabProfile = cmsCreateLab4Profile(&whitePoint);
    
    // 18. Create XYZ profile
    hXYZProfile = cmsCreateXYZProfileTHR(ctx);
    
    // 19. Create transform
    if (hLabProfile && hXYZProfile) {
        transform = cmsCreateTransformTHR(ctx, hLabProfile, 
                                         TYPE_Lab_16, 
                                         hXYZProfile, 
                                         TYPE_XYZ_16, 
                                         0,  // Intent
                                         0); // Flags
    }
    
    // 20. IT8 operations (simulated - we don't have an IT8 handle)
    // cmsIT8SetPropertyMulti would need an IT8 handle
    
    // 21. Get transform context ID
    if (transform) {
        cmsContext transformCtx = cmsGetTransformContextID(transform);
        (void)transformCtx; // Use variable to avoid unused warning
    }
    
    // Cleanup section (reverse order of creation)
    
    // 22. Delete transform
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    // 23. Close profiles
    if (hXYZProfile) {
        cmsCloseProfile(hXYZProfile);
    }
    if (hLabProfile) {
        cmsCloseProfile(hLabProfile);
    }
    if (hNullProfile) {
        cmsCloseProfile(hNullProfile);
    }
    if (hProfileStream) {
        cmsCloseProfile(hProfileStream);
    }
    if (hProfileFile) {
        cmsCloseProfile(hProfileFile);
    }
    
    // 24. Free MLU
    if (mlu2) {
        cmsMLUfree(mlu2);
    }
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    // 25. Free pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // 26. Free tone curves
    if (dupToneCurve) {
        cmsFreeToneCurve(dupToneCurve);
    }
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    // Close file handles
    assert_file_closed(&temp_file);;
    assert_file_closed(&temp_file2);;
    
    // 27. Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&temp_file2);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
}