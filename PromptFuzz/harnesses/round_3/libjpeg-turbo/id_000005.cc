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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":38,"unique_branches":{},"library_calls":["tjInitDecompress","tjInitCompress","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDecompressHeader2","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjAlloc","tjDestroy","tjDestroy","tjDecompress2","tjGetErrorStr2","tjFree","tjDestroy","tjDestroy","tjCompress2","tjFree","tjDestroy","tjDestroy","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tj3InitVersion","tj3Alloc","tj3DecodeYUVPlanes8","tj3Get","tj3Destroy","tj3InitVersion","tj3SaveImage12","tj3Destroy","tjPlaneHeight","tjFree","tjFree","tjFree","tj3Free","tjFree","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjInitCompress","tjDecompressHeader2","tjAlloc","tjDecompress2","tjCompress2","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tj3InitVersion","tj3Alloc","tj3DecodeYUVPlanes8","tj3Get","tj3Destroy","tj3InitVersion","tj3SaveImage12","tj3Destroy","tjPlaneHeight","tjFree","tjFree","tjFree","tj3Free","tjFree","tjDestroy","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<127) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_22, fuzz_int32_t_array_size_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
	
		const int * fuzzer_var_tj3DecodeYUVPlanes8_34_2 = fuzz_int32_t_array_22;//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small to be meaningful
    }

    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *srcBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *dstPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    short *image12 = nullptr;
    int width = 0, height = 0;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int ret = 0;

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize handles
    tjhandle decompHandle = tjInitDecompress();
    tjhandle compHandle = tjInitCompress();
    if (!decompHandle || !compHandle) {
        if (decompHandle) tjDestroy(decompHandle);
        if (compHandle) tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG header from input data
    fseek(in_file, 0, SEEK_SET);
    unsigned char *jpegInput = (unsigned char *)malloc(size);
    if (!jpegInput) {
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(jpegInput, 1, size, in_file);
    
    // Decompress header to get image dimensions
    if (tjDecompressHeader2(decompHandle, jpegInput, (unsigned long)size, 
                           &width, &height, &subsamp) != 0) {
        free(jpegInput);
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        free(jpegInput);
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate source buffer for RGB image
    int pitch = width * tjPixelSize[pixelFormat];
    srcBuf = (unsigned char *)tjAlloc(pitch * height);
    if (!srcBuf) {
        free(jpegInput);
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Decompress JPEG to RGB
    if (tjDecompress2(decompHandle, jpegInput, (unsigned long)size, srcBuf,
                     fuzz_int32_t_1, pitch, fuzz_int32_t_2, pixelFormat, fuzz_int32_t_3) != 0) {
        // Use error string API
        char *errorStr = tjGetErrorStr2(decompHandle);
        (void)errorStr; // Use error string to satisfy API requirement
        free(jpegInput);
        tjFree(srcBuf);
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use tjCompress2 to compress RGB to JPEG
    if (tjCompress2(compHandle, srcBuf, fuzz_int32_t_4, pitch, fuzz_int32_t_5, pixelFormat,
                   &jpegBuf, &jpegSize, subsamp, fuzz_int32_t_6, fuzz_int32_t_7) != 0) {
        free(jpegInput);
        tjFree(srcBuf);
        tjDestroy(decompHandle);
        tjDestroy(compHandle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use tjEncodeYUV2 to encode RGB to interleaved YUV
    unsigned long yuvSize = tjBufSizeYUV2(width, 4, height, subsamp);
    yuvBuf = (unsigned char *)tjAlloc(yuvSize);
    if (yuvBuf) {
        if (tjEncodeYUV2(compHandle, srcBuf, fuzz_int32_t_8, pitch, fuzz_int32_t_9, pixelFormat,
                        yuvBuf, subsamp, fuzz_int32_t_10) != 0) {
            // Error handled below
        }
    }

    // 3. Use tjEncodeYUVPlanes to encode RGB to planar YUV
    // Calculate strides for planar YUV
    int yStride = tjPlaneWidth(fuzz_int32_t_11, fuzz_int32_t_12, subsamp) * tjPixelSize[TJPF_GRAY];
    int uvStride = tjPlaneWidth(fuzz_int32_t_13, fuzz_int32_t_14, subsamp) * tjPixelSize[TJPF_GRAY];
    strides[0] = yStride;
    strides[1] = uvStride;
    strides[2] = uvStride;

    // Allocate plane buffers
    for (int i = 0; i < 3; i++) {
        int planeWidth = tjPlaneWidth(fuzz_int32_t_15, fuzz_int32_t_16, subsamp);
        int planeHeight = tjPlaneHeight(fuzz_int32_t_17, fuzz_int32_t_18, subsamp);
        dstPlanes[i] = (unsigned char *)tjAlloc(planeWidth * planeHeight);
    }

    if (dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
        if (tjEncodeYUVPlanes(compHandle, srcBuf, fuzz_int32_t_19, pitch, fuzz_int32_t_20, pixelFormat,
                             dstPlanes, strides, subsamp, fuzz_int32_t_21) != 0) {
            // Error handled below
        }
    }

    // 4. Use tj3DecodeYUVPlanes8 (requires new API handle)
    tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
    if (handle3) {
        // Allocate destination buffer for decoded image
        dstBuf = (unsigned char *)tj3Alloc(pitch * height);
        if (dstBuf && dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
            // Decode planar YUV back to RGB
            tj3DecodeYUVPlanes8(handle3, (const unsigned char *const *)dstPlanes, 
                               fuzzer_var_tj3DecodeYUVPlanes8_34_2, dstBuf, fuzz_int32_t_23, pitch, fuzz_int32_t_24, pixelFormat);
        }
        // 5. Use tj3Get to get a parameter
        int paramValue = tj3Get(handle3, TJPARAM_SUBSAMP);
        (void)paramValue; // Use the value to satisfy API requirement
        
        tj3Destroy(handle3);
    }

    // 6. Use tj3SaveImage12 to save a 12-bit image
    // Convert 8-bit image to 12-bit by scaling
    int image12Size = width * height * tjPixelSize[pixelFormat];
    image12 = (short *)malloc(image12Size * sizeof(short));
    if (image12 && srcBuf) {
        for (int i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
            image12[i] = (short)(srcBuf[i] << 4); // Scale 8-bit to 12-bit
        }
        
        // Save to temporary file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Note: tj3SaveImage12 expects the new API handle
            tjhandle saveHandle = tj3Init(TJINIT_COMPRESS);
            if (saveHandle) {
                // We'll save as 12-bit grayscale for simplicity
                tj3SaveImage12(saveHandle, "output_file", image12, 
                              fuzz_int32_t_25, fuzz_int32_t_26, fuzz_int32_t_27, TJPF_GRAY);
                tj3Destroy(saveHandle);
            }
            assert_file_closed(&out_file);;
        }
    }

    // 7. Use tjPlaneHeight
    int yPlaneHeight = tjPlaneHeight(fuzz_int32_t_28, fuzz_int32_t_29, subsamp);
    (void)yPlaneHeight; // Use the value to satisfy API requirement

    // Cleanup
    free(jpegInput);
    if (srcBuf) tjFree(srcBuf);
    if (jpegBuf) tjFree(jpegBuf);
    if (yuvBuf) tjFree(yuvBuf);
    if (dstBuf) tj3Free(dstBuf);
    if (image12) free(image12);
    
    for (int i = 0; i < 3; i++) {
        if (dstPlanes[i]) tjFree(dstPlanes[i]);
    }
    
    tjDestroy(decompHandle);
    tjDestroy(compHandle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}