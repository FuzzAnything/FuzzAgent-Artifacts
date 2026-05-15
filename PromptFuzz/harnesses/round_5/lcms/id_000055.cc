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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsfilelength","cmsIT8Alloc","cmsDeleteContext","cmsIT8DefineDblFormat","cmsIT8SetDataRowCol","cmsBuildTabulatedToneCurveFloat","cmsDictAlloc","cmsDictDup","cmsMLUalloc","cmsMLUsetASCII","cmsDictAddEntry","cmsMLUfree","cmsDictFree","cmsDictFree","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsfilelength","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8SetDataRowCol","cmsBuildTabulatedToneCurveFloat","cmsDictAlloc","cmsDictDup","cmsMLUalloc","cmsMLUsetASCII","cmsDictAddEntry","cmsMLUfree","cmsDictFree","cmsDictFree","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need minimum data for operations

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
	return 0;
	}
	

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use cmsfilelength to get file length
    long file_len = cmsfilelength(in_file);
    
    // 2. Create an IT8 handle (simulated - in real scenario would come from parsing)
    // We'll create a simple IT8 structure for demonstration
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (!hIT8) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Define double format for IT8
    cmsIT8DefineDblFormat(hIT8, "%.6f");

    // 4. Set data in IT8 table (use first few bytes as string data)
    // Convert first min(100, size) bytes to null-terminated string
    size_t str_len = size < 100 ? size : 100;
    char *str_val = new char[str_len + 1];
    memcpy(str_val, data, str_len);
    str_val[str_len] = '\0';
    
    // Set data in row 0, column 0
    cmsIT8SetDataRowCol(hIT8, 0, 0, str_val);
    delete[] str_val;

    // 5. Create a tone curve from input data (convert bytes to floats)
    // Use at least 2 entries for tone curve
    size_t num_entries = size / sizeof(float);
    if (num_entries < 2) num_entries = 2;
    
    // Allocate and populate float array
    cmsFloat32Number *curve_values = new cmsFloat32Number[num_entries];
    for (size_t i = 0; i < num_entries && i * sizeof(float) < size; i++) {
        // Convert bytes to float (simplified - just for fuzzing)
        float val = 0.0f;
        size_t bytes_to_copy = (size - i * sizeof(float)) >= sizeof(float) ? 
                              sizeof(float) : size - i * sizeof(float);
        memcpy(&val, data + i * sizeof(float), bytes_to_copy);
        curve_values[i] = val;
    }
    
    cmsToneCurve *tone_curve = cmsBuildTabulatedToneCurveFloat(context, 
                                                              (cmsUInt32Number)num_entries, 
                                                              curve_values);
    delete[] curve_values;

    // 6. Create and duplicate a dictionary
    cmsHANDLE hDict = cmsDictAlloc(context);
    if (hDict) {
        cmsHANDLE hDictDup = cmsDictDup(hDict);
        if (hDictDup) {
            // Add a simple entry to the dictionary
            cmsMLU *mlu_display = cmsMLUalloc(context, 1);
            if (mlu_display) {
                cmsMLUsetASCII(mlu_display, "en", "US", "Test Value");
                cmsDictAddEntry(hDictDup, L"TestKey", L"TestValue", mlu_display, NULL);
                cmsMLUfree(mlu_display);
            }
            cmsDictFree(hDictDup);
        }
        cmsDictFree(hDict);
    }

    // Clean up
    if (tone_curve) {
        cmsFreeToneCurve(tone_curve);
    }
    
    cmsIT8Free(hIT8);
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}