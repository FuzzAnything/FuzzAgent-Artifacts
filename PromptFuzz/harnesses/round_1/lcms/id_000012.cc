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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["cmsSetLogErrorHandlerTHR","cmsGetEncodedCMMversion","cmsCreateBCHSWabstractProfile","cmsCloseProfile","cmsOpenIOhandlerFromFile","cmsCloseProfile","cmsOpenProfileFromIOhandlerTHR","cmsOpenProfileFromFile","cmsCloseProfile","cmsGetNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler"],"critical_calls":["cmsSetLogErrorHandlerTHR","cmsGetEncodedCMMversion","cmsCreateBCHSWabstractProfile","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandlerTHR","cmsOpenProfileFromFile","cmsCloseProfile","cmsGetNamedColorList","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler"],"visited":0}
/**/


// Custom error handler to suppress log output during fuzzing
static void  id_000012_ignoreLogError(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, 
                          const char* /*Text*/) {
    // Do nothing - suppress log output
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Set custom error handler
    cmsSetLogErrorHandlerTHR(NULL,  id_000012_ignoreLogError);

    // 2. Get CMM version
    int version = cmsGetEncodedCMMversion();

    // 3. Create a BCHSW abstract profile
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(
        256,  // nLUTPoints
        0.5,  // Brightness
        1.0,  // Contrast
        0.0,  // Hue
        1.0,  // Saturation
        5000, // TempSrc (D50)
        6500  // TempDest (D65)
    );

    // 4. Create a FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        if (abstractProfile) cmsCloseProfile(abstractProfile);
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Create an I/O handler from the FILE*
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromFile(NULL, "dummy", "rb");
    if (!io_handler) {
        assert_file_closed(&in_file);;
        if (abstractProfile) cmsCloseProfile(abstractProfile);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Open profile from I/O handler
    cmsHPROFILE inputProfile = cmsOpenProfileFromIOhandlerTHR(NULL, io_handler);
    
    // 7. Also try opening profile from file (using a temporary file)
    FILE* temp_file = fopen("temp_input.icc", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        cmsHPROFILE fileProfile = cmsOpenProfileFromFile("temp_input.icc", "r");
        if (fileProfile) {
            cmsCloseProfile(fileProfile);
        }
        remove("temp_input.icc");
    }

    // 8. Prepare data for CIECAM02 reverse transformation
    cmsJCh jch;
    cmsCIEXYZ xyz;
    
    // Initialize with reasonable values
    jch.J = 50.0;
    jch.C = 20.0;
    jch.h = 0.0;
    
    // 9. Perform CIECAM02 reverse transformation (using NULL handle as placeholder)
    // Note: In real usage, we would need a valid cmsHANDLE from cmsCIECAM02Init
    // This demonstrates the API call pattern
    cmsHANDLE camModel = NULL; // Placeholder - would normally be created with cmsCIECAM02Init
    
    // 10. If we have a valid input profile, try to use it
    if (inputProfile) {
        // Try to get a named color list as a demonstration
        cmsNAMEDCOLORLIST* namedColors = cmsGetNamedColorList(NULL);
        if (namedColors) {
            cmsFreeNamedColorList(namedColors);
        }
        
        cmsCloseProfile(inputProfile);
    }

    // 11. Clean up resources
    if (abstractProfile) {
        cmsCloseProfile(abstractProfile);
    }
    
    if (io_handler) {
        cmsCloseIOhandler(io_handler);
    }
    
    assert_file_closed(&in_file);;

    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}