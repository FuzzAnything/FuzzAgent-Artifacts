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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 2
//<Quality> {"density":28,"unique_branches":{"tj3Set":[[750,3,750,23,0,0,4,0],[751,9,751,33,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3Set","tj3Set","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Decompress8","tj3Set","tj3Compress8","tj3Destroy","tj3InitVersion","tj3Free","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3CompressFromYUVPlanes8","tj3Free","tjInitCompress","tjAlloc","TJBUFSIZE","tjCompress","tjFree","tjDestroy","tj3Destroy","tj3Free","tj3Free"],"critical_calls":["tj3InitVersion","tj3Set","tj3Set","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Decompress8","tj3Set","tj3Compress8","tj3Destroy","tj3InitVersion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3CompressFromYUVPlanes8","tj3Free","tjInitCompress","tjAlloc","TJBUFSIZE","tjCompress","tjFree","tjDestroy","tj3Destroy","tj3Free","tj3Free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have minimum data for JPEG header
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;
    unsigned char *yuvBuf = nullptr;
    unsigned char *planes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    int width = 0, height = 0;
    int pixelFormat = TJPF_RGB;
    int quality = 85;
    int subsamp = TJSAMP_420;
    unsigned long compressedSize = 0;
    
    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the input data into a buffer
    unsigned char *inputBuf = (unsigned char *)malloc(size);
    if (!inputBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(inputBuf, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Initialize TurboJPEG handle
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        free(inputBuf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some parameters
    tj3Set(handle, TJPARAM_QUALITY, quality);
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    
    // First, try to decompress the input as JPEG using tj3Decompress8
    // We need to get the header first
    if (tj3DecompressHeader(handle, inputBuf, size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            int pitch = width * tjPixelSize[pixelFormat];
            unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
            
            if (dstBuf) {
                // Decompress using tj3Decompress8
                if (tj3Decompress8(handle, inputBuf, size, dstBuf, pitch, pixelFormat) == 0) {
                    // Now compress it back using tj3Compress8
                    tj3Set(handle, TJPARAM_QUALITY, 75);
                    unsigned char *compressedBuf = nullptr;
                    size_t compressedSizeNew = 0;
                    
                    if (tj3Compress8(handle, dstBuf, width, pitch, height, 
                                     pixelFormat, &compressedBuf, &compressedSizeNew) == 0) {
                        jpegBuf = compressedBuf;
                        jpegSize = compressedSizeNew;
                    }
                }
                free(dstBuf);
            }
        }
    }
    
    // Reset handle for new operations
    tj3Destroy(handle);
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        if (jpegBuf) tj3Free(jpegBuf);
        free(inputBuf);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try decompressing to YUV using tj3DecompressToYUV8
    if (width > 0 && height > 0) {
        int align = 1;
        size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
        yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
        
        if (yuvBuf) {
            if (tj3DecompressToYUV8(handle, inputBuf, size, yuvBuf, align) == 0) {
                // Prepare planes for YUV compression
                int yPlaneWidth = tj3YUVPlaneWidth(0, width, subsamp);
                int yPlaneHeight = tj3YUVPlaneHeight(0, height, subsamp);
                int uvPlaneWidth = tj3YUVPlaneWidth(1, width, subsamp);
                int uvPlaneHeight = tj3YUVPlaneHeight(1, height, subsamp);
                
                unsigned char *yPlane = yuvBuf;
                unsigned char *uPlane = yPlane + yPlaneWidth * yPlaneHeight;
                unsigned char *vPlane = uPlane + uvPlaneWidth * uvPlaneHeight;
                
                planes[0] = yPlane;
                planes[1] = uPlane;
                planes[2] = vPlane;
                
                strides[0] = yPlaneWidth;
                strides[1] = uvPlaneWidth;
                strides[2] = uvPlaneWidth;
                
                // Compress from YUV planes using tj3CompressFromYUVPlanes8
                unsigned char *yuvCompressedBuf = nullptr;
                size_t yuvCompressedSize = 0;
                
                if (tj3CompressFromYUVPlanes8(handle, planes, width, strides, 
                                              height, &yuvCompressedBuf, &yuvCompressedSize) == 0) {
                    // Free previous compressed buffer if exists
                    if (jpegBuf) tj3Free(jpegBuf);
                    jpegBuf = yuvCompressedBuf;
                    jpegSize = yuvCompressedSize;
                }
            }
        }
    }
    
    // Now use the old API tjCompress
    if (width > 0 && height > 0) {
        tjhandle oldHandle = tjInitCompress();
        if (oldHandle) {
            int pitch = width * 3; // Assuming RGB
            unsigned char *srcBuf = (unsigned char *)malloc(pitch * height);
            
            if (srcBuf) {
                // Create a simple test pattern
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        int idx = (y * pitch) + (x * 3);
                        srcBuf[idx] = (x * 255) / width;     // R
                        srcBuf[idx + 1] = (y * 255) / height; // G
                        srcBuf[idx + 2] = 128;               // B
                    }
                }
                
                unsigned char *oldCompressedBuf = (unsigned char *)tjAlloc(TJBUFSIZE(width, height));
                if (oldCompressedBuf) {
                    if (tjCompress(oldHandle, srcBuf, width, pitch, height, 3,
                                   oldCompressedBuf, &compressedSize, subsamp, quality, 0) == 0) {
                        // Successfully used old API
                    }
                    tjFree(oldCompressedBuf);
                }
                free(srcBuf);
            }
            tjDestroy(oldHandle);
        }
    }
    
    // Cleanup
    if (handle) tj3Destroy(handle);
    if (jpegBuf) tj3Free(jpegBuf);
    if (yuvBuf) tj3Free(yuvBuf);
    free(inputBuf);
    
    assert_file_closed(&in_file);
	return 0;
}