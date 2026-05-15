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
#include <zlib.h>
}
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"gzrewind":[[356,36,356,61,0,0,4,0]]},"library_calls":["gzopen","inflateInit2_","gzclose","inflateSetDictionary","gzread","gzrewind","inflate","inflateGetDictionary","inflateCodesUsed","adler32","adler32","gzopen","gzprintf","gzclose","inflateEnd","gzclose"],"critical_calls":["gzopen","inflateInit2_","inflateSetDictionary","gzread","gzrewind","inflate","inflateGetDictionary","inflateCodesUsed","adler32","adler32","gzopen","gzprintf","gzclose","inflateEnd","gzclose"],"visited":0}
/**/


// Helper function to read a value from fuzzer data
template<typename T>
bool read_from_data(const uint8_t*& data, size_t& size, T& value) {
    if (size < sizeof(T)) return false;
    memcpy(&value, data, sizeof(T));
    data += sizeof(T);
    size -= sizeof(T);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    const uint8_t* original_data = data;
    size_t original_size = size;
    
    // Write compressed data to a temporary file
    FILE* temp_file = fopen("input_file", "wb");
    if (!temp_file) {
	assert_file_closed(&temp_file);
	return 0;
	}
	
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // 1. Open the compressed file for reading
    gzFile gz_in = gzopen("input_file", "rb");
    if (!gz_in) {
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 2. Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Read windowBits from fuzzer data
    int windowBits = 15; // Default
    if (size > 0) {
        windowBits = (data[0] % 31) + 8; // Valid range: 8-15
        if (windowBits < 8) windowBits = 8;
        data++;
        size--;
    }
    
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Try to set dictionary if we have enough data
    Bytef dictionary[256];
    uInt dictLength = 0;
    
    if (size > 64) { // Need at least some data for dictionary
        dictLength = (size > 256) ? 256 : size;
        memcpy(dictionary, data, dictLength);
        data += dictLength;
        size -= dictLength;
        
        // Try to set dictionary (may fail if data wasn't compressed with dictionary)
        inflateSetDictionary(&strm, dictionary, dictLength);
    }
    
    // 4. Allocate buffers for decompression
    const size_t out_buf_size = 4096;
    Bytef out_buf[out_buf_size];
    Bytef read_buf[1024];
    
    // 5. Read from gz file using gzread
    int bytes_read = 0;
    while ((bytes_read = gzread(gz_in, read_buf, sizeof(read_buf))) > 0) {
        // Process the read data if needed
    }
    
    // 6. Reset the stream position and try inflate
    gzrewind(gz_in);
    
    // Prepare input buffer with original compressed data
    strm.avail_in = original_size;
    strm.next_in = (Bytef*)original_data;
    strm.avail_out = out_buf_size;
    strm.next_out = out_buf;
    
    // 7. Perform inflation
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // 8. Try to get dictionary (even if not set)
    uInt dict_len_ret = 256;
    Bytef dict_ret[256];
    inflateGetDictionary(&strm, dict_ret, &dict_len_ret);
    
    // 9. Get inflate codes used
    unsigned long codes_used = inflateCodesUsed(&strm);
    
    // 10. Calculate Adler-32 checksum of decompressed data
    uLong adler = adler32(0L, Z_NULL, 0);
    if (strm.total_out > 0) {
        adler = adler32(adler, out_buf, strm.total_out < out_buf_size ? 
                       (uInt)strm.total_out : out_buf_size);
    }
    
    // 11. Try to write to output file using gzprintf (open for writing)
    gzFile gz_out = gzopen("output_file", "wb");
    if (gz_out) {
        gzprintf(gz_out, "Decompressed %lu bytes, Adler-32: %lu\n", 
                strm.total_out, adler);
        gzclose(gz_out);
    }
    
    // 12. Clean up
    inflateEnd(&strm);
    gzclose(gz_in);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}