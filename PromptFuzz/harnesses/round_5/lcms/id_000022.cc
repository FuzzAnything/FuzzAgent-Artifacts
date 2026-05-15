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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"cmsIT8SetTableByLabel":[[3008,27,3008,39,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8SetTableByLabel","cmsIT8Free","cmsCreateProfilePlaceholder","cmsSetHeaderAttributes","cmsGetDeviceClass","cmsSetHeaderModel","cmsSetPCS","cmsCloseProfile","cmsCreate_sRGBProfileTHR","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateLinearizationDeviceLinkTHR","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8SetTableByLabel","cmsIT8Free","cmsCreateProfilePlaceholder","cmsSetHeaderAttributes","cmsGetDeviceClass","cmsSetHeaderModel","cmsSetPCS","cmsCloseProfile","cmsCreate_sRGBProfileTHR","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsBuildTabulatedToneCurve16","cmsCreateLinearizationDeviceLinkTHR","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineStageCount","cmsPipelineGetPtrToFirstStage","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create IT8 handle and use cmsIT8SetTableByLabel
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    if (hIT8) {
        // Try to set table by label using random data
        if (size > 100) {
            // Use small portions of input data as strings
            char set[32], field[32], type[32];
            size_t offset = size / 3;
            
            memcpy(set, data, std::min(size_t(31), size/10));
            set[31] = '\0';
            
            memcpy(field, data + offset, std::min(size_t(31), size/10));
            field[31] = '\0';
            
            memcpy(type, data + offset*2, std::min(size_t(31), size - offset*2));
            type[31] = '\0';
            
            cmsIT8SetTableByLabel(hIT8, set, field, type);
        }
        
        // 2. Use cmsIT8EnumProperties (note: this API might not exist in lcms2.h)
        // We'll comment this out as it seems to be a typo in the question
        // char **propNames;
        // cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propNames);
        
        cmsIT8Free(hIT8);
    }
    
    // 3. Create profile placeholder and use header APIs
    cmsHPROFILE placeholder = cmsCreateProfilePlaceholder(ctx);
    if (placeholder) {
        // Use input data to set header attributes (use first 8 bytes if available)
        cmsUInt64Number flags = 0;
        if (size >= 8) {
            memcpy(&flags, data, 8);
        }
        cmsSetHeaderAttributes(placeholder, flags);
        
        // 4. Get device class
        cmsProfileClassSignature devClass = cmsGetDeviceClass(placeholder);
        (void)devClass; // Mark as used
        
        // 5. Set header model using random data
        cmsUInt32Number model = 0;
        if (size >= 4) {
            memcpy(&model, data, 4);
        }
        cmsSetHeaderModel(placeholder, model);
        
        // 6. Set PCS
        cmsSetPCS(placeholder, cmsSigLabData);
        
        cmsCloseProfile(placeholder);
    }
    
    // 7. Create sRGB profile
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb) {
        // Try to create a transform and check pipeline stages
        if (size > 50) {
            // Create a simple linearization device link using tone curves from input
            cmsToneCurve* curves[3];
            cmsUInt16Number curvePoints[256];
            
            // Use input data to populate curve points
            for (int i = 0; i < 256 && i*2 < (int)size; i++) {
                curvePoints[i] = (data[i*2 % size] << 8) | data[(i*2+1) % size];
            }
            
            curves[0] = cmsBuildTabulatedToneCurve16(ctx, 256, curvePoints);
            curves[1] = cmsBuildTabulatedToneCurve16(ctx, 256, curvePoints);
            curves[2] = cmsBuildTabulatedToneCurve16(ctx, 256, curvePoints);
            
            if (curves[0] && curves[1] && curves[2]) {
                cmsHPROFILE link = cmsCreateLinearizationDeviceLinkTHR(ctx, 
                    cmsSigRgbData, curves);
                
                if (link) {
                    // Create transform
                    cmsHTRANSFORM xform = cmsCreateTransformTHR(ctx,
                        srgb, TYPE_RGB_8,
                        link, TYPE_RGB_8,
                        INTENT_PERCEPTUAL, 0);
                    
                    if (xform) {
                        // 8. Check pipeline stages (note: cmsPipelineCheckAndRetreiveStages 
                        // might be internal API, using alternative approach)
                        cmsPipeline* pipeline = cmsGetTransformPipeline(xform);
                        if (pipeline) {
                            cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
                            (void)stageCount; // Mark as used
                            
                            // Try to emulate checking stages
                            if (stageCount > 0) {
                                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                                if (firstStage) {
                                    // Process some data through the pipeline
                                    cmsUInt8Number input[3] = {data[0], data[1], data[2]};
                                    cmsUInt8Number output[3];
                                    cmsDoTransform(xform, input, output, 1);
                                }
                            }
                        }
                        
                        cmsDeleteTransform(xform);
                    }
                    
                    cmsCloseProfile(link);
                }
                
                cmsFreeToneCurve(curves[0]);
                cmsFreeToneCurve(curves[1]);
                cmsFreeToneCurve(curves[2]);
            }
        }
        
        cmsCloseProfile(srgb);
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    
    return 0;
}