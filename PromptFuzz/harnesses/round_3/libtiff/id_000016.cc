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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"TIFFErrorExtR":[[119,9,119,26,0,0,4,1]]},"library_calls":["TIFFSetErrorHandler","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFIsMSB2LSB","TIFFForceStrileArrayWriting","TIFFFieldWithName","TIFFFieldWriteCount","TIFFRawStripSize","TIFFClose","TIFFOpenOptionsFree","TIFFSetErrorHandler"],"critical_calls":["TIFFSetErrorHandler","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFIsMSB2LSB","TIFFForceStrileArrayWriting","TIFFFieldWithName","TIFFFieldWriteCount","TIFFRawStripSize","TIFFClose","TIFFOpenOptionsFree","TIFFSetErrorHandler"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
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
    
    // Open the TIFF file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set custom error handler
    TIFFErrorHandler oldHandler = TIFFSetErrorHandler(nullptr);
    
    // Open TIFF file using file descriptor
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    TIFF* tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2, 
                                  (thandle_t)(intptr_t)fd,
                                  nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr,
                                  opts);
    
    if (tif) {
        // Check if file uses MSB to LSB byte order
        int isMSB2LSB = TIFFIsMSB2LSB(tif);
        
        // Force writing of strile array (even though we're reading)
        // This might fail on read-only file, but we call it anyway
        TIFFForceStrileArrayWriting(tif);
        
        // Look up a field by name
        const TIFFField* field = TIFFFieldWithName(tif, fuzz_str_3);
        if (field) {
            // Get write count for the field
            int writeCount = TIFFFieldWriteCount(field);
        }
        
        // Get raw strip size for first strip
        uint32_t strip = 0;
        tmsize_t rawStripSize = TIFFRawStripSize(tif, fuzz_uint32_t_4);
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Free open options
    TIFFOpenOptionsFree(opts);
    
    // Restore original error handler
    TIFFSetErrorHandler(oldHandler);
    
    // Close file
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}