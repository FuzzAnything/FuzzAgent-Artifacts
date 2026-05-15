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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"cmsDictDup":[[1175,12,1175,25,0,0,4,0],[1177,13,1177,107,0,0,4,1]],"cmsDictFree":[[1114,17,1114,45,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataRowCol","cmsIT8DefineDblFormat","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictDup","cmsBuildTabulatedToneCurveFloat","cmsfilelength","cmsIT8Free","cmsDictFree","cmsDictFree","cmsFreeToneCurve","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataRowCol","cmsIT8DefineDblFormat","cmsDictAlloc","cmsMLUalloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUsetASCII","cmsDictAddEntry","cmsMLUfree","cmsMLUfree","cmsDictDup","cmsBuildTabulatedToneCurveFloat","cmsfilelength","cmsIT8Free","cmsDictFree","cmsDictFree","cmsFreeToneCurve","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 1. Create IT8 handle and use cmsIT8SetDataRowCol
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        // Create a simple string from part of the input data
        size_t str_len = size > 50 ? 50 : size;
        char* temp_str = (char*)malloc(str_len + 1);
        if (temp_str) {
            memcpy(temp_str, data, str_len);
            temp_str[str_len] = '\0';
            
            // Set data at row 0, column 0
            cmsIT8SetDataRowCol(it8, 0, 0, temp_str);
            free(temp_str);
        }
        
        // 4. Use cmsIT8DefineDblFormat
        cmsIT8DefineDblFormat(it8, "%.6f");
    }
    
    // 2. Create a dictionary and duplicate it using cmsDictDup
    cmsHANDLE dict = cmsDictAlloc(context);
    cmsHANDLE dict_dup = nullptr;
    if (dict) {
        // Add some entries to the dictionary
        cmsMLU* display_name = cmsMLUalloc(context, 1);
        cmsMLU* display_value = cmsMLUalloc(context, 1);
        
        if (display_name && display_value) {
            cmsMLUsetASCII(display_name, "en", "US", "TestName");
            cmsMLUsetASCII(display_value, "en", "US", "TestValue");
            
            // Use part of input data for dictionary entry name/value
            size_t dict_data_len = size > 20 ? 20 : size;
            wchar_t* wname = (wchar_t*)malloc((dict_data_len + 1) * sizeof(wchar_t));
            wchar_t* wvalue = (wchar_t*)malloc((dict_data_len + 1) * sizeof(wchar_t));
            
            if (wname && wvalue) {
                // Convert input bytes to wide chars (simplified conversion)
                for (size_t i = 0; i < dict_data_len; i++) {
                    wname[i] = (wchar_t)data[i];
                    wvalue[i] = (wchar_t)data[(i + 10) % size];
                }
                wname[dict_data_len] = L'\0';
                wvalue[dict_data_len] = L'\0';
                
                cmsDictAddEntry(dict, wname, wvalue, display_name, display_value);
                
                free(wname);
                free(wvalue);
            }
            
            cmsMLUfree(display_name);
            cmsMLUfree(display_value);
        }
        
        // Duplicate the dictionary
        dict_dup = cmsDictDup(dict);
    }
    
    // 3. Create a tone curve using cmsBuildTabulatedToneCurveFloat
    cmsToneCurve* tone_curve = nullptr;
    if (size >= 4 * sizeof(cmsFloat32Number)) {
        // Use input data as float values for tone curve
        cmsFloat32Number* curve_values = (cmsFloat32Number*)malloc(size);
        if (curve_values) {
            // Copy and convert input data to floats (simplified)
            size_t num_entries = size / sizeof(cmsFloat32Number);
            if (num_entries > 256) num_entries = 256;  // Reasonable limit
            
            for (size_t i = 0; i < num_entries; i++) {
                // Create normalized float values from input bytes
                curve_values[i] = (cmsFloat32Number)data[i % size] / 255.0f;
            }
            
            tone_curve = cmsBuildTabulatedToneCurveFloat(context, (cmsUInt32Number)num_entries, curve_values);
            free(curve_values);
        }
    }
    
    // 5. Use cmsfilelength with a FILE* created from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file length
        long file_len = cmsfilelength(in_file);
        (void)file_len;  // Use the result to avoid unused variable warning
        
        assert_file_closed(&in_file);;
    }
    
    // Create output file for testing
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (it8) cmsIT8Free(it8);
    if (dict) cmsDictFree(dict);
    if (dict_dup) cmsDictFree(dict_dup);
    if (tone_curve) cmsFreeToneCurve(tone_curve);
    if (context) cmsDeleteContext(context);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}