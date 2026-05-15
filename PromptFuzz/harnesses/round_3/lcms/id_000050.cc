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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 6
//<Quality> {"density":20,"unique_branches":{"cmsGetTransformPipeline":[[1448,9,1448,22,0,0,4,0]],"cmsIT8SetTableByLabel":[[3008,9,3008,23,0,0,4,0],[3008,27,3008,39,0,0,4,1],[3011,9,3011,23,0,0,4,1],[3015,9,3015,19,0,0,4,0]],"cmsGetTransformOutputFormat":[[1440,9,1440,22,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8SetPropertyHex","cmsIT8Free","cmsGetTransformPipeline","cmsPipelineSetSaveAs8bitsFlag","cmsReadTag","cmsGetToneCurveSegment","cmsGetToneCurveParametricType","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetTransformOutputFormat","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsIT8SetPropertyHex","cmsIT8Free","cmsGetTransformPipeline","cmsPipelineSetSaveAs8bitsFlag","cmsReadTag","cmsGetToneCurveSegment","cmsGetToneCurveParametricType","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Write input data to a temporary file for cube file reading
    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;
    
    // Create device link profile from cube file
    cmsHPROFILE deviceLink = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "input_cube.cube");
    if (!deviceLink) {
        remove("input_cube.cube");
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    
    // Create transform using the device link
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(sRGB, TYPE_RGB_8, 
                                                 deviceLink, TYPE_RGB_8, 
                                                 INTENT_PERCEPTUAL, 0);
    
    // Get transform output format
    cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
    
    // Create IT8 handle and use IT8 APIs
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        // Set table by label (using sample data)
        cmsIT8SetTableByLabel(it8, "DATA", "SAMPLE1", "NUMBER");
        
        // Set property in hex
        cmsIT8SetPropertyHex(it8, "COLOR", 0xFFAABB);
        
        cmsIT8Free(it8);
    }
    
    // Get pipeline from transform and set save as 8 bits flag
    cmsPipeline *pipeline = cmsGetTransformPipeline(transform);
    if (pipeline) {
        cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
    }
    
    // Get tone curve from device link (try to read red channel tone curve)
    cmsToneCurve *toneCurve = (cmsToneCurve*)cmsReadTag(deviceLink, cmsSigRedTRCTag);
    if (toneCurve) {
        // Get first segment of tone curve
        const cmsCurveSegment *segment = cmsGetToneCurveSegment(0, toneCurve);
        
        // Get parametric type
        cmsInt32Number paramType = cmsGetToneCurveParametricType(toneCurve);
        
        // Use segment and paramType to avoid unused variable warnings
        (void)segment;
        (void)paramType;
    }
    
    // Cleanup
    if (transform) cmsDeleteTransform(transform);
    if (sRGB) cmsCloseProfile(sRGB);
    if (deviceLink) cmsCloseProfile(deviceLink);
    
    remove("input_cube.cube");
    cmsDeleteContext(ctx);
    
    assert_file_closed(&cube_file);
	return 0;
}