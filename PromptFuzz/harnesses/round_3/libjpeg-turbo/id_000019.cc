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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"tj3SetICCProfile":[[1230,7,1230,35,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3SetICCProfile","tj3JPEGBufSize","tjBufSizeYUV2","tjAlloc","tjEncodeYUV3","tj3Alloc","tj3DecodeYUV8","tj3SaveImage8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3YUVPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tjCompressFromYUVPlanes","tjFree","tj3Free","tjFree","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3SetICCProfile","tj3JPEGBufSize","tjBufSizeYUV2","tjAlloc","tjEncodeYUV3","tj3Alloc","tj3DecodeYUV8","tj3SaveImage8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneSize","tj3YUVPlaneHeight","tj3Alloc","tj3EncodeYUVPlanes8","tjCompressFromYUVPlanes","tjFree","tj3Free","tjFree","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<106) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    tjhandle handle = nullptr;
    int width = 64, height = 64;  // Fixed dimensions for fuzzing
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    int align = 4;
    int flags = 0;
    int jpegQual = 85;
    
    // Initialize handle
    handle = tj3Init(TJINIT_COMPRESS | TJINIT_DECOMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // Split input data for different purposes
    size_t offset = 0;
    
    // 1. Use part of data as ICC profile
    size_t icc_size = size / 10;  // Use 10% for ICC
    if (icc_size > 0) {
        unsigned char* icc_buf = (unsigned char*)data;
        tj3SetICCProfile(handle, icc_buf, icc_size);
        offset += icc_size;
    }
    
    // 2. Create source buffer for encoding
    size_t src_size = width * height * tjPixelSize[pixelFormat];
    unsigned char* src_buf = nullptr;
    if (offset + src_size <= size) {
        src_buf = (unsigned char*)data + offset;
        offset += src_size;
    } else {
        // Not enough data, use what we have
        src_buf = (unsigned char*)data + offset;
        src_size = size - offset;
        offset = size;
    }
    
    // 3. Calculate JPEG buffer size using tj3JPEGBufSize
    size_t jpeg_buf_size = tj3JPEGBufSize(fuzz_int32_t_1, fuzz_int32_t_2, subsamp);
    
    // 4. Encode to YUV using tjEncodeYUV3
    size_t yuv_buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    unsigned char* yuv_buf = nullptr;
    if (yuv_buf_size > 0) {
        yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
        if (yuv_buf) {
            if (tjEncodeYUV3(handle, src_buf, fuzz_int32_t_3, width*tjPixelSize[pixelFormat], 
                           fuzz_int32_t_4, pixelFormat, yuv_buf, fuzz_int32_t_5, subsamp, fuzz_int32_t_6) == 0) {
                
                // 5. Decode YUV back using tj3DecodeYUV8
                unsigned char* decoded_buf = (unsigned char*)tj3Alloc(src_size);
                if (decoded_buf) {
                    tj3DecodeYUV8(handle, yuv_buf, fuzz_int32_t_7, decoded_buf, 
                                fuzz_int32_t_8, width*tjPixelSize[pixelFormat], fuzz_int32_t_9, pixelFormat);
                    
                    // 6. Save decoded image using tj3SaveImage8
                    tj3SaveImage8(handle, fuzz_str_10, decoded_buf, 
                                fuzz_int32_t_11, width*tjPixelSize[pixelFormat], fuzz_int32_t_12, pixelFormat);
                    
                    tj3Free(decoded_buf);
                }
                
                // 7. Encode to YUV planes using tj3EncodeYUVPlanes8
                int strides[3];
                unsigned char* planes[3];
                
                // Allocate planes
                for (int i = 0; i < 3; i++) {
                    strides[i] = tj3YUVPlaneWidth(fuzz_int32_t_13, fuzz_int32_t_14, subsamp);
                    size_t plane_size = tj3YUVPlaneSize(fuzz_int32_t_15, fuzz_int32_t_16, strides[i], 
                                                      tj3YUVPlaneHeight(fuzz_int32_t_17, fuzz_int32_t_18, subsamp), subsamp);
                    planes[i] = (unsigned char*)tj3Alloc(plane_size);
                }
                
                if (planes[0] && planes[1] && planes[2]) {
                    tj3EncodeYUVPlanes8(handle, src_buf, fuzz_int32_t_19, 
                                      width*tjPixelSize[pixelFormat], fuzz_int32_t_20, 
                                      pixelFormat, planes, strides);
                    
                    // 8. Compress from YUV planes using tjCompressFromYUVPlanes
                    unsigned char* jpeg_buf = nullptr;
                    unsigned long jpeg_size = 0;
                    
                    if (tjCompressFromYUVPlanes(handle, (const unsigned char**)planes, 
                                              fuzz_int32_t_21, strides, fuzz_int32_t_22, subsamp, 
                                              &jpeg_buf, &jpeg_size, fuzz_int32_t_23, fuzz_int32_t_24) == 0) {
                        tjFree(jpeg_buf);
                    }
                }
                
                // Free planes
                for (int i = 0; i < 3; i++) {
                    if (planes[i]) tj3Free(planes[i]);
                }
            }
            tjFree(yuv_buf);
        }
    }
    
    // Clean up
    tj3Destroy(handle);
    
    return 0;
}