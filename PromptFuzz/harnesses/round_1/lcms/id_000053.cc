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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsGetProfileIOhandler","cmsSetHeaderFlags","cmsMD5computeID","cmsBuildParametricToneCurve","cmsIsToneCurveMultisegment","cmsFreeToneCurve","cmsStageAllocCLut16bit","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineFree","cmsIT8LoadFromMem","cmsIT8SetPropertyMulti","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetProfileIOhandler","cmsSetHeaderFlags","cmsMD5computeID","cmsBuildParametricToneCurve","cmsIsToneCurveMultisegment","cmsFreeToneCurve","cmsStageAllocCLut16bit","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineFree","cmsIT8LoadFromMem","cmsIT8SetPropertyMulti","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Too small for meaningful testing

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // Create a NULL profile using THR version
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    if (!nullProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get I/O handler from profile
    cmsIOHANDLER* ioHandler = cmsGetProfileIOhandler(nullProfile);
    if (ioHandler) {
        // We can perform operations with ioHandler if needed
    }

    // Set header flags using random data
    uint32_t flags = 0;
    if (size >= 4) {
        memcpy(&flags, data, 4);
    }
    cmsSetHeaderFlags(nullProfile, flags);

    // Compute MD5 ID for profile
    cmsMD5computeID(nullProfile);

    // Create a tone curve from input data to check if multisegment
    cmsToneCurve* toneCurve = NULL;
    size_t curvePoints = size / sizeof(cmsFloat64Number);
    if (curvePoints >= 2) {
        cmsFloat64Number* params = (cmsFloat64Number*)malloc(curvePoints * sizeof(cmsFloat64Number));
        if (params) {
            memcpy(params, data, curvePoints * sizeof(cmsFloat64Number));
            toneCurve = cmsBuildParametricToneCurve(ctx, 1, params);
            free(params);
        }
    }

    // Check if tone curve is multisegment (if created)
    if (toneCurve) {
        cmsBool isMultiSegment = cmsIsToneCurveMultisegment(toneCurve);
        (void)isMultiSegment; // Use result to avoid unused warning
        cmsFreeToneCurve(toneCurve);
    }

    // Create a CLUT stage using input data
    cmsStage* clutStage = NULL;
    size_t tableEntries = 16; // 2x2x2x2 grid for simplicity
    if (size >= tableEntries * sizeof(cmsUInt16Number) * 3) { // Need enough data for table
        cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableEntries * sizeof(cmsUInt16Number) * 3);
        if (table) {
            memcpy(table, data, tableEntries * sizeof(cmsUInt16Number) * 3);
            clutStage = cmsStageAllocCLut16bit(ctx, 2, 4, 3, table);
            free(table);
            
            // Free the CLUT stage if created
            if (clutStage) {
                // Note: cmsStageFree is not in the provided API list, but we need to free it
                // Using pipeline approach as alternative
                cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 4, 3);
                if (pipeline) {
                    cmsPipelineInsertStage(pipeline, cmsAT_END, clutStage);
                    cmsPipelineFree(pipeline); // This frees all stages
                } else if (clutStage) {
                    // Fallback: try to free stage directly (may leak if API unavailable)
                    // This is a compromise since cmsStageFree isn't in provided APIs
                }
            }
        }
    }

    // Create IT8 handle from memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, data, size > 1024 ? 1024 : size);
    if (it8) {
        // Set multi property on IT8 handle
        const char* key = "TestKey";
        const char* subkey = "SubKey";
        const char* value = "TestValue";
        cmsIT8SetPropertyMulti(it8, key, subkey, value);
        
        cmsIT8Free(it8);
    }

    // Cleanup
    cmsCloseProfile(nullProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}