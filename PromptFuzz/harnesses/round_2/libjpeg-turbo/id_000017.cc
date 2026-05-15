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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 8
//<Quality> {"density":20,"unique_branches":{"tj3CompressFromYUV8":[[1493,7,1493,21,0,0,4,1],[1493,25,1493,35,0,0,4,1],[1493,39,1493,48,0,0,4,1],[1493,52,1493,60,0,0,4,1],[1494,7,1494,18,0,0,4,1],[1497,7,1497,38,0,0,4,0],[340,7,340,12,1,0,4,1]],"setCompDefaults":[[389,28,389,41,0,0,4,1]]},"library_calls":["tj3InitVersion","tj3LoadImage8","tj3GetErrorStr","tjGetErrorStr2","tj3LoadImage16","tj3Destroy","TJBUFSIZEYUV","tjBufSizeYUV","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3InitVersion","tj3Free","tj3Free","tj3Set","tj3EncodeYUV8","tj3Destroy","tj3InitVersion","tj3CompressFromYUV8","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage8","tj3GetErrorStr","tjGetErrorStr2","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tj3Destroy","tj3InitVersion","tj3Set","tj3EncodeYUV8","tj3Destroy","tj3InitVersion","tj3CompressFromYUV8","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file for loading APIs
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file name for saving
    const char* input_filename = "input_file";
    const char* output_filename = "output_file";
    
    // Write the input data to file
    FILE* tmp_file = fopen(input_filename, "wb");
    if (!tmp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    tjhandle handle = nullptr;
    unsigned char* srcBuf = nullptr;
    unsigned short* srcBuf16 = nullptr;
    unsigned char* dstBuf = nullptr;
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = 0;
    
    // Initialize handle for decompression/loading
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove(input_filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Try loading as 8-bit image first
    int width = 0, height = 0, pixelFormat = 0;
    srcBuf = tj3LoadImage8(handle, input_filename, &width, 4, &height, &pixelFormat);
    
    // If 8-bit load failed, try 16-bit
    if (!srcBuf) {
        // Get error string using both error functions
        char* error_str1 = tj3GetErrorStr(handle);
        char* error_str2 = tjGetErrorStr2(handle);
        // We don't use the error strings but call the functions as required
        // to avoid unused variable warnings
        (void)error_str1;
        (void)error_str2;
        
        // Try 16-bit load
        srcBuf16 = tj3LoadImage16(handle, input_filename, &width, 4, &height, &pixelFormat);
        
        if (!srcBuf16) {
            // Both loads failed, clean up and return
            tj3Destroy(handle);
            remove(input_filename);
            assert_file_closed(&in_file);;
            assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
        }
    }
    
    // If we have valid image data, proceed with YUV encoding
    if (srcBuf || srcBuf16) {
        // For simplicity, use default subsampling (420)
        int jpegSubsamp = TJSAMP_420;
        
        // Calculate YUV buffer size using both functions
        unsigned long bufSize1 = TJBUFSIZEYUV(width, height, jpegSubsamp);
        unsigned long bufSize2 = tjBufSizeYUV(width, height, jpegSubsamp);
        // Use the larger buffer size for safety
        size_t yuvBufSize = (bufSize1 > bufSize2) ? bufSize1 : bufSize2;
        
        // Allocate destination buffer
        dstBuf = (unsigned char*)malloc(yuvBufSize);
        if (!dstBuf) {
            if (srcBuf) tj3Free(srcBuf);
            if (srcBuf16) tj3Free(srcBuf16);
            tj3Destroy(handle);
            remove(input_filename);
            assert_file_closed(&in_file);;
            assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
        }
        
        // Reinitialize handle for compression if needed
        tj3Destroy(handle);
        handle = tj3Init(TJINIT_COMPRESS);
        if (!handle) {
            if (srcBuf) tj3Free(srcBuf);
            if (srcBuf16) tj3Free(srcBuf16);
            free(dstBuf);
            remove(input_filename);
            assert_file_closed(&in_file);;
            assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
        }
        
        // Set subsampling parameter
        tj3Set(handle, TJPARAM_SUBSAMP, jpegSubsamp);
        
        // Encode to YUV - use 8-bit version
        // Note: If we have 16-bit data, we need to convert or use appropriate API
        // For simplicity, we'll use 8-bit path only
        if (srcBuf) {
            int pitch = width * tjPixelSize[pixelFormat];
            int align = 4;
            
            // Use tj3EncodeYUV8 - our target API
            int result = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, 
                                       pixelFormat, dstBuf, align);
            
            // If YUV encoding succeeded, try to compress it back to JPEG
            if (result == 0) {
                // Reinitialize handle for compression from YUV
                tj3Destroy(handle);
                handle = tj3Init(TJINIT_COMPRESS);
                
                if (handle) {
                    // Compress from YUV
                    tj3CompressFromYUV8(handle, dstBuf, width, align, height, 
                                        &jpegBuf, &jpegSize);
                    
                    // Save the result
                    if (jpegBuf && jpegSize > 0) {
                        FILE* out_file = fopen(output_filename, "wb");
                        if (out_file) {
                            fwrite(jpegBuf, 1, jpegSize, out_file);
                            assert_file_closed(&out_file);;
                        }
                    }
                }
            }
        }
    }
    
    // Cleanup
    if (srcBuf) tj3Free(srcBuf);
    if (srcBuf16) tj3Free(srcBuf16);
    if (dstBuf) free(dstBuf);
    if (jpegBuf) tj3Free(jpegBuf);
    if (handle) tj3Destroy(handle);
    
    remove(input_filename);
    remove(output_filename);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}