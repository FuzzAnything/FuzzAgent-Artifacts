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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"deflateBound_z":[[917,28,917,49,0,0,4,0]]},"library_calls":["gzdopen","gzseek","gzread","gzclose","deflateInit2_","deflateBound_z","deflateEnd","gzclose","uncompress2","inflateInit2_","inflatePrime","inflateGetHeader","inflateGetDictionary","deflateInit2_","deflatePrime","deflateEnd","inflateEnd","crc32","crc32","crc32_combine","gzclose"],"critical_calls":["gzdopen","gzseek","gzread","deflateInit2_","deflateBound_z","deflateEnd","uncompress2","inflateInit2_","inflatePrime","inflateGetHeader","inflateGetDictionary","deflateInit2_","deflatePrime","deflateEnd","inflateEnd","crc32","crc32","crc32_combine","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Create a FILE* to read the input data (compressed zlib data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create a gzFile from the file descriptor for seeking
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Use gzseek to navigate within the compressed stream
    // Seek to middle of input (if large enough) to test partial reads
    if (size > 10) {
        off_t offset = size / 2;
        if (gzseek(gz_file, offset, fuzz_int32_t_2) != -1) {
            // Read some data after seeking
            Bytef buffer[100];
            gzread(gz_file, buffer, sizeof(buffer));
        }
    }

    // 4. Initialize zlib structures for inflation
    z_stream strm;
    Bytef *compressed_data = nullptr;
    uLong compressed_size = 0;
    uLong uncompressed_size = 0;
    Bytef *uncompressed_data = nullptr;
    int ret = Z_OK;

    // 5. Allocate and set up the source data for uncompress2
    if (size > 0) {
        compressed_data = (Bytef *)malloc(size);
        if (!compressed_data) {
            gzclose(gz_file);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }
        memcpy(compressed_data, data, size);
        compressed_size = (uLong)size;
        
        // 6. Get upper bound for decompressed size using deflateBound_z
        // Note: deflateBound_z is for compression bounds, but we can use it
        // with a dummy stream to estimate decompression size
        z_stream dummy_strm;
        memset(&dummy_strm, 0, sizeof(dummy_strm));
        deflateInit2(&dummy_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
        z_size_t bound = deflateBound_z(&dummy_strm, (z_size_t)size);
        deflateEnd(&dummy_strm);
        
        // 7. Allocate destination buffer for uncompress2
        uncompressed_size = (uLong)(bound > 0 ? bound : size * 2);
        uncompressed_data = (Bytef *)malloc(uncompressed_size);
        if (!uncompressed_data) {
            free(compressed_data);
            gzclose(gz_file);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }

        // 8. Use uncompress2 to decompress the data
        uLong source_len = compressed_size;
        ret = uncompress2(uncompressed_data, &uncompressed_size, 
                         compressed_data, &source_len);
    }

    // 9. Set up inflation stream for advanced operations
    memset(&strm, 0, sizeof(strm));
    if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
        // 10. Use inflatePrime to insert bits into the bit buffer
        if (size > 2) {
            inflatePrime(&strm, fuzz_int32_t_3, data[0]);  // Insert first byte as bits
        }

        // 11. Use inflateGetHeader to retrieve gzip header
        gz_header header;
        memset(&header, 0, sizeof(header));
        inflateGetHeader(&strm, &header);

        // 12. Use inflateGetDictionary to retrieve dictionary
        Bytef dictionary[32768];
        uInt dict_length = sizeof(dictionary);
        inflateGetDictionary(&strm, dictionary, &dict_length);

        // 13. Set up a deflation stream for deflatePrime
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) == Z_OK) {
            // 14. Use deflatePrime to insert bits
            if (size > 1) {
                deflatePrime(&def_strm, fuzz_int32_t_4, data[1]);  // Insert second byte as bits
            }
            deflateEnd(&def_strm);
        }

        inflateEnd(&strm);
    }

    // 15. Use crc32_combine to combine CRC32 values
    if (size > 20) {
        // Calculate CRC32 for first and second halves
        uLong crc1 = crc32(fuzz_uint64_t_5, data, 10);
        uLong crc2 = crc32(fuzz_uint64_t_6, data + 10, size - 10);
        off_t len2 = size - 10;
        uLong combined_crc = crc32_combine(crc1, crc2, len2);
        (void)combined_crc;  // Use variable to avoid unused warning
    }

    // 16. Clean up resources
    if (compressed_data) free(compressed_data);
    if (uncompressed_data) free(uncompressed_data);
    
    // 17. Close gzFile and FILE*
    if (gz_file) gzclose(gz_file);
    if (in_file) assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}