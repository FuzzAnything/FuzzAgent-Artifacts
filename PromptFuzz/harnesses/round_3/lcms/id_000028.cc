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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 220, nr_unique_branch: 25
//<Quality> {"density":20,"unique_branches":{"Clamp_L_doubleV2":[[238,9,238,14,0,0,4,0],[238,9,238,14,0,0,4,1],[239,9,239,18,0,0,4,0],[239,9,239,18,0,0,4,1]],"NonlinearCompression":[[154,17,154,22,0,0,4,0],[154,17,154,22,0,0,4,1],[155,13,155,28,0,0,4,1]],"cmsCIECAM02Done":[[436,9,436,14,0,0,4,0]],"cmsIT8GetDataRowColDbl":[[2839,9,2839,23,0,0,4,0]],"SetData":[[1788,9,1788,30,0,0,4,0]],"ChromaticAdaptation":[[116,17,116,22,0,0,4,0],[116,17,116,22,0,0,4,1]],"cmsGetTagCount":[[643,9,643,20,0,0,4,1]],"cmsCIECAM02Init":[[369,8,369,87,0,0,4,1],[387,5,387,27,0,0,4,1],[393,5,393,23,0,0,4,1],[399,5,399,22,0,0,4,1],[405,5,405,12,0,0,4,0],[417,9,417,34,0,0,4,1]],"GetData":[[1762,9,1762,17,0,0,4,1],[1762,21,1762,37,0,0,4,0]],"Clamp_ab_doubleV2":[[248,9,248,32,0,0,4,0],[248,9,248,32,0,0,4,1],[249,9,249,32,0,0,4,0],[249,9,249,32,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetTagCount","cmsGetTagSignature","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext","cmsCIECAM02Init","cmsIT8Alloc","cmsIT8SetTable","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8SetDataDbl","cmsIT8SetDataRowColDbl","cmsIT8GetDataRowColDbl","cmsFloat2LabEncodedV2","cmsGetProfileIOhandler","cmsSaveProfileToIOhandler","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetTagCount","cmsGetTagSignature","cmsBuildGamma","cmsCreateRGBProfile","cmsFreeToneCurve","cmsCIECAM02Init","cmsIT8Alloc","cmsIT8SetTable","cmsIT8SetPropertyStr","cmsIT8SetPropertyStr","cmsIT8SetDataDbl","cmsIT8SetDataRowColDbl","cmsIT8GetDataRowColDbl","cmsFloat2LabEncodedV2","cmsGetProfileIOhandler","cmsSaveProfileToIOhandler","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to create an event that reads color profile data from a file, processes it with CIECAM02 color appearance modeling, manipulates IT8 color data, and saves the processed profile back to a file:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open the input file as a color profile
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get tag signature from the profile
    cmsUInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        cmsTagSignature tagSig = cmsGetTagSignature(hProfile, 0);
    }
    
    // Create an RGB profile with default parameters
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};  // D65 white point
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},    // Red
        {0.30, 0.60, 1.0},    // Green
        {0.15, 0.06, 1.0}     // Blue
    };
    
    // Create gamma curves for RGB channels
    cmsToneCurve* transferFunction[3];
    for (int i = 0; i < 3; i++) {
        transferFunction[i] = cmsBuildGamma(context, 2.2);
    }
    
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunction);
    
    // Clean up gamma curves
    for (int i = 0; i < 3; i++) {
        cmsFreeToneCurve(transferFunction[i]);
    }
    
    if (!rgbProfile) {
        cmsCloseProfile(hProfile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize CIECAM02 color appearance model
    cmsViewingConditions vc = {
        {0.95047, 1.00000, 1.08883},  // D65 white point in XYZ
        20.0,     // Yb
        100.0,    // La
        1,        // Average surround
        1.0       // D_value
    };
    
    cmsHANDLE hCAM02 = cmsCIECAM02Init(context, &vc);
    
    // Create an IT8 handle for color data management
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8) {
        // Set up a simple IT8 table
        cmsIT8SetTable(hIT8, 0);
        cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer");
        cmsIT8SetPropertyStr(hIT8, "DESCRIPTOR", "Test Color Data");
        
        // Add some data to the IT8 table
        // Create a 2x2 table with sample data
        const char* samples[] = {"R", "G", "B", "X", "Y", "Z"};
        
        // Set data using double values from input data
        // We'll use the first few bytes as double values
        for (int row = 0; row < 2 && (row * 3 + 2) * sizeof(double) < size; row++) {
            for (int col = 0; col < 3 && (row * 3 + col) * sizeof(double) < size; col++) {
                double val = 0.0;
                // Extract double from input data (simplified - assumes proper alignment)
                if ((row * 3 + col + 1) * sizeof(double) <= size) {
                    memcpy(&val, data + (row * 3 + col) * sizeof(double), sizeof(double));
                }
                
                // Set data in IT8 table
                char patchName[32];
                snprintf(patchName, sizeof(patchName), "Patch_%d", row * 3 + col + 1);
                cmsIT8SetDataDbl(hIT8, patchName, samples[col], val);
                
                // Also set in row/col format
                cmsIT8SetDataRowColDbl(hIT8, row, col, val);
                
                // Retrieve the data we just set
                double retrieved = cmsIT8GetDataRowColDbl(hIT8, row, col);
            }
        }
    }
    
    // Convert a Lab value using Float2LabEncodedV2
    if (size >= sizeof(cmsCIELab)) {
        cmsCIELab lab;
        memcpy(&lab, data, sizeof(cmsCIELab));
        
        cmsUInt16Number wLab[3];
        cmsFloat2LabEncodedV2(wLab, &lab);
    }
    
    // Create output file for saving profile
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create I/O handler for the output file
        cmsIOHANDLER* io = cmsGetProfileIOhandler(rgbProfile);
        if (io) {
            // Save the RGB profile to the I/O handler (which writes to our file)
            cmsUInt32Number savedSize = cmsSaveProfileToIOhandler(rgbProfile, io);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (hCAM02) {
        cmsCIECAM02Done(hCAM02);
    }
    
    cmsCloseProfile(rgbProfile);
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    // Remove the output file if it was created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}