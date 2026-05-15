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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineDup","cmsPipelineFree","cmsFreeToneCurve","cmsPipelineFree","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetProfileInfoASCII","cmsSetHeaderFlags","cmsSetAlarmCodes","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8Free","cmsCIE94DeltaE","cmsReadTag","cmsNamedColorCount","cmsNamedColorInfo","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineDup","cmsPipelineFree","cmsFreeToneCurve","cmsPipelineFree","cmsFreeNamedColorList","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Include necessary lcms headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get profile info ASCII
    char buffer[256];
    cmsUInt32Number bytes_written = cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, 
                                                          "EN", "US", buffer, sizeof(buffer));
    
    // 3. Set header flags
    cmsUInt32Number flags = *reinterpret_cast<const cmsUInt32Number*>(data) % 0xFFFF;
    cmsSetHeaderFlags(hProfile, flags);
    
    // 4. Set alarm codes
    cmsUInt16Number alarm_codes[16];
    for (int i = 0; i < 16 && i * 2 + 1 < size; i++) {
        alarm_codes[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    cmsSetAlarmCodes(alarm_codes);
    
    // 5. Create IT8 handle and enumerate properties
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Add some sample properties
        const char *prop_names[] = {"Prop1", "Prop2", nullptr};
        const char **subprops = nullptr;
        cmsUInt32Number count = cmsIT8EnumPropertyMulti(hIT8, "Sample", &subprops);
        cmsIT8Free(hIT8);
    }
    
    // 6. Create Lab colors and compute Delta E
    cmsCIELab lab1, lab2;
    if (size >= 2 * sizeof(cmsCIELab)) {
        memcpy(&lab1, data, sizeof(cmsCIELab));
        memcpy(&lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));
        cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    }
    
    // 7. Try to get named color info if available
    cmsNAMEDCOLORLIST* namedColorList = static_cast<cmsNAMEDCOLORLIST*>(
        cmsReadTag(hProfile, cmsSigNamedColor2Tag));
    if (namedColorList && cmsNamedColorCount(namedColorList) > 0) {
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number pcs[3], colorant[16];
        cmsNamedColorInfo(namedColorList, 0, name, prefix, suffix, pcs, colorant);
    }
    
    // 8. Create and duplicate pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create tone curves
        cmsToneCurve* curves[3];
        cmsFloat64Number gamma = 2.2;
        for (int i = 0; i < 3; i++) {
            curves[i] = cmsBuildGamma(ctx, gamma);
        }
        
        // Add tone curve stage
        cmsStage* stage = cmsStageAllocToneCurves(ctx, 3, curves);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
        }
        
        // Duplicate pipeline
        cmsPipeline* dupPipeline = cmsPipelineDup(pipeline);
        if (dupPipeline) {
            cmsPipelineFree(dupPipeline);
        }
        
        // Clean up curves
        for (int i = 0; i < 3; i++) {
            if (curves[i]) cmsFreeToneCurve(curves[i]);
        }
        cmsPipelineFree(pipeline);
    }
    
    // 9. CIECAM02 forward transform (if we have a model)
    // Note: We need to create a CIECAM02 model first, but for fuzzing we'll skip
    // since creating a proper model requires specific parameters
    // This demonstrates the API pattern if we had a model
    // cmsCIECAM02Forward(hModel, &xyz, &jch);
    
    // Cleanup
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}