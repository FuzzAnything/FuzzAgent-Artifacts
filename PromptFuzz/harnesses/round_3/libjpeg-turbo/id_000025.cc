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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["tjInitDecompress","tjDecompressHeader2","tjDestroy","tjDestroy","tjPlaneHeight","tjPlaneHeight","tjPlaneHeight","tjAlloc","tjDestroy","tjDecompress2","tjFree","tjDestroy","tj3InitVersion","tjFree","tjDestroy","tj3GetScalingFactors","tj3SetScalingFactor","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3Free","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjAlloc","tjDecodeYUV","tjFree","tjFree","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjAlloc","tjAlloc","tjAlloc","tjDecodeYUVPlanes","tjFree","tjFree","tjFree","tjFree","tjFree","tj3Destroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader2","tjPlaneHeight","tjPlaneHeight","tjPlaneHeight","tjAlloc","tjDecompress2","tj3InitVersion","tj3GetScalingFactors","tj3SetScalingFactor","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3Free","tjBufSizeYUV","tjAlloc","tjEncodeYUV","tjAlloc","tjDecodeYUV","tjFree","tjFree","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjAlloc","tjAlloc","tjAlloc","tjDecodeYUVPlanes","tjFree","tjFree","tjFree","tjFree","tjFree","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<187) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_30, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_31, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_32, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_33, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_34, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_35, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_36, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_37, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_38, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_39, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_40, fuzz_int32_t_array_size_40, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_41, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_42, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_43, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_44, fdp);
	
		const int * fuzzer_var_tjDecodeYUVPlanes_38_2 = fuzz_int32_t_array_40;//fuzzer shim end}




    // Early exit if data is too small for basic JPEG operations
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    tjhandle handle3 = nullptr;
    unsigned char *rgbBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *yuvPackedBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int width = 0, height = 0;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int pitch = 0;
    int align = 4;
    int flags = 0;
    
    // 1. Initialize decompressor using old API
    handle = tjInitDecompress();
    if (!handle) {
	return 0;
	}
	
    
    // 2. Decompress JPEG header to get dimensions
    if (tjDecompressHeader2(handle, const_cast<unsigned char*>(data), 
                           static_cast<unsigned long>(size), 
                           &width, &height, &subsamp) != 0) {
        tjDestroy(handle);
        return 0;
    }
    
    // Skip if dimensions are invalid
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }
    
    // 3. Use tjPlaneHeight to get YUV plane heights
    int yHeight = tjPlaneHeight(fuzz_int32_t_1, fuzz_int32_t_2, subsamp);
    int uHeight = tjPlaneHeight(fuzz_int32_t_3, fuzz_int32_t_4, subsamp);
    int vHeight = tjPlaneHeight(fuzz_int32_t_5, fuzz_int32_t_6, subsamp);
    
    // 4. Allocate RGB buffer for decompression
    pitch = width * tjPixelSize[pixelFormat];
    rgbBuf = static_cast<unsigned char*>(tjAlloc(pitch * height));
    if (!rgbBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    // 5. Decompress JPEG to RGB
    if (tjDecompress2(handle, data, static_cast<unsigned long>(size),
                     rgbBuf, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, pixelFormat, fuzz_int32_t_10) != 0) {
        tjFree(rgbBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // 6. Initialize new API handle
    handle3 = tj3InitVersion(TJINIT_COMPRESS, fuzz_int32_t_11);
    if (!handle3) {
        tjFree(rgbBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // 7. Use tj3GetScalingFactors
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    if (scalingFactors && numScalingFactors > 0) {
        // Use the first scaling factor (usually 1/1)
        tj3SetScalingFactor(handle3, scalingFactors[0]);
    }
    
    // 8. Encode RGB to YUV using tj3EncodeYUV8 (new API)
    size_t yuvSize = tj3YUVBufSize(fuzz_int32_t_12, fuzz_int32_t_13, fuzz_int32_t_14, subsamp);
    yuvBuf = static_cast<unsigned char*>(tj3Alloc(yuvSize));
    if (yuvBuf) {
        if (tj3EncodeYUV8(handle3, rgbBuf, fuzz_int32_t_15, fuzz_int32_t_16, fuzz_int32_t_17, 
                         pixelFormat, yuvBuf, fuzz_int32_t_18) == 0) {
            // 9. Decode YUV to RGB using tj3DecodeYUV8 (new API)
            dstBuf = static_cast<unsigned char*>(tj3Alloc(pitch * height));
            if (dstBuf) {
                tj3DecodeYUV8(handle3, yuvBuf, fuzz_int32_t_19, dstBuf, 
                             fuzz_int32_t_20, fuzz_int32_t_21, fuzz_int32_t_22, pixelFormat);
                tj3Free(dstBuf);
                dstBuf = nullptr;
            }
        }
        tj3Free(yuvBuf);
        yuvBuf = nullptr;
    }
    
    // 10. Encode RGB to YUV using old API (tjEncodeYUV)
    unsigned long yuvSizeOld = tjBufSizeYUV(fuzz_int32_t_23, fuzz_int32_t_24, subsamp);
    yuvPackedBuf = static_cast<unsigned char*>(tjAlloc(yuvSizeOld));
    if (yuvPackedBuf) {
        if (tjEncodeYUV(handle, rgbBuf, fuzz_int32_t_25, fuzz_int32_t_26, fuzz_int32_t_27, 
                       tjPixelSize[pixelFormat], yuvPackedBuf, subsamp, fuzz_int32_t_28) == 0) {
            // 11. Decode YUV to RGB using old API (tjDecodeYUV)
            dstBuf = static_cast<unsigned char*>(tjAlloc(pitch * height));
            if (dstBuf) {
                tjDecodeYUV(handle, yuvPackedBuf, fuzz_int32_t_29, subsamp, dstBuf,
                           fuzz_int32_t_30, fuzz_int32_t_31, fuzz_int32_t_32, pixelFormat, fuzz_int32_t_33);
                tjFree(dstBuf);
                dstBuf = nullptr;
            }
        }
        tjFree(yuvPackedBuf);
        yuvPackedBuf = nullptr;
    }
    
    // 12. Test planar YUV operations (if supported)
    // First encode to planar YUV
    unsigned char *yPlane = nullptr;
    unsigned char *uPlane = nullptr;
    unsigned char *vPlane = nullptr;
    int strides[3] = {0};
    
    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 || subsamp == TJSAMP_444) {
        // Calculate plane sizes
        int yWidth = tjPlaneWidth(fuzz_int32_t_34, fuzz_int32_t_35, subsamp);
        int uWidth = tjPlaneWidth(fuzz_int32_t_36, fuzz_int32_t_37, subsamp);
        int vWidth = tjPlaneWidth(fuzz_int32_t_38, fuzz_int32_t_39, subsamp);
        
        strides[0] = yWidth;
        strides[1] = uWidth;
        strides[2] = vWidth;
        
        // Allocate planes
        yPlane = static_cast<unsigned char*>(tjAlloc(yWidth * yHeight));
        uPlane = static_cast<unsigned char*>(tjAlloc(uWidth * uHeight));
        vPlane = static_cast<unsigned char*>(tjAlloc(vWidth * vHeight));
        
        if (yPlane && uPlane && vPlane) {
            const unsigned char *planes[3] = {yPlane, uPlane, vPlane};
            
            // 13. Decode planar YUV to RGB using tjDecodeYUVPlanes
            dstBuf = static_cast<unsigned char*>(tjAlloc(pitch * height));
            if (dstBuf) {
                // Note: We're using random data here since we don't have actual planar YUV
                // In a real scenario, we'd need to properly encode to planar format first
                tjDecodeYUVPlanes(handle, planes, fuzzer_var_tjDecodeYUVPlanes_38_2, subsamp, dstBuf,
                                 fuzz_int32_t_41, fuzz_int32_t_42, fuzz_int32_t_43, pixelFormat, fuzz_int32_t_44);
                tjFree(dstBuf);
                dstBuf = nullptr;
            }
        }
    }
    
    // Cleanup
    if (yPlane) tjFree(yPlane);
    if (uPlane) tjFree(uPlane);
    if (vPlane) tjFree(vPlane);
    if (rgbBuf) tjFree(rgbBuf);
    if (handle3) tj3Destroy(handle3);
    if (handle) tjDestroy(handle);
    
    return 0;
}