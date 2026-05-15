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
#include <turbojpeg.h>
}
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"tj3Compress8":[[87,7,87,35,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3LoadImage8","tj3Destroy","TJBUFSIZE","tj3Free","tj3Destroy","tj3Compress8","tj3Free","tj3Destroy","tjInitTransform","tjFree","tj3Free","tj3Destroy","tjFree","tj3Free","tjDestroy","tj3Destroy","tjDecompress2","tjDestroy","tjFree","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage8","TJBUFSIZE","tj3Compress8","tjInitTransform","tjDecompress2","tjDestroy","tjFree","tj3Free","tj3Destroy"],"visited":1}
/*Here's a C++ fuzzing driver that uses all the specified libjpeg-turbo APIs to achieve a complete JPEG compression/decompression/transformation cycle:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;  // Too small for meaningful JPEG data
    }

    // Step 1: Write input data to a temporary file
    const char* input_filename = "input_file";
    FILE* in_file = fopen(input_filename, "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Step 2: Load the image using tj3LoadImage8
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove(input_filename);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    int width, height, pixelFormat;
    unsigned char* srcBuf = tj3LoadImage8(handle, input_filename, &width, fuzz_int32_t_1, &height, &pixelFormat);
    if (!srcBuf) {
        remove(input_filename);
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Allocate JPEG buffer using TJBUFSIZE
    unsigned long jpegBufSize = TJBUFSIZE(width, height);
    unsigned char* jpegBuf = (unsigned char*)malloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Free(srcBuf);
        remove(input_filename);
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 4: Compress the image using tj3Compress8
    size_t compressedSize;
    unsigned char* compressedBuf = nullptr;
    int pitch = width * tjPixelSize[pixelFormat];
    
    int compressResult = tj3Compress8(handle, srcBuf, width, pitch, height, pixelFormat, 
                                       &compressedBuf, &compressedSize);
    if (compressResult != 0 || !compressedBuf) {
        free(jpegBuf);
        tj3Free(srcBuf);
        remove(input_filename);
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 5: Create transform handle
    tjhandle transHandle = tjInitTransform();
    if (!transHandle) {
        free(jpegBuf);
        tjFree(compressedBuf);
        tj3Free(srcBuf);
        remove(input_filename);
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 6: Allocate destination buffer for decompression
    unsigned char* dstBuf = (unsigned char*)malloc(width * height * tjPixelSize[pixelFormat]);
    if (!dstBuf) {
        free(jpegBuf);
        tjFree(compressedBuf);
        tj3Free(srcBuf);
        remove(input_filename);
        tjDestroy(transHandle);
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 7: Decompress using tjDecompress2
    int decompressResult = tjDecompress2(transHandle, compressedBuf, compressedSize,
                                         dstBuf, width, pitch, height, pixelFormat, fuzz_int32_t_2);

    // Clean up all allocated resources
    free(dstBuf);
    tjDestroy(transHandle);
    free(jpegBuf);
    tjFree(compressedBuf);
    tj3Free(srcBuf);
    remove(input_filename);
    tj3Destroy(handle);

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}