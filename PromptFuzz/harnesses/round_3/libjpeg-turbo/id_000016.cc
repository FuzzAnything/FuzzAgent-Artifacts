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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 13
//<Quality> {"density":23,"unique_branches":{"tj3DecompressToYUVPlanes8":[[2274,23,2274,36,0,0,4,0]],"tj3Decompress8":[[202,28,202,41,0,0,4,0],[227,7,227,17,0,0,4,1]],"start_pass":[[157,7,157,22,0,0,4,1],[168,7,168,22,0,0,4,0],[170,13,170,40,0,0,4,0],[248,5,248,20,0,0,4,1],[261,5,261,20,0,0,4,0],[285,21,285,33,0,0,4,0],[285,21,285,33,0,0,4,1]],"jsimd_set_idct_ifast":[[1230,7,1230,19,0,0,4,1],[1234,7,1234,49,0,0,4,0],[45,30,45,50,10,0,4,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjDestroy","tjDestroy","tjLoadImage","tjFree","tjDestroy","tjDestroy","tjCompress2","tjFree","tjFree","tjDestroy","tjDestroy","tjDecompress2","tjBufSizeYUV","tjDecompressToYUV","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjDecompressToYUVPlanes","tj3InitVersion","tj3GetErrorStr","tj3Destroy","tj3Free","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tjLoadImage","tjCompress2","tjDecompress2","tjBufSizeYUV","tjDecompressToYUV","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjDecompressToYUVPlanes","tj3InitVersion","tj3GetErrorStr","tj3Destroy","tj3Free","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":4}
/**/


// Include libjpeg-turbo headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;  // Need enough data for meaningful JPEG operations
    }

    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Initialize TurboJPEG handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Load image from file using tjLoadImage
    int width, height, pixelFormat;
    unsigned char *srcBuf = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    if (!srcBuf || width <= 0 || height <= 0) {
        if (srcBuf) tjFree(srcBuf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Calculate pitch
    int pitch = width * tjPixelSize[pixelFormat];

    // Compress the loaded image using tjCompress2
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int jpegSubsamp = TJSAMP_420;
    int jpegQual = 85;
    int flags = TJFLAG_FASTDCT;

    int result = tjCompress2(compress_handle, srcBuf, width, pitch, height,
                             pixelFormat, &jpegBuf, &jpegSize, jpegSubsamp,
                             fuzz_int32_t_3, fuzz_int32_t_4);
    if (result != 0 || !jpegBuf || jpegSize == 0) {
        tjFree(srcBuf);
        if (jpegBuf) tjFree(jpegBuf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Decompress the JPEG using tjDecompress2
    unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
    if (dstBuf) {
        result = tjDecompress2(decompress_handle, jpegBuf, jpegSize,
                               dstBuf, width, pitch, height,
                               pixelFormat, fuzz_int32_t_5);
        // Ignore result for fuzzing purposes
        free(dstBuf);
    }

    // Decompress to YUV using tjDecompressToYUV
    unsigned long yuvBufSize = tjBufSizeYUV(width, height, jpegSubsamp);
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
    if (yuvBuf) {
        result = tjDecompressToYUV(decompress_handle, jpegBuf, jpegSize,
                                   yuvBuf, fuzz_int32_t_6);
        free(yuvBuf);
    }

    // Decompress to YUV planes using tjDecompressToYUVPlanes
    int strides[3];
    unsigned char *yuvPlanes[3];
    
    // Calculate plane sizes and allocate
    strides[0] = tjPlaneWidth(fuzz_int32_t_7, width, jpegSubsamp);
    strides[1] = tjPlaneWidth(fuzz_int32_t_8, width, jpegSubsamp);
    strides[2] = tjPlaneWidth(fuzz_int32_t_9, width, jpegSubsamp);
    
    for (int i = 0; i < 3; i++) {
        int planeHeight = tjPlaneHeight(fuzz_int32_t_10, height, jpegSubsamp);
        yuvPlanes[i] = (unsigned char *)malloc(strides[i] * planeHeight);
    }
    
    if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
        result = tjDecompressToYUVPlanes(decompress_handle, jpegBuf, jpegSize,
                                         yuvPlanes, width, strides, height, fuzz_int32_t_11);
    }
    
    // Free YUV plane buffers
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) free(yuvPlanes[i]);
    }

    // Get error string using tj3GetErrorStr (convert tjhandle to tjhandle for tj3)
    // Note: We need to create a tj3 handle for tj3GetErrorStr
    tjhandle tj3_handle = tj3InitVersion(fuzz_int32_t_12, fuzz_int32_t_13);
    if (tj3_handle) {
        const char *errorStr = tj3GetErrorStr(tj3_handle);
        // Use errorStr to avoid unused variable warning
        if (errorStr) {
            // Do nothing, just using the API as required
        }
        tj3Destroy(tj3_handle);
    }

    // Use tj3Free to free a buffer
    void *test_buffer = malloc(100);
    if (test_buffer) {
        tj3Free(test_buffer);  // tj3Free can free any buffer
    }

    // Cleanup
    tjFree(srcBuf);
    tjFree(jpegBuf);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}