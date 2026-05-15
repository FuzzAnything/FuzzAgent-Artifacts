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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 2
//<Quality> {"density":30,"unique_branches":{"ComputeChromaticAdaptation":[[218,9,218,58,0,0,4,0],[219,9,219,58,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateLab2ProfileTHR","cmsDeleteContext","cmsxyY2XYZ","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsDeleteContext","cmsCreateXYZProfileTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8GetPatchName","cmsIT8GetData","cmsCreateExtendedTransform","cmsGetTransformContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsPipelineFree","cmsDoTransform","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsGetColorSpace","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateLab2ProfileTHR","cmsxyY2XYZ","cmsCreate_sRGBProfileTHR","cmsCreateXYZProfileTHR","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8GetPatchName","cmsIT8GetData","cmsCreateExtendedTransform","cmsGetTransformContextID","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineGetPtrToFirstStage","cmsStageData","cmsPipelineFree","cmsDoTransform","cmsOpenIOhandlerFromMem","cmsOpenProfileFromIOhandlerTHR","cmsGetColorSpace","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs to achieve a color transformation workflow:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if there's not enough data for basic operations
    if (size < 64) {
	return 0;
	}
	
    
    // Initialize LCMS context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Create white point for Lab profile
    cmsCIExyY whitePoint;
    const size_t minSize = sizeof(cmsCIExyY);
    if (size >= minSize) {
        memcpy(&whitePoint, data, minSize);
    } else {
        // Use default white point if insufficient data
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }
    
    // 1. Create Lab profile with white point
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, &whitePoint);
    if (!labProfile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Convert white point from xyY to XYZ
    cmsCIEXYZ xyzWhitePoint;
    cmsxyY2XYZ(&xyzWhitePoint, &whitePoint);
    
    // Create additional profiles for extended transform
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(context);
    if (!srgbProfile) {
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create XYZ profile
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    if (!xyzProfile) {
        cmsCloseProfile(srgbProfile);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Create IT8 handle from input data and enumerate data format
    cmsHANDLE it8Handle = nullptr;
    if (size > 0) {
        it8Handle = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)(size < 4096 ? size : 4096));
        
        if (it8Handle) {
            char** sampleNames = nullptr;
            int formatCount = cmsIT8EnumDataFormat(it8Handle, &sampleNames);
            
            // Free sample names if allocated
            if (sampleNames) {
                for (int i = 0; i < formatCount; i++) {
                    if (sampleNames[i]) {
                        free(sampleNames[i]);
                    }
                }
                free(sampleNames);
            }
            
            // 4. Use the IT8 data in various ways
            const char* patchName = cmsIT8GetPatchName(it8Handle, 0, nullptr);
            const char* dataValue = cmsIT8GetData(it8Handle, "patch1", "sample1");
        }
    }
    
    // Prepare for extended transform
    cmsUInt32Number nProfiles = 3;
    std::vector<cmsHPROFILE> profiles(3);
    profiles[0] = labProfile;
    profiles[1] = srgbProfile;
    profiles[2] = xyzProfile;
    
    std::vector<cmsBool> bpc(nProfiles, FALSE);
    std::vector<cmsUInt32Number> intents(nProfiles, INTENT_PERCEPTUAL);
    std::vector<cmsFloat64Number> adaptationStates(nProfiles, 1.0);
    
    cmsUInt32Number InputFormat = TYPE_Lab_DBL;
    cmsUInt32Number OutputFormat = TYPE_RGB_DBL;
    cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
    
    // 5. Create extended transform
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(
        context,
        nProfiles,
        profiles.data(),
        bpc.data(),
        intents.data(),
        adaptationStates.data(),
        nullptr,  // No gamut profile
        0,        // No gamut PCS position
        InputFormat,
        OutputFormat,
        dwFlags
    );
    
    if (transform) {
        // 6. Get transform context ID
        cmsContext transformContext = cmsGetTransformContextID(transform);
        
        // Create a simple pipeline to test cmsStageData
        cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
        if (pipeline) {
            // Add an identity stage
            cmsStage* stage = cmsStageAllocIdentity(context, 3);
            if (stage) {
                cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
                
                // 7. Get stage data
                void* stageData = cmsStageData(stage);
                
                // Get first stage from pipeline
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
                if (firstStage) {
                    void* firstStageData = cmsStageData(firstStage);
                }
                
                cmsPipelineFree(pipeline);
            }
        }
        
        // Test the transform with some color values
        cmsCIELab labColor;
        cmsFloat64Number rgbColor[3];
        
        // Use parts of input data for color values if available
        if (size >= sizeof(cmsCIELab)) {
            memcpy(&labColor, data, sizeof(cmsCIELab));
        } else {
            labColor.L = 50.0;
            labColor.a = 0.0;
            labColor.b = 0.0;
        }
        
        // Perform color transformation
        cmsDoTransform(transform, &labColor, rgbColor, 1);
        
        // 8. Create IO handler from memory
        size_t ioDataSize = size > 1024 ? 1024 : size;
        if (ioDataSize > 0) {
            // Create a mutable copy of the data for the IO handler
            std::vector<uint8_t> ioData(data, data + ioDataSize);
            cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromMem(
                context, 
                ioData.data(), 
                (cmsUInt32Number)ioDataSize, 
                "r"
            );
            
            if (ioHandler) {
                // Try to open profile from the IO handler
                cmsHPROFILE memProfile = cmsOpenProfileFromIOhandlerTHR(context, ioHandler);
                if (memProfile) {
                    // Use the profile
                    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(memProfile);
                    cmsCloseProfile(memProfile);
                }
                cmsCloseIOhandler(ioHandler);
            }
        }
        
        cmsDeleteTransform(transform);
    }
    
    // 9. Cleanup
    if (it8Handle) {
        cmsIT8Free(it8Handle);
    }
    
    cmsCloseProfile(xyzProfile);
    cmsCloseProfile(srgbProfile);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    
    return 0;
}