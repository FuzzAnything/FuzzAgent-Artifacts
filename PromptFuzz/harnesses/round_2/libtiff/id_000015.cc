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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"TIFFClientOpenExt":[[346,9,346,51,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFReadCustomDirectory","TIFFNumberOfDirectories","TIFFUnlinkDirectory","uv_encode","LogLuv32fromXYZ","TIFFWriteCustomDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFReadCustomDirectory","TIFFNumberOfDirectories","TIFFUnlinkDirectory","uv_encode","LogLuv32fromXYZ","TIFFWriteCustomDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to achieve the event of reading, processing, and writing TIFF data:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from fuzzer data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the input file for reading
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 100 * 1024 * 1024);
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 500 * 1024 * 1024);
	int fuzz_fd_0 = fuzz_fileno(fopen("input_file", "rb"));

    
    TIFF *tif = TIFFClientOpenExt("input_file", "rm", 
                                  (thandle_t)fuzz_fd_0,
                                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Process TIFF file using the specified APIs
    
    // 1. TIFFReadCustomDirectory - read custom directory at offset 0
    toff_t diroff = 0;
    int read_custom_result = TIFFReadCustomDirectory(tif, diroff, NULL);
    
    // 2. TIFFUnlinkDirectory - try to unlink directory if exists
    if (TIFFNumberOfDirectories(tif) > 0) {
        int unlink_result = TIFFUnlinkDirectory(tif, 0);
    }
    
    // 3. uv_encode - encode UV values
    double u = 0.2, v = 0.3;
    int uv_result = uv_encode(u, v, 0);
    
    // 4. LogLuv32fromXYZ - convert XYZ to LogLuv32
    float xyz[3] = {0.5f, 0.5f, 0.5f};
    uint32_t logluv_value = LogLuv32fromXYZ(xyz, 0);
    
    // 5. TIFFWriteCustomDirectory - write custom directory
    uint64_t custom_dir_offset = 0;
    int write_custom_result = TIFFWriteCustomDirectory(tif, &custom_dir_offset);
    
    // Create output file for writing processed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data to output file
        uint32_t processed_data[4] = {
            static_cast<uint32_t>(read_custom_result),
            static_cast<uint32_t>(uv_result),
            logluv_value,
            static_cast<uint32_t>(write_custom_result)
        };
        fwrite(processed_data, sizeof(uint32_t), 4, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	return 0;
}