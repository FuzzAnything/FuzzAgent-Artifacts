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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":45,"unique_branches":{"tj3DecompressHeader":[[1876,26,1876,39,0,0,4,0]]},"library_calls":["tjInitDecompress","tjDestroy","tjDestroy","tjAlloc","tjDestroy","tjDecompressHeader3","tjFree","tjDestroy","tjFree","tjDestroy","tj3GetScalingFactors","tjFree","tjDestroy","tj3YUVBufSize","tj3JPEGBufSize","tjFree","tjDestroy","tjAlloc","tjAlloc","tjAlloc","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDecompress2","tjFree","tjFree","tjFree","tjFree","tjDestroy","tj3EncodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjAlloc","tjDecodeYUVPlanes","tjFree","tjCompressFromYUV","tjFree","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjAlloc","tjDecompressHeader3","tj3GetScalingFactors","tj3YUVBufSize","tj3JPEGBufSize","tjAlloc","tjAlloc","tjAlloc","tjDecompress2","tj3EncodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjAlloc","tjDecodeYUVPlanes","tjCompressFromYUV","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;
    }

    // Create a decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Write the input data to a file to simulate libjpeg-turbo output
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Load the input as a JPEG image using old API (simulating libjpeg-turbo output)
    FILE* jpeg_file = fopen("input_file", "rb");
    if (!jpeg_file) {
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fseek(jpeg_file, 0, SEEK_END);
    long jpeg_size = ftell(jpeg_file);
    fseek(jpeg_file, 0, SEEK_SET);
    
    unsigned char* jpeg_buf = tjAlloc(jpeg_size);
    if (!jpeg_buf) {
        assert_file_closed(&jpeg_file);;
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fread(jpeg_buf, 1, jpeg_size, jpeg_file);
    assert_file_closed(&jpeg_file);;

    // Get JPEG header info
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(handle, jpeg_buf, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Limit dimensions to avoid excessive memory usage
    if (width > 4096 || height > 4096 || width <= 0 || height <= 0) {
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get scaling factors (new API)
    int numScalingFactors;
    tjscalingfactor* scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    if (!scalingFactors || numScalingFactors <= 0) {
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Calculate maximum buffer sizes
    size_t yuv_buf_size = tj3YUVBufSize(width, 1, height, jpegSubsamp);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    if (yuv_buf_size == 0 || jpeg_buf_size == 0) {
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate buffers
    unsigned char* yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
    unsigned char* raw_buf = (unsigned char*)tjAlloc(width * height * 3); // RGB buffer
    unsigned char* output_jpeg_buf = (unsigned char*)tjAlloc(jpeg_buf_size);
    unsigned long output_jpeg_size = jpeg_buf_size;

    if (!yuv_buf || !raw_buf || !output_jpeg_buf) {
        if (yuv_buf) tjFree(yuv_buf);
        if (raw_buf) tjFree(raw_buf);
        if (output_jpeg_buf) tjFree(output_jpeg_buf);
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decompress JPEG to raw RGB
    if (tjDecompress2(handle, jpeg_buf, jpeg_size, raw_buf, width, 0, height, TJPF_RGB, 0) != 0) {
        cleanup:
        tjFree(yuv_buf);
        tjFree(raw_buf);
        tjFree(output_jpeg_buf);
        tjFree(jpeg_buf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Encode raw RGB to YUV using new API
    // Note: We need a new handle for new API, but we'll reuse the old one
    // This is a hack since we don't have tj3Init in the allowed APIs
    if (tj3EncodeYUV8(handle, raw_buf, width, width * 3, height, TJPF_RGB, yuv_buf, 1) != 0) {
        goto cleanup;
    }

    // Get YUV plane dimensions using new API
    int y_plane_width = tj3YUVPlaneWidth(0, width, jpegSubsamp);
    int y_plane_height = tj3YUVPlaneHeight(0, height, jpegSubsamp);
    
    // Prepare planes for tjDecodeYUVPlanes
    // Calculate plane offsets based on subsampling
    int y_size = y_plane_width * y_plane_height;
    int uv_width = tj3YUVPlaneWidth(1, width, jpegSubsamp);
    int uv_height = tj3YUVPlaneHeight(1, height, jpegSubsamp);
    int uv_size = uv_width * uv_height;
    
    const unsigned char* src_planes[3];
    int strides[3];
    
    src_planes[0] = yuv_buf; // Y plane
    src_planes[1] = yuv_buf + y_size; // U plane
    src_planes[2] = yuv_buf + y_size + uv_size; // V plane
    
    strides[0] = y_plane_width;
    strides[1] = uv_width;
    strides[2] = uv_width;

    // Decode YUV planes back to raw RGB using old API
    unsigned char* decoded_buf = (unsigned char*)tjAlloc(width * height * 3);
    if (!decoded_buf) {
        goto cleanup;
    }
    
    if (tjDecodeYUVPlanes(handle, src_planes, strides, jpegSubsamp, decoded_buf, width, width * 3, height, TJPF_RGB, 0) != 0) {
        tjFree(decoded_buf);
        goto cleanup;
    }

    // Compress from YUV to JPEG using old API
    if (tjCompressFromYUV(handle, yuv_buf, width, 1, height, jpegSubsamp, &output_jpeg_buf, &output_jpeg_size, 85, 0) != 0) {
        tjFree(decoded_buf);
        goto cleanup;
    }

    // Clean up
    tjFree(decoded_buf);
    tjFree(yuv_buf);
    tjFree(raw_buf);
    tjFree(output_jpeg_buf);
    tjFree(jpeg_buf);
    tjDestroy(handle);
    
    // Clean up temp file
    remove("input_file");
    
    assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}