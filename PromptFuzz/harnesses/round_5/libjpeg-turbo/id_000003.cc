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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 129, nr_unique_branch: 25
//<Quality> {"density":43,"unique_branches":{"tj3GetICCProfile":[[1984,7,1984,30,0,0,4,1],[1994,7,1994,21,0,0,4,1],[1996,7,1996,15,0,0,4,0],[1997,9,1997,62,0,0,4,1],[1999,9,1999,32,0,0,4,1]],"tj3Compress16":[[87,7,87,35,0,0,4,1],[90,7,90,21,0,0,4,1],[90,25,90,35,0,0,4,1],[90,39,90,48,0,0,4,1],[90,52,90,63,0,0,4,1],[91,7,91,22,0,0,4,1],[91,26,91,49,0,0,4,1],[91,53,91,68,0,0,4,1],[92,7,92,23,0,0,4,1],[95,7,95,22,0,0,4,0],[95,26,95,45,0,0,4,0]],"tj3Set":[[742,3,742,24,0,0,4,0],[725,7,725,16,3,0,4,1],[725,20,725,29,3,0,4,1]],"tj3SetICCProfile":[[1230,7,1230,35,0,0,4,1],[1233,7,1233,29,0,0,4,1],[1239,7,1239,13,0,0,4,0],[1239,17,1239,24,0,0,4,0],[1240,9,1240,66,0,0,4,1]],"tjDecompressHeader3":[[1933,7,1933,29,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3Destroy","tj3LoadImage16","tj3GetErrorCode","tj3Alloc","tj3Set","tj3Compress16","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy","tj3Alloc","tj3SetICCProfile","tj3Free","tj3GetICCProfile","tj3Free","tjDecompressHeader3","tj3Transform","tjBufSizeYUV2","tj3Alloc","tjDecompressToYUV2","tj3Alloc","tjDecodeYUV","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjDecodeYUVPlanes","tjBufSizeYUV2","tj3Alloc","tjEncodeYUV3","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3LoadImage16","tj3GetErrorCode","tj3Set","tj3Compress16","tj3Free","tj3Alloc","tj3SetICCProfile","tj3Free","tj3GetICCProfile","tj3Free","tjDecompressHeader3","tj3Transform","tjBufSizeYUV2","tj3Alloc","tjDecompressToYUV2","tj3Alloc","tjDecodeYUV","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjDecodeYUVPlanes","tjBufSizeYUV2","tj3Alloc","tjEncodeYUV3","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful operations

    // Create temporary files for image operations
    const char* input_filename = "input_file";
    const char* output_filename = "output_file";
    
    // Write input data to a temporary file for tj3LoadImage16
    FILE* in_file = fopen(input_filename, "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Initialize TurboJPEG handles
    tjhandle decompressor = tj3Init(TJINIT_DECOMPRESS);
    tjhandle compressor = tj3Init(TJINIT_COMPRESS);
    tjhandle transformer = tj3Init(TJINIT_TRANSFORM);
    
    if (!decompressor || !compressor || !transformer) {
        if (decompressor) tj3Destroy(decompressor);
        if (compressor) tj3Destroy(compressor);
        if (transformer) tj3Destroy(transformer);
        remove(input_filename);
        remove(output_filename);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 1. Try to load a 16-bit image from file
    int width = 0, height = 0, pixelFormat = 0;
    unsigned short* loaded_image = tj3LoadImage16(compressor, input_filename, &width, 4, &height, &pixelFormat);
    int error_code = tj3GetErrorCode(compressor);  // Check error code

    // 2. If image loading failed, use input data directly as JPEG
    unsigned char* jpeg_buf = nullptr;
    size_t jpeg_size = size;
    
    if (!loaded_image) {
        jpeg_buf = (unsigned char*)tj3Alloc(size);
        if (jpeg_buf) {
            memcpy(jpeg_buf, data, size);
        }
    } else {
        // Convert loaded 16-bit image to JPEG
        tj3Set(compressor, TJPARAM_BOTTOMUP, 0);
        unsigned char* compressed_buf = nullptr;
        size_t compressed_size = 0;
        
        if (tj3Compress16(compressor, loaded_image, width, 0, height, pixelFormat, 
                         &compressed_buf, &compressed_size) == 0) {
            jpeg_buf = compressed_buf;
            jpeg_size = compressed_size;
        }
        tj3Free(loaded_image);
    }

    if (!jpeg_buf) {
        tj3Destroy(decompressor);
        tj3Destroy(compressor);
        tj3Destroy(transformer);
        remove(input_filename);
        remove(output_filename);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Set ICC profile (using part of input data)
    size_t icc_size = size > 1000 ? 500 : size / 2;
    unsigned char* icc_buf = (unsigned char*)tj3Alloc(icc_size);
    if (icc_buf) {
        memcpy(icc_buf, data, icc_size > size ? size : icc_size);
        tj3SetICCProfile(compressor, icc_buf, icc_size);
        tj3Free(icc_buf);
    }

    // 4. Get ICC profile back
    unsigned char* retrieved_icc = nullptr;
    size_t retrieved_icc_size = 0;
    tj3GetICCProfile(compressor, &retrieved_icc, &retrieved_icc_size);
    if (retrieved_icc) {
        tj3Free(retrieved_icc);
    }

    // 5. Decompress JPEG header to get dimensions
    int jpeg_width, jpeg_height, jpeg_subsamp;
    if (tjDecompressHeader3(decompressor, jpeg_buf, (unsigned long)jpeg_size,
                           &jpeg_width, &jpeg_height, &jpeg_subsamp, nullptr) == 0) {
        
        // 6. Transform the JPEG image
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = jpeg_width / 2;
        transform.r.h = jpeg_height / 2;
        transform.op = TJXOP_NONE;
        transform.options = TJXOPT_CROP;
        
        unsigned char* dst_bufs[1] = {nullptr};
        size_t dst_sizes[1] = {0};
        
        if (tj3Transform(transformer, jpeg_buf, jpeg_size, 1, dst_bufs, dst_sizes, &transform) == 0) {
            if (dst_bufs[0]) {
                // 7. Decompress transformed JPEG to YUV
                int align = 4;
                size_t yuv_buf_size = tjBufSizeYUV2(jpeg_width / 2, align, jpeg_height / 2, jpeg_subsamp);
                unsigned char* yuv_buf = (unsigned char*)tj3Alloc(yuv_buf_size);
                
                if (yuv_buf && tjDecompressToYUV2(decompressor, dst_bufs[0], (unsigned long)dst_sizes[0],
                                                 yuv_buf, jpeg_width / 2, align, jpeg_height / 2, 0) == 0) {
                    
                    // 8. Decode YUV to RGB using tjDecodeYUV
                    int pitch = tjPixelSize[TJPF_RGB] * (jpeg_width / 2);
                    unsigned char* rgb_buf1 = (unsigned char*)tj3Alloc(pitch * (jpeg_height / 2));
                    
                    if (rgb_buf1) {
                        tjDecodeYUV(decompressor, yuv_buf, align, jpeg_subsamp,
                                   rgb_buf1, jpeg_width / 2, pitch, jpeg_height / 2, TJPF_RGB, 0);
                        
                        // 9. Also decode using planes API
                        unsigned char* rgb_buf2 = (unsigned char*)tj3Alloc(pitch * (jpeg_height / 2));
                        if (rgb_buf2) {
                            // Prepare plane pointers
                            const unsigned char* src_planes[3];
                            int strides[3];
                            int plane_width = tjPlaneWidth(0, jpeg_width / 2, jpeg_subsamp);
                            int plane_height = tjPlaneHeight(0, jpeg_height / 2, jpeg_subsamp);
                            
                            src_planes[0] = yuv_buf;  // Y plane
                            strides[0] = plane_width;
                            
                            src_planes[1] = yuv_buf + plane_width * plane_height;  // U plane
                            strides[1] = tjPlaneWidth(1, jpeg_width / 2, jpeg_subsamp);
                            
                            src_planes[2] = src_planes[1] + strides[1] * tjPlaneHeight(1, jpeg_height / 2, jpeg_subsamp);  // V plane
                            strides[2] = tjPlaneWidth(2, jpeg_width / 2, jpeg_subsamp);
                            
                            tjDecodeYUVPlanes(decompressor, src_planes, strides, jpeg_subsamp,
                                            rgb_buf2, jpeg_width / 2, pitch, jpeg_height / 2, TJPF_RGB, 0);
                            
                            // 10. Encode RGB back to YUV using tjEncodeYUV3
                            size_t yuv2_buf_size = tjBufSizeYUV2(jpeg_width / 2, align, jpeg_height / 2, jpeg_subsamp);
                            unsigned char* yuv2_buf = (unsigned char*)tj3Alloc(yuv2_buf_size);
                            
                            if (yuv2_buf) {
                                tjEncodeYUV3(compressor, rgb_buf1, jpeg_width / 2, pitch, jpeg_height / 2,
                                           TJPF_RGB, yuv2_buf, align, jpeg_subsamp, 0);
                                
                                tj3Free(yuv2_buf);
                            }
                            
                            tj3Free(rgb_buf2);
                        }
                        tj3Free(rgb_buf1);
                    }
                    tj3Free(yuv_buf);
                }
                tj3Free(dst_bufs[0]);
            }
        }
    }

    // Clean up
    if (jpeg_buf != (unsigned char*)data) {
        tj3Free(jpeg_buf);
    }
    
    tj3Destroy(decompressor);
    tj3Destroy(compressor);
    tj3Destroy(transformer);
    
    remove(input_filename);
    remove(output_filename);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}