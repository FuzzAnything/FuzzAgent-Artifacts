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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 0
//<Quality> {"density":39,"unique_branches":{},"library_calls":["tj3InitVersion","tjInitTransform","tj3Destroy","tjDestroy","tj3Set","tj3Set","tj3Alloc","tj3Destroy","tjDestroy","tjDecompressHeader3","tj3Alloc","tj3Decompress16","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Alloc","tj3Compress8","tj3Free","tj3Free","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3Alloc","tj3Alloc","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Alloc","tjCompress2","tjFree","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitTransform","tj3Set","tj3Set","tj3Alloc","tjDecompressHeader3","tj3Alloc","tj3Decompress16","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Alloc","tj3Compress8","tj3Free","tj3Free","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tj3Alloc","tj3Alloc","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Alloc","tjCompress2","tjFree","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs to achieve the event of decompressing and recompressing JPEG images with different bit depths and formats:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
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
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Initialize handles
    tjhandle handle_new = tj3InitVersion(TJINIT_COMPRESS | TJINIT_DECOMPRESS, fuzz_int32_t_1);
    tjhandle handle_old = tjInitTransform();
    
    if (!handle_new || !handle_old) {
        if (handle_new) tj3Destroy(handle_new);
        if (handle_old) tjDestroy(handle_old);
        return 0;
    }
    
    // Set some parameters
    tj3Set(handle_new, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle_new, TJPARAM_QUALITY, fuzz_int32_t_2);
    
    // Variables for decompression
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = size;
    
    // Allocate buffer for input data
    jpegBuf = (unsigned char *)tj3Alloc(size);
    if (!jpegBuf) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        return 0;
    }
    memcpy(jpegBuf, data, size);
    
    // 1. First try to decompress using tj3Decompress16
    // Get header info first
    int result = tjDecompressHeader3(handle_old, jpegBuf, (unsigned long)size, 
                                     &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (result == 0 && width > 0 && height > 0) {
        // Limit dimensions for performance
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
        
        int pitch = width * tjPixelSize[TJPF_RGB];
        unsigned short *dstBuf16 = (unsigned short *)tj3Alloc(pitch * height * sizeof(unsigned short));
        
        if (dstBuf16) {
            // Call tj3Decompress16
            result = tj3Decompress16(handle_new, jpegBuf, jpegSize, dstBuf16, pitch, TJPF_RGB);
            
            if (result == 0) {
                // 2. Now compress the 16-bit data using tj3Compress12
                // We need to convert 16-bit to 12-bit data
                int numPixels = width * height * 3; // RGB
                short *srcBuf12 = (short *)tj3Alloc(numPixels * sizeof(short));
                
                if (srcBuf12) {
                    // Convert 16-bit to 12-bit (scale down)
                    for (int i = 0; i < numPixels; i++) {
                        srcBuf12[i] = (short)(dstBuf16[i] >> 4); // 16-bit to 12-bit
                    }
                    
                    unsigned char *jpegBuf12 = nullptr;
                    size_t jpegSize12 = 0;
                    
                    // Call tj3Compress12
                    result = tj3Compress12(handle_new, srcBuf12, width, width * 3 * sizeof(short), 
                                          height, TJPF_RGB, &jpegBuf12, &jpegSize12);
                    
                    if (jpegBuf12) {
                        tj3Free(jpegBuf12);
                    }
                    
                    tj3Free(srcBuf12);
                }
                
                // 3. Also compress using tj3Compress8 (convert 16-bit to 8-bit)
                unsigned char *srcBuf8 = (unsigned char *)tj3Alloc(width * height * 3);
                
                if (srcBuf8) {
                    // Convert 16-bit to 8-bit
                    for (int i = 0; i < width * height * 3; i++) {
                        srcBuf8[i] = (unsigned char)(dstBuf16[i] >> 8);
                    }
                    
                    unsigned char *jpegBuf8 = nullptr;
                    size_t jpegSize8 = 0;
                    
                    // Call tj3Compress8
                    result = tj3Compress8(handle_new, srcBuf8, width, width * 3, 
                                         height, TJPF_RGB, &jpegBuf8, &jpegSize8);
                    
                    if (jpegBuf8) {
                        tj3Free(jpegBuf8);
                    }
                    
                    tj3Free(srcBuf8);
                }
                
                // 4. Try YUV compression using tj3CompressFromYUV8
                // First encode to YUV
                size_t yuvBufSize = tj3YUVBufSize(width, fuzz_int32_t_3, height, TJSAMP_420);
                unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
                
                if (yuvBuf) {
                    result = tj3EncodeYUV8(handle_new, (const unsigned char *)dstBuf16, 
                                          width, width * 3 * sizeof(unsigned short), 
                                          height, TJPF_RGB, yuvBuf, fuzz_int32_t_4);
                    
                    if (result == 0) {
                        unsigned char *jpegBufYUV = nullptr;
                        size_t jpegSizeYUV = 0;
                        
                        // Call tj3CompressFromYUV8
                        result = tj3CompressFromYUV8(handle_new, yuvBuf, width, fuzz_int32_t_5, 
                                                    height, &jpegBufYUV, &jpegSizeYUV);
                        
                        if (jpegBufYUV) {
                            tj3Free(jpegBufYUV);
                        }
                    }
                    
                    tj3Free(yuvBuf);
                }
                
                // 5. Try YUV planes compression using tjCompressFromYUVPlanes
                // Prepare YUV planes
                int strides[3];
                unsigned char *yuvPlanes[3];
                
                int yWidth = tjPlaneWidth(fuzz_int32_t_6, width, TJSAMP_420);
                int yHeight = tjPlaneHeight(fuzz_int32_t_7, height, TJSAMP_420);
                int uvWidth = tjPlaneWidth(fuzz_int32_t_8, width, TJSAMP_420);
                int uvHeight = tjPlaneHeight(fuzz_int32_t_9, height, TJSAMP_420);
                
                strides[0] = yWidth;
                strides[1] = uvWidth;
                strides[2] = uvWidth;
                
                yuvPlanes[0] = (unsigned char *)tj3Alloc(yWidth * yHeight);
                yuvPlanes[1] = (unsigned char *)tj3Alloc(uvWidth * uvHeight);
                yuvPlanes[2] = (unsigned char *)tj3Alloc(uvWidth * uvHeight);
                
                if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
                    // Fill with some data (simplified - in real case would convert from RGB)
                    memset(yuvPlanes[0], 128, yWidth * yHeight);
                    memset(yuvPlanes[1], 128, uvWidth * uvHeight);
                    memset(yuvPlanes[2], 128, uvWidth * uvHeight);
                    
                    unsigned char *jpegBufPlanes = nullptr;
                    unsigned long jpegSizePlanes = 0;
                    
                    // Call tjCompressFromYUVPlanes
                    result = tjCompressFromYUVPlanes(handle_old, 
                                                    (const unsigned char **)yuvPlanes, 
                                                    width, strides, height, 
                                                    TJSAMP_420, &jpegBufPlanes, 
                                                    &jpegSizePlanes, fuzz_int32_t_10, fuzz_int32_t_11);
                    
                    if (jpegBufPlanes) {
                        tjFree(jpegBufPlanes);
                    }
                }
                
                if (yuvPlanes[0]) tj3Free(yuvPlanes[0]);
                if (yuvPlanes[1]) tj3Free(yuvPlanes[1]);
                if (yuvPlanes[2]) tj3Free(yuvPlanes[2]);
            }
            
            tj3Free(dstBuf16);
        }
        
        // 6. Try tjCompress2 with 8-bit data
        unsigned char *testBuf8 = (unsigned char *)tj3Alloc(width * height * 3);
        if (testBuf8) {
            // Fill with pattern
            for (int i = 0; i < width * height * 3; i++) {
                testBuf8[i] = (unsigned char)(i % 256);
            }
            
            unsigned char *jpegBuf2 = nullptr;
            unsigned long jpegSize2 = 0;
            
            // Call tjCompress2
            result = tjCompress2(handle_old, testBuf8, width, width * 3, 
                                height, TJPF_RGB, &jpegBuf2, &jpegSize2, 
                                TJSAMP_420, fuzz_int32_t_12, fuzz_int32_t_13);
            
            if (jpegBuf2) {
                tjFree(jpegBuf2);
            }
            
            tj3Free(testBuf8);
        }
    }
    
    // Cleanup
    if (jpegBuf) tj3Free(jpegBuf);
    if (handle_new) tj3Destroy(handle_new);
    if (handle_old) tjDestroy(handle_old);
    
    return 0;
}