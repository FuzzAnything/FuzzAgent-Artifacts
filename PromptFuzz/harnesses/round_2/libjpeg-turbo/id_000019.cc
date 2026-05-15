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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"tj3EncodeYUVPlanes8":[[1597,7,1597,17,0,0,4,0]]},"library_calls":["tjInitDecompress","tj3LoadImage8","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tjEncodeYUV2","tj3GetErrorStr","tjGetErrorStr2","tjDestroy","tjFree","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tjEncodeYUV2","tj3GetErrorStr","tjGetErrorStr2","tjDestroy","tjFree","tj3GetErrorStr","tjGetErrorStr2","tjDestroy"],"critical_calls":["tjInitDecompress","tj3LoadImage8","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tjEncodeYUV2","tj3GetErrorStr","tjGetErrorStr2","tjDestroy","tjFree","tj3GetErrorStr","tjGetErrorStr2","tjDestroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs to achieve an event where we:
1. Load an image from memory (treated as a file) using both 8-bit and 16-bit loaders
2. Encode the loaded image to YUV format
3. Calculate buffer sizes for YUV data
4. Handle errors using both error string functions

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
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
    
    // Write input data to a temporary file
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    tjhandle handle = nullptr;
    unsigned char *img8 = nullptr;
    unsigned short *img16 = nullptr;
    unsigned char *yuvBuf = nullptr;
    int width = 0, height = 0, pixelFormat = 0;
    
    // Initialize a decompressor handle (for loading images)
    handle = tjInitDecompress();
    if (!handle) {
        // Clean up and return
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 1. Load image as 8-bit
    img8 = tj3LoadImage8(handle, "input_file", &width, 1, &height, &pixelFormat);
    if (img8) {
        // Try to encode to YUV8
        // First calculate buffer size using both methods
        unsigned long bufSize1 = TJBUFSIZEYUV(width, height, TJSAMP_420);
        unsigned long bufSize2 = tjBufSizeYUV(width, height, TJSAMP_420);
        
        // Use the larger buffer size for safety
        unsigned long yuvSize = bufSize1 > bufSize2 ? bufSize1 : bufSize2;
        
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Reasonable size limit
            yuvBuf = (unsigned char *)malloc(yuvSize);
            if (yuvBuf) {
                // Create a compressor handle for encoding
                tjhandle compHandle = tjInitCompress();
                if (compHandle) {
                    // Encode to YUV
                    int result = tjEncodeYUV2(compHandle, img8, width, 0, height, 
                                              pixelFormat, yuvBuf, TJSAMP_420, 0);
                    
                    // Get error string if encoding failed
                    if (result != 0) {
                        char *errorStr1 = tj3GetErrorStr(handle);
                        char *errorStr2 = tjGetErrorStr2(handle);
                        // Error strings are for debugging, we don't need to use them further
                        (void)errorStr1;
                        (void)errorStr2;
                    }
                    
                    tjDestroy(compHandle);
                }
                
                free(yuvBuf);
                yuvBuf = nullptr;
            }
            
            tjFree(img8);
            img8 = nullptr;
        }
    } else {
        // Try to load as 16-bit if 8-bit fails
        img16 = tj3LoadImage16(handle, "input_file", &width, 1, &height, &pixelFormat);
        if (img16) {
            // For 16-bit images, we need to convert to 8-bit for tj3EncodeYUV8
            // First calculate buffer sizes
            unsigned long bufSize1 = TJBUFSIZEYUV(width, height, TJSAMP_420);
            unsigned long bufSize2 = tjBufSizeYUV(width, height, TJSAMP_420);
            unsigned long yuvSize = bufSize1 > bufSize2 ? bufSize1 : bufSize2;
            
            if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) {
                // We can't directly encode 16-bit with tj3EncodeYUV8, so we'll
                // create a simple 8-bit conversion for demonstration
                int pixelCount = width * height;
                unsigned char *img8_from_16 = (unsigned char *)malloc(pixelCount * 3); // RGB
                if (img8_from_16) {
                    // Simple conversion: take lower 8 bits
                    for (int i = 0; i < pixelCount * 3; i++) {
                        img8_from_16[i] = (unsigned char)(img16[i] & 0xFF);
                    }
                    
                    yuvBuf = (unsigned char *)malloc(yuvSize);
                    if (yuvBuf) {
                        tjhandle compHandle = tjInitCompress();
                        if (compHandle) {
                            // Note: We're using the legacy API here since tj3EncodeYUV8
                            // requires a different handle type
                            int result = tjEncodeYUV2(compHandle, img8_from_16, width, 0, height,
                                                      TJPF_RGB, yuvBuf, TJSAMP_420, 0);
                            
                            if (result != 0) {
                                char *errorStr1 = tj3GetErrorStr(handle);
                                char *errorStr2 = tjGetErrorStr2(handle);
                                (void)errorStr1;
                                (void)errorStr2;
                            }
                            
                            tjDestroy(compHandle);
                        }
                        free(yuvBuf);
                        yuvBuf = nullptr;
                    }
                    free(img8_from_16);
                }
                
                tjFree((unsigned char *)img16);
                img16 = nullptr;
            }
        }
    }
    
    // Get error strings one more time to ensure all APIs are called
    if (handle) {
        char *errorStr1 = tj3GetErrorStr(handle);
        char *errorStr2 = tjGetErrorStr2(handle);
        (void)errorStr1;
        (void)errorStr2;
    }
    
    // Clean up
    if (handle) {
        tjDestroy(handle);
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}