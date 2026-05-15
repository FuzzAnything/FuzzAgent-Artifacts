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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"tj3TransformBufSize":[[2877,7,2877,24,0,0,4,0]],"tj3GetICCProfile":[[1984,7,1984,30,0,0,4,0],[1985,9,1985,28,0,0,4,0],[1986,11,1986,17,0,0,4,0]],"tj3SetICCProfile":[[1230,7,1230,35,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3LoadImage16","tj3Alloc","tj3SetICCProfile","tj3GetICCProfile","tj3Free","tj3InitVersion","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3DecompressHeader","tj3TransformBufSize","tj3Get","tj3Get","tj3YUVBufSize","tj3Alloc","tjDecompressToYUV2","tj3TransformBufSize","tj3InitVersion","tj3Alloc","tjDecodeYUV","tj3YUVBufSize","tj3Alloc","tjEncodeYUV3","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tjDecodeYUVPlanes","tj3Free","tj3GetErrorCode","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage16","tj3Alloc","tj3SetICCProfile","tj3GetICCProfile","tj3Free","tj3InitVersion","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3DecompressHeader","tj3TransformBufSize","tj3Get","tj3Get","tj3YUVBufSize","tj3Alloc","tjDecompressToYUV2","tj3TransformBufSize","tj3InitVersion","tj3Alloc","tjDecodeYUV","tj3YUVBufSize","tj3Alloc","tjEncodeYUV3","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tjDecodeYUVPlanes","tj3Free","tj3GetErrorCode","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs in a coherent sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 256) {
        return 0;  // Need minimum data
    }

    // Write input data to a file for tj3LoadImage16
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output files
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    tjhandle handle_compress = nullptr;
    tjhandle handle_decompress = nullptr;
    tjhandle handle_transform = nullptr;
    unsigned char *transformed_buf = nullptr;
    unsigned char *yuv_buf = nullptr;
    unsigned char *rgb_buf = nullptr;
    unsigned short *image16 = nullptr;
    unsigned char *icc_buf = nullptr;
    size_t icc_size = 0;
    int width = 0, height = 0, pixel_format = 0;

    // 1. Load 16-bit image from file
    handle_decompress = tj3Init(TJINIT_DECOMPRESS);
    if (handle_decompress) {
        // First close and reopen with actual filename
        assert_file_closed(&in_file);;
        FILE *tmp_file = fopen("input_file", "wb");
        if (tmp_file) {
            fwrite(data, 1, size, tmp_file);
            assert_file_closed(&tmp_file);;
            
            image16 = tj3LoadImage16(handle_decompress, "input_file", &width, 4, &height, &pixel_format);
            // Clean up temp file
            remove("input_file");
        }
    }

    // 2. Set ICC profile (using part of input data as ICC data)
    if (handle_decompress) {
        size_t icc_data_size = size > 1024 ? 1024 : size / 2;
        icc_buf = (unsigned char *)tj3Alloc(icc_data_size);
        if (icc_buf) {
            memcpy(icc_buf, data, icc_data_size);
            tj3SetICCProfile(handle_decompress, icc_buf, icc_data_size);
        }
    }

    // 3. Get ICC profile back
    if (handle_decompress) {
        unsigned char *retrieved_icc = nullptr;
        size_t retrieved_size = 0;
        tj3GetICCProfile(handle_decompress, &retrieved_icc, &retrieved_size);
        if (retrieved_icc) {
            tj3Free(retrieved_icc);
        }
    }

    // 4. Transform JPEG (if we have valid JPEG data)
    handle_transform = tj3Init(TJINIT_TRANSFORM);
    if (handle_transform && size > 100) {
        // Use first part of input as JPEG data
        const unsigned char *jpeg_data = data;
        size_t jpeg_size = size > 4096 ? 4096 : size;

        // Allocate destination buffer
        size_t dst_size = tj3TransformBufSize(handle_transform, nullptr);
        if (dst_size > 0) {
            transformed_buf = (unsigned char *)tj3Alloc(dst_size);
            if (transformed_buf) {
                size_t actual_dst_size = 0;
                tjtransform transform;
                memset(&transform, 0, sizeof(transform));
                transform.r.x = 0;
                transform.r.y = 0;
                transform.r.w = 100;
                transform.r.h = 100;
                transform.op = TJXOP_NONE;
                transform.options = TJXOPT_CROP;

                tj3Transform(handle_transform, jpeg_data, jpeg_size, 1, 
                           &transformed_buf, &actual_dst_size, &transform);
            }
        }
    }

    // 5. Decompress transformed JPEG to YUV
    if (handle_decompress && transformed_buf) {
        // Try to get header first
        if (tj3DecompressHeader(handle_decompress, transformed_buf, 
                               tj3TransformBufSize(handle_transform, nullptr)) == 0) {
            width = tj3Get(handle_decompress, TJPARAM_JPEGWIDTH);
            height = tj3Get(handle_decompress, TJPARAM_JPEGHEIGHT);
            
            if (width > 0 && height > 0) {
                size_t yuv_size = tj3YUVBufSize(width, 4, height, TJSAMP_420);
                yuv_buf = (unsigned char *)tj3Alloc(yuv_size);
                if (yuv_buf) {
                    tjDecompressToYUV2(handle_decompress, transformed_buf, 
                                      tj3TransformBufSize(handle_transform, nullptr),
                                      yuv_buf, width, 4, height, 0);
                }
            }
        }
    }

    // 6. Encode YUV to RGB
    if (yuv_buf && width > 0 && height > 0) {
        handle_compress = tj3Init(TJINIT_COMPRESS);
        if (handle_compress) {
            // First decode YUV to RGB
            size_t rgb_size = width * height * 3;  // 3 bytes per pixel for RGB
            rgb_buf = (unsigned char *)tj3Alloc(rgb_size);
            if (rgb_buf) {
                tjDecodeYUV(handle_compress, yuv_buf, 4, TJSAMP_420,
                           rgb_buf, width, 0, height, TJPF_RGB, 0);
            }
        }
    }

    // 7. Encode RGB to YUV
    if (rgb_buf && width > 0 && height > 0 && handle_compress) {
        size_t encoded_yuv_size = tj3YUVBufSize(width, 4, height, TJSAMP_420);
        unsigned char *encoded_yuv = (unsigned char *)tj3Alloc(encoded_yuv_size);
        if (encoded_yuv) {
            tjEncodeYUV3(handle_compress, rgb_buf, width, 0, height,
                        TJPF_RGB, encoded_yuv, 4, TJSAMP_420, 0);
            tj3Free(encoded_yuv);
        }
    }

    // 8. Decode YUV planes
    if (yuv_buf && width > 0 && height > 0 && handle_decompress) {
        // Create plane pointers for YUV420
        const unsigned char *planes[3];
        int strides[3];
        int y_width = tj3YUVPlaneWidth(0, width, TJSAMP_420);
        int y_height = tj3YUVPlaneHeight(0, height, TJSAMP_420);
        int uv_width = tj3YUVPlaneWidth(1, width, TJSAMP_420);
        int uv_height = tj3YUVPlaneHeight(1, height, TJSAMP_420);
        
        // Calculate offsets (simplified - actual layout depends on alignment)
        planes[0] = yuv_buf;  // Y plane
        planes[1] = yuv_buf + y_width * y_height;  // U plane
        planes[2] = planes[1] + uv_width * uv_height;  // V plane
        
        strides[0] = y_width;
        strides[1] = uv_width;
        strides[2] = uv_width;
        
        unsigned char *decoded_rgb = (unsigned char *)tj3Alloc(width * height * 3);
        if (decoded_rgb) {
            tjDecodeYUVPlanes(handle_decompress, planes, strides, TJSAMP_420,
                             decoded_rgb, width, 0, height, TJPF_RGB, 0);
            tj3Free(decoded_rgb);
        }
    }

    // 9. Get error codes
    if (handle_decompress) {
        int error_code = tj3GetErrorCode(handle_decompress);
        (void)error_code;  // Use error code to avoid unused variable warning
    }

    // Cleanup
    if (image16) tj3Free(image16);
    if (transformed_buf) tj3Free(transformed_buf);
    if (yuv_buf) tj3Free(yuv_buf);
    if (rgb_buf) tj3Free(rgb_buf);
    if (icc_buf) tj3Free(icc_buf);
    
    if (handle_compress) tj3Destroy(handle_compress);
    if (handle_decompress) tj3Destroy(handle_decompress);
    if (handle_transform) tj3Destroy(handle_transform);
    
    assert_file_closed(&out_file);;
    if (in_file) assert_file_closed(&in_file);;
    
    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}