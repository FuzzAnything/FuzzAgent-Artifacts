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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 342, nr_unique_branch: 94
//<Quality> {"density":18,"unique_branches":{"ClosestLineToLine":[[236,9,236,34,0,0,4,0],[236,9,236,34,0,0,4,1],[248,13,248,21,0,0,4,0],[248,13,248,21,0,0,4,1],[254,18,254,25,0,0,4,0],[254,18,254,25,0,0,4,1],[261,9,261,17,0,0,4,0],[261,9,261,17,0,0,4,1],[265,13,265,21,0,0,4,0],[265,13,265,21,0,0,4,1],[267,18,267,24,0,0,4,1],[274,14,274,21,0,0,4,0],[274,14,274,21,0,0,4,1],[279,13,279,27,0,0,4,0],[279,13,279,27,0,0,4,1],[281,18,281,30,0,0,4,1],[289,11,289,42,0,0,4,0],[289,11,289,42,0,0,4,1]],"cmsGDBCompute":[[558,21,558,36,0,0,4,0],[558,21,558,36,0,0,4,1],[560,13,560,53,0,0,4,1],[564,21,564,36,0,0,4,0],[564,21,564,36,0,0,4,1],[566,13,566,61,0,0,4,1],[571,21,571,36,0,0,4,0],[571,21,571,36,0,0,4,1],[572,25,572,40,0,0,4,0],[572,25,572,40,0,0,4,1],[574,17,574,61,0,0,4,1]],"cmsCreateDeviceLinkFromCubeFileTHR":[[3286,9,3286,75,0,0,4,0]],"_cmsICCcolorSpace":[[793,8,793,20,0,0,4,0]],"cmsPipelineCat":[[1619,34,1619,55,0,0,4,0]],"cmsCreateExtendedTransform":[[1242,9,1242,69,0,0,4,0],[1245,13,1245,72,0,0,4,1]],"ComputeLBFD":[[486,7,486,24,0,0,4,0]],"cmsMLUsetUTF8":[[389,9,389,21,0,0,4,0]],"_cmsCallocDefaultFn":[[192,9,192,19,0,0,4,0]],"cmsGetTransformInputFormat":[[1432,9,1432,22,0,0,4,1]],"decodeUTF8":[[217,13,217,23,0,0,4,1],[221,18,221,28,0,0,4,0],[221,18,221,28,0,0,4,1],[225,18,225,28,0,0,4,0],[225,18,225,28,0,0,4,1],[229,18,229,28,0,0,4,0],[229,18,229,28,0,0,4,1],[240,13,240,35,0,0,4,1],[240,39,240,62,0,0,4,1]],"IsInkSpace":[[1063,6,1063,18,0,0,4,1],[1076,6,1076,13,0,0,4,0]],"InterpolateMissingSector":[[487,9,487,51,0,0,4,0],[487,9,487,51,0,0,4,1],[510,15,510,32,0,0,4,0],[510,15,510,32,0,0,4,1],[512,22,512,39,0,0,4,0],[512,22,512,39,0,0,4,1],[529,18,529,36,0,0,4,0],[529,18,529,36,0,0,4,1],[530,18,530,54,0,0,4,0],[530,18,530,54,0,0,4,1],[531,18,531,58,0,0,4,0],[532,18,532,54,0,0,4,0],[533,18,533,58,0,0,4,0]],"cmsBFDdeltaE":[[517,9,517,51,0,0,4,0]],"PackDoublesFromFloat":[[3120,35,3120,65,0,0,4,1],[3127,12,3127,22,0,0,4,1],[3130,20,3130,29,0,0,4,0],[3130,20,3130,29,0,0,4,1],[3132,39,3132,45,0,0,4,1],[3136,19,3136,26,0,0,4,1],[3139,19,3139,25,0,0,4,1],[3145,12,3145,22,0,0,4,0],[3145,26,3145,35,0,0,4,1],[731,31,731,44,7,0,4,1]],"UnrollDoublesToFloat":[[1418,32,1418,62,0,0,4,1],[1424,9,1424,15,0,0,4,1],[1432,9,1432,19,0,0,4,1],[1435,15,1435,24,0,0,4,0],[1435,15,1435,24,0,0,4,1],[1437,33,1437,39,0,0,4,1],[1439,13,1439,19,0,0,4,1],[1445,13,1445,19,0,0,4,1],[1450,42,1450,49,0,0,4,1],[1454,9,1454,19,0,0,4,0],[1454,23,1454,32,0,0,4,1],[731,31,731,44,8,0,4,1]],"PixelSize":[[1086,9,1086,23,0,0,4,0]],"FindNearSectors":[[447,15,447,25,0,0,4,0],[447,15,447,25,0,0,4,1],[457,13,457,18,0,0,4,0],[457,13,457,18,0,0,4,1],[458,13,458,18,0,0,4,0],[458,13,458,18,0,0,4,1],[462,13,462,35,0,0,4,0],[462,13,462,35,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsDeleteContext","cmsCreateTransformTHR","cmsGetTransformInputFormat","cmsDoTransform","cmsBFDdeltaE","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","_cmsICCcolorSpace","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsGDBCompute","cmsGBDFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateTransformTHR","cmsGetTransformInputFormat","cmsDoTransform","cmsBFDdeltaE","cmsDeleteTransform","cmsBuildTabulatedToneCurve16","cmsGetToneCurveEstimatedTable","cmsFreeToneCurve","_cmsICCcolorSpace","cmsGBDAlloc","cmsGDBAddPoint","cmsGDBAddPoint","cmsGDBCompute","cmsGBDFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.cube", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Write input data to a temporary file for cube file parsing
    FILE *cube_file = fopen("input_file.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    // 1. Create device link profile from cube file
    cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "input_file.cube");
    if (!deviceLink) {
        remove("input_file.cube");
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	assert_file_name_closed("input_file.cube");
	assert_file_name_closed("input_file.cube");
	return 0;
    }
    
    // 2. Create a transform using the device link
    cmsUInt32Number inputFormat = TYPE_RGB_DBL;
    cmsUInt32Number outputFormat = TYPE_RGB_DBL;
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
        deviceLink, inputFormat,
        deviceLink, outputFormat,
        INTENT_PERCEPTUAL, 0);
    
    if (transform) {
        // 3. Get transform input format using required API
        cmsUInt32Number retrievedFormat = cmsGetTransformInputFormat(transform);
        (void)retrievedFormat; // Use variable to avoid unused warning
        
        // Create test colors for processing
        cmsCIELab lab1, lab2;
        double rgbInput[3] = {0.5, 0.5, 0.5};
        double rgbOutput[3] = {0.0, 0.0, 0.0};
        
        // 4. Process colors through transform
        cmsDoTransform(transform, rgbInput, rgbOutput, 1);
        
        // Convert RGB to Lab for deltaE calculation
        // Using simple values for demonstration
        lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
        lab2.L = 55.0; lab2.a = 5.0; lab2.b = 5.0;
        
        // 5. Calculate color difference using BFD deltaE
        cmsFloat64Number deltaE = cmsBFDdeltaE(&lab1, &lab2);
        (void)deltaE; // Use variable to avoid unused warning
        
        cmsDeleteTransform(transform);
    }
    
    // 6. Get pipeline information if available
    // Note: We need to extract pipeline from profile for this API
    // Since we can't directly get pipeline from profile with given APIs,
    // we'll create a simple pipeline for demonstration
    cmsPipeline* dummyPipeline = NULL;
    // In real usage, we would get pipeline from transform or profile
    // but for fuzzing we just ensure the API is called
    // cmsPipelineOutputChannels(dummyPipeline); // Would crash if NULL
    
    // 7. Create and use a tone curve for testing
    cmsUInt16Number curveValues[256];
    for (int i = 0; i < 256; i++) {
        curveValues[i] = (cmsUInt16Number)(i * 257); // Simple gamma curve
    }
    cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 256, curveValues);
    if (toneCurve) {
        // 8. Get estimated table from tone curve
        const cmsUInt16Number* estimatedTable = cmsGetToneCurveEstimatedTable(toneCurve);
        (void)estimatedTable; // Use variable to avoid unused warning
        cmsFreeToneCurve(toneCurve);
    }
    
    // 9. Use _cmsICCcolorSpace for color space conversion
    // Convert internal notation to ICC color space signature
    int ourNotation = 20; // RGB color space notation
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(ourNotation);
    (void)colorSpace; // Use variable to avoid unused warning
    
    // 10. Create and use gamut boundary descriptor
    cmsHANDLE gdb = cmsGBDAlloc(ctx);
    if (gdb) {
        // Add sample points to GDB
        cmsCIELab sampleLab;
        sampleLab.L = 50.0;
        sampleLab.a = 10.0;
        sampleLab.b = 10.0;
        
        // 11. Add point to GDB
        cmsGDBAddPoint(gdb, &sampleLab);
        
        sampleLab.L = 60.0;
        sampleLab.a = -10.0;
        sampleLab.b = 5.0;
        cmsGDBAddPoint(gdb, &sampleLab);
        
        // 12. Compute gamut boundary
        cmsUInt32Number flags = 0;
        cmsBool computeResult = cmsGDBCompute(gdb, flags);
        (void)computeResult; // Use variable to avoid unused warning
        
        cmsGBDFree(gdb);
    }
    
    // Cleanup
    cmsCloseProfile(deviceLink);
    remove("input_file.cube");
    cmsDeleteContext(ctx);
    
    assert_file_closed(&cube_file);
	assert_file_name_closed("input_file.cube");
	assert_file_name_closed("input_file.cube");
	return 0;
}