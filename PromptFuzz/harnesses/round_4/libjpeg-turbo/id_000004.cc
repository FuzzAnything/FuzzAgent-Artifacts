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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 2
//<Quality> {"density":36,"unique_branches":{"tj3InitVersion":[[616,3,616,24,0,0,4,0],[618,9,618,16,0,0,4,1]]},"library_calls":["tj3InitVersion","tjInitTransform","tj3Destroy","tjDestroy","tjDecompressHeader3","tj3DecompressHeader","tj3Destroy","tjDestroy","tj3Get","tj3Get","tj3Get","tj3Destroy","tjDestroy","tj3Transform","tj3GetICCProfile","tj3Free","tjPlaneWidth","tjPlaneSizeYUV","tjAlloc","tjDecompressToYUVPlanes","tjAlloc","tjDecodeYUVPlanes","tjPlaneWidth","tjPlaneSizeYUV","tjAlloc","tjEncodeYUVPlanes","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tjFree","tjFree","tjFree","tj3Free","tjGetScalingFactors","tj3GetErrorStr","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitTransform","tjDecompressHeader3","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3Transform","tj3GetICCProfile","tj3Free","tjPlaneWidth","tjPlaneSizeYUV","tjAlloc","tjDecompressToYUVPlanes","tjAlloc","tjDecodeYUVPlanes","tjPlaneWidth","tjPlaneSizeYUV","tjAlloc","tjEncodeYUVPlanes","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tjFree","tjFree","tjFree","tj3Free","tjGetScalingFactors","tj3GetErrorStr","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<98) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_9, fuzz_int32_t_array_size_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_18, fuzz_int32_t_array_size_18, fdp);
	
		const int * fuzzer_var_tjDecodeYUVPlanes_21_2 = fuzz_int32_t_array_9;
		const int * fuzzer_var_tj3DecodeYUVPlanes8_27_2 = fuzz_int32_t_array_18;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Initialize TurboJPEG handles
    tjhandle handle3 = tj3InitVersion(TJINIT_DECOMPRESS, fuzz_int32_t_1);
    tjhandle handle2 = tjInitTransform();
    
    if (!handle3 || !handle2) {
        if (handle3) tj3Destroy(handle3);
        if (handle2) tjDestroy(handle2);
        return 0;
    }
    
    // Step 1: Try to decompress header to get image dimensions
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int headerResult = tjDecompressHeader3(handle2, data, size, 
                                          &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // If header decompression failed, try with newer API
    if (headerResult != 0) {
        // Try with newer API
        if (tj3DecompressHeader(handle3, data, size) != 0) {
            // Both failed, clean up and return
            tj3Destroy(handle3);
            tjDestroy(handle2);
            return 0;
        }
        width = tj3Get(handle3, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle3, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(handle3, TJPARAM_SUBSAMP);
    }
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tj3Destroy(handle3);
        tjDestroy(handle2);
        return 0;
    }
    
    // Step 2: Apply transformation using tj3Transform
    tjtransform transform;
    memset(&transform, 0, sizeof(tjtransform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    
    unsigned char *dstBuf = nullptr;
    size_t dstSize = 0;
    
    if (tj3Transform(handle3, data, size, fuzz_int32_t_2, &dstBuf, &dstSize, &transform) == 0 && dstBuf) {
        // Step 3: Get ICC profile from transformed image
        unsigned char *iccBuf = nullptr;
        size_t iccSize = 0;
        if (tj3GetICCProfile(handle3, &iccBuf, &iccSize) == 0 && iccBuf) {
            // Free ICC profile buffer
            tj3Free(iccBuf);
        }
        
        // Step 4: Decompress transformed JPEG to YUV planes using older API
        int strides[3] = {0};
        unsigned char *yuvPlanes[3] = {nullptr};
        
        // Calculate plane sizes
        for (int i = 0; i < 3; i++) {
            strides[i] = tjPlaneWidth(fuzz_int32_t_3, transform.r.w, fuzz_int32_t_4);
            size_t planeSize = tjPlaneSizeYUV(fuzz_int32_t_5, transform.r.w, strides[i], transform.r.h, fuzz_int32_t_6);
            yuvPlanes[i] = (unsigned char *)tjAlloc(planeSize);
        }
        
        if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
            // Decompress to YUV planes using older API
            tjDecompressToYUVPlanes(handle2, dstBuf, fuzz_uint64_t_7, 
                                   yuvPlanes, transform.r.w, strides, transform.r.h, fuzz_int32_t_8);
            
            // Step 5: Decode YUV planes to RGB using both APIs
            
            // First with older API: tjDecodeYUVPlanes
            int pitch = transform.r.w * tjPixelSize[TJPF_RGB];
            unsigned char *rgbBuf1 = (unsigned char *)tjAlloc(pitch * transform.r.h);
            if (rgbBuf1) {
                const unsigned char *srcPlanes[3] = {yuvPlanes[0], yuvPlanes[1], yuvPlanes[2]};
                tjDecodeYUVPlanes(handle2, srcPlanes, fuzzer_var_tjDecodeYUVPlanes_21_2, fuzz_int32_t_10,
                                 rgbBuf1, transform.r.w, pitch, transform.r.h, TJPF_RGB, fuzz_int32_t_11);
                
                // Step 6: Encode RGB back to YUV planes using older API
                unsigned char *yuvPlanes2[3] = {nullptr};
                int strides2[3] = {0};
                
                // Allocate new YUV planes
                for (int i = 0; i < 3; i++) {
                    strides2[i] = tjPlaneWidth(fuzz_int32_t_12, transform.r.w, fuzz_int32_t_13);
                    size_t planeSize = tjPlaneSizeYUV(fuzz_int32_t_14, transform.r.w, strides2[i], transform.r.h, fuzz_int32_t_15);
                    yuvPlanes2[i] = (unsigned char *)tjAlloc(planeSize);
                }
                
                if (yuvPlanes2[0] && yuvPlanes2[1] && yuvPlanes2[2]) {
                    tjEncodeYUVPlanes(handle2, rgbBuf1, transform.r.w, pitch, transform.r.h,
                                     TJPF_RGB, yuvPlanes2, strides2, fuzz_int32_t_16, fuzz_int32_t_17);
                    
                    // Step 7: Decode YUV planes with newer API: tj3DecodeYUVPlanes8
                    unsigned char *rgbBuf2 = (unsigned char *)tj3Alloc(pitch * transform.r.h);
                    if (rgbBuf2) {
                        const unsigned char *srcPlanes2[3] = {yuvPlanes2[0], yuvPlanes2[1], yuvPlanes2[2]};
                        tj3DecodeYUVPlanes8(handle3, srcPlanes2, fuzzer_var_tj3DecodeYUVPlanes8_27_2,
                                           rgbBuf2, transform.r.w, pitch, transform.r.h, TJPF_RGB);
                        
                        tj3Free(rgbBuf2);
                    }
                    
                    // Free second set of YUV planes
                    for (int i = 0; i < 3; i++) {
                        if (yuvPlanes2[i]) tjFree(yuvPlanes2[i]);
                    }
                }
                
                tjFree(rgbBuf1);
            }
        }
        
        // Free first set of YUV planes
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
        }
        
        // Free transformed buffer
        tj3Free(dstBuf);
    }
    
    // Step 8: Get scaling factors using older API
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // Step 9: Get error string using newer API (for demonstration)
    const char *errorStr = tj3GetErrorStr(handle3);
    
    // Clean up
    tj3Destroy(handle3);
    tjDestroy(handle2);
    
    return 0;
}