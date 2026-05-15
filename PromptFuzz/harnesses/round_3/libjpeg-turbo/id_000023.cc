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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 76.5, nr_unique_branch: 2
//<Quality> {"density":51,"unique_branches":{"tj3GetErrorCode":[[707,15,707,33,0,0,4,0]],"tj3GetErrorStr":[[682,15,682,36,0,0,4,1]]},"library_calls":["tj3InitVersion","tjInitDecompress","tj3Destroy","tjDecompressHeader3","tjGetErrorCode","tj3GetErrorStr","tjDestroy","tj3Destroy","tjDestroy","tj3Destroy","tj3GetScalingFactors","tj3SetScalingFactor","tj3GetErrorStr","tj3Alloc","tjDestroy","tj3Destroy","tj3Decompress12","tj3GetErrorStr","tj3Free","tjDestroy","tj3Destroy","tj3Alloc","tj3Alloc","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy","tjDecompressToYUVPlanes","tjGetErrorCode","tjBufSizeYUV2","tj3Alloc","tj3Alloc","tjDecodeYUV","tj3Free","tj3Free","tj3Compress12","tj3GetErrorStr","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tjInitDecompress","tjDecompressHeader3","tj3GetScalingFactors","tj3SetScalingFactor","tj3GetErrorStr","tj3Alloc","tj3Decompress12","tj3Alloc","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tj3Alloc","tjDecompressToYUVPlanes","tjGetErrorCode","tjBufSizeYUV2","tj3Alloc","tj3Alloc","tjDecodeYUV","tj3Free","tj3Free","tj3Compress12","tj3GetErrorStr","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Destroy"],"visited":1}
/*Here's a fuzz driver that uses the specified libjpeg-turbo APIs to achieve the event of decompressing, processing, and recompressing a JPEG image with scaling and YUV plane operations:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
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
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for JPEG header
    
    tjhandle handle = nullptr;
    tjhandle legacy_handle = nullptr;
    unsigned char* jpegBuf = nullptr;
    unsigned char* dstBuf12 = nullptr;
    unsigned char* recompressedBuf = nullptr;
    unsigned char** dstPlanes = nullptr;
    int* strides = nullptr;
    size_t jpegSize = 0;
    
    // Initialize TurboJPEG handles
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    legacy_handle = tjInitDecompress();
    if (!legacy_handle) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Try to decompress header first
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int result = tjDecompressHeader3(legacy_handle, data, size, 
                                    &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (result != 0) {
        // Get error information using required APIs
        int error_code = tjGetErrorCode(legacy_handle);
        const char* error_str = tj3GetErrorStr(handle);
        (void)error_code;  // Use variables to avoid warnings
        (void)error_str;
        
        tjDestroy(legacy_handle);
        tj3Destroy(handle);
        return 0;
    }
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(legacy_handle);
        tj3Destroy(handle);
        return 0;
    }
    
    // Get scaling factors using required API
    int numScalingFactors = 0;
    tjscalingfactor* scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    
    if (scalingFactors && numScalingFactors > 0) {
        // Use the first scaling factor
        tjscalingfactor scalingFactor = scalingFactors[0];
        
        // Set scaling factor using required API
        result = tj3SetScalingFactor(handle, scalingFactor);
        if (result != 0) {
            const char* error_str = tj3GetErrorStr(handle);
            (void)error_str;
        }
    }
    
    // Allocate buffer for 12-bit decompression
    int pitch = width * tjPixelSize[TJPF_RGB];
    dstBuf12 = (unsigned char*)tj3Alloc(pitch * height * sizeof(short));
    if (!dstBuf12) {
        tjDestroy(legacy_handle);
        tj3Destroy(handle);
        return 0;
    }
    
    // Decompress to 12-bit buffer using required API
    result = tj3Decompress12(handle, data, size, (short*)dstBuf12, pitch, TJPF_RGB);
    
    if (result != 0) {
        const char* error_str = tj3GetErrorStr(handle);
        (void)error_str;
        
        tj3Free(dstBuf12);
        tjDestroy(legacy_handle);
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate and prepare for YUV plane decompression
    int align = 4;  // Common alignment
    int planeCount = 3;  // Y, U, V planes
    dstPlanes = (unsigned char**)tj3Alloc(planeCount * sizeof(unsigned char*));
    strides = (int*)tj3Alloc(planeCount * sizeof(int));
    
    if (!dstPlanes || !strides) {
        if (dstPlanes) tj3Free(dstPlanes);
        if (strides) tj3Free(strides);
        tj3Free(dstBuf12);
        tjDestroy(legacy_handle);
        tj3Destroy(handle);
        return 0;
    }
    
    // Calculate plane sizes and allocate
    for (int i = 0; i < planeCount; i++) {
        int planeWidth = tjPlaneWidth(fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3);
        int planeHeight = tjPlaneHeight(fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6);
        strides[i] = planeWidth;
        
        size_t planeSize = tjPlaneSizeYUV(fuzz_int32_t_7, planeWidth, strides[i], planeHeight, fuzz_int32_t_8);
        dstPlanes[i] = (unsigned char*)tj3Alloc(planeSize);
        
        if (!dstPlanes[i]) {
            // Clean up allocated planes
            for (int j = 0; j < i; j++) {
                tj3Free(dstPlanes[j]);
            }
            tj3Free(dstPlanes);
            tj3Free(strides);
            tj3Free(dstBuf12);
            tjDestroy(legacy_handle);
            tj3Destroy(handle);
            return 0;
        }
    }
    
    // Decompress to YUV planes using required API
    result = tjDecompressToYUVPlanes(legacy_handle, data, size, 
                                    dstPlanes, fuzz_int32_t_9, strides, fuzz_int32_t_10, fuzz_int32_t_11);
    
    if (result != 0) {
        int error_code = tjGetErrorCode(legacy_handle);
        (void)error_code;
    }
    
    // Prepare packed YUV buffer for decoding
    size_t yuvBufSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
    unsigned char* packedYUV = (unsigned char*)tj3Alloc(yuvBufSize);
    
    if (packedYUV) {
        // Decode YUV back to RGB using required API
        unsigned char* decodedRGB = (unsigned char*)tj3Alloc(pitch * height);
        if (decodedRGB) {
            // We need to pack the planes first (simplified - just use one plane)
            // In real usage, you'd pack YUV planes properly
            result = tjDecodeYUV(legacy_handle, dstPlanes[0], fuzz_int32_t_12, fuzz_int32_t_13,
                                decodedRGB, fuzz_int32_t_14, pitch, fuzz_int32_t_15, TJPF_RGB, fuzz_int32_t_16);
            
            tj3Free(decodedRGB);
        }
        tj3Free(packedYUV);
    }
    
    // Compress 12-bit data back to JPEG using required API
    result = tj3Compress12(handle, (short*)dstBuf12, fuzz_int32_t_17, pitch, fuzz_int32_t_18, 
                          TJPF_RGB, &jpegBuf, &jpegSize);
    
    if (result != 0) {
        const char* error_str = tj3GetErrorStr(handle);
        (void)error_str;
    }
    
    // Clean up all allocated resources
    if (dstPlanes) {
        for (int i = 0; i < planeCount; i++) {
            if (dstPlanes[i]) {
                tj3Free(dstPlanes[i]);
            }
        }
        tj3Free(dstPlanes);
    }
    
    if (strides) {
        tj3Free(strides);
    }
    
    if (dstBuf12) {
        tj3Free(dstBuf12);
    }
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    tjDestroy(legacy_handle);
    tj3Destroy(handle);
    
    return 0;
}