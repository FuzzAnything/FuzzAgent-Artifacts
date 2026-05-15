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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["tjInitDecompress","tjDestroy","tjDecompressHeader3","tjGetErrorStr","tjDestroy","tjDestroy","tjBufSizeYUV","tjDestroy","tjDecompress","tjGetErrorStr","tjDecompressToYUV","tjGetErrorStr","tjPlaneWidth","tjPlaneHeight","tjDestroy","tjDecompressToYUVPlanes","tjGetErrorStr","tjDestroy","tjDecodeYUVPlanes","tjGetErrorStr","tj3InitVersion","tj3CompressFromYUVPlanes8","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader3","tjBufSizeYUV","tjDecompress","tjGetErrorStr","tjDecompressToYUV","tjGetErrorStr","tjPlaneWidth","tjPlaneHeight","tjDecompressToYUVPlanes","tjGetErrorStr","tjDecodeYUVPlanes","tjGetErrorStr","tj3InitVersion","tj3CompressFromYUVPlanes8","tj3Free","tj3Destroy","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_7, fuzz_int32_t_array_size_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_14, fuzz_int32_t_array_size_14, fdp);
	
		const int * fuzzer_var_tjDecodeYUVPlanes_18_2 = fuzz_int32_t_array_7;
		const unsigned char * fuzzer_var_tj3CompressFromYUVPlanes8_21_1[3] = {fuzz_str_11,fuzz_str_12,fuzz_str_13};
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_21_3 = fuzz_int32_t_array_14;//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG header
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned long jpegSize = size;
    
    // We need to copy the data to a non-const buffer for the old API
    unsigned char *jpegBuf = (unsigned char *)malloc(size);
    if (!jpegBuf) {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(jpegBuf, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Get JPEG header information
    int result = tjDecompressHeader3(handle, jpegBuf, jpegSize, 
                                     &width, &height, &jpegSubsamp, &jpegColorspace);
    if (result != 0) {
        // Try to get error string
        char *errorStr = tjGetErrorStr();
        free(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate buffers for different operations
    int pixelFormat = TJPF_RGB;
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Buffer for tjDecompress
    unsigned char *dstBuf = (unsigned char *)malloc(width * height * pixelSize);
    if (!dstBuf) {
        free(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Buffer for tjDecompressToYUV (interleaved YUV)
    unsigned long yuvBufSize = tjBufSizeYUV(width, height, jpegSubsamp);
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
    if (!yuvBuf) {
        free(dstBuf);
        free(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform tjDecompress (to RGB)
    result = tjDecompress(handle, jpegBuf, jpegSize, dstBuf, 
                          width, fuzz_int32_t_1, height, pixelSize, fuzz_int32_t_2);
    if (result != 0) {
        char *errorStr = tjGetErrorStr();
    }

    // Perform tjDecompressToYUV
    result = tjDecompressToYUV(handle, jpegBuf, jpegSize, yuvBuf, fuzz_int32_t_3);
    if (result != 0) {
        char *errorStr = tjGetErrorStr();
    }

    // Prepare for YUV planes operations
    // For tjDecompressToYUVPlanes and tjDecodeYUVPlanes
    const int maxPlanes = 3;
    unsigned char *yuvPlanes[maxPlanes] = {nullptr};
    int strides[maxPlanes] = {0};
    
    // Calculate plane dimensions
    for (int i = 0; i < maxPlanes; i++) {
        int planeWidth = tjPlaneWidth(fuzz_int32_t_4, width, jpegSubsamp);
        int planeHeight = tjPlaneHeight(fuzz_int32_t_5, height, jpegSubsamp);
        strides[i] = planeWidth;
        yuvPlanes[i] = (unsigned char *)malloc(planeWidth * planeHeight);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) free(yuvPlanes[j]);
            free(yuvBuf);
            free(dstBuf);
            free(jpegBuf);
            tjDestroy(handle);
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Perform tjDecompressToYUVPlanes
    result = tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, 
                                     yuvPlanes, width, strides, height, fuzz_int32_t_6);
    if (result != 0) {
        char *errorStr = tjGetErrorStr();
    }

    // Prepare another buffer for decoded RGB
    unsigned char *decodedBuf = (unsigned char *)malloc(width * height * pixelSize);
    if (!decodedBuf) {
        for (int i = 0; i < maxPlanes; i++) free(yuvPlanes[i]);
        free(yuvBuf);
        free(dstBuf);
        free(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform tjDecodeYUVPlanes
    result = tjDecodeYUVPlanes(handle, (const unsigned char **)yuvPlanes, 
                               fuzzer_var_tjDecodeYUVPlanes_18_2, jpegSubsamp, decodedBuf, 
                               width, fuzz_int32_t_8, height, pixelFormat, fuzz_int32_t_9);
    if (result != 0) {
        char *errorStr = tjGetErrorStr();
    }

    // Now use the new API for compression
    tjhandle newHandle = tj3InitVersion(TJINIT_COMPRESS, fuzz_int32_t_10);
    if (newHandle) {
        unsigned char *newJpegBuf = nullptr;
        size_t newJpegSize = 0;
        
        // Perform tj3CompressFromYUVPlanes8
        result = tj3CompressFromYUVPlanes8(newHandle, 
                                          fuzzer_var_tj3CompressFromYUVPlanes8_21_1, 
                                          width, fuzzer_var_tj3CompressFromYUVPlanes8_21_3, height, 
                                          &newJpegBuf, &newJpegSize);
        
        if (result == 0 && newJpegBuf) {
            // Write the compressed output to a file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(newJpegBuf, 1, newJpegSize, out_file);
                assert_file_closed(&out_file);;
            }
            tj3Free(newJpegBuf);
        }
        tj3Destroy(newHandle);
    }

    // Cleanup
    for (int i = 0; i < maxPlanes; i++) {
        free(yuvPlanes[i]);
    }
    free(decodedBuf);
    free(yuvBuf);
    free(dstBuf);
    free(jpegBuf);
    tjDestroy(handle);

    assert_file_closed(&in_file);
	return 0;
}