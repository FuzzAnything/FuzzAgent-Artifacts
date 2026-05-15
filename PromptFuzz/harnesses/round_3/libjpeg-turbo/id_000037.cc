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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"tj3Decompress8":[[165,7,165,37,0,0,4,0]]},"library_calls":["tj3InitVersion","tjDecompressHeader3","tj3Alloc","tj3Decompress8","tjGetScalingFactors","tj3YUVBufSize","tj3Alloc","tjEncodeYUV","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3CompressFromYUVPlanes8","tj3JPEGBufSize","tj3Alloc","tjCompress","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tjDecompressHeader3","tj3Alloc","tj3Decompress8","tjGetScalingFactors","tj3YUVBufSize","tj3Alloc","tjEncodeYUV","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3Alloc","tj3DecodeYUV8","tj3Free","tj3CompressFromYUVPlanes8","tj3JPEGBufSize","tj3Alloc","tjCompress","tjCompressFromYUVPlanes","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	
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
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_8, fuzz_int32_t_array_size_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_11, fuzz_int32_t_array_size_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_16_3 = fuzz_int32_t_array_8;
		const int * fuzzer_var_tjCompressFromYUVPlanes_20_3 = fuzz_int32_t_array_11;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful JPEG data
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *rgbBuf = nullptr;
    unsigned char *compressedBuf = nullptr;
    unsigned char *planes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    
    // Initialize handle using newer API
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // First, try to decompress the input as JPEG using newer API
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(handle, data, size, &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        // If not valid JPEG, create synthetic data
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }
    
    // Allocate RGB buffer for decompression
    int pitch = width * tjPixelSize[TJPF_RGB];
    rgbBuf = (unsigned char *)tj3Alloc(pitch * height);
    if (!rgbBuf) goto cleanup;
    
    // Decompress using newer API (tj3Decompress8)
    if (tj3Decompress8(handle, data, size, rgbBuf, pitch, TJPF_RGB) == 0) {
        // Get scaling factors using older API
        int numScalingFactors;
        tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
        
        if (scalingFactors && numScalingFactors > 0) {
            // Use first scaling factor
            tjscalingfactor sf = scalingFactors[0];
            
            // Calculate YUV buffer size and allocate
            int align = 4;
            size_t yuvSize = tj3YUVBufSize(width, fuzz_int32_t_1, height, jpegSubsamp);
            yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
            if (!yuvBuf) goto cleanup;
            
            // Encode RGB to YUV using older API (tjEncodeYUV)
            if (tjEncodeYUV(handle, rgbBuf, width, pitch, height, 
                           tjPixelSize[TJPF_RGB], yuvBuf, jpegSubsamp, fuzz_int32_t_2) == 0) {
                
                // Prepare YUV planes for newer API
                int planeWidth = tj3YUVPlaneWidth(fuzz_int32_t_3, width, jpegSubsamp);
                int planeHeight = tj3YUVPlaneHeight(fuzz_int32_t_4, height, jpegSubsamp);
                
                // Allocate plane buffers
                for (int i = 0; i < 3; i++) {
                    int stride = tj3YUVPlaneWidth(fuzz_int32_t_5, width, jpegSubsamp);
                    int pHeight = tj3YUVPlaneHeight(fuzz_int32_t_6, height, jpegSubsamp);
                    size_t planeSize = stride * pHeight;
                    planes[i] = (unsigned char *)tj3Alloc(planeSize);
                    strides[i] = stride;
                }
                
                // Decode YUV to RGB using newer API (tj3DecodeYUV8)
                unsigned char *rgbBuf2 = (unsigned char *)tj3Alloc(pitch * height);
                if (rgbBuf2) {
                    tj3DecodeYUV8(handle, yuvBuf, fuzz_int32_t_7, rgbBuf2, width, pitch, height, TJPF_RGB);
                    tj3Free(rgbBuf2);
                }
                
                // Now compress from YUV planes using newer API (tj3CompressFromYUVPlanes8)
                size_t jpegSize;
                if (tj3CompressFromYUVPlanes8(handle, (const unsigned char **)planes, 
                                             width, fuzzer_var_tj3CompressFromYUVPlanes8_16_3, height, 
                                             &jpegBuf, &jpegSize) == 0) {
                    
                    // Also compress using older API (tjCompressFromYUVPlanes)
                    unsigned char *jpegBuf2 = nullptr;
                    unsigned long jpegSize2 = 0;
                    
                    // First get buffer size using newer API
                    size_t bufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
                    compressedBuf = (unsigned char *)tj3Alloc(bufSize);
                    
                    if (compressedBuf) {
                        unsigned long compressedSize = bufSize;
                        
                        // Compress using older API (tjCompress)
                        if (tjCompress(handle, rgbBuf, width, pitch, height, 
                                      tjPixelSize[TJPF_RGB], compressedBuf, 
                                      &compressedSize, jpegSubsamp, fuzz_int32_t_9, fuzz_int32_t_10) == 0) {
                            
                            // Finally compress from YUV planes using older API
                            tjCompressFromYUVPlanes(handle, (const unsigned char **)planes, 
                                                   width, fuzzer_var_tjCompressFromYUVPlanes_20_3, height, jpegSubsamp, 
                                                   &jpegBuf2, &jpegSize2, fuzz_int32_t_12, fuzz_int32_t_13);
                            
                            if (jpegBuf2) tjFree(jpegBuf2);
                        }
                    }
                }
            }
        }
    }
    
cleanup:
    // Free all allocated resources
    if (planes[0]) tj3Free(planes[0]);
    if (planes[1]) tj3Free(planes[1]);
    if (planes[2]) tj3Free(planes[2]);
    if (jpegBuf) tj3Free(jpegBuf);
    if (yuvBuf) tj3Free(yuvBuf);
    if (rgbBuf) tj3Free(rgbBuf);
    if (compressedBuf) tj3Free(compressedBuf);
    if (handle) tj3Destroy(handle);
    
    return 0;
}