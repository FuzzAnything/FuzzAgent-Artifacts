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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 2
//<Quality> {"density":34,"unique_branches":{"_tj3LoadImageFromFileHandle8":[[317,26,317,35,0,0,4,0]],"tj3Destroy":[[641,7,641,12,0,0,4,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjDestroy","tjDestroy","tjLoadImage","tjDestroy","tjDestroy","tjCompress2","tjDecompress2","tjBufSizeYUV","tjAlloc","tjDecompressToYUV","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjDecompressToYUVPlanes","tjFree","tjFree","tj3InitVersion","tj3Alloc","tj3Decompress8","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tj3Alloc","tj3Decompress16","tj3Free","tj3Free","tj3Free","tj3GetErrorStr","tj3Destroy","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tjLoadImage","tjCompress2","tjDecompress2","tjBufSizeYUV","tjAlloc","tjDecompressToYUV","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjDecompressToYUVPlanes","tjFree","tjFree","tj3InitVersion","tj3Alloc","tj3Decompress8","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tj3Alloc","tj3Decompress16","tj3Free","tj3Free","tj3Free","tj3GetErrorStr","tj3Destroy","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":0}
/*Looking at this task, I need to create a fuzzer that uses the specified libjpeg-turbo APIs to process JPEG images. The key is to use the input data as a JPEG file and perform various operations on it. Here's my solution:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0; // Too small to be a valid JPEG
    }
    
    // Write input data to a file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Initialize legacy handles
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
    
    // Load image from file
    int width, height, pixelFormat;
    unsigned char *uncompressed_buf = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    
    if (!uncompressed_buf) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Try to compress the loaded image
    unsigned char *jpeg_buf = nullptr;
    unsigned long jpeg_size = 0;
    int jpeg_subsamp = TJSAMP_420;
    int jpeg_qual = 75;
    
    int compress_result = tjCompress2(compress_handle, uncompressed_buf, width, fuzz_int32_t_3, height, 
                                      pixelFormat, &jpeg_buf, &jpeg_size, jpeg_subsamp, fuzz_int32_t_4, fuzz_int32_t_5);
    
    if (compress_result == 0 && jpeg_buf && jpeg_size > 0) {
        // Decompress using legacy API
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned char *decompressed_buf = (unsigned char*)malloc(width * height * tjPixelSize[pixelFormat]);
        
        if (decompressed_buf) {
            tjDecompress2(decompress_handle, jpeg_buf, jpeg_size, decompressed_buf, 
                         width, pitch, height, pixelFormat, fuzz_int32_t_6);
            
            // Calculate YUV buffer size
            unsigned long yuv_buf_size = tjBufSizeYUV(width, height, jpeg_subsamp);
            unsigned char *yuv_buf = nullptr;
            
            if (yuv_buf_size > 0) {
                yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
                if (yuv_buf) {
                    // Decompress to YUV
                    tjDecompressToYUV(decompress_handle, jpeg_buf, jpeg_size, yuv_buf, fuzz_int32_t_7);
                    
                    // Calculate plane dimensions
                    int plane_width = tjPlaneWidth(fuzz_int32_t_8, width, jpeg_subsamp);
                    int plane_height = tjPlaneHeight(fuzz_int32_t_9, height, jpeg_subsamp);
                    
                    // Decompress to YUV planes
                    int strides[3];
                    unsigned char *yuv_planes[3];
                    
                    for (int i = 0; i < 3; i++) {
                        strides[i] = tjPlaneWidth(fuzz_int32_t_10, width, jpeg_subsamp);
                        int plane_h = tjPlaneHeight(fuzz_int32_t_11, height, jpeg_subsamp);
                        yuv_planes[i] = (unsigned char*)tjAlloc(strides[i] * plane_h);
                    }
                    
                    tjDecompressToYUVPlanes(decompress_handle, jpeg_buf, jpeg_size, 
                                           yuv_planes, width, strides, height, fuzz_int32_t_12);
                    
                    // Free YUV planes
                    for (int i = 0; i < 3; i++) {
                        if (yuv_planes[i]) tjFree(yuv_planes[i]);
                    }
                    
                    tjFree(yuv_buf);
                }
            }
            
            free(decompressed_buf);
        }
        
        // Initialize new API handle
        tjhandle new_handle = tj3InitVersion(fuzz_int32_t_13, fuzz_int32_t_14); // TJINIT_BOTH for version 3.0.0
        
        if (new_handle) {
            // Use new API to decompress to 8-bit
            unsigned char *new_decompressed_buf8 = (unsigned char*)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
            if (new_decompressed_buf8) {
                tj3Decompress8(new_handle, jpeg_buf, jpeg_size, new_decompressed_buf8, 
                              width * tjPixelSize[pixelFormat], pixelFormat);
                
                // Encode to YUV planes using new API
                int new_strides[3];
                unsigned char *new_yuv_planes[3];
                
                for (int i = 0; i < 3; i++) {
                    new_strides[i] = tjPlaneWidth(fuzz_int32_t_15, width, jpeg_subsamp);
                    int plane_h = tjPlaneHeight(fuzz_int32_t_16, height, jpeg_subsamp);
                    new_yuv_planes[i] = (unsigned char*)tj3Alloc(new_strides[i] * plane_h);
                }
                
                tj3EncodeYUVPlanes8(new_handle, uncompressed_buf, width, 
                                   width * tjPixelSize[pixelFormat], height, 
                                   pixelFormat, new_yuv_planes, new_strides);
                
                // Decompress to 16-bit using new API
                unsigned short *new_decompressed_buf16 = (unsigned short*)tj3Alloc(width * height * tjPixelSize[pixelFormat] * 2);
                if (new_decompressed_buf16) {
                    tj3Decompress16(new_handle, jpeg_buf, jpeg_size, new_decompressed_buf16, 
                                   width * tjPixelSize[pixelFormat] * 2, pixelFormat);
                    tj3Free(new_decompressed_buf16);
                }
                
                // Free new API resources
                for (int i = 0; i < 3; i++) {
                    if (new_yuv_planes[i]) tj3Free(new_yuv_planes[i]);
                }
                
                tj3Free(new_decompressed_buf8);
            }
            
            // Get error string (even if no error)
            char *error_str = tj3GetErrorStr(new_handle);
            
            // Clean up new API handle
            tj3Destroy(new_handle);
        }
        
        // Free compressed buffer
        if (jpeg_buf) tjFree(jpeg_buf);
    }
    
    // Clean up all resources
    if (uncompressed_buf) tjFree(uncompressed_buf);
    if (compress_handle) tjDestroy(compress_handle);
    if (decompress_handle) tjDestroy(decompress_handle);
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}