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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 0
//<Quality> {"density":35,"unique_branches":{},"library_calls":["tj3InitVersion","tj3Alloc","tj3Destroy","tjInitDecompress","tj3Free","tj3Destroy","tjDecompressHeader3","tjGetErrorStr2","tjDestroy","tj3Free","tj3Destroy","tjAlloc","tjDestroy","tj3Free","tj3Destroy","tjDecompress2","tjGetErrorStr2","tjFree","tjDestroy","tj3Free","tj3Destroy","TJBUFSIZEYUV","tjInitCompress","tjFree","tjDestroy","tj3Free","tj3Destroy","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjFree","tjDestroy","tjFree","tjDestroy","tj3Free","tj3Destroy","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjDestroy","tjFree","tjDestroy","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3Alloc","tjInitDecompress","tjDecompressHeader3","tjAlloc","tjDecompress2","TJBUFSIZEYUV","tjInitCompress","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tjGetErrorStr2","tjFree","tjDestroy","tjFree","tjDestroy","tj3Free","tj3Destroy"],"visited":1}
/*Here's a C++ fuzzer program that uses the specified libjpeg-turbo APIs to perform JPEG decompression followed by YUV plane encoding:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small for meaningful JPEG data
    }

    // Initialize TurboJPEG handles
    tjhandle handle3 = nullptr;
    tjhandle handle_decompress = nullptr;
    tjhandle handle_compress = nullptr;
    
    unsigned char *jpegBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // Step 1: Use tj3DecompressHeader to get JPEG dimensions
    handle3 = tj3Init(TJINIT_DECOMPRESS);
    if (!handle3) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Allocate buffer for JPEG data
    jpegBuf = (unsigned char *)tj3Alloc(size);
    if (!jpegBuf) {
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    memcpy(jpegBuf, data, size);
    
    // Get JPEG header information
    int width, height, jpegSubsamp, jpegColorspace;
    
    // Note: tj3DecompressHeader is newer API, but we need to use compatible approach
    // First get dimensions using older API that returns all needed info
    handle_decompress = tjInitDecompress();
    if (!handle_decompress) {
        tj3Free(jpegBuf);
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Use tjDecompressHeader3 which provides subsampling info
    if (tjDecompressHeader3(handle_decompress, jpegBuf, (unsigned long)size, 
                           &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        char *errorStr = tjGetErrorStr2(handle_decompress);
        if (errorStr) {
            // Log error if needed
        }
        tjDestroy(handle_decompress);
        tj3Free(jpegBuf);
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 2: Decompress JPEG to RGB using tjDecompress2
    int pitch = width * tjPixelSize[TJPF_RGB];
    dstBuf = (unsigned char *)tjAlloc(pitch * height);
    if (!dstBuf) {
        tjDestroy(handle_decompress);
        tj3Free(jpegBuf);
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    int flags = TJFLAG_FASTDCT;
    if (tjDecompress2(handle_decompress, jpegBuf, (unsigned long)size, 
                      dstBuf, width, pitch, height, TJPF_RGB, flags) != 0) {
        char *errorStr = tjGetErrorStr2(handle_decompress);
        if (errorStr) {
            // Log error if needed
        }
        tjFree(dstBuf);
        tjDestroy(handle_decompress);
        tj3Free(jpegBuf);
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 3: Calculate YUV buffer size using TJBUFSIZEYUV
    unsigned long yuvBufSize = TJBUFSIZEYUV(width, height, jpegSubsamp);
    
    // Step 4: Encode to YUV planes using tjEncodeYUVPlanes
    handle_compress = tjInitCompress();
    if (!handle_compress) {
        tjFree(dstBuf);
        tjDestroy(handle_decompress);
        tj3Free(jpegBuf);
        tj3Destroy(handle3);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Calculate strides for YUV planes
    strides[0] = tjPlaneWidth(0, width, jpegSubsamp);
    strides[1] = tjPlaneWidth(1, width, jpegSubsamp);
    strides[2] = tjPlaneWidth(2, width, jpegSubsamp);
    
    // Allocate YUV plane buffers
    for (int i = 0; i < 3; i++) {
        int planeHeight = tjPlaneHeight(i, height, jpegSubsamp);
        yuvPlanes[i] = (unsigned char *)tjAlloc(strides[i] * planeHeight);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) {
                tjFree(yuvPlanes[j]);
            }
            tjDestroy(handle_compress);
            tjFree(dstBuf);
            tjDestroy(handle_decompress);
            tj3Free(jpegBuf);
            tj3Destroy(handle3);
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	return 0;
        }
    }
    
    // Encode RGB to YUV planes
    if (tjEncodeYUVPlanes(handle_compress, dstBuf, width, pitch, height, 
                          TJPF_RGB, yuvPlanes, strides, jpegSubsamp, flags) != 0) {
        char *errorStr = tjGetErrorStr2(handle_compress);
        if (errorStr) {
            // Log error if needed
        }
    }
    
    // Clean up all allocated resources
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) {
            tjFree(yuvPlanes[i]);
        }
    }
    
    if (handle_compress) {
        tjDestroy(handle_compress);
    }
    
    if (dstBuf) {
        tjFree(dstBuf);
    }
    
    if (handle_decompress) {
        tjDestroy(handle_decompress);
    }
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    if (handle3) {
        tj3Destroy(handle3);
    }
    
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	return 0;
}