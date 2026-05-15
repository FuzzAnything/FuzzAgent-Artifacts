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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 18
//<Quality> {"density":19,"unique_branches":{"ParseFloatNumber":[[721,9,721,23,0,0,4,0],[723,16,723,32,0,0,4,0],[728,12,728,19,0,0,4,1],[734,9,734,23,0,0,4,1],[752,9,752,16,0,0,4,0],[752,20,752,43,0,0,4,0],[752,20,752,43,0,0,4,1],[757,13,757,20,0,0,4,0],[760,13,760,27,0,0,4,0],[760,13,760,27,0,0,4,1],[763,17,763,24,0,0,4,0],[766,17,766,31,0,0,4,0],[769,21,769,28,0,0,4,0],[773,16,773,23,0,0,4,0],[773,16,773,23,0,0,4,1],[777,17,777,85,0,0,4,0],[780,17,780,24,0,0,4,0]],"SetData":[[1788,9,1788,30,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsIT8Alloc","cmsCloseProfile","cmsDeleteContext","cmsIT8SetIndexColumn","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8GetDataDbl","cmsGetProfileInfoUTF8","cmsGetProfileInfoUTF8","cmsCreateTransform","cmsDoTransformLineStride","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsIT8SetPropertyDbl","cmsIT8GetPropertyDbl","cmsIT8SetDataRowCol","cmsIT8GetDataDbl","cmsGetProfileInfoUTF8","cmsGetProfileInfoUTF8","cmsCreateTransform","cmsDoTransformLineStride","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/

extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Split data into two parts: for profile and for IT8
    size_t profile_size = size / 2;
    size_t it8_size = size - profile_size;
    
    // Create a profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, profile_size);
    if (!profile) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (!it8) {
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Set some IT8 properties using data
    char prop_name[32] = "TestProperty";
    char sample_name[32] = "TestSample";
    char patch_name[32] = "Patch1";
    
    // Set index column
    cmsIT8SetIndexColumn(it8, sample_name);
    
    // Set property as double (using data bytes as value)
    double prop_value = *reinterpret_cast<const double*>(data + profile_size);
    cmsIT8SetPropertyDbl(it8, prop_name, prop_value);
    
    // Get property as double
    double retrieved_prop = cmsIT8GetPropertyDbl(it8, prop_name);
    
    // Set data row (simulate some data)
    int row = 0;
    int col = 0;
    for (size_t i = 0; i < it8_size && i < 20; i++) {
        char val[3];
        snprintf(val, sizeof(val), "%d", data[profile_size + i] % 100);
        cmsIT8SetDataRowCol(it8, row, col, val);
        col++;
        if (col > 5) {
            col = 0;
            row++;
        }
    }
    
    // Get data as double
    double data_value = cmsIT8GetDataDbl(it8, patch_name, sample_name);
    
    // Get MLU from profile info and duplicate it
    cmsMLU* mlu = NULL;
    char buffer[256];
    cmsUInt32Number buffer_size = sizeof(buffer);
    
    // Try to get profile info
    cmsGetProfileInfoUTF8(profile, cmsInfoDescription, "en", "US", buffer, buffer_size);
    
    // Note: cmsMLUdup is not in the provided API list, so we cannot use it
    // We'll use another available MLU function instead
    if (cmsGetProfileInfoUTF8(profile, cmsInfoDescription, "en", "US", buffer, buffer_size) > 0) {
        // We have the info in buffer
    }
    
    // Create a simple transformation
    cmsHTRANSFORM transform = cmsCreateTransform(
        profile, TYPE_RGB_8,
        profile, TYPE_RGB_8,
        INTENT_PERCEPTUAL, 0
    );
    
    if (transform) {
        // Prepare input/output buffers
        unsigned char input_buffer[9];  // 3 pixels
        unsigned char output_buffer[9];
        
        // Fill input buffer with data
        for (int i = 0; i < 9 && i < (int)size; i++) {
            input_buffer[i] = data[i];
        }
        
        // Perform transformation with stride
        cmsDoTransformLineStride(
            transform,
            input_buffer,
            output_buffer,
            3,      // PixelsPerLine (3 pixels)
            1,      // LineCount
            3,      // BytesPerLineIn (3 bytes per pixel for RGB)
            3,      // BytesPerLineOut
            0,      // BytesPerPlaneIn (planar not used)
            0       // BytesPerPlaneOut
        );
        
        // Clean up transform
        cmsDeleteTransform(transform);
    }
    
    // Clean up
    cmsIT8Free(it8);
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    return 0;
}