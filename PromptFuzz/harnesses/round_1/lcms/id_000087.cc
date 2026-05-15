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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"encodeUTF8":[[290,28,290,50,0,0,4,1],[315,47,315,69,0,0,4,1]],"cmsMLUgetUTF8":[[617,9,617,33,0,0,4,0]]},"library_calls":["cmsOpenProfileFromStream","cmsOpenProfileFromFile","cmsCreateContext","cmsGetTagCount","cmsGetProfileInfoUTF8","cmsGetTagCount","cmsGetProfileInfoUTF8","cmsBuildGamma","cmsGetToneCurveSegment","cmsBFDdeltaE","cmsStageAllocCLutFloatGranular","cmsFreeToneCurveTriple","cmsCIECAM02Done","cmsStageFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsOpenProfileFromStream","cmsOpenProfileFromFile","cmsCreateContext","cmsGetTagCount","cmsGetProfileInfoUTF8","cmsGetTagCount","cmsGetProfileInfoUTF8","cmsBuildGamma","cmsGetToneCurveSegment","cmsBFDdeltaE","cmsStageAllocCLutFloatGranular","cmsFreeToneCurveTriple","cmsCIECAM02Done","cmsStageFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs to achieve a coherent event: opening and analyzing an ICC profile, then performing color difference calculations and CLUT operations.

*/


extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create a memory file for profile reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create a temporary file for writing output (if needed)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Try to open profile from memory stream
    cmsHPROFILE hProfileStream = cmsOpenProfileFromStream(in_file, "r");
    
    // Also try to open from a file (after writing data to disk)
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    cmsHPROFILE hProfileFile = cmsOpenProfileFromFile("input_file", "r");

    // Initialize default context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    cmsToneCurve* toneCurves[3] = {NULL, NULL, NULL};
    cmsHANDLE ciecam = NULL;
    
    // Use cmsGetTagCount if profiles are valid
    if (hProfileStream) {
        cmsInt32Number tagCount = cmsGetTagCount(hProfileStream);
        
        // Try to get UTF8 profile info
        char buffer[256];
        cmsUInt32Number infoSize = cmsGetProfileInfoUTF8(hProfileStream, cmsInfoDescription,
                                                        "EN", "US", buffer, sizeof(buffer));
    }
    
    if (hProfileFile) {
        cmsInt32Number tagCount = cmsGetTagCount(hProfileFile);
        
        char buffer[256];
        cmsUInt32Number infoSize = cmsGetProfileInfoUTF8(hProfileFile, cmsInfoDescription,
                                                        "EN", "US", buffer, sizeof(buffer));
    }

    // Create simple tone curves for testing cmsGetToneCurveSegment
    for (int i = 0; i < 3; i++) {
        toneCurves[i] = cmsBuildGamma(ctx, 2.2);
    }
    
    // Use cmsGetToneCurveSegment if we have tone curves
    if (toneCurves[0]) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(0, toneCurves[0]);
    }

    // Calculate BFD delta E between two colors
    cmsCIELab lab1 = {50.0, 10.0, 20.0};
    cmsCIELab lab2 = {55.0, 15.0, 25.0};
    cmsFloat64Number deltaE = cmsBFDdeltaE(&lab1, &lab2);

    // Create a simple CLUT for testing cmsStageAllocCLutFloatGranular
    cmsUInt32Number clutPoints[] = {3, 3, 3};  // 3x3x3 CLUT
    cmsFloat32Number table[3 * 3 * 3 * 3];     // 3 input, 3 output channels
    
    // Initialize with simple data
    for (size_t i = 0; i < sizeof(table)/sizeof(table[0]); i++) {
        table[i] = (cmsFloat32Number)(i % 10) / 10.0f;
    }
    
    // Create CLUT stage
    cmsStage* clutStage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, 3, 3, table);

    // Clean up tone curves using cmsFreeToneCurveTriple
    if (toneCurves[0] && toneCurves[1] && toneCurves[2]) {
        cmsFreeToneCurveTriple(toneCurves);
    }

    // Clean up CIECAM02 if we had one
    if (ciecam) {
        cmsCIECAM02Done(ciecam);
    }

    // Clean up
    if (clutStage) {
        cmsStageFree(clutStage);
    }
    
    if (hProfileStream) {
        cmsCloseProfile(hProfileStream);
    }
    
    if (hProfileFile) {
        cmsCloseProfile(hProfileFile);
    }
    
    if (ctx) {
        cmsDeleteContext(ctx);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");

    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}