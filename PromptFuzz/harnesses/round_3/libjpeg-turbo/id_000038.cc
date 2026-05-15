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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 250, nr_unique_branch: 88
//<Quality> {"density":25,"unique_branches":{"decode_mcu":[[783,7,783,75,0,0,4,1],[784,7,784,32,0,0,4,1],[792,9,792,16,0,0,4,0],[793,11,793,44,0,0,4,1]],"jsimd_set_idct_islow":[[1159,7,1159,19,0,0,4,1],[1163,7,1163,49,0,0,4,0],[46,30,46,50,8,0,4,0]],"start_pass":[[157,7,157,22,0,0,4,0],[159,13,159,40,0,0,4,0],[168,7,168,22,0,0,4,1],[248,5,248,20,0,0,4,0],[254,21,254,33,0,0,4,0],[254,21,254,33,0,0,4,1],[261,5,261,20,0,0,4,1]],"tj3YUVPlaneSize":[[1092,7,1092,18,0,0,4,0]],"tj3DecompressToYUVPlanes8":[[2257,7,2257,16,0,0,4,1],[2274,23,2274,36,0,0,4,1],[2307,11,2307,20,0,0,4,1],[2312,9,2312,18,0,0,4,1]],"tj3Compress8":[[100,7,100,17,0,0,4,0]],"decode_mcu_fast":[[677,18,677,45,0,0,4,0],[677,18,677,45,0,0,4,1],[678,23,678,31,0,0,4,0],[684,9,684,10,0,0,4,0],[684,9,684,10,0,0,4,1],[690,9,690,33,0,0,4,0],[697,11,697,16,0,0,4,0],[701,9,701,33,0,0,4,0],[701,37,701,42,0,0,4,0],[703,19,703,31,0,0,4,0],[703,19,703,31,0,0,4,1],[708,13,708,14,0,0,4,0],[708,13,708,14,0,0,4,1],[715,15,715,22,0,0,4,0],[739,7,739,32,0,0,4,1],[231,7,231,26,3,0,4,1],[415,7,415,22,4,0,4,0],[415,7,415,22,4,0,4,1],[231,7,231,26,7,0,4,0],[231,7,231,26,7,0,4,1],[235,12,235,33,7,0,4,0],[235,12,235,33,7,0,4,1],[240,9,240,16,7,0,4,1],[415,7,415,22,8,0,4,0],[415,7,415,22,8,0,4,1],[415,7,415,22,19,0,4,0],[415,7,415,22,19,0,4,1],[398,7,398,17,26,0,4,1],[398,7,398,17,27,0,4,1],[398,7,398,17,28,0,4,1],[398,7,398,17,29,0,4,1],[398,7,398,17,30,0,4,1],[398,7,398,17,31,0,4,1],[415,7,415,22,33,0,4,0],[415,7,415,22,33,0,4,1],[398,7,398,17,40,0,4,0],[398,7,398,17,40,0,4,1],[401,9,401,16,40,0,4,1],[398,7,398,17,41,0,4,1],[398,7,398,17,42,0,4,0],[398,7,398,17,42,0,4,1],[401,9,401,16,42,0,4,1],[398,7,398,17,43,0,4,0],[398,7,398,17,43,0,4,1],[401,9,401,16,43,0,4,1],[398,7,398,17,44,0,4,0],[398,7,398,17,44,0,4,1],[401,9,401,16,44,0,4,1],[398,7,398,17,45,0,4,1],[398,7,398,17,60,0,4,1],[398,7,398,17,61,0,4,0],[398,7,398,17,61,0,4,1],[401,9,401,16,61,0,4,1],[398,7,398,17,62,0,4,1],[398,7,398,17,63,0,4,1],[398,7,398,17,64,0,4,1],[398,7,398,17,65,0,4,1],[398,7,398,17,66,0,4,1],[398,7,398,17,67,0,4,1],[398,7,398,17,68,0,4,1],[398,7,398,17,69,0,4,1],[398,7,398,17,70,0,4,1],[398,7,398,17,71,0,4,0],[398,7,398,17,71,0,4,1],[401,9,401,16,71,0,4,1]],"tjPlaneSizeYUV":[[1110,10,1110,23,0,0,4,1]],"tj3Decompress8":[[202,28,202,41,0,0,4,1],[227,7,227,17,0,0,4,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjDestroy","tjDestroy","tjLoadImage","tjAlloc","tjGetScalingFactors","tjCompress2","tjAlloc","tjDecompress2","tjFree","tjBufSizeYUV","tjAlloc","tjDecompressToYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjPlaneSizeYUV","tjPlaneWidth","tjDecompressToYUVPlanes","tjFree","tjFree","tj3InitVersion","tjFree","tjFree","tjDestroy","tjDestroy","tj3GetErrorStr","tj3Alloc","tj3Decompress12","tj3Free","tj3Compress8","tj3Free","tjFree","tjFree","tj3Destroy","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tjLoadImage","tjAlloc","tjGetScalingFactors","tjCompress2","tjAlloc","tjDecompress2","tjFree","tjBufSizeYUV","tjAlloc","tjDecompressToYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjPlaneSizeYUV","tjPlaneWidth","tjDecompressToYUVPlanes","tjFree","tjFree","tj3InitVersion","tj3GetErrorStr","tj3Alloc","tj3Decompress12","tj3Free","tj3Compress8","tj3Free","tjFree","tjFree","tj3Destroy","tjDestroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
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
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    // Step 1: Create temporary input file with the provided data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Initialize old-style TurboJPEG handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Load image using old API (from file)
    int width, height, pixelFormat, align = 4;
    unsigned char *loaded_image = tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);
    if (!loaded_image) {
        // If loading fails, use default values and create dummy image
        width = 64;
        height = 64;
        pixelFormat = TJPF_RGB;
        loaded_image = (unsigned char *)tjAlloc(width * height * 3);
        if (loaded_image) {
            memset(loaded_image, 128, width * height * 3);
        }
    }
    
    // Step 4: Get scaling factors (old API)
    int num_scaling_factors;
    tjscalingfactor *scaling_factors = tjGetScalingFactors(&num_scaling_factors);
    
    // Step 5: Compress using old API (tjCompress2)
    unsigned char *jpeg_buf_old = nullptr;
    unsigned long jpeg_size_old = 0;
    int jpeg_subsamp = TJSAMP_420;
    int jpeg_qual = 85;
    
    if (loaded_image) {
        tjCompress2(compress_handle, loaded_image, width, 0, height, pixelFormat,
                    &jpeg_buf_old, &jpeg_size_old, jpeg_subsamp, fuzz_int32_t_3, fuzz_int32_t_4);
    }
    
    // Step 6: Decompress using old API (tjDecompress2)
    if (jpeg_buf_old && jpeg_size_old > 0) {
        unsigned char *decomp_buf_old = (unsigned char *)tjAlloc(width * height * tjPixelSize[pixelFormat]);
        if (decomp_buf_old) {
            tjDecompress2(decompress_handle, jpeg_buf_old, jpeg_size_old,
                         decomp_buf_old, width, 0, height, pixelFormat, fuzz_int32_t_5);
            tjFree(decomp_buf_old);
        }
    }
    
    // Step 7: Calculate YUV buffer size and decompress to YUV (old API)
    unsigned long yuv_buf_size = tjBufSizeYUV(width, height, jpeg_subsamp);
    if (jpeg_buf_old && jpeg_size_old > 0 && yuv_buf_size > 0) {
        unsigned char *yuv_buf = (unsigned char *)tjAlloc(yuv_buf_size);
        if (yuv_buf) {
            tjDecompressToYUV(decompress_handle, jpeg_buf_old, jpeg_size_old, yuv_buf, fuzz_int32_t_6);
            
            // Step 8: Calculate plane dimensions
            int plane_width = tjPlaneWidth(fuzz_int32_t_7, width, jpeg_subsamp);
            int plane_height = tjPlaneHeight(fuzz_int32_t_8, height, jpeg_subsamp);
            
            // Step 9: Decompress to YUV planes (old API)
            unsigned char *yuv_planes[3];
            int strides[3] = {0, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                yuv_planes[i] = (unsigned char *)tjAlloc(tjPlaneSizeYUV(i, width, 0, height, jpeg_subsamp));
                strides[i] = tjPlaneWidth(i, width, jpeg_subsamp);
            }
            
            if (yuv_planes[0] && yuv_planes[1] && yuv_planes[2]) {
                tjDecompressToYUVPlanes(decompress_handle, jpeg_buf_old, jpeg_size_old,
                                       yuv_planes, width, strides, height, fuzz_int32_t_9);
            }
            
            for (int i = 0; i < 3; i++) {
                if (yuv_planes[i]) tjFree(yuv_planes[i]);
            }
            tjFree(yuv_buf);
        }
    }
    
    // Step 10: Initialize new-style TurboJPEG 3.0 handle
    tjhandle handle3 = tj3InitVersion(TJINIT_DECOMPRESS, fuzz_int32_t_10);
    if (!handle3) {
        // Clean up old API resources
        if (jpeg_buf_old) tjFree(jpeg_buf_old);
        if (loaded_image) tjFree(loaded_image);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 11: Get error string from new API
    const char *error_str = tj3GetErrorStr(handle3);
    
    // Step 12: Decompress using new API (12-bit)
    if (jpeg_buf_old && jpeg_size_old > 0) {
        short *decomp_buf12 = (short *)tj3Alloc(width * height * 3 * sizeof(short));
        if (decomp_buf12) {
            tj3Decompress12(handle3, jpeg_buf_old, jpeg_size_old,
                           decomp_buf12, fuzz_int32_t_11, TJPF_RGB);
            tj3Free(decomp_buf12);
        }
    }
    
    // Step 13: Compress using new API (8-bit)
    if (loaded_image) {
        unsigned char *jpeg_buf_new = nullptr;
        size_t jpeg_size_new = 0;
        
        tj3Compress8(handle3, loaded_image, width, fuzz_int32_t_12, height, pixelFormat,
                     &jpeg_buf_new, &jpeg_size_new);
        
        if (jpeg_buf_new) {
            tj3Free(jpeg_buf_new);
        }
    }
    
    // Step 14: Clean up all resources
    if (jpeg_buf_old) tjFree(jpeg_buf_old);
    if (loaded_image) tjFree(loaded_image);
    
    tj3Destroy(handle3);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}