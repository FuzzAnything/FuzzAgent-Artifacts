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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 68
//<Quality> {"density":1,"unique_branches":{"cmsCreateDeviceLinkFromCubeFileTHR":[[3240,9,3240,14,0,0,4,1],[3245,9,3245,36,0,0,4,0],[3245,9,3245,36,0,0,4,1],[3250,9,3250,48,0,0,4,0],[3250,9,3250,48,0,0,4,1],[3254,9,3254,18,0,0,4,1],[3266,9,3266,25,0,0,4,1],[3269,9,3269,23,0,0,4,1],[3276,9,3276,21,0,0,4,1],[3286,9,3286,75,0,0,4,1],[3289,9,3289,76,0,0,4,1],[3290,9,3290,64,0,0,4,1],[3294,9,3294,31,0,0,4,0],[3294,9,3294,31,0,0,4,1],[3297,9,3297,25,0,0,4,0],[3297,9,3297,25,0,0,4,1]],"GrowNamedColorList":[[727,9,727,27,0,0,4,1]],"InSymbol":[[858,21,858,32,0,0,4,0],[859,21,859,32,0,0,4,0]],"cmsBuildParametricToneCurve":[[889,9,889,18,0,0,4,0]],"cmsWhitePointFromTemp":[[62,9,62,19,0,0,4,1],[62,23,62,33,0,0,4,1],[69,13,69,23,0,0,4,0],[69,13,69,23,0,0,4,1],[69,27,69,39,0,0,4,0]],"ParseCube":[[3096,12,3096,28,0,0,4,0],[3096,12,3096,28,0,0,4,1],[3096,32,3096,53,0,0,4,0],[3096,32,3096,53,0,0,4,1],[3101,9,3101,20,0,0,4,1],[3110,9,3110,25,0,0,4,1],[3115,9,3115,25,0,0,4,1],[3121,9,3121,26,0,0,4,1],[3129,9,3129,26,0,0,4,1],[3137,9,3137,33,0,0,4,1],[3138,9,3138,33,0,0,4,1],[3146,9,3146,19,0,0,4,0],[3146,9,3146,19,0,0,4,1],[3150,9,3150,16,0,0,4,0],[3150,9,3150,16,0,0,4,1],[3151,9,3151,34,0,0,4,1],[3152,9,3152,35,0,0,4,1],[3156,9,3156,19,0,0,4,0],[3156,9,3156,19,0,0,4,1],[3157,9,3157,19,0,0,4,0],[3157,9,3157,19,0,0,4,1],[3159,17,3159,32,0,0,4,1],[3188,17,3188,29,0,0,4,1],[3219,17,3219,66,0,0,4,0]],"DefaultEvalParametricFn":[[384,5,384,11,0,0,4,0],[387,13,387,51,0,0,4,0],[387,13,387,51,0,0,4,1],[395,17,395,26,0,0,4,0],[399,21,399,26,0,0,4,0],[399,21,399,26,0,0,4,1],[436,5,436,11,0,0,4,0],[438,13,438,51,0,0,4,0],[438,13,438,51,0,0,4,1],[445,17,445,25,0,0,4,0],[445,17,445,25,0,0,4,1],[448,17,448,26,0,0,4,0],[452,21,452,26,0,0,4,0],[452,21,452,26,0,0,4,1],[501,17,501,22,0,0,4,1]],"NextCh":[[594,9,594,50,0,0,4,0],[598,13,598,60,0,0,4,0],[598,13,598,60,0,0,4,1],[600,17,600,36,0,0,4,1]]},"library_calls":["cmsCreateDeviceLinkFromCubeFile","cmsGetHeaderAttributes","cmsSetHeaderAttributes","cmsSetPCS","cmsGetProfileContextID","cmsMLUalloc","cmsMLUsetASCII","cmsAllocNamedColorList","cmsAppendNamedColor","cmsBuildParametricToneCurve","cmsWhitePointFromTemp","cmsCreateTransform","cmsGetTransformContextID","cmsStageAllocCLutFloatGranular","cmsDoTransform","cmsStageFree","cmsFreeToneCurve","cmsFreeNamedColorList","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsDoTransform","cmsStageAllocCLutFloatGranular","cmsGetTransformContextID","cmsCreateTransform","cmsWhitePointFromTemp","cmsBuildParametricToneCurve","cmsAppendNamedColor","cmsAllocNamedColorList","cmsMLUsetASCII","cmsMLUalloc","cmsGetProfileContextID","cmsSetPCS","cmsSetHeaderAttributes","cmsGetHeaderAttributes","cmsCreateDeviceLinkFromCubeFile","cmsStageFree","cmsFreeToneCurve","cmsFreeNamedColorList","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*To create a fuzz driver that uses the specified LCMS APIs together in a coherent workflow, I'll design a scenario where we:

