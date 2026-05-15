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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"tj3EncodeYUVPlanes8":[[1592,7,1592,38,0,0,4,0]]},"library_calls":["tjLoadImage","tj3InitVersion","tj3Compress8","tjGetErrorCode","tj3DecompressHeader","tj3Get","tj3Get","tj3Decompress12","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3CompressFromYUVPlanes8","tj3Free","tj3TransformBufSize","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tjLoadImage","tj3InitVersion","tj3Compress8","tjGetErrorCode","tj3DecompressHeader","tj3Get","tj3Get","tj3Decompress12","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3CompressFromYUVPlanes8","tj3Free","tj3TransformBufSize","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<115) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_24, fuzz_int32_t_array_size_24, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
	
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_18_3 = fuzz_int32_t_array_24;//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit for small inputs
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;
    unsigned char *yuvBuf = nullptr;
    unsigned char *planes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    short *dstBuf12 = nullptr;
    int width = 0, height = 0, pixelFormat = 0;
    int ret = 0;
    
    // 1. Write input data to a temporary file for tjLoadImage
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Save the input to a file for tjLoadImage
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // 2. Use tjLoadImage to load the image
    int align = 4;
    unsigned char *srcBuf = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    if (!srcBuf || width <= 0 || height <= 0) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Initialize handle
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        free(srcBuf);
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Use tj3Compress8 to compress the loaded image
    int pitch = width * tjPixelSize[pixelFormat];
    ret = tj3Compress8(handle, srcBuf, fuzz_int32_t_3, pitch, fuzz_int32_t_4, fuzz_int32_t_5, &jpegBuf, &jpegSize);
    if (ret != 0 || !jpegBuf || jpegSize == 0) {
        // Try to continue even if compression fails
    }
    
    // Check error code using tjGetErrorCode
    int errorCode = tjGetErrorCode(handle);
    
    // 5. Use tj3Decompress12 to decompress to 12-bit data
    if (jpegBuf && jpegSize > 0) {
        // First get header info
        tj3DecompressHeader(handle, jpegBuf, jpegSize);
        int decompWidth = tj3Get(handle, TJPARAM_JPEGWIDTH);
        int decompHeight = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        if (decompWidth > 0 && decompHeight > 0) {
            // Allocate 12-bit buffer
            dstBuf12 = (short*)malloc(decompWidth * decompHeight * sizeof(short) * 3);
            if (dstBuf12) {
                int pitch12 = decompWidth * sizeof(short) * 3;
                ret = tj3Decompress12(handle, jpegBuf, jpegSize, dstBuf12, pitch12, TJPF_RGB);
            }
        }
    }
    
    // 6. Prepare for YUV operations
    if (srcBuf && width > 0 && height > 0) {
        // Calculate YUV buffer size
        size_t yuvSize = tj3YUVBufSize(fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, TJSAMP_420);
        if (yuvSize > 0) {
            yuvBuf = (unsigned char*)tj3Alloc(yuvSize);
            if (yuvBuf) {
                // Encode to YUV
                ret = tj3EncodeYUV8(handle, srcBuf, fuzz_int32_t_9, pitch, fuzz_int32_t_10, fuzz_int32_t_11, yuvBuf, fuzz_int32_t_12);
                
                // 7. Use tj3CompressFromYUV8
                if (ret == 0) {
                    unsigned char *jpegBufFromYUV = nullptr;
                    size_t jpegSizeFromYUV = 0;
                    ret = tj3CompressFromYUV8(handle, yuvBuf, fuzz_int32_t_13, fuzz_int32_t_14, fuzz_int32_t_15, &jpegBufFromYUV, &jpegSizeFromYUV);
                    if (jpegBufFromYUV) {
                        tj3Free(jpegBufFromYUV);
                    }
                }
                
                // 8. Prepare YUV planes for tj3CompressFromYUVPlanes8
                for (int i = 0; i < 3; i++) {
                    int planeWidth = tj3YUVPlaneWidth(fuzz_int32_t_16, fuzz_int32_t_17, TJSAMP_420);
                    int planeHeight = tj3YUVPlaneHeight(fuzz_int32_t_18, fuzz_int32_t_19, TJSAMP_420);
                    strides[i] = planeWidth;
                    size_t planeSize = tj3YUVPlaneSize(fuzz_int32_t_20, planeWidth, strides[i], planeHeight, TJSAMP_420);
                    planes[i] = (unsigned char*)tj3Alloc(planeSize);
                }
                
                // Encode to YUV planes
                if (planes[0] && planes[1] && planes[2]) {
                    ret = tj3EncodeYUVPlanes8(handle, srcBuf, fuzz_int32_t_21, pitch, fuzz_int32_t_22, fuzz_int32_t_23, planes, strides);
                    
                    // 9. Use tj3CompressFromYUVPlanes8
                    if (ret == 0) {
                        unsigned char *jpegBufFromYUVPlanes = nullptr;
                        size_t jpegSizeFromYUVPlanes = 0;
                        ret = tj3CompressFromYUVPlanes8(handle, planes, fuzz_int32_t_25, fuzzer_var_tj3CompressFromYUVPlanes8_18_3, fuzz_int32_t_26, &jpegBufFromYUVPlanes, &jpegSizeFromYUVPlanes);
                        if (jpegBufFromYUVPlanes) {
                            tj3Free(jpegBufFromYUVPlanes);
                        }
                    }
                }
            }
        }
    }
    
    // 10. Use tj3TransformBufSize
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width > 100 ? 100 : width / 2;
    transform.r.h = height > 100 ? 100 : height / 2;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    
    if (jpegBuf && jpegSize > 0) {
        size_t transformBufSize = tj3TransformBufSize(handle, &transform);
        // We could use this size to allocate a buffer for transformation
    }
    
    // Cleanup
    if (srcBuf) free(srcBuf);
    if (jpegBuf) tj3Free(jpegBuf);
    if (dstBuf12) free(dstBuf12);
    if (yuvBuf) tj3Free(yuvBuf);
    for (int i = 0; i < 3; i++) {
        if (planes[i]) tj3Free(planes[i]);
    }
    if (handle) tj3Destroy(handle);
    
    assert_file_closed(&in_file);;
    remove("input_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}