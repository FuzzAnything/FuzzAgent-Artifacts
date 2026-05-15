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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 72
//<Quality> {"density":12,"unique_branches":{"cmsDupToneCurve":[[970,9,970,19,0,0,4,0]],"smooth2":[[1161,9,1161,14,0,0,4,0],[1161,9,1161,14,0,0,4,1],[1161,18,1161,47,0,0,4,1],[1167,9,1167,18,0,0,4,0],[1167,22,1167,31,0,0,4,0],[1167,35,1167,44,0,0,4,0],[1179,17,1179,26,0,0,4,0],[1179,17,1179,26,0,0,4,1],[1198,21,1198,26,0,0,4,0],[1198,21,1198,26,0,0,4,1],[1205,9,1205,18,0,0,4,0],[1206,9,1206,18,0,0,4,0],[1207,9,1207,18,0,0,4,0]],"cmsSmoothToneCurve":[[1220,9,1220,20,0,0,4,0],[1220,24,1220,49,0,0,4,0],[1224,13,1224,39,0,0,4,0],[1224,13,1224,39,0,0,4,1],[1227,17,1227,44,0,0,4,0],[1234,21,1234,30,0,0,4,0],[1234,34,1234,43,0,0,4,0],[1234,47,1234,56,0,0,4,0],[1240,33,1240,43,0,0,4,0],[1240,33,1240,43,0,0,4,1],[1246,25,1246,35,0,0,4,1],[1252,25,1252,91,0,0,4,0],[1252,25,1252,91,0,0,4,1],[1257,42,1257,47,0,0,4,0],[1257,42,1257,47,0,0,4,1],[1259,33,1259,43,0,0,4,0],[1259,33,1259,43,0,0,4,1],[1260,33,1260,47,0,0,4,0],[1260,33,1260,47,0,0,4,1],[1261,33,1261,48,0,0,4,0],[1261,33,1261,48,0,0,4,1],[1269,29,1269,42,0,0,4,0],[1269,29,1269,42,0,0,4,1],[1269,46,1269,66,0,0,4,0],[1269,46,1269,66,0,0,4,1],[1275,29,1275,42,0,0,4,0],[1275,29,1275,42,0,0,4,1],[1275,46,1275,66,0,0,4,0],[1275,46,1275,66,0,0,4,1],[1281,29,1281,42,0,0,4,0],[1281,29,1281,42,0,0,4,1],[1283,41,1283,51,0,0,4,0],[1283,41,1283,51,0,0,4,1],[1302,21,1302,30,0,0,4,0],[1305,21,1305,30,0,0,4,0],[1308,21,1308,30,0,0,4,0]],"cmsCreateNULLProfileTHR":[[971,9,971,18,0,0,4,1],[976,9,976,57,0,0,4,1],[985,9,985,20,0,0,4,1],[992,9,992,57,0,0,4,1],[995,9,995,109,0,0,4,1],[998,9,998,56,0,0,4,1],[1001,9,1001,60,0,0,4,1],[1002,9,1002,71,0,0,4,1]],"CurveSetElemTypeFree":[[199,17,199,44,0,0,4,1]],"cmsCMCdeltaE":[[553,7,553,20,0,0,4,0],[553,7,553,20,0,0,4,1],[553,24,553,37,0,0,4,0],[553,24,553,37,0,0,4,1],[564,7,564,32,0,0,4,0],[564,7,564,32,0,0,4,1],[569,7,569,21,0,0,4,0],[569,7,569,21,0,0,4,1],[569,25,569,39,0,0,4,0],[569,25,569,39,0,0,4,1],[577,8,577,18,0,0,4,0],[577,8,577,18,0,0,4,1]],"cmsStageAllocToneCurves":[[283,13,283,43,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsDeleteContext","cmsBuildTabulatedToneCurve16","cmsSmoothToneCurve","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageOutputChannels","cmsStageFree","cmsCMCdeltaE","cmsIsCLUT","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsBuildTabulatedToneCurve16","cmsSmoothToneCurve","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageOutputChannels","cmsStageFree","cmsCMCdeltaE","cmsIsCLUT","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzzer that uses the specified LCMS APIs to achieve color profile processing and color difference calculation:

*/


