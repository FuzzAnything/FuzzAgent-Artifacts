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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"cmsPipelineUnlinkStage":[[1571,9,1571,25,0,0,4,0],[1582,9,1582,23,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8GetData","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPluginTHR","cmsGBDAlloc","cmsGBDFree","cmsIT8Free","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsGetHeaderModel","cmsAllocNamedColorList","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsIT8Alloc","cmsIT8EnumPropertyMulti","cmsIT8GetData","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsFreeToneCurve","cmsPluginTHR","cmsGBDAlloc","cmsGBDFree","cmsIT8Free","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsPipelineFree","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Write input data to a temporary file for cmsOpenProfileFromFileTHR
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        cmsDeleteContext(context);
        assert_file_closed(&temp_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // Open profile from file
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // Create Lab2 profile
    cmsCIExyY white_point = {0.3457, 0.3585, 1.0}; // D65 white point
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, &white_point);
    
    // Get header model if input profile exists
    cmsUInt32Number model = 0;
    if (input_profile) {
        model = cmsGetHeaderModel(input_profile);
    }
    
    // Allocate named color list
    cmsNAMEDCOLORLIST *named_color_list = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    
    // Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // Build gamma curve
    cmsToneCurve *gamma_curve = cmsBuildGamma(context, 2.2);
    
    // Create tone curves array for stage allocation
    cmsToneCurve *curves[3] = {gamma_curve, gamma_curve, gamma_curve};
    
    // Allocate tone curves stage
    cmsStage *tone_curve_stage = cmsStageAllocToneCurves(context, 3, curves);
    
    // IT8 operations - create IT8 handle first
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    const char **subproperty_names = nullptr;
    cmsUInt32Number prop_count = 0;
    
    if (it8_handle) {
        // Try to enumerate properties
        prop_count = cmsIT8EnumPropertyMulti(it8_handle, "Sample", &subproperty_names);
        
        // Get some data from IT8
        const char *it8_data = cmsIT8GetData(it8_handle, "Patch1", "Sample1");
    }
    
    // Insert stage into pipeline
    if (pipeline && tone_curve_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, tone_curve_stage);
        
        // Unlink the stage
        cmsStage *unlinked_stage = nullptr;
        cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinked_stage);
        
        // Free the unlinked stage
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // Free tone curve
    if (gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    // Plugin operation
    cmsPluginTHR(context, nullptr);
    
    // GBD operations
    cmsHANDLE gbd_handle = cmsGBDAlloc(context);
    if (gbd_handle) {
        cmsGBDFree(gbd_handle);
    }
    
    // Free IT8 handle
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    // Free named color list
    if (named_color_list) {
        cmsFreeNamedColorList(named_color_list);
    }
    
    // Close profiles if they were opened
    if (input_profile) {
        cmsCloseProfile(input_profile);
    }
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    
    // Free pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // Delete context
    cmsDeleteContext(context);
    
    // Clean up temporary file
    remove("input_file");
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}