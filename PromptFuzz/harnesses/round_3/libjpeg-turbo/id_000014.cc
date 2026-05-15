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
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["tjInitDecompress","tj3InitVersion","tjDestroy","tj3Destroy","tjDestroy","tjAlloc","tj3Destroy","tjDestroy","tjDecompressHeader3","tjFree","tj3Destroy","tjDestroy","tj3Set","tj3Set","tj3JPEGBufSize","TJBUFSIZEYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjFree","tjFree","tj3Destroy","tjDestroy","tjDecompressToYUVPlanes","tj3GetErrorStr","tjFree","tjFree","tj3Destroy","tjDestroy","tj3GetICCProfile","tjAlloc","tjFree","tjFree","tjFree","tj3Destroy","tjDestroy","tjDecodeYUVPlanes","tj3GetErrorStr","tj3TransformBufSize","tjFree","tjFree","tjFree","tjFree","tj3Destroy","tjDestroy"],"critical_calls":["tjInitDecompress","tj3InitVersion","tjAlloc","tjDecompressHeader3","tj3Set","tj3Set","tj3JPEGBufSize","TJBUFSIZEYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjDecompressToYUVPlanes","tj3GetICCProfile","tjAlloc","tjDecodeYUVPlanes","tj3GetErrorStr","tj3TransformBufSize","tjFree","tjFree","tjFree","tjFree","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_6, fuzz_int32_t_array_size_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	
		const int * fuzzer_var_tjDecodeYUVPlanes_36_2 = fuzz_int32_t_array_6;//fuzzer shim end}




    // Create file handle for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decompressor handle
    tjhandle handle_old = tjInitDecompress();
    if (!handle_old) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize new API handle
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    if (!handle_new) {
        tjDestroy(handle_old);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read JPEG file into memory
    fseek(in_file, 0, SEEK_END);
    long jpeg_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (jpeg_size <= 0) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    unsigned char *jpeg_buf = (unsigned char *)tjAlloc(jpeg_size);
    if (!jpeg_buf) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    fread(jpeg_buf, 1, jpeg_size, in_file);
    assert_file_closed(&in_file);;

    // Get image dimensions using old API
    int width, height, jpeg_subsamp, jpeg_colorspace;
    if (tjDecompressHeader3(handle_old, jpeg_buf, jpeg_size, 
                           &width, &height, &jpeg_subsamp, &jpeg_colorspace) != 0) {
        tjFree(jpeg_buf);
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set parameters using new API
    tj3Set(handle_new, TJPARAM_BOTTOMUP, fuzz_int32_t_1);
    tj3Set(handle_new, TJPARAM_FASTUPSAMPLE, fuzz_int32_t_2);

    // Calculate buffer sizes
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, jpeg_subsamp);
    unsigned long yuv_buf_size = TJBUFSIZEYUV(width, height, jpeg_subsamp);

    // Allocate YUV planes
    unsigned char **yuv_planes = new unsigned char*[3];
    int strides[3] = {0, 0, 0};
    
    for (int i = 0; i < 3; i++) {
        int plane_width = tjPlaneWidth(fuzz_int32_t_3, width, jpeg_subsamp);
        int plane_height = tjPlaneHeight(fuzz_int32_t_4, height, jpeg_subsamp);
        strides[i] = plane_width;
        yuv_planes[i] = (unsigned char *)tjAlloc(plane_width * plane_height);
        if (!yuv_planes[i]) {
            for (int j = 0; j < i; j++) {
                tjFree(yuv_planes[j]);
            }
            delete[] yuv_planes;
            tjFree(jpeg_buf);
            tj3Destroy(handle_new);
            tjDestroy(handle_old);
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Decompress to YUV planes using old API
    if (tjDecompressToYUVPlanes(handle_old, jpeg_buf, jpeg_size, 
                                yuv_planes, width, strides, height, fuzz_int32_t_5) != 0) {
        // Get error string using new API
        char *error_str = tj3GetErrorStr(handle_new);
        // Clean up
        for (int i = 0; i < 3; i++) {
            tjFree(yuv_planes[i]);
        }
        delete[] yuv_planes;
        tjFree(jpeg_buf);
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get ICC profile using new API
    unsigned char *icc_buf = nullptr;
    size_t icc_size = 0;
    tj3GetICCProfile(handle_new, &icc_buf, &icc_size);

    // Allocate RGB buffer for decoding
    int pixel_format = TJPF_RGB;
    int pitch = width * tjPixelSize[pixel_format];
    unsigned char *rgb_buf = (unsigned char *)tjAlloc(width * height * tjPixelSize[pixel_format]);
    if (!rgb_buf) {
        if (icc_buf) tjFree(icc_buf);
        for (int i = 0; i < 3; i++) {
            tjFree(yuv_planes[i]);
        }
        delete[] yuv_planes;
        tjFree(jpeg_buf);
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode YUV planes to RGB using old API
    if (tjDecodeYUVPlanes(handle_old, (const unsigned char **)yuv_planes, 
                          fuzzer_var_tjDecodeYUVPlanes_36_2, jpeg_subsamp, rgb_buf, width, pitch, 
                          height, pixel_format, fuzz_int32_t_7) != 0) {
        // Get error string using new API
        char *error_str = tj3GetErrorStr(handle_new);
    }

    // Calculate transform buffer size using new API
    tjtransform transform;
    memset(&transform, 0, sizeof(tjtransform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = TJXOPT_CROP;
    
    size_t transform_buf_size = tj3TransformBufSize(handle_new, &transform);

    // Clean up
    tjFree(rgb_buf);
    if (icc_buf) tjFree(icc_buf);
    for (int i = 0; i < 3; i++) {
        tjFree(yuv_planes[i]);
    }
    delete[] yuv_planes;
    tjFree(jpeg_buf);
    tj3Destroy(handle_new);
    tjDestroy(handle_old);

    assert_file_closed(&in_file);
	return 0;
}