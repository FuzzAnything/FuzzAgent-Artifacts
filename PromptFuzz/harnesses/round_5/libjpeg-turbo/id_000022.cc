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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 192, nr_unique_branch: 44
//<Quality> {"density":32,"unique_branches":{"tj3DecodeYUV8":[[2725,7,2725,38,0,0,4,0]],"getDstSubsamp":[[2794,7,2794,17,0,0,4,1],[2797,16,2797,50,0,0,4,1],[2799,7,2799,39,0,0,4,1],[2799,43,2799,76,0,0,4,1],[2800,7,2800,35,0,0,4,1],[2800,39,2800,68,0,0,4,1]],"tj3Transform":[[2955,21,2955,49,0,0,4,0],[2959,9,2959,22,0,0,4,0],[2960,11,2960,23,0,0,4,1],[2960,27,2960,39,0,0,4,1],[2960,43,2960,55,0,0,4,1],[2960,59,2960,71,0,0,4,1],[2964,11,2964,24,0,0,4,0],[2968,11,2968,24,0,0,4,0],[2990,9,2990,22,0,0,4,0],[2993,11,2993,39,0,0,4,1],[2995,11,2995,51,0,0,4,1],[2996,11,2996,52,0,0,4,1]],"tj3CompressFromYUV8":[[1497,7,1497,38,0,0,4,0]],"jtransform_request_workspace":[[1699,7,1699,17,0,0,4,0],[1701,9,1701,46,0,0,4,1],[1703,9,1703,46,0,0,4,1],[1705,9,1705,44,0,0,4,1],[1714,11,1714,48,0,0,4,1],[1723,13,1723,53,0,0,4,1],[1724,13,1724,34,0,0,4,1],[1725,13,1725,71,0,0,4,1],[1729,9,1729,45,0,0,4,1],[1738,11,1738,50,0,0,4,1],[1747,13,1747,54,0,0,4,1],[1748,13,1748,35,0,0,4,1],[1749,13,1749,73,0,0,4,1],[1754,9,1754,44,0,0,4,0],[1760,9,1760,44,0,0,4,0],[1768,5,1768,21,0,0,4,1],[1814,5,1814,21,0,0,4,1],[1823,5,1823,12,0,0,4,0],[1825,11,1825,46,0,0,4,1],[1826,11,1826,48,0,0,4,1],[1828,16,1828,46,0,0,4,0],[1831,11,1831,47,0,0,4,1],[1832,11,1832,50,0,0,4,1],[1834,16,1834,46,0,0,4,0]]},"library_calls":["tjInitCompress","tjInitTransform","tjDestroy","tjDecompressHeader3","TJBUFSIZE","tjAlloc","tj3JPEGBufSize","tj3Alloc","tjFree","tj3Free","tjDestroy","tjDestroy","tjCompress2","tjTransform","tjBufSizeYUV2","tjEncodeYUV3","tj3InitVersion","tj3DecodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3CompressFromYUV8","tj3CompressFromYUVPlanes8","tj3Free","tj3Transform","tj3Free","tj3Free","tjDestroy","tjFree","tjFree","tjFree","tj3Free","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitTransform","tjDecompressHeader3","TJBUFSIZE","tjAlloc","tj3JPEGBufSize","tj3Alloc","tjCompress2","tjTransform","tjBufSizeYUV2","tjEncodeYUV3","tj3InitVersion","tj3DecodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3CompressFromYUV8","tj3CompressFromYUVPlanes8","tj3Free","tj3Transform","tj3Free","tj3Free","tjDestroy","tjFree","tjFree","tjFree","tj3Free","tjDestroy","tjDestroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs to achieve a comprehensive event involving JPEG compression, transformation, and YUV processing:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Initialize handles
    tjhandle compress_handle = tjInitCompress();
    if (!compress_handle) {
	return 0;
	}
	
    
    tjhandle transform_handle = tjInitTransform();
    if (!transform_handle) {
        tjDestroy(compress_handle);
        return 0;
    }

    // Parse JPEG header from input data
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int result = tjDecompressHeader3(transform_handle, data, size, 
                                     &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (result != 0 || width <= 0 || height <= 0) {
        // If not valid JPEG, create synthetic data
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    // Allocate buffers using different allocation methods
    unsigned long classic_bufsize = TJBUFSIZE(width, height);
    unsigned char *classic_buffer = tjAlloc(classic_bufsize);
    
    size_t new_bufsize = tj3JPEGBufSize(width, height, jpegSubsamp);
    void *new_buffer = tj3Alloc(new_bufsize);

    if (!classic_buffer || !new_buffer) {
        if (classic_buffer) tjFree(classic_buffer);
        if (new_buffer) tj3Free(new_buffer);
        tjDestroy(transform_handle);
        tjDestroy(compress_handle);
        return 0;
    }

    // Use a portion of input data as source image
    int pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    
    // Create synthetic source data if input is not valid JPEG
    unsigned char *src_buf = (unsigned char *)malloc(width * height * 3);
    if (src_buf) {
        // Fill with pattern from input data
        for (int i = 0; i < width * height * 3 && i < (int)size; i++) {
            src_buf[i] = data[i % size];
        }

        // Test compression with classic API
        unsigned char *jpegBuf = nullptr;
        unsigned long jpegSize = 0;
        int jpegQual = 85;
        int flags = TJFLAG_FASTDCT;
        
        result = tjCompress2(compress_handle, src_buf, width, pitch, height,
                            pixelFormat, &jpegBuf, &jpegSize, jpegSubsamp, 
                            jpegQual, flags);
        
        if (result == 0 && jpegBuf && jpegSize > 0) {
            // Test transformation
            tjtransform transform;
            memset(&transform, 0, sizeof(transform));
            transform.r.x = 0;
            transform.r.y = 0;
            transform.r.w = width / 2;
            transform.r.h = height / 2;
            transform.op = TJXOP_NONE;
            transform.options = TJXOPT_CROP;
            
            unsigned char *dstBufs[1] = {nullptr};
            unsigned long dstSizes[1] = {0};
            
            result = tjTransform(transform_handle, jpegBuf, jpegSize, 1,
                                dstBufs, dstSizes, &transform, flags);
            
            if (result == 0 && dstBufs[0] && dstSizes[0] > 0) {
                // Test YUV encoding with classic API
                int align = 1;
                unsigned char *yuvBuf = (unsigned char *)malloc(
                    tjBufSizeYUV2(width, align, height, jpegSubsamp));
                
                if (yuvBuf) {
                    result = tjEncodeYUV3(compress_handle, src_buf, width, pitch,
                                         height, pixelFormat, yuvBuf, align,
                                         jpegSubsamp, flags);
                    
                    if (result == 0) {
                        // Test YUV decoding with new API
                        unsigned char *decodedBuf = 
                            (unsigned char *)malloc(width * height * 3);
                        
                        if (decodedBuf) {
                            // Create a handle for new API
                            tjhandle new_handle = tj3Init(TJINIT_TRANSFORM);
                            if (new_handle) {
                                result = tj3DecodeYUV8(new_handle, yuvBuf, align,
                                                      decodedBuf, width, pitch,
                                                      height, pixelFormat);
                                
                                // Test YUV plane dimensions
                                for (int comp = 0; comp < 3; comp++) {
                                    int planeWidth = tj3YUVPlaneWidth(comp, width, 
                                                                     jpegSubsamp);
                                    int planeHeight = tj3YUVPlaneHeight(comp, height, 
                                                                       jpegSubsamp);
                                    size_t planeSize = tj3YUVPlaneSize(comp, width,
                                                                       width, height,
                                                                       jpegSubsamp);
                                    (void)planeWidth;
                                    (void)planeHeight;
                                    (void)planeSize;
                                }
                                
                                // Test compression from YUV with new API
                                unsigned char *jpegBuf2 = nullptr;
                                size_t jpegSize2 = 0;
                                result = tj3CompressFromYUV8(new_handle, yuvBuf,
                                                             width, align, height,
                                                             &jpegBuf2, &jpegSize2);
                                
                                // Test compression from YUV planes
                                if (result == 0) {
                                    const unsigned char *srcPlanes[3] = {yuvBuf, 
                                                                        yuvBuf + width*height,
                                                                        yuvBuf + width*height + 
                                                                        (width/2)*(height/2)};
                                    int strides[3] = {width, width/2, width/2};
                                    unsigned char *jpegBuf3 = nullptr;
                                    size_t jpegSize3 = 0;
                                    
                                    result = tj3CompressFromYUVPlanes8(new_handle,
                                                                      srcPlanes,
                                                                      width, strides,
                                                                      height,
                                                                      &jpegBuf3,
                                                                      &jpegSize3);
                                    
                                    if (jpegBuf3) tj3Free(jpegBuf3);
                                }
                                
                                // Test transformation with new API
                                if (jpegBuf2 && jpegSize2 > 0) {
                                    unsigned char *dstBufs2[1] = {nullptr};
                                    size_t dstSizes2[1] = {0};
                                    
                                    result = tj3Transform(new_handle, jpegBuf2, jpegSize2,
                                                         1, dstBufs2, dstSizes2,
                                                         &transform);
                                    
                                    if (dstBufs2[0]) tj3Free(dstBufs2[0]);
                                }
                                
                                if (jpegBuf2) tj3Free(jpegBuf2);
                                tjDestroy(new_handle);
                            }
                            free(decodedBuf);
                        }
                    }
                    free(yuvBuf);
                }
            }
            
            if (dstBufs[0]) tjFree(dstBufs[0]);
            if (jpegBuf) tjFree(jpegBuf);
        }
        
        free(src_buf);
    }

    // Clean up
    if (classic_buffer) tjFree(classic_buffer);
    if (new_buffer) tj3Free(new_buffer);
    tjDestroy(transform_handle);
    tjDestroy(compress_handle);

    return 0;
}