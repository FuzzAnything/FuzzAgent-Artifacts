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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 0
//<Quality> {"density":42,"unique_branches":{},"library_calls":["tjInitCompress","tjInitDecompress","tjInitTransform","tjDestroy","tjDestroy","tjDestroy","tjDecompressHeader3","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjBufSizeYUV","tjBufSizeYUV2","tj3YUVPlaneWidth","tjDecompressToYUVPlanes","tjAlloc","tjFree","tjDestroy","tjDestroy","tjDestroy","tjDecompressToYUVPlanes","tjBufSizeYUV2","tjAlloc","tjEncodeYUV3","tjAlloc","tj3DecodeYUV8","tjAlloc","tjDecompress","tjSaveImage","tjFree","tjCompressFromYUV","tj3Transform","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tjInitTransform","tjDecompressHeader3","tjBufSizeYUV","tjBufSizeYUV2","tj3YUVPlaneWidth","tjDecompressToYUVPlanes","tjAlloc","tjDecompressToYUVPlanes","tjBufSizeYUV2","tjAlloc","tjEncodeYUV3","tjAlloc","tj3DecodeYUV8","tjAlloc","tjDecompress","tjSaveImage","tjFree","tjCompressFromYUV","tj3Transform","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
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
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    // Initialize handles
    tjhandle handleCompress = tjInitCompress();
    tjhandle handleDecompress = tjInitDecompress();
    tjhandle handleTransform = tjInitTransform();
    if (!handleCompress || !handleDecompress || !handleTransform) {
        if (handleCompress) tjDestroy(handleCompress);
        if (handleDecompress) tjDestroy(handleDecompress);
        if (handleTransform) tjDestroy(handleTransform);
        return 0;
    }

    // Use input as JPEG data
    const unsigned char *jpegBuf = data;
    unsigned long jpegSize = size;
    
    // Get JPEG header info
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(handleDecompress, jpegBuf, jpegSize, 
                           &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        tjDestroy(handleCompress);
        tjDestroy(handleDecompress);
        tjDestroy(handleTransform);
        return 0;
    }

    // Limit dimensions to avoid excessive memory usage
    if (width > 4096 || height > 4096 || width <= 0 || height <= 0) {
        tjDestroy(handleCompress);
        tjDestroy(handleDecompress);
        tjDestroy(handleTransform);
        return 0;
    }

    // Calculate YUV buffer sizes using different APIs
    unsigned long yuvBufSize1 = tjBufSizeYUV(width, height, jpegSubsamp);
    unsigned long yuvBufSize2 = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
    
    // Decompress to YUV planes
    int strides[3] = {0};
    unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
    
    // Calculate plane widths
    for (int i = 0; i < 3; i++) {
        int planeWidth = tj3YUVPlaneWidth(fuzz_int32_t_1, width, jpegSubsamp);
        strides[i] = planeWidth;
    }
    
    if (tjDecompressToYUVPlanes(handleDecompress, jpegBuf, jpegSize,
                                yuvPlanes, width, strides, height, fuzz_int32_t_2) == 0) {
        // Allocate YUV planar buffers
        for (int i = 0; i < 3; i++) {
            if (strides[i] > 0 && height > 0) {
                yuvPlanes[i] = (unsigned char *)tjAlloc(strides[i] * height);
                if (!yuvPlanes[i]) {
                    for (int j = 0; j < i; j++) tjFree(yuvPlanes[j]);
                    tjDestroy(handleCompress);
                    tjDestroy(handleDecompress);
                    tjDestroy(handleTransform);
                    return 0;
                }
            }
        }
        
        // Decompress to YUV planes again with allocated buffers
        if (tjDecompressToYUVPlanes(handleDecompress, jpegBuf, jpegSize,
                                    yuvPlanes, width, strides, height, fuzz_int32_t_3) == 0) {
            
            // Create a single YUV buffer for tj3DecodeYUV8
            int align = 4;
            unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
            unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvBufSize);
            if (yuvBuf) {
                // Encode YUV planes to packed YUV format
                if (tjEncodeYUV3(handleCompress, (const unsigned char *)data, 
                                 width, fuzz_int32_t_4, height, TJPF_RGB, yuvBuf, fuzz_int32_t_5, 
                                 jpegSubsamp, fuzz_int32_t_6) == 0) {
                    
                    // Decode YUV back to RGB using tj3DecodeYUV8
                    int pixelFormat = TJPF_RGB;
                    int pitch = width * tjPixelSize[pixelFormat];
                    unsigned char *rgbBuf = (unsigned char *)tjAlloc(pitch * height);
                    if (rgbBuf) {
                        tj3DecodeYUV8(handleDecompress, yuvBuf, fuzz_int32_t_7, 
                                      rgbBuf, width, pitch, height, pixelFormat);
                        
                        // Decompress directly using tjDecompress
                        unsigned char *rgbBuf2 = (unsigned char *)tjAlloc(pitch * height);
                        if (rgbBuf2) {
                            tjDecompress(handleDecompress, (unsigned char *)jpegBuf, jpegSize,
                                         rgbBuf2, width, pitch, height, 
                                         tjPixelSize[pixelFormat], fuzz_int32_t_8);
                            
                            // Save image to file
                            tjSaveImage("output_file", rgbBuf, width, pitch, 
                                        height, pixelFormat, fuzz_int32_t_9);
                            
                            tjFree(rgbBuf2);
                        }
                        
                        // Compress from YUV
                        unsigned char *recompressedBuf = nullptr;
                        unsigned long recompressedSize = 0;
                        if (tjCompressFromYUV(handleCompress, yuvBuf, width, fuzz_int32_t_10, 
                                              height, jpegSubsamp, &recompressedBuf, 
                                              &recompressedSize, fuzz_int32_t_11, fuzz_int32_t_12) == 0) {
                            
                            // Apply transformation
                            tjtransform transform;
                            transform.r.x = 0;
                            transform.r.y = 0;
                            transform.r.w = width / 2;
                            transform.r.h = height / 2;
                            transform.op = TJXOP_NONE;
                            transform.options = 0;
                            transform.data = nullptr;
                            transform.customFilter = nullptr;
                            
                            unsigned char *transformedBuf = nullptr;
                            unsigned long transformedSize = 0;
                            
                            if (tj3Transform(handleTransform, recompressedBuf, 
                                            recompressedSize, 1, &transformedBuf, 
                                            &transformedSize, &transform) == 0) {
                                tjFree(transformedBuf);
                            }
                            
                            tjFree(recompressedBuf);
                        }
                        
                        tjFree(rgbBuf);
                    }
                }
                tjFree(yuvBuf);
            }
        }
        
        // Free YUV planes
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) {
                tjFree(yuvPlanes[i]);
            }
        }
    }

    // Cleanup
    tjDestroy(handleCompress);
    tjDestroy(handleDecompress);
    tjDestroy(handleTransform);

    return 0;
}