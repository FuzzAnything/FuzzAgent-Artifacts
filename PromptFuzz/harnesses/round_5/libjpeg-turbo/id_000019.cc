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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 155, nr_unique_branch: 37
//<Quality> {"density":31,"unique_branches":{"prepare_for_pass":[[589,9,589,28,0,0,4,1]],"decode_mcu_slow":[[211,7,211,70,3,0,4,1]],"jinit_c_main_controller":[[167,7,167,25,0,0,4,0]],"jpeg_write_raw_data":[[158,7,158,47,0,0,4,1],[161,7,161,30,0,0,4,1],[164,7,164,43,0,0,4,1],[166,7,166,50,0,0,4,1],[172,7,172,30,0,0,4,1],[183,7,183,39,0,0,4,0],[183,7,183,39,0,0,4,1],[188,7,188,37,0,0,4,1],[192,7,192,42,0,0,4,1],[194,7,194,52,0,0,4,1]],"jpeg_finish_compress":[[185,7,185,43,0,0,4,0]],"jpeg_start_compress":[[63,26,63,44,0,0,4,0]],"start_pass_main":[[63,7,63,25,0,0,4,0]],"jinit_compress_master":[[42,7,42,26,0,0,4,1]],"tj3CompressFromYUVPlanes8":[[1347,7,1347,26,0,0,4,1],[1349,7,1349,38,0,0,4,1],[1358,7,1358,22,0,0,4,1],[1364,7,1364,27,0,0,4,1],[1366,15,1366,40,0,0,4,0],[1366,15,1366,40,0,0,4,1],[1376,9,1376,23,0,0,4,1],[1376,27,1376,38,0,0,4,1],[1379,9,1379,74,0,0,4,1],[1382,19,1382,30,0,0,4,0],[1382,19,1382,30,0,0,4,1],[1384,15,1384,22,0,0,4,0],[1384,26,1384,41,0,0,4,0],[1387,7,1387,16,0,0,4,1],[1403,17,1403,47,0,0,4,0],[1403,17,1403,47,0,0,4,1],[1408,17,1408,42,0,0,4,0],[1408,17,1408,42,0,0,4,1],[1412,11,1412,20,0,0,4,1],[1435,45,1435,57,0,0,4,1]]},"library_calls":["tjInitCompress","tjInitTransform","tjDestroy","tjDestroy","tjDecompressHeader3","tjAlloc","tjDestroy","tjDestroy","tjCompress2","tjTransform","TJBUFSIZE","tjAlloc","tjFree","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3CompressFromYUVPlanes8","tj3Transform","tj3Free","tj3Free","tj3Free","tj3Free","tjFree","tjFree","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitTransform","tjDecompressHeader3","tjAlloc","tjCompress2","tjTransform","TJBUFSIZE","tjAlloc","tjFree","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3CompressFromYUVPlanes8","tj3Transform","tj3Free","tj3Free","tj3Free","tj3Free","tjFree","tjFree","tjFree","tjDestroy","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
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
    tjhandle handle_compress = tjInitCompress();
    tjhandle handle_transform = tjInitTransform();
    if (!handle_compress || !handle_transform) {
        if (handle_compress) tjDestroy(handle_compress);
        if (handle_transform) tjDestroy(handle_transform);
        return 0;
    }

    // Parse input as JPEG header first
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    if (tjDecompressHeader3(handle_compress, data, size, &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        // If not valid JPEG, create synthetic image data
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    // Ensure reasonable dimensions
    if (width <= 0) width = 64;
    if (height <= 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Allocate source buffer for compression
    int pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned char* srcBuf = (unsigned char*)tjAlloc(width * height * tjPixelSize[pixelFormat]);
    if (!srcBuf) {
        tjDestroy(handle_compress);
        tjDestroy(handle_transform);
        return 0;
    }

    // Fill source buffer with fuzzer data (cyclic)
    size_t srcBufSize = width * height * tjPixelSize[pixelFormat];
    for (size_t i = 0; i < srcBufSize; i++) {
        srcBuf[i] = data[i % size];
    }

    // 1. Compress using tjCompress2
    unsigned char* jpegBuf1 = nullptr;
    unsigned long jpegSize1 = 0;
    int jpegQual = 85;
    int flags = 0;
    
    if (tjCompress2(handle_compress, srcBuf, width, pitch, height, pixelFormat,
                    &jpegBuf1, &jpegSize1, jpegSubsamp, jpegQual, flags) == 0) {
        
        // 2. Transform using tjTransform
        tjtransform transform;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.op = TJXOP_NONE;
        transform.options = 0;
        transform.data = nullptr;
        transform.customFilter = nullptr;
        
        unsigned char* dstBuf = nullptr;
        unsigned long dstSize = 0;
        if (tjTransform(handle_transform, jpegBuf1, jpegSize1, 1, &dstBuf, &dstSize,
                        &transform, flags) == 0) {
            
            // 3. Use TJBUFSIZE for buffer allocation
            unsigned long bufsize = TJBUFSIZE(width, height);
            unsigned char* tempBuf = (unsigned char*)tjAlloc(bufsize);
            if (tempBuf) {
                // Could use this buffer for additional operations
                tjFree(tempBuf);
            }
            
            // 4. Prepare YUV data for new API compression
            int align = 1;
            size_t yuvBufSize = tj3YUVBufSize(width, align, height, jpegSubsamp);
            unsigned char* yuvBuf = nullptr;
            if (yuvBufSize > 0) {
                yuvBuf = (unsigned char*)tj3Alloc(yuvBufSize);
                if (yuvBuf) {
                    // Encode to YUV (simplified - would need proper conversion)
                    // For fuzzing, we'll fill with pattern
                    for (size_t i = 0; i < yuvBufSize && i < size; i++) {
                        yuvBuf[i] = data[i];
                    }
                    
                    // 5. Compress from YUV using tj3CompressFromYUV8
                    unsigned char* jpegBuf2 = nullptr;
                    size_t jpegSize2 = 0;
                    tj3CompressFromYUV8(handle_compress, yuvBuf, width, align, height,
                                        &jpegBuf2, &jpegSize2);
                    
                    if (jpegBuf2) {
                        tj3Free(jpegBuf2);
                    }
                    
                    // 6. Prepare YUV planes for plane-based compression
                    int strides[3];
                    const unsigned char* srcPlanes[3];
                    unsigned char* planeBufs[3] = {nullptr, nullptr, nullptr};
                    
                    for (int i = 0; i < 3; i++) {
                        strides[i] = tj3YUVPlaneWidth(i, width, jpegSubsamp);
                        int planeHeight = tj3YUVPlaneHeight(i, height, jpegSubsamp);
                        size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], planeHeight, jpegSubsamp);
                        
                        if (planeSize > 0) {
                            planeBufs[i] = (unsigned char*)tj3Alloc(planeSize);
                            if (planeBufs[i]) {
                                // Fill plane with data
                                size_t fillSize = (planeSize < size) ? planeSize : size;
                                memcpy(planeBufs[i], data, fillSize);
                                srcPlanes[i] = planeBufs[i];
                            }
                        }
                    }
                    
                    // 7. Compress from YUV planes using tj3CompressFromYUVPlanes8
                    unsigned char* jpegBuf3 = nullptr;
                    size_t jpegSize3 = 0;
                    tj3CompressFromYUVPlanes8(handle_compress, srcPlanes, width, strides, height,
                                              &jpegBuf3, &jpegSize3);
                    
                    if (jpegBuf3) {
                        // 8. Transform using tj3Transform
                        unsigned char* dstBuf2 = nullptr;
                        size_t dstSize2 = 0;
                        tj3Transform(handle_transform, jpegBuf3, jpegSize3, 1,
                                     &dstBuf2, &dstSize2, &transform);
                        
                        if (dstBuf2) {
                            tj3Free(dstBuf2);
                        }
                        
                        tj3Free(jpegBuf3);
                    }
                    
                    // Free plane buffers
                    for (int i = 0; i < 3; i++) {
                        if (planeBufs[i]) {
                            tj3Free(planeBufs[i]);
                        }
                    }
                    
                    tj3Free(yuvBuf);
                }
            }
            
            if (dstBuf) {
                tjFree(dstBuf);
            }
        }
        
        if (jpegBuf1) {
            tjFree(jpegBuf1);
        }
    }

    // Cleanup
    tjFree(srcBuf);
    tjDestroy(handle_compress);
    tjDestroy(handle_transform);
    
    return 0;
}