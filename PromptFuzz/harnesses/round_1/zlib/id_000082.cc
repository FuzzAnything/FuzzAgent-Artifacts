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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"inflate":[[709,17,709,33,0,0,4,0]]},"library_calls":["zlibCompileFlags","compressBound","compress","uncompress2","inflateInit2_","inflateGetHeader","inflate","inflateMark","inflateCodesUsed","inflateEnd","deflateInit2_","deflateTune","deflate","deflateUsed","deflateEnd","gzdopen","gzclose"],"critical_calls":["zlibCompileFlags","compressBound","compress","uncompress2","inflateInit2_","inflateGetHeader","inflate","inflateMark","inflateCodesUsed","inflateEnd","deflateInit2_","deflateTune","deflate","deflateUsed","deflateEnd","gzdopen","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need sufficient data for meaningful testing
    
    // First, let's get zlib compilation flags
    uLong compileFlags = zlibCompileFlags();
    
    // We'll process the input data through a complete compress-decompress cycle
    // using compress/uncompress2 APIs
    
    // Step 1: Compress the input data using compress()
    uLongf compressedSize = compressBound(static_cast<uLong>(size));
    Bytef* compressedData = static_cast<Bytef*>(malloc(compressedSize));
    if (!compressedData) {
	return 0;
	}
	
    
    int compressResult = compress(compressedData, &compressedSize, 
                                  reinterpret_cast<const Bytef*>(data), 
                                  static_cast<uLong>(size));
    
    if (compressResult != Z_OK) {
        free(compressedData);
        return 0;
    }
    
    // Step 2: Create a temporary file for gzgetc testing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(compressedData);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Write compressed data to file for gz operations
    fwrite(compressedData, 1, compressedSize, out_file);
    assert_file_closed(&out_file);;
    
    // Step 3: Test uncompress2 with the compressed data
    uLongf uncompressedSize = static_cast<uLong>(size * 2);  // Allocate extra space
    Bytef* uncompressedData = static_cast<Bytef*>(malloc(uncompressedSize));
    if (!uncompressedData) {
        free(compressedData);
        assert_file_closed(&out_file);
	return 0;
    }
    
    uLong sourceLen = compressedSize;
    int uncompressResult = uncompress2(uncompressedData, &uncompressedSize,
                                       compressedData, &sourceLen);
    
    // Step 4: Set up inflation stream for header reading
    z_stream inflateStream;
    memset(&inflateStream, 0, sizeof(inflateStream));
    
    if (inflateInit2(&inflateStream, MAX_WBITS) != Z_OK) {
        free(uncompressedData);
        free(compressedData);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Prepare for inflation with header reading
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inflateStream, &header);
    
    // Feed compressed data to inflation stream
    inflateStream.next_in = compressedData;
    inflateStream.avail_in = static_cast<uInt>(compressedSize);
    
    Bytef inflateBuffer[1024];
    inflateStream.next_out = inflateBuffer;
    inflateStream.avail_out = sizeof(inflateBuffer);
    
    // Inflate a bit to get header info
    int inflateResult = inflate(&inflateStream, Z_BLOCK);
    
    // Mark current position in inflation stream
    long markPosition = inflateMark(&inflateStream);
    
    // Get inflation codes used
    unsigned long codesUsed = inflateCodesUsed(&inflateStream);
    
    // Clean up inflation stream
    inflateEnd(&inflateStream);
    
    // Step 5: Set up deflation stream with tuning
    z_stream deflateStream;
    memset(&deflateStream, 0, sizeof(deflateStream));
    
    if (deflateInit2(&deflateStream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        free(uncompressedData);
        free(compressedData);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Tune deflation parameters using random values from input data
    deflateTune(&deflateStream, 
                data[0] % 256,      // good_length
                data[1] % 256,      // max_lazy  
                data[2] % 256,      // nice_length
                data[3] % 256);     // max_chain
    
    // Feed uncompressed data to deflation stream
    deflateStream.next_in = uncompressedData;
    deflateStream.avail_in = static_cast<uInt>(uncompressedSize);
    
    Bytef deflateBuffer[1024];
    deflateStream.next_out = deflateBuffer;
    deflateStream.avail_out = sizeof(deflateBuffer);
    
    // Deflate some data
    int deflateResult = deflate(&deflateStream, Z_FINISH);
    
    // Check used bits
    int usedBits = 0;
    deflateUsed(&deflateStream, &usedBits);
    
    // Clean up deflation stream
    deflateEnd(&deflateStream);
    
    // Step 6: Test gzgetc with the output file
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        if (gz) {
            // Read a few bytes using gzgetc
            for (int i = 0; i < 10 && i < size; i++) {
                int ch = gzgetc(gz);
                if (ch == -1) break;
            }
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }
    
    // Clean up all allocated resources
    free(uncompressedData);
    free(compressedData);
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}