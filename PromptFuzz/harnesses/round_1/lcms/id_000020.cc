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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsCreateLab2ProfileTHR","cmsxyY2XYZ","cmsCreateExtendedTransform","cmsGetTransformContextID","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsCreateLab2ProfileTHR","cmsxyY2XYZ","cmsCreateExtendedTransform","cmsGetTransformContextID","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create an IT8 handle from memory using input data
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
    if (it8) {
        // 2. Use cmsIT8EnumDataFormat with the IT8 handle
        char** sampleNames = NULL;
        int formatCount = cmsIT8EnumDataFormat(it8, &sampleNames);
        // Clean up sample names if allocated
        if (sampleNames) {
            for (int i = 0; i < formatCount; i++) {
                // Sample names are owned by IT8, no need to free
            }
        }
        
        cmsIT8Free(it8);
    }
    
    // 3. Create IO handler from memory for profile operations
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromMem(context, (void*)data, 
                                                      (cmsUInt32Number)(size > 1000 ? 1000 : size), 
                                                      "r");
    cmsHPROFILE inputProfile = NULL;
    if (ioHandler) {
        inputProfile = cmsOpenProfileFromIOhandlerTHR(context, ioHandler);
        // Note: cmsOpenProfileFromIOhandlerTHR takes ownership of ioHandler
    }
    
    // 4. Create Lab profile using cmsCreateLab2ProfileTHR
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;  // D65 white point
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, &whitePoint);
    
    // 5. Convert white point using cmsxyY2XYZ
    cmsCIEXYZ xyzWhitePoint;
    cmsxyY2XYZ(&xyzWhitePoint, &whitePoint);
    
    // 6. Create extended transform if we have profiles
    cmsHTRANSFORM transform = NULL;
    if (inputProfile && labProfile) {
        cmsUInt32Number nProfiles = 2;
        cmsHPROFILE profiles[2] = {inputProfile, labProfile};
        cmsBool bpc[2] = {FALSE, FALSE};
        cmsUInt32Number intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
        cmsFloat64Number adaptationStates[2] = {1.0, 1.0};
        
        transform = cmsCreateExtendedTransform(
            context,
            nProfiles,
            profiles,
            bpc,
            intents,
            adaptationStates,
            NULL,  // No gamut profile
            0,     // No gamut PCS position
            TYPE_Lab_DBL,   // Input format
            TYPE_Lab_DBL,   // Output format
            cmsFLAGS_NOCACHE
        );
        
        if (transform) {
            // 7. Get transform context ID
            cmsContext transformContext = cmsGetTransformContextID(transform);
            
            // 8. Get stage data from transform pipeline
            const cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
            if (pipeline) {
                const cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    void* stageData = cmsStageData(firstStage);
                    // stageData can be used if needed
                }
            }
            
            cmsDeleteTransform(transform);
        }
    }
    
    // Clean up
    if (labProfile) cmsCloseProfile(labProfile);
    if (inputProfile) cmsCloseProfile(inputProfile);
    cmsDeleteContext(context);
    
    return 0;
}