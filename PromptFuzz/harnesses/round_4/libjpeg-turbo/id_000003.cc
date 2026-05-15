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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 93, nr_unique_branch: 2
//<Quality> {"density":31,"unique_branches":{"tj3InitVersion":[[611,19,611,40,0,0,4,0]],"tjDecompressHeader3":[[340,7,340,12,1,0,4,0]]},"library_calls":["tj3InitVersion","tjDecompressHeader3","tj3Destroy","tj3SetICCProfile","tjInitDecompress","tj3Destroy","tjBufSizeYUV2","tjDestroy","tj3Destroy","tjAlloc","tjDestroy","tj3Destroy","tjDecompressToYUV2","tjFree","tjDestroy","tj3Destroy","tjPlaneSizeYUV","tjPlaneSizeYUV","tjPlaneSizeYUV","tjFree","tjDestroy","tj3Destroy","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjFree","tjDestroy","tj3Destroy","tjDecodeYUVPlanes","tjInitCompress","tjAlloc","tjEncodeYUV","tjCompressFromYUV","tjFree","tjFree","tjDestroy","tjFree","tjFree","tjDestroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tjDecompressHeader3","tj3SetICCProfile","tjInitDecompress","tjBufSizeYUV2","tjAlloc","tjDecompressToYUV2","tjPlaneSizeYUV","tjPlaneSizeYUV","tjPlaneSizeYUV","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjDecodeYUVPlanes","tjInitCompress","tjAlloc","tjEncodeYUV","tjCompressFromYUV","tjFree","tjFree","tjDestroy","tjFree","tjFree","tjDestroy","tj3Destroy"],"visited":0}
/**/


// Include libjpeg-turbo headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for meaningful operations

    // Initialize handles for both old and new APIs
    tjhandle new_handle = nullptr;
    tjhandle old_decompress_handle = nullptr;
    tjhandle old_compress_handle = nullptr;
    
    // Step 1: Use new API to decompress header
    new_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!new_handle) {
	return 0;
	}
	
    
    int width, height, jpegSubsamp, jpegColorspace;
    int result = tjDecompressHeader3(old_decompress_handle, data, size, 
                                     &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (result != 0 || width <= 0 || height <= 0) {
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 2: Set ICC profile from input data (use first portion as fake ICC)
    size_t icc_size = size > 200 ? 200 : size / 2;
    unsigned char* icc_buf = (unsigned char*)malloc(icc_size);
    if (icc_buf) {
        memcpy(icc_buf, data, icc_size);
        tj3SetICCProfile(new_handle, icc_buf, icc_size);
        free(icc_buf);
    }
    
    // Step 3: Decompress JPEG to YUV planes using old API
    old_decompress_handle = tjInitDecompress();
    if (!old_decompress_handle) {
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Calculate YUV buffer size and allocate
    int align = 1;
    unsigned long yuv_buf_size = tjBufSizeYUV2(width, fuzz_int32_t_1, height, jpegSubsamp);
    if (yuv_buf_size == 0) {
        tjDestroy(old_decompress_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    unsigned char* yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
    if (!yuv_buf) {
        tjDestroy(old_decompress_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Decompress to YUV
    result = tjDecompressToYUV2(old_decompress_handle, data, size, 
                                yuv_buf, width, fuzz_int32_t_2, height, fuzz_int32_t_3);
    if (result != 0) {
        tjFree(yuv_buf);
        tjDestroy(old_decompress_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 4: Prepare YUV planes for tjDecodeYUVPlanes
    // Calculate plane dimensions
    int strides[3];
    const unsigned char* src_planes[3];
    
    // For simplicity, use planar YUV format
    int y_plane_size = tjPlaneSizeYUV(fuzz_int32_t_4, width, fuzz_int32_t_5, height, jpegSubsamp);
    int u_plane_size = tjPlaneSizeYUV(fuzz_int32_t_6, width, fuzz_int32_t_7, height, jpegSubsamp);
    int v_plane_size = tjPlaneSizeYUV(fuzz_int32_t_8, width, fuzz_int32_t_9, height, jpegSubsamp);
    
    if (y_plane_size <= 0 || u_plane_size <= 0 || v_plane_size <= 0) {
        tjFree(yuv_buf);
        tjDestroy(old_decompress_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Split YUV buffer into planes (simplified - actual layout depends on subsampling)
    unsigned char* y_plane = yuv_buf;
    unsigned char* u_plane = yuv_buf + y_plane_size;
    unsigned char* v_plane = u_plane + u_plane_size;
    
    src_planes[0] = y_plane;
    src_planes[1] = u_plane;
    src_planes[2] = v_plane;
    
    strides[0] = tjPlaneWidth(fuzz_int32_t_10, width, jpegSubsamp);
    strides[1] = tjPlaneWidth(fuzz_int32_t_11, width, jpegSubsamp);
    strides[2] = tjPlaneWidth(fuzz_int32_t_12, width, jpegSubsamp);
    
    // Allocate RGB buffer for decoded output
    int pixel_format = TJPF_RGB;
    int pitch = width * tjPixelSize[pixel_format];
    unsigned char* rgb_buf = (unsigned char*)tjAlloc(pitch * height);
    if (!rgb_buf) {
        tjFree(yuv_buf);
        tjDestroy(old_decompress_handle);
        tj3Destroy(new_handle);
        return 0;
    }
    
    // Step 5: Decode YUV planes to RGB using old API
    result = tjDecodeYUVPlanes(old_decompress_handle, src_planes, strides, 
                               jpegSubsamp, rgb_buf, width, pitch, height, 
                               pixel_format, fuzz_int32_t_13);
    
    // Step 6: Encode RGB back to YUV using old API
    old_compress_handle = tjInitCompress();
    if (old_compress_handle) {
        // Allocate YUV buffer for encoding
        unsigned char* encoded_yuv = (unsigned char*)tjAlloc(yuv_buf_size);
        if (encoded_yuv) {
            // Use different subsampling for variety
            int encode_subsamp = TJSAMP_420;
            result = tjEncodeYUV(old_compress_handle, rgb_buf, width, pitch, 
                                 height, tjPixelSize[pixel_format], encoded_yuv, 
                                 encode_subsamp, fuzz_int32_t_14);
            
            // Step 7: Compress from YUV back to JPEG using old API
            if (result == 0) {
                unsigned char* jpeg_buf = nullptr;
                unsigned long jpeg_size = 0;
                int jpeg_qual = 85; // Medium quality
                
                result = tjCompressFromYUV(old_compress_handle, encoded_yuv, 
                                           width, fuzz_int32_t_15, height, encode_subsamp,
                                           &jpeg_buf, &jpeg_size, fuzz_int32_t_16, fuzz_int32_t_17);
                
                if (jpeg_buf) {
                    tjFree(jpeg_buf);
                }
            }
            
            tjFree(encoded_yuv);
        }
        tjDestroy(old_compress_handle);
    }
    
    // Cleanup
    tjFree(rgb_buf);
    tjFree(yuv_buf);
    tjDestroy(old_decompress_handle);
    tj3Destroy(new_handle);
    
    return 0;
}