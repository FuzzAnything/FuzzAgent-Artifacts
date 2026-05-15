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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 8
//<Quality> {"density":19,"unique_branches":{"tj3CompressFromYUVPlanes8":[[1347,7,1347,26,0,0,4,0]],"tj3Decompress16":[[168,61,168,70,0,0,4,1],[169,7,169,22,0,0,4,1],[169,26,169,49,0,0,4,1],[172,7,172,22,0,0,4,1],[184,7,184,45,0,0,4,0],[184,7,184,45,0,0,4,1],[284,7,284,41,0,0,4,0]]},"library_calls":["tjLoadImage","tjInitDecompress","tjDecompressHeader","tjDestroy","tj3InitVersion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneHeight","tj3YUVPlaneHeight","tjPlaneHeight","tjPlaneHeight","tj3InitVersion","tj3CompressFromYUVPlanes8","tj3Free","tj3Destroy","tj3Free","tj3Destroy","tj3InitVersion","tj3SetCroppingRegion","tj3Alloc","tj3Decompress16","tj3InitVersion","tj3Compress12","tj3Free","tj3Destroy","tj3Free","tj3Destroy","tjFree"],"critical_calls":["tjLoadImage","tjInitDecompress","tjDecompressHeader","tjDestroy","tj3InitVersion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneHeight","tj3YUVPlaneHeight","tjPlaneHeight","tjPlaneHeight","tj3InitVersion","tj3CompressFromYUVPlanes8","tj3Free","tj3Destroy","tj3Free","tj3Destroy","tj3InitVersion","tj3SetCroppingRegion","tj3Alloc","tj3Decompress16","tj3InitVersion","tj3Compress12","tj3Free","tj3Destroy","tj3Free","tj3Destroy","tjFree"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
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
	  // Too small to be meaningful
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. Use tjLoadImage to load the JPEG from file
    int width, height, pixelFormat;
    unsigned char *loadedImage = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    
    // Write input data to file for tjLoadImage
    FILE *temp_in = fopen("input_file", "wb");
    if (temp_in) {
        fwrite(data, 1, size, temp_in);
        assert_file_closed(&temp_in);;
    }
    
    if (loadedImage) {
        // 2. Use tjDecompressHeader to get image dimensions
        tjhandle old_handle = tjInitDecompress();
        if (old_handle) {
            int width2, height2;
            tjDecompressHeader(old_handle, (unsigned char *)data, (unsigned long)size, &width2, &height2);
            tjDestroy(old_handle);
        }
        
        // 3. Decompress to YUV format using new API
        tjhandle handle1 = tj3Init(TJINIT_DECOMPRESS);
        if (handle1) {
            // Calculate YUV buffer size
            int align = 4;
            size_t yuvSize = tj3YUVBufSize(width, fuzz_int32_t_3, height, TJSAMP_420);
            if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) {  // Reasonable size limit
                unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
                if (yuvBuf) {
                    tj3DecompressToYUV8(handle1, (unsigned char *)data, size, yuvBuf, fuzz_int32_t_4);
                    
                    // 4. Use YUV plane height functions
                    int yHeight = tj3YUVPlaneHeight(fuzz_int32_t_5, height, TJSAMP_420);
                    int uvHeight = tj3YUVPlaneHeight(fuzz_int32_t_6, height, TJSAMP_420);
                    int oldYHeight = tjPlaneHeight(fuzz_int32_t_7, height, TJSAMP_420);
                    int oldUVHeight = tjPlaneHeight(fuzz_int32_t_8, height, TJSAMP_420);
                    
                    // 5. Compress from YUV planes
                    const unsigned char *srcPlanes[3];
                    int strides[3];
                    // For simplicity, use the same YUV buffer with offsets
                    srcPlanes[0] = yuvBuf;  // Y plane
                    srcPlanes[1] = yuvBuf + width * height;  // U plane
                    srcPlanes[2] = yuvBuf + width * height + (width/2) * (height/2);  // V plane
                    strides[0] = width;
                    strides[1] = width/2;
                    strides[2] = width/2;
                    
                    unsigned char *jpegBuf = nullptr;
                    size_t jpegSize = 0;
                    tjhandle handle2 = tj3Init(TJINIT_COMPRESS);
                    if (handle2) {
                        tj3CompressFromYUVPlanes8(handle2, srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
                        tj3Free(jpegBuf);
                        tj3Destroy(handle2);
                    }
                    
                    tj3Free(yuvBuf);
                }
            }
            tj3Destroy(handle1);
        }
        
        // 6. Set cropping region and decompress to 16-bit
        tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
        if (handle3) {
            // Set a cropping region (center 50% of image)
            tjregion cropRegion;
            cropRegion.x = width / 4;
            cropRegion.y = height / 4;
            cropRegion.w = width / 2;
            cropRegion.h = height / 2;
            tj3SetCroppingRegion(handle3, cropRegion);
            
            // Decompress to 16-bit buffer
            int pitch = width * 3 * sizeof(unsigned short);  // RGB, 16-bit per channel
            unsigned short *dstBuf16 = (unsigned short *)tj3Alloc(pitch * height);
            if (dstBuf16) {
                tj3Decompress16(handle3, (unsigned char *)data, size, dstBuf16, pitch, TJPF_RGB);
                
                // 7. Compress 12-bit data (using 16-bit buffer as input)
                tjhandle handle4 = tj3Init(TJINIT_COMPRESS);
                if (handle4) {
                    unsigned char *jpegBuf12 = nullptr;
                    size_t jpegSize12 = 0;
                    // Note: tj3Compress12 expects 12-bit data in 16-bit containers
                    // We'll use the 16-bit buffer but treat it as 12-bit
                    tj3Compress12(handle4, (short *)dstBuf16, width, pitch, height, TJPF_RGB, &jpegBuf12, &jpegSize12);
                    tj3Free(jpegBuf12);
                    tj3Destroy(handle4);
                }
                
                tj3Free(dstBuf16);
            }
            tj3Destroy(handle3);
        }
        
        tjFree(loadedImage);
    }
    
    // Clean up temporary files
    remove("input_file");
    assert_file_closed(&in_file);;
    
    assert_file_closed(&temp_in);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}