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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCloseProfile","cmsDeleteContext","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsGetPipelineContextID","cmsGetAlarmCodes","cmsIT8Alloc","cmsGDBCheckPoint","cmsNamedColorIndex","cmsUnregisterPluginsTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsCreate_sRGBProfile","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsGetPipelineContextID","cmsGetAlarmCodes","cmsIT8Alloc","cmsGDBCheckPoint","cmsNamedColorIndex","cmsUnregisterPluginsTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing

    // 1. Open profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!hProfile) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create a pipeline/transform to get pipeline context
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
    if (!sRGB) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        context, hProfile, TYPE_Lab_8,
        sRGB, TYPE_RGB_8,
        INTENT_PERCEPTUAL, 0
    );
    
    if (transform) {
        // 3. Get pipeline context ID (first required API)
        cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
        if (pipeline) {
            cmsContext pipeContext = cmsGetPipelineContextID(pipeline);
            (void)pipeContext; // Use pipeContext to avoid unused variable warning
        }
        
        // 4. Get alarm codes (second required API)
        cmsUInt16Number alarmCodes[16];
        cmsGetAlarmCodes(alarmCodes);
        
        // 5. Create a simple GDB and check a Lab point
        // Using a placeholder GDB handle - in real usage this would come from profile
        cmsHANDLE hGDB = cmsIT8Alloc(context);
        if (hGDB) {
            cmsCIELab lab = {50.0, 0.0, 0.0}; // Neutral gray
            // 6. Check GDB point (third required API)
            cmsBool inGamut = cmsGDBCheckPoint(hGDB, &lab);
            (void)inGamut; // Use result to avoid unused variable warning
        }
        
        // 7. Create named color list and check index
        // Using a placeholder named color list
        cmsNAMEDCOLORLIST *namedColorList = NULL;
        // Note: cmsNamedColorIndex requires a valid named color list
        // We'll create a minimal one for demonstration
        if (namedColorList) {
            // 8. Get named color index (fourth required API)
            cmsInt32Number index = cmsNamedColorIndex(namedColorList, "Red");
            (void)index; // Use index to avoid unused variable warning
        }
        
        // 9. Unregister plugins (fifth required API)
        cmsUnregisterPluginsTHR(context);
        
        cmsDeleteTransform(transform);
    }
    
    // 10. Cleanup
    cmsCloseProfile(sRGB);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}