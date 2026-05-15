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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 88, nr_unique_branch: 25
//<Quality> {"density":16,"unique_branches":{"GetPoint":[[339,9,339,19,0,0,4,1],[339,23,339,37,0,0,4,1],[339,41,339,54,0,0,4,1],[347,9,347,18,0,0,4,1],[347,22,347,31,0,0,4,1],[347,35,347,51,0,0,4,1],[347,55,347,71,0,0,4,1]],"QuantizeToSector":[[173,9,173,26,0,0,4,1],[175,9,175,26,0,0,4,1]],"cmsGetPostScriptColorResource":[[1537,9,1537,32,0,0,4,0],[1542,9,1542,32,0,0,4,1]],"cmsWriteRawTag":[[2139,9,2139,55,0,0,4,1],[2141,9,2141,34,0,0,4,1],[2157,9,2157,32,0,0,4,1]],"ToSpherical":[[130,8,130,19,0,0,4,0]],"cmsGetPostScriptCSA":[[1589,9,1589,23,0,0,4,1],[1594,9,1594,13,0,0,4,1]],"freeOneTag":[[1653,13,1653,32,0,0,4,1]],"cmsGetToneCurveParametricType":[[1413,9,1413,28,0,0,4,1]],"GenerateCSA":[[1046,9,1046,61,0,0,4,1],[1057,13,1057,40,0,0,4,0],[1058,13,1058,40,0,0,4,0],[1093,9,1093,20,0,0,4,1]],"cmsGDBAddPoint":[[367,9,367,20,0,0,4,1],[370,9,370,31,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsDeleteContext","cmsGetPCS","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsGetToneCurveParametricType","cmsWriteRawTag","cmsCloseProfile","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsFreeToneCurve","cmsSetDeviceClass","cmsFloat2LabEncoded","cmsGetPostScriptCSA","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetPCS","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsGetToneCurveParametricType","cmsWriteRawTag","cmsCloseProfile","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsFreeToneCurve","cmsSetDeviceClass","cmsFloat2LabEncoded","cmsGetPostScriptCSA","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if input is too small
    if (size < 16) {
	return 0;
	}
	

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // Create a temporary file for cube file data
    FILE *cube_file = fopen("input_file", "wb");
    if (!cube_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;

    // Create device link profile from cube file
    cmsHPROFILE hProfile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "input_file");
    if (!hProfile) {
        remove("input_file");
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Get PCS color space
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);

    // Create dummy tone curves for linearization device link
    cmsToneCurve* transferFunctions[3] = {NULL, NULL, NULL};
    
    // Create a simple gamma curve
    cmsToneCurve* curve = cmsBuildGamma(ctx, 2.2);
    if (curve) {
        transferFunctions[0] = transferFunctions[1] = transferFunctions[2] = curve;
        
        // Create linearization device link
        cmsHPROFILE linearProfile = cmsCreateLinearizationDeviceLink(
            cmsSigRgbData, 
            transferFunctions
        );
        
        if (linearProfile) {
            // Get parametric type of tone curve
            cmsInt32Number paramType = cmsGetToneCurveParametricType(curve);
            
            // Write raw tag to profile
            cmsWriteRawTag(hProfile, cmsSigCopyrightTag, data, 
                          size > 1024 ? 1024 : size);
            
            cmsCloseProfile(linearProfile);
        }
        
        // Create gamut boundary descriptor
        cmsHANDLE hGDB = cmsGBDAlloc(ctx);
        if (hGDB) {
            // Add point to GDB
            cmsCIELab lab = {50.0, 0.0, 0.0};
            cmsGDBAddPoint(hGDB, &lab);
            cmsGBDFree(hGDB);
        }
        
        cmsFreeToneCurve(curve);
    }

    // Set device class
    cmsSetDeviceClass(hProfile, cmsSigLinkClass);

    // Convert Lab to encoded form
    cmsCIELab labValue = {75.0, 20.0, -30.0};
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncoded(encodedLab, &labValue);

    // Get PostScript CSA
    char buffer[4096];
    cmsUInt32Number bytesWritten = cmsGetPostScriptCSA(
        ctx, 
        hProfile, 
        INTENT_PERCEPTUAL, 
        0, 
        buffer, 
        sizeof(buffer)
    );

    // Create output file and write CSA data if available
    if (bytesWritten > 0) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(buffer, 1, bytesWritten, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    remove("input_file");
    cmsDeleteContext(ctx);

    assert_file_closed(&cube_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}