1. Create a device link profile from a cube file
2. Modify the profile by setting PCS and attributes
3. Create supporting data structures (MLU, named color list, tone curve, white point)
4. Create a CLUT stage for transformation processing

Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data
    if (size < 256) {
        return 0;
    }

    // Write input data to a temporary file for cube file processing
    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;

    cmsContext context = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsMLU *mlu = nullptr;
    cmsNAMEDCOLORLIST *namedColorList = nullptr;
    cmsToneCurve *toneCurve = nullptr;
    cmsStage *clutStage = nullptr;
    
    try {
        // 1. Create device link profile from cube file
        profile = cmsCreateDeviceLinkFromCubeFile("input_cube.cube");
        if (!profile) {
            throw std::runtime_error("Failed to create profile from cube file");
        }

        // 2. Get and set header attributes
        cmsUInt64Number flags = 0;
        cmsGetHeaderAttributes(profile, &flags);
        // Modify flags - set some bits based on input data
        flags |= (static_cast<cmsUInt64Number>(data[0]) << 32) | data[1];
        cmsSetHeaderAttributes(profile, flags);

        // 3. Set PCS (Profile Connection Space)
        // Use input data to determine which PCS to set
        cmsColorSpaceSignature pcs = cmsSigLabData;
        if (data[2] % 3 == 0) {
            pcs = cmsSigXYZData;
        } else if (data[2] % 3 == 1) {
            pcs = cmsSigLabData;
        } else {
            pcs = cmsSigLuvData;
        }
        cmsSetPCS(profile, pcs);

        // 4. Create and populate MLU (Multi Localized Unicode)
        context = cmsGetProfileContextID(profile);
        mlu = cmsMLUalloc(context, 1);
        if (mlu) {
            const char *language = "EN";
            const char *country = "US";
            const char *asciiString = "Fuzz Test Description";
            cmsMLUsetASCII(mlu, language, country, asciiString);
        }

        // 5. Create named color list
        namedColorList = cmsAllocNamedColorList(context, 
                                                static_cast<cmsUInt32Number>(data[3] % 100), 
                                                3, 
                                                "Prefix", 
                                                "Suffix");
        if (namedColorList) {
            // Append a named color using data from input
            char colorName[32];
            snprintf(colorName, sizeof(colorName), "Color%d", data[4]);
            cmsUInt16Number pcsColor[3] = {
                static_cast<cmsUInt16Number>(data[5] * 257),
                static_cast<cmsUInt16Number>(data[6] * 257),
                static_cast<cmsUInt16Number>(data[7] * 257)
            };
            cmsUInt16Number colorants[16] = {0};
            for (int i = 0; i < 3 && (8 + i) < size; i++) {
                colorants[i] = static_cast<cmsUInt16Number>(data[8 + i] * 257);
            }
            cmsAppendNamedColor(namedColorList, colorName, pcsColor, colorants);
        }

        // 6. Create parametric tone curve
        cmsFloat64Number params[10] = {0};
        int paramCount = (data[9] % 5) + 1; // 1-5 parameters
        for (int i = 0; i < paramCount && (10 + i) < size; i++) {
            params[i] = static_cast<cmsFloat64Number>(data[10 + i]) / 255.0 * 10.0;
        }
        toneCurve = cmsBuildParametricToneCurve(context, 
                                                static_cast<cmsInt32Number>(data[8] % 5), 
                                                params);

        // 7. Calculate white point from temperature
        cmsCIExyY whitePoint;
        cmsFloat64Number tempK = 2500.0 + (static_cast<cmsFloat64Number>(data[11]) / 255.0 * 10000.0);
        cmsWhitePointFromTemp(&whitePoint, tempK);

        // 8. Create transform to get context ID
        if (profile) {
            transform = cmsCreateTransform(profile, 
                                          TYPE_Lab_DBL, 
                                          profile, 
                                          TYPE_Lab_DBL, 
                                          INTENT_PERCEPTUAL, 
                                          0);
            if (transform) {
                cmsContext transformContext = cmsGetTransformContextID(transform);
                (void)transformContext; // Use context if needed
            }
        }

        // 9. Create CLUT (Color Look-Up Table) stage
        cmsUInt32Number clutPoints[3] = {5, 5, 5};
        size_t tableSize = 5 * 5 * 5 * 3; // 5x5x5 grid, 3 output channels
        cmsFloat32Number *table = static_cast<cmsFloat32Number*>(
            malloc(tableSize * sizeof(cmsFloat32Number)));
        
        if (table) {
            // Fill table with data from input
            for (size_t i = 0; i < tableSize && (12 + i) < size; i++) {
                table[i] = static_cast<cmsFloat32Number>(data[12 + i]) / 255.0f;
            }
            
            clutStage = cmsStageAllocCLutFloatGranular(context, 
                                                      clutPoints, 
                                                      3, // input channels
                                                      3, // output channels
                                                      table);
            free(table);
        }

        // 10. Perform some transformations to exercise the setup
        if (transform) {
            cmsCIELab inputLab, outputLab;
            // Use input data for Lab values
            inputLab.L = static_cast<cmsFloat64Number>(data[13]) / 255.0 * 100.0;
            inputLab.a = static_cast<cmsFloat64Number>(data[14]) / 255.0 * 200.0 - 100.0;
            inputLab.b = static_cast<cmsFloat64Number>(data[15]) / 255.0 * 200.0 - 100.0;
            
            cmsDoTransform(transform, &inputLab, &outputLab, 1);
        }

    } catch (...) {
        // Catch any exceptions to ensure cleanup
    }

    // Cleanup all allocated resources
    if (clutStage) {
        cmsStageFree(clutStage);
    }
    
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }

    // Clean up temporary file
    remove("input_cube.cube");

    assert_file_closed(&cube_file);
	return 0;
}