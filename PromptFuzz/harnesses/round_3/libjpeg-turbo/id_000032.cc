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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 95, nr_unique_branch: 4
//<Quality> {"density":19,"unique_branches":{"tj3YUVBufSize":[[1044,9,1044,16,0,0,4,0]],"tj3YUVPlaneWidth":[[1121,7,1121,16,0,0,4,0]],"tj3YUVPlaneHeight":[[1155,7,1155,17,0,0,4,0]],"tjPlaneHeight":[[1178,10,1178,23,0,0,4,0]]},"library_calls":["tjLoadImage","tjInitDecompress","tjDecompressHeader","tj3SetCroppingRegion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneHeight","tjPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3CompressFromYUVPlanes8","tj3Alloc","tj3Decompress16","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tjFree"],"critical_calls":["tjLoadImage","tjInitDecompress","tjDecompressHeader","tj3SetCroppingRegion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneHeight","tjPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneWidth","tj3CompressFromYUVPlanes8","tj3Alloc","tj3Decompress16","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tjFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
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
    
    // 1. Load image using tjLoadImage
    int width, height, pixelFormat;
    unsigned char *img_buffer = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    
    // Write input data to file for tjLoadImage
    FILE *temp_in = fopen("input_file", "wb");
    if (temp_in) {
        fwrite(data, 1, size, temp_in);
        assert_file_closed(&temp_in);;
    }
    
    if (img_buffer) {
        // 2. Decompress header using tjDecompressHeader
        tjhandle handle = tjInitDecompress();
        if (handle) {
            int width2, height2;
            tjDecompressHeader(handle, (unsigned char *)data, (unsigned long)size, &width2, &height2);
            
            // 3. Set cropping region
            tjregion cropRegion = {0, 0, width2/2, height2/2};
            tj3SetCroppingRegion(handle, cropRegion);
            
            // 4. Decompress to YUV8
            size_t yuv_size = tj3YUVBufSize(width2, fuzz_int32_t_3, height2, TJSAMP_420);
            unsigned char *yuv_buffer = (unsigned char *)tj3Alloc(yuv_size);
            if (yuv_buffer) {
                tj3DecompressToYUV8(handle, data, size, yuv_buffer, fuzz_int32_t_4);
                
                // 5. Get YUV plane height using both functions
                int plane_h1 = tj3YUVPlaneHeight(fuzz_int32_t_5, height2, TJSAMP_420);
                int plane_h2 = tjPlaneHeight(fuzz_int32_t_6, height2, TJSAMP_420);
                
                // 6. Compress from YUV planes
                unsigned char *jpeg_buf1 = nullptr;
                size_t jpeg_size1 = 0;
                
                // Prepare plane pointers for 4:2:0 format
                const unsigned char *src_planes[3];
                int strides[3];
                int y_width = tj3YUVPlaneWidth(fuzz_int32_t_7, width2, TJSAMP_420);
                int uv_width = tj3YUVPlaneWidth(fuzz_int32_t_8, width2, TJSAMP_420);
                
                src_planes[0] = yuv_buffer;  // Y plane
                src_planes[1] = yuv_buffer + y_width * plane_h1;  // U plane
                src_planes[2] = src_planes[1] + uv_width * (plane_h1/2);  // V plane
                
                strides[0] = y_width;
                strides[1] = uv_width;
                strides[2] = uv_width;
                
                tj3CompressFromYUVPlanes8(handle, src_planes, width2, strides, height2, &jpeg_buf1, &jpeg_size1);
                
                // 7. Decompress to 16-bit
                unsigned short *dst16 = (unsigned short *)tj3Alloc(width2 * height2 * 3 * sizeof(unsigned short));
                if (dst16) {
                    tj3Decompress16(handle, data, size, dst16, width2 * 3, TJPF_RGB);
                    
                    // 8. Compress 12-bit (using the 16-bit data as if it were 12-bit)
                    unsigned char *jpeg_buf2 = nullptr;
                    size_t jpeg_size2 = 0;
                    tj3Compress12(handle, (short *)dst16, width2, width2 * 3 * sizeof(unsigned short), 
                                 height2, TJPF_RGB, &jpeg_buf2, &jpeg_size2);
                    
                    // Write output if compression succeeded
                    if (jpeg_buf2 && jpeg_size2 > 0) {
                        fwrite(jpeg_buf2, 1, jpeg_size2, out_file);
                    }
                    
                    // Clean up
                    if (jpeg_buf2) tj3Free(jpeg_buf2);
                    tj3Free(dst16);
                }
                
                if (jpeg_buf1) tj3Free(jpeg_buf1);
                tj3Free(yuv_buffer);
            }
            
            tjDestroy(handle);
        }
        
        tjFree(img_buffer);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_in);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}