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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 4
//<Quality> {"density":5,"unique_branches":{"cmsCreateRGBProfileTHR":[[160,13,160,79,0,0,4,0],[212,9,212,13,0,0,4,0]],"DecideCurveType":[[1441,9,1441,31,0,0,4,0]],"_cmsBuildRGB2XYZtransferMatrix":[[306,9,306,46,0,0,4,0]]},"library_calls":["cmsBuildTabulatedToneCurve16","cmsCreateRGBProfile","cmsFreeToneCurve","cmsReadTag","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsMLUtranslationsCount","cmsPipelineAlloc","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsPipelineFree","cmsCloseProfile"],"critical_calls":["cmsBuildTabulatedToneCurve16","cmsCreateRGBProfile","cmsFreeToneCurve","cmsReadTag","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsMLUtranslationsCount","cmsPipelineAlloc","cmsStageAllocIdentity","cmsGetStageContextID","cmsStageFree","cmsPipelineFree","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	
    
    // Create RGB profile using the input data as parameters
    cmsCIExyY whitePoint;
    whitePoint.x = (double)(data[0] + 1) / 256.0;
    whitePoint.y = (double)(data[1] + 1) / 256.0;
    whitePoint.Y = 1.0;
    
    cmsCIExyYTRIPLE primaries;
    primaries.Red.x = (double)(data[2] + 1) / 256.0;
    primaries.Red.y = (double)(data[3] + 1) / 256.0;
    primaries.Red.Y = 1.0;
    primaries.Green.x = (double)(data[4] + 1) / 256.0;
    primaries.Green.y = (double)(data[5] + 1) / 256.0;
    primaries.Green.Y = 1.0;
    primaries.Blue.x = (double)(data[6] + 1) / 256.0;
    primaries.Blue.y = (double)(data[7] + 1) / 256.0;
    primaries.Blue.Y = 1.0;
    
    // Create tone curves from input data
    cmsToneCurve* curves[3];
    cmsUInt16Number curveValues[256];
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 256 && (i * 256 + j + 8) < size; j++) {
            curveValues[j] = (cmsUInt16Number)(data[i * 256 + j + 8] * 257);
        }
        curves[i] = cmsBuildTabulatedToneCurve16(NULL, 256, curveValues);
    }
    
    // Create RGB profile
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, curves);
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }
    
    if (!rgbProfile) {
	return 0;
	}
	
    
    // Read a tag from the profile
    cmsTagSignature tag = cmsSigCopyrightTag;
    void* tagData = cmsReadTag(rgbProfile, tag);
    
    // Create an I/O handler for writing
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(NULL, "output_file", fuzz_str_1);
        if (io) {
            // Close the I/O handler
            cmsCloseIOhandler(io);
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Get MLU translations count if we have MLU data
    if (tagData) {
        cmsMLU* mlu = (cmsMLU*)tagData;
        cmsUInt32Number translationCount = cmsMLUtranslationsCount(mlu);
        (void)translationCount; // Use variable to avoid unused warning
    }
    
    // Get stage context ID - we need to create a pipeline first
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, fuzz_uint32_t_2, fuzz_uint32_t_3);
    if (pipeline) {
        cmsStage* stage = cmsStageAllocIdentity(NULL, fuzz_uint32_t_4);
        if (stage) {
            cmsContext stageContext = cmsGetStageContextID(stage);
            (void)stageContext; // Use variable to avoid unused warning
            cmsStageFree(stage);
        }
        cmsPipelineFree(pipeline);
    }
    
    // Clean up
    cmsCloseProfile(rgbProfile);
    
    assert_file_closed(&out_file);
	return 0;
}