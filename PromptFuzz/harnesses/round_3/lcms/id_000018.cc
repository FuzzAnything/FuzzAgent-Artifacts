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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 52.5, nr_unique_branch: 43
//<Quality> {"density":21,"unique_branches":{"encodeUTF8":[[277,12,277,30,0,0,4,0],[277,12,277,30,0,0,4,1],[277,34,277,37,0,0,4,0],[277,34,277,37,0,0,4,1],[279,13,279,26,0,0,4,0],[279,13,279,26,0,0,4,1],[279,30,279,43,0,0,4,1],[283,17,283,30,0,0,4,0],[283,17,283,30,0,0,4,1],[283,34,283,47,0,0,4,1],[288,17,288,34,0,0,4,0],[288,17,288,34,0,0,4,1],[290,21,290,24,0,0,4,0],[290,21,290,24,0,0,4,1],[290,28,290,50,0,0,4,0],[290,28,290,50,0,0,4,1],[294,22,294,40,0,0,4,0],[294,22,294,40,0,0,4,1],[296,21,296,24,0,0,4,0],[296,21,296,24,0,0,4,1],[296,28,296,43,0,0,4,0],[296,47,296,69,0,0,4,0],[303,22,303,41,0,0,4,0],[303,22,303,41,0,0,4,1],[305,21,305,24,0,0,4,0],[305,21,305,24,0,0,4,1],[305,28,305,43,0,0,4,0],[305,47,305,69,0,0,4,0],[315,21,315,24,0,0,4,0],[315,21,315,24,0,0,4,1],[315,28,315,43,0,0,4,0],[315,47,315,69,0,0,4,0],[315,47,315,69,0,0,4,1]],"cmsGetToneCurveParametricType":[[1413,9,1413,28,0,0,4,1]],"cmsGetProfileInfoUTF8":[[1042,9,1042,20,0,0,4,0],[1042,9,1042,20,0,0,4,1]],"cmsMLUgetUTF8":[[602,9,602,20,0,0,4,1],[606,9,606,21,0,0,4,0],[606,9,606,21,0,0,4,1],[611,9,611,23,0,0,4,1],[614,9,614,24,0,0,4,1],[617,9,617,33,0,0,4,0],[617,9,617,33,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenIOhandlerFromStream","cmsPipelineAlloc","cmsMLUalloc","cmsMLUsetWide","cmsDictAlloc","cmsBuildGamma","cmsEstimateGamma","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsOpenProfileFromMem","cmsGetHeaderRenderingIntent","cmsGetProfileInfoUTF8","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPatchByName","cmsIT8GetData","cmsIT8Free","cmsCloseProfile","cmsDictFree","cmsMLUfree","cmsPipelineFree","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromStream","cmsPipelineAlloc","cmsMLUalloc","cmsMLUsetWide","cmsDictAlloc","cmsBuildGamma","cmsEstimateGamma","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsOpenProfileFromMem","cmsGetHeaderRenderingIntent","cmsGetProfileInfoUTF8","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPatchByName","cmsIT8GetData","cmsIT8Free","cmsCloseProfile","cmsDictFree","cmsMLUfree","cmsPipelineFree","cmsCloseIOhandler","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for basic operations
    if (size < 128) {
	return 0;
	}
	
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open I/O handler from stream
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromStream(context, in_file);
    
    // Allocate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // Allocate MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    
    // Set wide string in MLU
    const wchar_t *wide_str = L"FuzzTest";
    cmsMLUsetWide(mlu, "en", "US", wide_str);
    
    // Allocate dictionary
    cmsHANDLE dict = cmsDictAlloc(context);
    
    // Create gamma tone curve
    cmsToneCurve *gamma_curve = cmsBuildGamma(context, 2.2);
    
    // Estimate gamma
    cmsFloat64Number estimated_gamma = cmsEstimateGamma(gamma_curve, 0.001);
    
    // Get parametric type
    cmsInt32Number param_type = cmsGetToneCurveParametricType(gamma_curve);
    
    // Free tone curve
    cmsFreeToneCurve(gamma_curve);
    
    // Now try to open profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    
    if (profile) {
        // Get rendering intent
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile);
        
        // Get profile info UTF8
        char buffer[256];
        cmsUInt32Number info_size = cmsGetProfileInfoUTF8(profile, cmsInfoDescription, 
                                                         "en", "US", buffer, sizeof(buffer));
        
        // Allocate IT8
        cmsHANDLE it8 = cmsIT8Alloc(context);
        
        if (it8) {
            // Set IT8 property
            cmsIT8SetPropertyStr(it8, "TestProperty", "TestValue");
            
            // Get patch by name
            int patch_index = cmsIT8GetPatchByName(it8, "Patch1");
            
            // Get IT8 data
            const char *patch_data = cmsIT8GetData(it8, "Patch1", "Sample1");
            
            // Free IT8
            cmsIT8Free(it8);
        }
        
        // Close profile
        cmsCloseProfile(profile);
    }
    
    // Free dictionary
    if (dict) cmsDictFree(dict);
    
    // Free MLU
    cmsMLUfree(mlu);
    
    // Free pipeline
    if (pipeline) cmsPipelineFree(pipeline);
    
    // Close I/O handler
    if (io_handler) cmsCloseIOhandler(io_handler);
    
    // Close FILE
    assert_file_closed(&in_file);;
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}