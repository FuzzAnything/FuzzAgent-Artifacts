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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 67, nr_unique_branch: 5
//<Quality> {"density":67,"unique_branches":{"start_input_bmp":[[462,5,462,11,0,0,4,0]],"start_input_ppm":[[824,16,824,62,0,0,4,0],[826,17,826,53,0,0,4,0],[865,7,865,21,0,0,4,0],[880,7,880,19,0,0,4,1]]},"library_calls":["tjInitDecompress","tj3InitVersion","tjDestroy","tj3LoadImage8","tj3Destroy","tjDestroy","tj3YUVBufSize","tj3Alloc","tj3Free","tj3Destroy","tjDestroy","tjEncodeYUV3","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3Alloc","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3DecodeYUV8","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tjEncodeYUVPlanes","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tj3DecodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tjTransform","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tjInitDecompress","tj3InitVersion","tj3LoadImage8","tj3YUVBufSize","tj3Alloc","tjEncodeYUV3","tj3Alloc","tj3DecodeYUV8","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tjEncodeYUVPlanes","tj3Alloc","tj3DecodeYUVPlanes8","tjTransform","tjFree","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
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
	  // Too small for meaningful JPEG data

    // Write input data to temporary file for tj3LoadImage8
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Initialize handles
    tjhandle old_handle = tjInitDecompress();
    if (!old_handle) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    tjhandle new_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!new_handle) {
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Load image using new API
    int width, height, pixelFormat;
    unsigned char *srcBuf = tj3LoadImage8(new_handle, "input_file", &width, 1, &height, &pixelFormat);
    if (!srcBuf) {
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Calculate YUV buffer size and allocate
    int align = 1;
    int subsamp = TJSAMP_420;
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Encode to YUV using old API
    int pitch = width * tjPixelSize[pixelFormat];
    int flags = 0;
    int encode_result = tjEncodeYUV3(old_handle, srcBuf, width, pitch, height, pixelFormat,
                                     yuvBuf, align, subsamp, flags);
    if (encode_result != 0) {
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Decode YUV back using new API
    unsigned char *decodedBuf = (unsigned char *)tj3Alloc(pitch * height);
    if (!decodedBuf) {
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    int decode_result = tj3DecodeYUV8(new_handle, yuvBuf, align, decodedBuf,
                                      width, pitch, height, pixelFormat);
    if (decode_result != 0) {
        tj3Free(decodedBuf);
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Prepare for YUV planes encoding
    int strides[3];
    unsigned char *yuvPlanes[3];
    for (int i = 0; i < 3; i++) {
        strides[i] = tj3YUVPlaneWidth(i, width, subsamp);
        int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
        size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
        yuvPlanes[i] = (unsigned char *)tj3Alloc(planeSize);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) tj3Free(yuvPlanes[j]);
            tj3Free(decodedBuf);
            tj3Free(yuvBuf);
            tj3Free(srcBuf);
            tj3Destroy(new_handle);
            tjDestroy(old_handle);
            remove("input_file");
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
        }
    }

    // Encode to YUV planes using old API
    int encode_planes_result = tjEncodeYUVPlanes(old_handle, srcBuf, width, pitch, height,
                                                 pixelFormat, yuvPlanes, strides, subsamp, flags);
    if (encode_planes_result != 0) {
        for (int i = 0; i < 3; i++) tj3Free(yuvPlanes[i]);
        tj3Free(decodedBuf);
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Decode YUV planes back using new API
    unsigned char *decodedPlanesBuf = (unsigned char *)tj3Alloc(pitch * height);
    if (!decodedPlanesBuf) {
        for (int i = 0; i < 3; i++) tj3Free(yuvPlanes[i]);
        tj3Free(decodedBuf);
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    int decode_planes_result = tj3DecodeYUVPlanes8(new_handle, yuvPlanes, strides,
                                                   decodedPlanesBuf, width, pitch, height, pixelFormat);
    if (decode_planes_result != 0) {
        tj3Free(decodedPlanesBuf);
        for (int i = 0; i < 3; i++) tj3Free(yuvPlanes[i]);
        tj3Free(decodedBuf);
        tj3Free(yuvBuf);
        tj3Free(srcBuf);
        tj3Destroy(new_handle);
        tjDestroy(old_handle);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Transform original JPEG data
    unsigned char *jpegBuf = (unsigned char *)data;
    unsigned long jpegSize = (unsigned long)size;
    
    tjtransform transform;
    memset(&transform, 0, sizeof(tjtransform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    
    unsigned char *dstBuf = nullptr;
    unsigned long dstSize = 0;
    
    int transform_result = tjTransform(old_handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform, flags);
    if (transform_result == 0 && dstBuf) {
        tjFree(dstBuf);
    }

    // Cleanup
    tj3Free(decodedPlanesBuf);
    for (int i = 0; i < 3; i++) tj3Free(yuvPlanes[i]);
    tj3Free(decodedBuf);
    tj3Free(yuvBuf);
    tj3Free(srcBuf);
    tj3Destroy(new_handle);
    tjDestroy(old_handle);
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}