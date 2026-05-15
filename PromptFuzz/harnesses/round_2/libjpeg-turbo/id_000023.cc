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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 176, nr_unique_branch: 19
//<Quality> {"density":22,"unique_branches":{"tjCompress":[[1300,24,1300,32,0,0,4,0],[1302,7,1302,21,0,0,4,1]],"tjCompress2":[[1283,7,1283,22,0,0,4,0]],"tjEncodeYUVPlanes":[[1720,7,1720,18,0,0,4,1],[1720,22,1720,46,0,0,4,1],[340,7,340,12,1,0,4,1]],"tj3EncodeYUVPlanes8":[[1587,7,1587,20,0,0,4,0]],"getPixelFormat":[[349,7,349,21,0,0,4,1],[350,7,350,21,0,0,4,0],[351,9,351,23,0,0,4,1]],"tj3Compress8":[[120,7,120,22,0,0,4,0]],"tj3Decompress8":[[168,61,168,70,0,0,4,1],[169,7,169,22,0,0,4,1],[169,26,169,49,0,0,4,1],[172,7,172,22,0,0,4,1],[184,7,184,45,0,0,4,0],[184,7,184,45,0,0,4,1],[284,7,284,41,0,0,4,0],[286,7,286,25,0,0,4,0]]},"library_calls":["tj3InitVersion","tjInitCompress","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3YUVBufSize","tj3Decompress8","tj3SaveImage8","tj3SetScalingFactor","tj3EncodeYUV8","tj3DecodeYUV8","tjBufSize","tjAlloc","tjCompress","tjFree","tj3EncodeYUV8","tjCompressFromYUV","tjFree","tjEncodeYUVPlanes","tjFree","tj3Free","tjFree","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitCompress","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3YUVBufSize","tj3Decompress8","tj3SaveImage8","tj3SetScalingFactor","tj3EncodeYUV8","tj3DecodeYUV8","tjBufSize","tjAlloc","tjCompress","tjFree","tj3EncodeYUV8","tjCompressFromYUV","tjFree","tjEncodeYUVPlanes","tjFree","tj3Free","tjFree","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
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
	 // Too small to be meaningful
    
    tjhandle handle = nullptr;
    tjhandle old_handle = nullptr;
    unsigned char *jpeg_buf = nullptr;
    unsigned char *yuv_buf = nullptr;
    unsigned char *dst_buf = nullptr;
    unsigned char **dst_planes = nullptr;
    int *strides = nullptr;
    unsigned char *output_buf = nullptr;
    unsigned long compressed_size = 0;
    unsigned long jpeg_size = 0;
    FILE *temp_file = nullptr;
    int width = 0, height = 0;
    const char *filename = "output_file";
    
    // Initialize both new and old API handles
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) goto cleanup;
    
    old_handle = tjInitCompress();
    if (!old_handle) goto cleanup;
    
    // Write input data to temp file for tj3SaveImage8
    temp_file = fopen("input_file", "wb");
    if (!temp_file) goto cleanup;
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    temp_file = nullptr;
    
    // Try to decompress header first
    if (tj3DecompressHeader(handle, data, size) != 0) {
        // If not valid JPEG, try to use as raw data
        width = 64;
        height = 64;
    } else {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    }
    
    if (width <= 0 || height <= 0) {
        width = 64;
        height = 64;
    }
    
    // Allocate buffers
    dst_buf = (unsigned char *)malloc(width * height * 3);
    if (!dst_buf) goto cleanup;
    
    yuv_buf = (unsigned char *)tj3Alloc(tj3YUVBufSize(width, 4, height, TJSAMP_420));
    if (!yuv_buf) goto cleanup;
    
    // 1. tj3SaveImage8 - save decompressed image
    if (tj3Decompress8(handle, data, size, dst_buf, 0, TJPF_RGB) == 0) {
        tj3SaveImage8(handle, filename, dst_buf, width, 0, height, TJPF_RGB);
    }
    
    // 2. tj3SetScalingFactor - set scaling and decompress again
    tjscalingfactor scaling;
    scaling.num = 1;
    scaling.denom = 2;
    tj3SetScalingFactor(handle, scaling);
    
    // 3. tj3DecodeYUV8 - decode YUV to RGB
    // First encode to YUV
    if (tj3EncodeYUV8(handle, dst_buf, width, 0, height, TJPF_RGB, yuv_buf, 4) == 0) {
        unsigned char *decoded_buf = (unsigned char *)malloc(width * height * 3);
        if (decoded_buf) {
            tj3DecodeYUV8(handle, yuv_buf, 4, decoded_buf, width, 0, height, TJPF_RGB);
            free(decoded_buf);
        }
    }
    
    // 4. tjCompress - old API compression
    compressed_size = tjBufSize(width, height, TJSAMP_420);
    output_buf = (unsigned char *)tjAlloc(compressed_size);
    if (output_buf) {
        tjCompress(old_handle, dst_buf, width, 0, height, 3, output_buf, &compressed_size, 
                   TJSAMP_420, 85, TJFLAG_FASTDCT);
        tjFree(output_buf);
        output_buf = nullptr;
    }
    
    // 5. tjCompressFromYUV - compress from YUV buffer
    if (tj3EncodeYUV8(handle, dst_buf, width, 0, height, TJPF_RGB, yuv_buf, 4) == 0) {
        unsigned char *jpeg_buf2 = nullptr;
        if (tjCompressFromYUV(old_handle, yuv_buf, width, 4, height, TJSAMP_420, 
                             &jpeg_buf2, &jpeg_size, 85, TJFLAG_FASTDCT) == 0) {
            if (jpeg_buf2) tjFree(jpeg_buf2);
        }
    }
    
    // 6. tjEncodeYUVPlanes - encode to YUV planes
    dst_planes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
    strides = (int *)malloc(3 * sizeof(int));
    if (dst_planes && strides) {
        for (int i = 0; i < 3; i++) {
            dst_planes[i] = nullptr;
            strides[i] = 0;
        }
        
        if (tjEncodeYUVPlanes(old_handle, dst_buf, width, 0, height, TJPF_RGB,
                             dst_planes, strides, TJSAMP_420, TJFLAG_FASTDCT) == 0) {
            // Free the allocated planes
            for (int i = 0; i < 3; i++) {
                if (dst_planes[i]) tjFree(dst_planes[i]);
            }
        }
    }

cleanup:
    // Clean up allocated resources
    if (dst_buf) free(dst_buf);
    if (yuv_buf) tj3Free(yuv_buf);
    if (output_buf) tjFree(output_buf);
    if (dst_planes) free(dst_planes);
    if (strides) free(strides);
    if (handle) tj3Destroy(handle);
    if (old_handle) tjDestroy(old_handle);
    if (temp_file) assert_file_closed(&temp_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	return 0;
}