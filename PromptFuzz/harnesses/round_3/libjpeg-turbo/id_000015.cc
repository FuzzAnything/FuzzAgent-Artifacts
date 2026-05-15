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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["tj3Alloc","tj3Free","tjInitDecompress","tj3Free","tjDecompressHeader3","tjGetErrorStr","tjDestroy","tj3Free","tjDestroy","tj3Free","tjInitCompress","tj3Set","tjDestroy","TJBUFSIZEYUV","tj3JPEGBufSize","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tjDecompressToYUVPlanes","tj3Alloc","tjDecodeYUVPlanes","tjInitTransform","tj3TransformBufSize","tj3Alloc","tjDestroy","tj3GetErrorCode","tj3GetErrorStr","tj3GetICCProfile","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Free"],"critical_calls":["tj3Alloc","tjInitDecompress","tjDecompressHeader3","tjInitCompress","tj3Set","tjDestroy","TJBUFSIZEYUV","tj3JPEGBufSize","tjPlaneWidth","tjPlaneHeight","tj3Alloc","tjDecompressToYUVPlanes","tj3Alloc","tjDecodeYUVPlanes","tjInitTransform","tj3TransformBufSize","tj3Alloc","tjDestroy","tj3GetErrorCode","tj3GetErrorStr","tj3GetICCProfile","tj3Free","tj3Free","tj3Free","tj3Free","tjDestroy","tj3Free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<95) return 0;

	
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
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_17, fuzz_int32_t_array_size_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	
		const int * fuzzer_var_tjDecodeYUVPlanes_20_2 = fuzz_int32_t_array_17;//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *dstPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    unsigned char *dstBuf = nullptr;
    unsigned char *iccBuf = nullptr;
    size_t iccSize = 0;
    unsigned char *transformBuf = nullptr;
    size_t transformBufSize = 0;
    tjtransform transform;
    int width = 0;
    int height = 0;
    int jpegSubsamp = 0;
    int pixelFormat = TJPF_RGB;

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    jpegSize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (jpegSize < 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (fread(jpegBuf, 1, jpegSize, in_file) != jpegSize) {
        tj3Free(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    assert_file_closed(&in_file);;

    // Initialize decompressor handle
    handle = tjInitDecompress();
    if (!handle) {
        tj3Free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decompress header to get image dimensions and subsampling
    if (tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp, nullptr) != 0) {
        char *errorStr = tjGetErrorStr();
        tjDestroy(handle);
        tj3Free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        tj3Free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Use tj3Set API (set JPEG quality parameter on a compressor handle)
    tjhandle compressHandle = tjInitCompress();
    if (compressHandle) {
        // Set JPEG quality to 85
        tj3Set(compressHandle, TJPARAM_QUALITY, fuzz_int32_t_1);
        tjDestroy(compressHandle);
    }

    // Use TJBUFSIZEYUV API
    unsigned long yuvBufSize = TJBUFSIZEYUV(fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4);

    // Use tj3JPEGBufSize API
    size_t jpegBufSize = tj3JPEGBufSize(fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7);

    // Allocate and set up YUV planes
    for (int i = 0; i < 3; i++) {
        strides[i] = tjPlaneWidth(fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);
        int planeHeight = tjPlaneHeight(fuzz_int32_t_11, fuzz_int32_t_12, fuzz_int32_t_13);
        if (strides[i] > 0 && planeHeight > 0) {
            dstPlanes[i] = (unsigned char *)tj3Alloc(strides[i] * planeHeight);
        }
    }

    // Use tjDecompressToYUVPlanes API
    if (tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, dstPlanes, fuzz_int32_t_14, strides, fuzz_int32_t_15, fuzz_int32_t_16) == 0) {
        // Allocate destination buffer for decoded image
        int pitch = width * tjPixelSize[pixelFormat];
        dstBuf = (unsigned char *)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
        
        if (dstBuf) {
            // Use tjDecodeYUVPlanes API
            tjDecodeYUVPlanes(handle, (const unsigned char **)dstPlanes, fuzzer_var_tjDecodeYUVPlanes_20_2, fuzz_int32_t_18, 
                             dstBuf, fuzz_int32_t_19, pitch, fuzz_int32_t_20, pixelFormat, fuzz_int32_t_21);
        }
    }

    // Setup transformation structure
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = 0;

    // Use tj3TransformBufSize API
    tjhandle transformHandle = tjInitTransform();
    if (transformHandle) {
        transformBufSize = tj3TransformBufSize(transformHandle, &transform);
        if (transformBufSize > 0) {
            transformBuf = (unsigned char *)tj3Alloc(transformBufSize);
        }
        tjDestroy(transformHandle);
    }

    // Use tj3GetErrorStr API
    if (tj3GetErrorCode(handle) != 0) {
        char *errorStr = tj3GetErrorStr(handle);
    }

    // Use tj3GetICCProfile API
    if (tj3GetICCProfile(handle, &iccBuf, &iccSize) == 0 && iccBuf) {
        // ICC profile retrieved successfully
    }

    // Cleanup
    if (iccBuf) {
        tj3Free(iccBuf);
    }
    
    if (transformBuf) {
        tj3Free(transformBuf);
    }
    
    if (dstBuf) {
        tj3Free(dstBuf);
    }
    
    for (int i = 0; i < 3; i++) {
        if (dstPlanes[i]) {
            tj3Free(dstPlanes[i]);
        }
    }
    
    tjDestroy(handle);
    tj3Free(jpegBuf);

    assert_file_closed(&in_file);
	return 0;
}