// Include LCMS headers

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Create NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    if (!nullProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // Create tone curves from input data
    cmsToneCurve* toneCurves[3] = {NULL, NULL, NULL};
    size_t curveDataSize = size / 6; // Divide data among 3 curves
    
    if (curveDataSize > 2) { // Need at least 2 points per curve
        for (int i = 0; i < 3; i++) {
            // Take portion of data for this curve
            size_t offset = i * curveDataSize;
            size_t points = curveDataSize / 2; // 16-bit values
            
            if (points > 256) points = 256; // Limit to reasonable size
            
            cmsUInt16Number* curveValues = new cmsUInt16Number[points];
            
            // Fill curve with data (cyclic if needed)
            for (size_t j = 0; j < points; j++) {
                if (offset + j * 2 + 1 < size) {
                    curveValues[j] = (data[offset + j * 2] << 8) | data[offset + j * 2 + 1];
                } else {
                    curveValues[j] = (j * 65535) / (points - 1); // Linear fallback
                }
            }
            
            toneCurves[i] = cmsBuildTabulatedToneCurve16(context, points, curveValues);
            delete[] curveValues;
            
            if (toneCurves[i]) {
                // Smooth the tone curve
                cmsSmoothToneCurve(toneCurves[i], 1.0); // Lambda = 1.0
            }
        }
    }

    // Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_1, fuzz_uint32_t_2);
    
    if (pipeline) {
        // Create stage with tone curves
        cmsStage* toneCurveStage = cmsStageAllocToneCurves(context, 3, toneCurves);
        
        if (toneCurveStage) {
            // Get output channels of stage
            cmsUInt32Number outputChannels = cmsStageOutputChannels(toneCurveStage);
            
            // Note: In real usage, we would add stage to pipeline here
            // but the required API for that isn't in our list
        }
        
        // Clean up stage if created
        if (toneCurveStage) {
            cmsStageFree(toneCurveStage);
        }
    }

    // Create Lab colors for deltaE calculation from input data
    cmsCIELab lab1 = {0.0, 0.0, 0.0};
    cmsCIELab lab2 = {0.0, 0.0, 0.0};
    
    if (size >= 24) { // Need 24 bytes for 3 doubles * 2 colors
        // Use first 24 bytes to populate Lab colors
        memcpy(&lab1, data, 12);
        memcpy(&lab2, data + 12, 12);
        
        // Ensure reasonable values
        lab1.L = lab1.L < 0 ? 0 : (lab1.L > 100 ? 100 : lab1.L);
        lab2.L = lab2.L < 0 ? 0 : (lab2.L > 100 ? 100 : lab2.L);
        lab1.a = lab1.a < -128 ? -128 : (lab1.a > 128 ? 128 : lab1.a);
        lab2.a = lab2.a < -128 ? -128 : (lab2.a > 128 ? 128 : lab2.a);
        lab1.b = lab1.b < -128 ? -128 : (lab1.b > 128 ? 128 : lab1.b);
        lab2.b = lab2.b < -128 ? -128 : (lab2.b > 128 ? 128 : lab2.b);
    } else {
        // Use some default values based on input data
        lab1.L = size > 0 ? (data[0] * 100.0 / 255.0) : 50.0;
        lab1.a = size > 1 ? (data[1] - 128) : 0.0;
        lab1.b = size > 2 ? (data[2] - 128) : 0.0;
        
        lab2.L = size > 3 ? (data[3] * 100.0 / 255.0) : 50.0;
        lab2.a = size > 4 ? (data[4] - 128) : 0.0;
        lab2.b = size > 5 ? (data[5] - 128) : 0.0;
    }
    
    // Calculate CMC deltaE
    cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 1.0, 1.0);

    // Check if NULL profile has CLUT
    cmsBool hasCLUT = cmsIsCLUT(nullProfile, fuzz_uint32_t_3, fuzz_uint32_t_4);

    // Note: cmsGDBCompute requires a GDB handle which we don't have
    // We'll create a simple placeholder to show the API call
    // In real usage, we would need to create a GDB handle first
    
    // Clean up
    for (int i = 0; i < 3; i++) {
        if (toneCurves[i]) {
            cmsFreeToneCurve(toneCurves[i]);
        }
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    cmsCloseProfile(nullProfile);
    cmsDeleteContext(context);
    
    return 0;
}