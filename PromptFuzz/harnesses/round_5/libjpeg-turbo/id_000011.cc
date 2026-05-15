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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["tjInitDecompress","tjInitTransform","tj3InitVersion","tjDestroy","tjDestroy","tj3Destroy","tjDecompressHeader","tjDecompressHeader2","tjGetScalingFactors","tj3YUVBufSize","tj3DecompressToYUV8","tj3Decompress12","tj3Decompress16","tjTransform","tjFree","tj3LoadImage16","tj3Free","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tjInitDecompress","tjInitTransform","tj3InitVersion","tjDecompressHeader","tjDecompressHeader2","tjGetScalingFactors","tj3YUVBufSize","tj3DecompressToYUV8","tj3Decompress12","tj3Decompress16","tjTransform","tjFree","tj3LoadImage16","tj3Free","tjDestroy","tjDestroy","tj3Destroy"],"visited":0}
/*Looking at the provided APIs, I can create a fuzzer that processes JPEG data through various decompression and transformation operations. Here's a comprehensive fuzzer that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful JPEG data
    
    // 1. Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Initialize handles for both old and new APIs
    tjhandle decompress_handle = tjInitDecompress();
    tjhandle transform_handle = tjInitTransform();
    tjhandle new_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!decompress_handle || !transform_handle || !new_handle) {
        if (decompress_handle) tjDestroy(decompress_handle);
        if (transform_handle) tjDestroy(transform_handle);
        if (new_handle) tj3Destroy(new_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate buffer for JPEG data
    unsigned char *jpeg_buf = (unsigned char *)data;
    unsigned long jpeg_size = (unsigned long)size;
    
    // 3. Get JPEG header information using old API
    int width_old = 0, height_old = 0;
    int width_old2 = 0, height_old2 = 0, jpeg_subsamp = 0;
    
    int result1 = tjDecompressHeader(decompress_handle, jpeg_buf, jpeg_size, 
                                     &width_old, &height_old);
    int result2 = tjDecompressHeader2(decompress_handle, jpeg_buf, jpeg_size,
                                      &width_old2, &height_old2, &jpeg_subsamp);
    
    // If headers are valid, proceed with decompression operations
    if (result1 == 0 && result2 == 0 && width_old > 0 && height_old > 0) {
        // 4. Get scaling factors
        int num_scaling_factors = 0;
        tjscalingfactor *scaling_factors = tjGetScalingFactors(&num_scaling_factors);
        
        // 5. Decompress to YUV using new API
        int align = 4;
        size_t yuv_buf_size = tj3YUVBufSize(width_old, align, height_old, TJSAMP_420);
        unsigned char *yuv_buffer = nullptr;
        
        if (yuv_buf_size > 0 && yuv_buf_size < 100 * 1024 * 1024) { // Limit to 100MB
            yuv_buffer = (unsigned char *)malloc(yuv_buf_size);
            if (yuv_buffer) {
                int result_yuv = tj3DecompressToYUV8(new_handle, jpeg_buf, size, 
                                                    yuv_buffer, align);
                // Result is ignored for fuzzing purposes
            }
        }
        
        // 6. Decompress to 12-bit format
        int pitch = width_old * 3 * sizeof(short); // 3 components for RGB
        short *dst_buf_12 = nullptr;
        if (width_old > 0 && height_old > 0 && pitch > 0 && 
            pitch < 100 * 1024 * 1024 / height_old) {
            dst_buf_12 = (short *)malloc(pitch * height_old);
            if (dst_buf_12) {
                int result_12 = tj3Decompress12(new_handle, jpeg_buf, size,
                                               dst_buf_12, pitch, TJPF_RGB);
                // Result is ignored for fuzzing purposes
            }
        }
        
        // 7. Decompress to 16-bit format
        unsigned short *dst_buf_16 = nullptr;
        if (width_old > 0 && height_old > 0 && pitch > 0 && 
            pitch < 100 * 1024 * 1024 / height_old) {
            dst_buf_16 = (unsigned short *)malloc(pitch * height_old);
            if (dst_buf_16) {
                int result_16 = tj3Decompress16(new_handle, jpeg_buf, size,
                                               dst_buf_16, pitch, TJPF_RGB);
                // Result is ignored for fuzzing purposes
            }
        }
        
        // 8. Apply transformation using old API
        if (jpeg_size > 0) {
            unsigned char *dst_bufs[1] = {nullptr};
            unsigned long dst_sizes[1] = {0};
            tjtransform transforms[1];
            
            // Initialize transformation
            memset(&transforms[0], 0, sizeof(tjtransform));
            transforms[0].r.x = 0;
            transforms[0].r.y = 0;
            transforms[0].r.w = width_old / 2;
            transforms[0].r.h = height_old / 2;
            transforms[0].op = TJXOP_NONE;
            transforms[0].options = TJXOPT_CROP;
            
            // Apply transformation
            int transform_result = tjTransform(transform_handle, jpeg_buf, jpeg_size,
                                              1, dst_bufs, dst_sizes, transforms, 0);
            
            // Clean up transformed buffer if allocation was successful
            if (transform_result == 0 && dst_bufs[0]) {
                tjFree(dst_bufs[0]);
            }
        }
        
        // 9. Load image from file using new API (16-bit)
        // First, write the input data to a file
        FILE *temp_file = fopen("input_file", "wb");
        if (temp_file) {
            fwrite(data, 1, size, temp_file);
            assert_file_closed(&temp_file);;
            
            // Now load it using tj3LoadImage16
            int loaded_width = 0, loaded_height = 0, loaded_pixel_format = 0;
            unsigned short *loaded_image = tj3LoadImage16(new_handle, "input_file",
                                                         &loaded_width, 4, 
                                                         &loaded_height, 
                                                         &loaded_pixel_format);
            
            if (loaded_image) {
                // Free the loaded image
                tj3Free(loaded_image);
            }
            
            // Clean up temporary file
            remove("input_file");
        }
        
        // Clean up allocated buffers
        if (yuv_buffer) free(yuv_buffer);
        if (dst_buf_12) free(dst_buf_12);
        if (dst_buf_16) free(dst_buf_16);
        
        // Free scaling factors array (if allocated by library)
        // Note: tjGetScalingFactors returns a pointer to static data, so no free needed
    }
    
    // 10. Clean up all handles
    if (decompress_handle) tjDestroy(decompress_handle);
    if (transform_handle) tjDestroy(transform_handle);
    if (new_handle) tj3Destroy(new_handle);
    
    // 11. Close file handle
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}