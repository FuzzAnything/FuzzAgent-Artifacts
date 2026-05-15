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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3LoadImage8","TJBUFSIZEYUV","tjBufSizeYUV","tj3Alloc","tj3EncodeYUV8","tj3GetErrorStr","tjGetErrorStr2","tj3Free","tj3SaveImage8","tj3Free","tj3LoadImage16","tj3Alloc","tj3SaveImage8","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3LoadImage8","TJBUFSIZEYUV","tjBufSizeYUV","tj3Alloc","tj3EncodeYUV8","tj3GetErrorStr","tjGetErrorStr2","tj3Free","tj3SaveImage8","tj3Free","tj3LoadImage16","tj3Alloc","tj3SaveImage8","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for saving images
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize handles for both compression and decompression
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    
    if (!decomp_handle || !comp_handle) {
        if (decomp_handle) tj3Destroy(decomp_handle);
        if (comp_handle) tj3Destroy(comp_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Try to load as 8-bit image first
    int width, height, pixelFormat;
    unsigned char *img8 = tj3LoadImage8(decomp_handle, "input_file", &width, 1, &height, &pixelFormat);
    
    if (img8) {
        // Calculate YUV buffer sizes using both required functions
        unsigned long buf_size1 = TJBUFSIZEYUV(width, height, TJSAMP_420);
        unsigned long buf_size2 = tjBufSizeYUV(width, height, TJSAMP_420);
        
        // Use the larger size for safety
        unsigned long yuv_size = buf_size1 > buf_size2 ? buf_size1 : buf_size2;
        unsigned char *yuv_buf = (unsigned char *)tj3Alloc(yuv_size);
        
        if (yuv_buf) {
            // Encode to YUV format
            int result = tj3EncodeYUV8(comp_handle, img8, width, 
                                       width * tjPixelSize[pixelFormat], 
                                       height, pixelFormat, yuv_buf, 1);
            
            // Get error strings using both required functions
            const char *error1 = tj3GetErrorStr(comp_handle);
            const char *error2 = tjGetErrorStr2(comp_handle);
            (void)error1; (void)error2; // Prevent unused variable warnings
            
            tj3Free(yuv_buf);
        }
        
        // Save the loaded image
        tj3SaveImage8(comp_handle, "output_file", img8, width, 
                      width * tjPixelSize[pixelFormat], height, pixelFormat);
        
        tj3Free(img8);
    }
    
    // Now try to load as 16-bit image
    rewind(in_file); // Reset file pointer
    
    unsigned short *img16 = tj3LoadImage16(decomp_handle, "input_file", &width, 1, &height, &pixelFormat);
    
    if (img16) {
        // Save the 16-bit image (converted to 8-bit for saving)
        unsigned char *img8_converted = (unsigned char *)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
        if (img8_converted) {
            // Simple conversion: take lower 8 bits
            for (size_t i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
                img8_converted[i] = (unsigned char)(img16[i] >> 8);
            }
            
            tj3SaveImage8(comp_handle, "output_file2", img8_converted, width,
                          width * tjPixelSize[pixelFormat], height, pixelFormat);
            
            tj3Free(img8_converted);
        }
        
        tj3Free(img16);
    }
    
    // Cleanup
    tj3Destroy(decomp_handle);
    tj3Destroy(comp_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    remove("output_file2");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}