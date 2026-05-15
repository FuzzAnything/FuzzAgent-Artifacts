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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 88, nr_unique_branch: 18
//<Quality> {"density":11,"unique_branches":{"TIFFWriteDirectoryTagLongLong8Array":[[2002,9,2002,18,0,0,4,1],[2006,14,2006,23,0,0,4,1]],"TIFFNumberOfDirectories":[[2029,9,2029,41,0,0,4,0],[2035,12,2035,27,0,0,4,1]],"TIFFClientOpenExt":[[516,21,516,34,0,0,4,1]],"TIFFWarningExt":[[66,9,66,28,0,0,4,0],[72,9,72,31,0,0,4,1]],"TIFFCheckpointDirectory":[[296,9,296,46,0,0,4,0]],"TIFFSwabArrayOfFloat":[[186,12,186,19,0,0,4,0],[186,12,186,19,0,0,4,1]],"TIFFWriteDirectorySec":[[507,9,507,18,0,0,4,1],[723,21,723,29,0,0,4,0],[725,25,728,62,0,0,4,1],[750,21,750,29,0,0,4,0],[763,25,763,62,0,0,4,0],[764,25,767,59,0,0,4,1],[1437,9,1437,18,0,0,4,1],[1449,9,1449,18,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFGetReadProc","_TIFFmalloc","_TIFFmemset","TIFFClientdata","TIFFRGBAImageOK","TIFFNumberOfDirectories","TIFFSetSubDirectory","TIFFVTileSize","TIFFSwabArrayOfFloat","TIFFCheckpointDirectory","TIFFWarningExt","TIFFClientdata","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetReadProc","_TIFFmalloc","_TIFFmemset","TIFFClientdata","TIFFRGBAImageOK","TIFFNumberOfDirectories","TIFFSetSubDirectory","TIFFVTileSize","TIFFSwabArrayOfFloat","TIFFCheckpointDirectory","TIFFWarningExt","TIFFClientdata","_TIFFfree","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<82) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from the fuzzer data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a string stream to use with TIFFStreamOpen
    std::ostringstream oss;
    
    // Open TIFF using stream interface
    TIFF *tif = TIFFStreamOpen(fuzz_str_1, &oss);
    if (!tif) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the read procedure
    TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
    
    // Create a buffer and initialize with _TIFFmemset
    const size_t buffer_size = 1024;
    void *buffer = _TIFFmalloc(buffer_size);
    if (buffer) {
        _TIFFmemset(buffer, fuzz_int32_t_2, buffer_size);
        
        // Try to read some data using the read procedure
        if (read_proc) {
            // Simulate reading from the input file
            fseek(in_file, 0, SEEK_SET);
            size_t bytes_read = fread(buffer, 1, buffer_size, in_file);
            if (bytes_read > 0) {
                // Use the read procedure (though it expects thandle_t)
                // We'll just demonstrate the call
                thandle_t handle = TIFFClientdata(tif);
                // Note: We can't actually use read_proc without proper handle setup
            }
        }
        
        // Check if RGBA image is OK
        char errmsg[1024];
        int rgba_ok = TIFFRGBAImageOK(tif, errmsg);
        
        // Try to set subdirectory (if multiple directories exist)
        if (TIFFNumberOfDirectories(tif) > 1) {
            // Try to set to first subdirectory
            TIFFSetSubDirectory(tif, fuzz_uint64_t_3);
        }
        
        // Calculate vertical tile size for 1 row
        uint32_t nrows = 1;
        tmsize_t vtile_size = TIFFVTileSize(tif, fuzz_uint32_t_4);
        
        // Create a float array for byte swapping demonstration
        std::vector<float> float_array(10);
        for (size_t i = 0; i < float_array.size(); i++) {
            float_array[i] = static_cast<float>(i);
        }
        
        // Perform byte swapping on float array
        TIFFSwabArrayOfFloat(float_array.data(), float_array.size());
        
        // Try to checkpoint directory
        TIFFCheckpointDirectory(tif);
        
        // Issue a warning (simulated condition)
        if (size < 100) {
            TIFFWarningExt(TIFFClientdata(tif), fuzz_str_5, 
                          fuzz_str_6);
        }
        
        _TIFFfree(buffer);
    }
    
    // Clean up
    TIFFClose(tif);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}