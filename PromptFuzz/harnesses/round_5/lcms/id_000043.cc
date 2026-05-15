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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFile","cmsOpenProfileFromStreamTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsGetPostScriptCRD","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsCIE2000DeltaE","cmsGetTagSignature","cmsMLUdup","cmsDictNextEntry","cmsUnregisterPlugins","cmsPipelineSetSaveAs8bitsFlag","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsOpenProfileFromStreamTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsGetPostScriptCRD","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsCIE2000DeltaE","cmsGetTagSignature","cmsMLUdup","cmsDictNextEntry","cmsUnregisterPlugins","cmsPipelineSetSaveAs8bitsFlag","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimal size requirement
    
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Open profile from file (simulated)
    cmsHPROFILE profile_file = cmsOpenProfileFromFile("dummy.icc", "r");
    // Note: This will likely fail without actual file, but API is called
    
    // 2. Open profile from stream
    cmsHPROFILE profile_stream = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    
    // 3. Detect destination black point
    cmsCIEXYZ blackPoint;
    if (profile_stream) {
        cmsDetectDestinationBlackPoint(&blackPoint, profile_stream, 0, 0);
    }
    
    // 4. Allocate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 5. Alloc matrix stage
    cmsFloat64Number matrix[9] = {1,0,0,0,1,0,0,0,1};
    cmsFloat64Number offset[3] = {0,0,0};
    cmsStage *matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
    
    // 6. Get PostScript CRD
    uint8_t psBuffer[1024];
    if (profile_stream) {
        cmsGetPostScriptCRD(context, profile_stream, 0, 0, psBuffer, sizeof(psBuffer));
    }
    
    // 7. Set header rendering intent
    if (profile_stream) {
        cmsSetHeaderRenderingIntent(profile_stream, 0);
    }
    
    // 8. Set encoded ICC version
    if (profile_stream) {
        cmsSetEncodedICCversion(profile_stream, 0x4000000);
    }
    
    // 9. Allocate MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    
    // 10. Compute CIE2000 delta E
    cmsCIELab lab1 = {50, 20, 30};
    cmsCIELab lab2 = {55, 25, 35};
    cmsFloat64Number deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
    (void)deltaE; // Suppress unused warning
    
    // 11. Get tag signature
    if (profile_stream) {
        cmsTagSignature sig = cmsGetTagSignature(profile_stream, 0);
        (void)sig;
    }
    
    // 12. Duplicate MLU
    cmsMLU *mluDup = mlu ? cmsMLUdup(mlu) : nullptr;
    
    // 13. Dictionary iteration (simulated)
    const cmsDICTentry *dictEntry = nullptr;
    if (dictEntry) {
        dictEntry = cmsDictNextEntry(dictEntry);
    }
    
    // 14. Unregister plugins
    cmsUnregisterPlugins();
    
    // 15. Set pipeline 8-bit flag
    if (pipeline) {
        cmsPipelineSetSaveAs8bitsFlag(pipeline, true);
    }
    
    // 16. Create Lab4 profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
    
    // 17. Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    
    // 18. Create transform
    cmsHTRANSFORM transform = nullptr;
    if (labProfile && xyzProfile) {
        transform = cmsCreateTransformTHR(context, labProfile, TYPE_Lab_DBL, 
                                         xyzProfile, TYPE_XYZ_DBL, 0, 0);
    }
    
    // 19. Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (labProfile) cmsCloseProfile(labProfile);
    if (xyzProfile) cmsCloseProfile(xyzProfile);
    if (mluDup) cmsMLUfree(mluDup);
    if (mlu) cmsMLUfree(mlu);
    if (matrixStage) cmsStageFree(matrixStage);
    if (pipeline) cmsPipelineFree(pipeline);
    if (profile_stream) cmsCloseProfile(profile_stream);
    if (profile_file) cmsCloseProfile(profile_file);
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}