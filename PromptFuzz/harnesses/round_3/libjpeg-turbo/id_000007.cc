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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 15.5, nr_unique_branch: 0
//<Quality> {"density":31,"unique_branches":{},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjDecompressHeader","tjDestroy","tjDestroy","tjAlloc","tjDestroy","tjDecompress","tjFree","tjDestroy","tj3GetScalingFactors","tjGetScalingFactors","tj3SetScalingFactor","tjBufSizeYUV2","tjAlloc","tjFree","tjDestroy","tj3DecodeYUV8","tjEncodeYUV2","tj3DecodeYUV8","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tj3CompressFromYUVPlanes8","tjFree","tjFree","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader3","tjDecompressHeader","tjAlloc","tjDecompress","tj3GetScalingFactors","tjGetScalingFactors","tj3SetScalingFactor","tjBufSizeYUV2","tjAlloc","tj3DecodeYUV8","tjEncodeYUV2","tj3DecodeYUV8","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tj3CompressFromYUVPlanes8","tjFree","tjFree","tjFree","tjFree","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<63) return 0;

	
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
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_13, fuzz_int32_t_array_size_13, fdp);
	
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_28_3 = fuzz_int32_t_array_13;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small to be meaningful
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *compressedBuf = nullptr;
    size_t compressedSize = 0;
    
    // Initialize TurboJPEG handle for both old and new APIs
    handle = tjInitDecompress();
    if (!handle) {
	return 0;
	}
	
    
    // First, try to decompress the input JPEG to get basic info
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(handle, data, size, &width, &height, 
                           &jpegSubsamp, &jpegColorspace) != 0) {
        // If header parsing fails, try with old API
        if (tjDecompressHeader(handle, (unsigned char *)data, size, 
                              &width, &height) != 0) {
            tjDestroy(handle);
            return 0;
        }
    }
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }
    
    // Calculate buffer sizes
    int pixelSize = 3; // RGB
    int pitch = width * pixelSize;
    unsigned long dstBufSize = width * height * pixelSize;
    
    // Allocate destination buffer for decompression
    dstBuf = (unsigned char *)tjAlloc(dstBufSize);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    // Decompress using old API (tjDecompress)
    int flags = 0;
    if (tjDecompress(handle, (unsigned char *)data, size, dstBuf, 
                     width, pitch, height, fuzz_int32_t_1, fuzz_int32_t_2) != 0) {
        tjFree(dstBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // Get scaling factors using both old and new APIs
    int numFactors = 0;
    tjscalingfactor *factors3 = tj3GetScalingFactors(&numFactors);
    tjscalingfactor *factorsOld = tjGetScalingFactors(&numFactors);
    
    if (factors3 && factorsOld && numFactors > 0) {
        // Set a scaling factor using new API
        tj3SetScalingFactor(handle, factors3[0]);
    }
    
    // Allocate YUV buffer
    int align = 4;
    size_t yuvSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
    if (yuvSize == 0) yuvSize = width * height * 2; // Fallback
    yuvBuf = (unsigned char *)tjAlloc(yuvSize);
    if (!yuvBuf) {
        tjFree(dstBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // Decode to YUV using new API
    // Note: We're using the decompressed RGB buffer as source
    if (tj3DecodeYUV8(handle, dstBuf, fuzz_int32_t_3, yuvBuf, 
                      width, pitch, height, TJPF_RGB) != 0) {
        // Try alternative approach
        // Encode to YUV first, then decode back
        if (tjEncodeYUV2(handle, dstBuf, width, pitch, height, 
                         TJPF_RGB, yuvBuf, jpegSubsamp, fuzz_int32_t_4) == 0) {
            // Now decode the YUV we just created
            tj3DecodeYUV8(handle, yuvBuf, fuzz_int32_t_5, dstBuf, 
                          width, pitch, height, TJPF_RGB);
        }
    }
    
    // Prepare YUV planes for compression
    int strides[3] = {0};
    int yuvWidth = tjPlaneWidth(fuzz_int32_t_6, width, jpegSubsamp);
    int yuvHeight = tjPlaneHeight(fuzz_int32_t_7, height, jpegSubsamp);
    strides[0] = yuvWidth;
    strides[1] = tjPlaneWidth(fuzz_int32_t_8, width, jpegSubsamp);
    strides[2] = tjPlaneWidth(fuzz_int32_t_9, width, jpegSubsamp);
    
    // Allocate plane buffers
    unsigned char *planes[3] = {nullptr};
    for (int i = 0; i < 3; i++) {
        int planeW = tjPlaneWidth(fuzz_int32_t_10, width, jpegSubsamp);
        int planeH = tjPlaneHeight(fuzz_int32_t_11, height, jpegSubsamp);
        size_t planeSize = tjPlaneSizeYUV(fuzz_int32_t_12, planeW, strides[i], planeH, jpegSubsamp);
        if (planeSize > 0) {
            planes[i] = (unsigned char *)tjAlloc(planeSize);
            if (planes[i]) {
                // Fill with some data (simplified - in real fuzzer would use actual YUV data)
                memset(planes[i], 128, planeSize);
            }
        }
    }
    
    // Compress from YUV planes using new API
    if (planes[0] && planes[1] && planes[2]) {
        const unsigned char *const srcPlanes[3] = {planes[0], planes[1], planes[2]};
        tj3CompressFromYUVPlanes8(handle, srcPlanes, width, fuzzer_var_tj3CompressFromYUVPlanes8_28_3, 
                                  height, &compressedBuf, &compressedSize);
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (planes[i]) tjFree(planes[i]);
    }
    if (compressedBuf) tjFree(compressedBuf);
    if (yuvBuf) tjFree(yuvBuf);
    if (dstBuf) tjFree(dstBuf);
    tjDestroy(handle);
    
    return 0;
}