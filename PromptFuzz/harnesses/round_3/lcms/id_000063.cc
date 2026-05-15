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
//<score> 72.5, nr_unique_branch: 5
//<Quality> {"density":29,"unique_branches":{"cmsDetectDestinationBlackPoint":[[453,10,453,38,0,0,4,0]],"RootOfLeastSquaresFitQuadraticCurve":[[31,44,31,45,1,0,4,1]],"isInkColorspace":[[194,5,194,24,0,0,4,1],[227,5,227,12,0,0,4,0]],"Type_LUT8_Read":[[2023,31,2023,61,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsCreateXYZProfileTHR","cmsDetectDestinationBlackPoint","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsBuildGamma","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsPipelineFree","cmsLab2LCh","cmsFloat2LabEncoded","cmsDoTransformStride","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsCreateLab2ProfileTHR","cmsCreateXYZProfileTHR","cmsDetectDestinationBlackPoint","cmsCreateMultiprofileTransformTHR","cmsPipelineAlloc","cmsPipelineDup","cmsBuildGamma","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsPipelineFree","cmsPipelineFree","cmsLab2LCh","cmsFloat2LabEncoded","cmsDoTransformStride","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
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
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Open profile from the file
    cmsHPROFILE input_profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    
    // Create Lab and XYZ profiles
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
    cmsHPROFILE lab_profile = cmsCreateLab2ProfileTHR(context, &white_point);
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    
    // Detect black point
    cmsCIEXYZ black_point;
    if (input_profile) {
        cmsDetectDestinationBlackPoint(&black_point, input_profile, INTENT_PERCEPTUAL, 0);
    }
    
    // Create multiprofile transform
    cmsHPROFILE profiles[3] = {input_profile ? input_profile : lab_profile, lab_profile, xyz_profile};
    cmsUInt32Number n_profiles = input_profile ? 3 : 2;
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        context, profiles, n_profiles, TYPE_Lab_16, TYPE_XYZ_16, 
        INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE);
    
    // Create and manipulate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        cmsPipeline *pipeline_dup = cmsPipelineDup(pipeline);
        if (pipeline_dup) {
            // Create and use tone curves
            cmsToneCurve *gamma_curve = cmsBuildGamma(context, 2.2);
            if (gamma_curve) {
                cmsToneCurve *reversed_curve = cmsReverseToneCurve(gamma_curve);
                cmsFreeToneCurve(reversed_curve);
                cmsFreeToneCurve(gamma_curve);
            }
            cmsPipelineFree(pipeline_dup);
        }
        cmsPipelineFree(pipeline);
    }
    
    // Convert color data if transform exists
    if (transform) {
        cmsCIELab lab_color = {50.0, 20.0, 30.0};
        cmsCIELCh lch_color;
        cmsLab2LCh(&lch_color, &lab_color);
        
        // Perform transform
        cmsUInt16Number input[3], output[3];
        cmsFloat2LabEncoded(input, &lab_color);
        cmsDoTransformStride(transform, input, output, 1, 0);
    }
    
    // Create and use I/O handler
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(context, "input_file", "r");
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }
    
    // Create and use named color list
    cmsNAMEDCOLORLIST *named_color_list = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    if (named_color_list) {
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number pcs[3], colorant[16];
        cmsNamedColorInfo(named_color_list, 0, name, prefix, suffix, pcs, colorant);
        cmsFreeNamedColorList(named_color_list);
    }
    
    // Create and use MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    if (mlu) {
        char obtained_lang[3], obtained_country[3];
        cmsMLUgetTranslation(mlu, "en", "US", obtained_lang, obtained_country);
        cmsMLUfree(mlu);
    }
    
    // Clean up
    if (transform) {
        cmsDeleteTransform(transform);
    }
    if (input_profile) {
        cmsCloseProfile(input_profile);
    }
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    cmsDeleteContext(context);
    
    // Clean up temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}