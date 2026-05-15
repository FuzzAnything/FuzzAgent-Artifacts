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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"tj3GetErrorStr":[[682,7,682,11,0,0,4,1]],"tj3LoadImage8":[[340,7,340,12,3,0,4,0]]},"library_calls":["tjInitDecompress","tj3LoadImage8","tj3LoadImage8","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tj3SetScalingFactor","tjAlloc","tjEncodeYUV2","tjFree","tj3Compress8","tjFree","tjGetErrorCode","tjGetErrorCode","tj3GetErrorStr","tjGetErrorStr2","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tj3LoadImage8","tj3LoadImage8","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tj3SetScalingFactor","tjAlloc","tjEncodeYUV2","tjFree","tj3Compress8","tjFree","tjGetErrorCode","tjGetErrorCode","tj3GetErrorStr","tjGetErrorStr2","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":0}
/*I'll create a fuzz driver that uses the specified libjpeg-turbo APIs to process a JPEG image. Here's the implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
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
        return 0; // Too small to be a valid JPEG
    }

    // Write input data to a temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (though not directly used by the APIs)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used to avoid warnings
    (void)out_fd;
    
    // Create decompress handle
    tjhandle decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Load image as 8-bit
    int width, height, pixelFormat;
    unsigned char *image8 = tj3LoadImage8(nullptr, "input_file", &width, 1, &height, &pixelFormat);
    
    // If 8-bit loading failed, try to decompress from memory
    if (!image8) {
        // Close and reopen the file to ensure it's at the beginning
        assert_file_closed(&in_file);;
        in_file = fopen("input_file", "wb");
        if (in_file) {
            fwrite(data, 1, size, in_file);
            assert_file_closed(&in_file);;
        }
        
        // Create a new file handle for reading
        in_file = fopen("input_file", "rb");
        if (in_file) {
            image8 = tj3LoadImage8(nullptr, "input_file", &width, 1, &height, &pixelFormat);
        }
    }
    
    // Load image as 16-bit (if supported)
    unsigned short *image16 = nullptr;
    if (image8) {
        image16 = tj3LoadImage16(nullptr, "input_file", &width, 1, &height, &pixelFormat);
    }
    
    // Calculate YUV buffer sizes using different APIs
    unsigned long yuv_size1 = 0;
    unsigned long yuv_size2 = 0;
    
    if (width > 0 && height > 0) {
        // Use TJBUFSIZEYUV
        yuv_size1 = TJBUFSIZEYUV(width, height, TJSAMP_420);
        
        // Use tjBufSizeYUV
        yuv_size2 = tjBufSizeYUV(width, height, TJSAMP_420);
    }
    
    // Create compress handle
    tjhandle compress_handle = tjInitCompress();
    if (compress_handle && image8 && width > 0 && height > 0) {
        // Set scaling factor
        tjscalingfactor scaling_factor;
        scaling_factor.num = 1;
        scaling_factor.denom = 1;
        int scaling_result = tj3SetScalingFactor(nullptr, scaling_factor);
        (void)scaling_result; // Result may fail with nullptr handle
        
        // Allocate YUV buffer
        unsigned char *yuv_buffer = nullptr;
        if (yuv_size1 > 0) {
            yuv_buffer = tjAlloc(yuv_size1);
            
            if (yuv_buffer) {
                // Encode to YUV using the old API
                int encode_result = tjEncodeYUV2(
                    compress_handle,
                    image8,
                    width,
                    0, // pitch (0 means width * pixel size)
                    height,
                    pixelFormat,
                    yuv_buffer,
                    TJSAMP_420,
                    0 // flags
                );
                (void)encode_result;
                
                // Free YUV buffer
                if (yuv_buffer) {
                    tjFree(yuv_buffer);
                }
            }
        }
        
        // Compress using the new API
        unsigned char *jpeg_buffer = nullptr;
        size_t jpeg_size = 0;
        
        int compress_result = tj3Compress8(
            nullptr, // Using nullptr handle since we don't have tj3InitVersion
            image8,
            width,
            0, // pitch
            height,
            pixelFormat,
            &jpeg_buffer,
            &jpeg_size
        );
        
        // Write compressed data to output file if successful
        if (compress_result == 0 && jpeg_buffer && jpeg_size > 0) {
            fwrite(jpeg_buffer, 1, jpeg_size, out_file);
        }
        
        // Free JPEG buffer
        if (jpeg_buffer) {
            tjFree(jpeg_buffer);
        }
    }
    
    // Check error codes
    int decompress_error = tjGetErrorCode(decompress_handle);
    int compress_error = tjGetErrorCode(compress_handle);
    (void)decompress_error;
    (void)compress_error;
    
    // Get error strings
    char *error_str1 = tj3GetErrorStr(nullptr);
    char *error_str2 = tjGetErrorStr2(compress_handle);
    (void)error_str1;
    (void)error_str2;
    
    // Free loaded images
    if (image8) {
        tjFree(image8);
    }
    if (image16) {
        tjFree((unsigned char *)image16);
    }
    
    // Clean up handles
    if (decompress_handle) {
        tjDestroy(decompress_handle);
    }
    if (compress_handle) {
        tjDestroy(compress_handle);
    }
    
    // Close files
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}