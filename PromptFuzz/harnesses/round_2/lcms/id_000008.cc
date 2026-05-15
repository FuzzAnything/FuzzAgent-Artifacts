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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"GetXFormColorSpaces":[[1042,31,1042,46,0,0,4,1]]},"library_calls":["cmsSetLogErrorHandler","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMemTHR","cmsGetTagOffsetAndSize","cmsCreateExtendedTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsDeleteTransform","cmsFloat2LabEncoded","cmsCloseProfile"],"critical_calls":["cmsSetLogErrorHandler","cmsOpenProfileFromFileTHR","cmsOpenProfileFromMemTHR","cmsGetTagOffsetAndSize","cmsCreateExtendedTransform","cmsGetTransformGamutCheckPipeline","cmsPipelineInputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsDeleteTransform","cmsFloat2LabEncoded","cmsCloseProfile"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Set up error handler
    cmsSetLogErrorHandler([](cmsContext, cmsUInt32Number, const char *) {
        // Error handler - do nothing for fuzzing
    });

    // 2. Try to open a profile from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(nullptr, "input_file", "rb");
    if (!hProfile) {
        // Try opening from memory instead
        rewind(in_file);
        if (size > 0) {
            hProfile = cmsOpenProfileFromMemTHR(nullptr, data, size);
        }
    }
    
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Get tag offset and size for first tag
    cmsUInt32Number offset = 0, tagSize = 0;
    cmsBool hasTag = cmsGetTagOffsetAndSize(hProfile, 0, &offset, &tagSize);
    
    // 4. Create extended transform with the profile
    cmsBool bpc = FALSE;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsFloat64Number adaptationState = 1.0;
    cmsUInt32Number inputFormat = TYPE_Lab_16;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    cmsUInt32Number flags = 0;
    
    cmsHPROFILE profiles[] = {hProfile};
    cmsHTRANSFORM hTransform = cmsCreateExtendedTransform(
        nullptr, 
        1, 
        profiles, 
        &bpc, 
        &intent, 
        &adaptationState, 
        nullptr,  // No gamut profile
        0,        // No gamut PCS position
        inputFormat, 
        outputFormat, 
        flags
    );
    
    // 5. If transform was created, get its pipeline and use pipeline APIs
    if (hTransform) {
        cmsPipeline *pipeline = cmsGetTransformGamutCheckPipeline(hTransform);
        if (pipeline) {
            // Get input channels
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            
            // Set save as 8 bits flag
            cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
        }
        
        // Clean up transform
        cmsDeleteTransform(hTransform);
    }
    
    // 6. Test cmsFloat2LabEncoded with some sample data
    if (size >= sizeof(cmsCIELab)) {
        cmsCIELab lab;
        memset(&lab, 0, sizeof(lab));
        if (size >= sizeof(cmsCIELab)) {
            memcpy(&lab, data, std::min(size, sizeof(cmsCIELab)));
        }
        
        cmsUInt16Number wLab[3];
        cmsFloat2LabEncoded(wLab, &lab);
    }
    
    // 7. Clean up
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}