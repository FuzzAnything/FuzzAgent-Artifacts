#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 33
//<Quality> {"density":5,"unique_branches":{"OJPEGReadBufferFill":[[2235,17,2235,23,0,0,4,0]],"TIFFCleanup":[[67,29,67,61,0,0,4,1]],"OJPEGVGetField":[[554,9,554,41,0,0,4,0]],"_WriteAsType":[[1925,14,1925,54,0,0,4,0]],"Fax3VGetField":[[1440,9,1440,34,0,0,4,0]],"TIFFWriteDirectoryTagSlong8Array":[[1790,9,1790,38,0,0,4,1],[1799,9,1799,18,0,0,4,0]],"TIFFWriteBufferSetup":[[692,9,692,31,0,0,4,1],[706,9,706,19,0,0,4,1]],"TIFFClampDoubleToUInt32":[[1519,9,1519,16,0,0,4,0],[1519,9,1519,16,0,0,4,1],[1521,9,1521,26,0,0,4,0],[1521,9,1521,26,0,0,4,1],[1521,30,1521,40,0,0,4,1]],"TIFFWriteDirectoryTagSampleformatArray":[[1582,17,1582,51,0,0,4,1],[1589,22,1589,57,0,0,4,1],[1598,29,1598,38,0,0,4,0],[1598,29,1598,38,0,0,4,1]],"DoubleToSrational":[[2976,9,2976,42,0,0,4,0]],"TIFFWriteDirectorySec":[[536,13,536,45,0,0,4,1],[805,21,808,57,0,0,4,1],[835,21,837,59,0,0,4,1],[1031,17,1031,36,0,0,4,0],[1032,25,1034,78,0,0,4,1],[1079,17,1079,33,0,0,4,0],[1080,25,1082,78,0,0,4,0],[1441,9,1441,77,0,0,4,0],[236,34,236,71,9,0,4,1],[236,34,236,71,43,0,4,1],[236,34,236,71,53,0,4,1],[236,34,236,71,63,0,4,0],[236,34,236,71,71,0,4,0]],"_TIFFCheckDirNumberAndOffset":[[5762,9,5762,20,0,0,4,0]]},"library_calls":["TIFFOpen","_TIFFmalloc","TIFFWriteBufferSetup","TIFFSwabShort","TIFFSwabFloat","TIFFRewriteDirectory","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","_TIFFmalloc","TIFFWriteBufferSetup","TIFFSwabShort","TIFFSwabFloat","TIFFRewriteDirectory","_TIFFfree","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    TIFF *tif = nullptr;
    void *buffer = nullptr;
    
    // Open TIFF file for reading and writing
    tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        // Clean up
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Setup read buffer
    buffer = _TIFFmalloc(fuzz_int64_t_2);
    if (buffer) {
        TIFFWriteBufferSetup(tif, buffer, fuzz_int64_t_3);
    }
    
    // Read and process some data
    uint16_t short_val = 0x1234;
    TIFFSwabShort(&short_val);
    
    // Prepare float array for byte swapping
    float float_array[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    // Note: TIFFSwabArrayOfFloat is not in the provided API list, so we'll use TIFFSwabFloat instead
    for (int i = 0; i < 4; i++) {
        TIFFSwabFloat(&float_array[i]);
    }
    
    // Prepare triple data (RGB) for byte swapping
    uint8_t triple_data[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // Note: TIFFSwabArrayOfTriples is not in the provided API list
    // We'll create a simple triple processing example
    if (size >= 12) {
        memcpy(triple_data, data, 12);
        // Simple byte reversal for triple data
        for (size_t i = 0; i < 12; i += 3) {
            uint8_t temp = triple_data[i];
            triple_data[i] = triple_data[i+2];
            triple_data[i+2] = temp;
        }
    }
    
    // Rewrite directory
    int rewrite_result = TIFFRewriteDirectory(tif);
    
    // Clean up
    if (buffer) {
        _TIFFfree(buffer);
    }
    
    TIFFClose(tif);
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}