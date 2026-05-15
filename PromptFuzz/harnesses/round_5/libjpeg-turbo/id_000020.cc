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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 350, nr_unique_branch: 266
//<Quality> {"density":35,"unique_branches":{"jinit_upsampler":[[446,7,446,30,0,0,4,0],[448,9,448,48,0,0,4,1],[448,52,448,77,0,0,4,1],[461,7,461,47,0,0,4,0],[472,7,472,30,0,0,4,1],[478,14,478,40,0,0,4,1],[483,44,483,70,0,0,4,0],[483,44,483,70,0,0,4,1],[496,9,496,35,0,0,4,1],[500,16,500,41,0,0,4,0],[500,16,500,41,0,0,4,1],[500,45,500,70,0,0,4,0],[504,16,504,45,0,0,4,0],[504,49,504,74,0,0,4,1],[521,16,521,41,0,0,4,1],[531,16,531,45,0,0,4,0],[532,16,532,45,0,0,4,0],[534,11,534,19,0,0,4,1],[544,13,544,43,0,0,4,0],[558,9,558,20,0,0,4,0],[558,9,558,20,0,0,4,1],[558,24,558,64,0,0,4,0]],"tj3CompressFromYUVPlanes8":[[1347,7,1347,26,0,0,4,0],[1435,45,1435,57,0,0,4,0]],"tj3DecodeYUV8":[[2725,7,2725,38,0,0,4,1],[2732,7,2732,35,0,0,4,1],[2740,9,2741,36,0,0,4,1],[2742,9,2743,36,0,0,4,1]],"setDecodeDefaults":[[2502,7,2502,35,0,0,4,1],[2515,15,2515,45,0,0,4,0],[2515,15,2515,45,0,0,4,1],[2518,30,2518,38,0,0,4,0],[2518,30,2518,38,0,0,4,1],[2519,30,2519,38,0,0,4,0],[2519,30,2519,38,0,0,4,1],[2523,28,2523,36,0,0,4,0],[2523,28,2523,36,0,0,4,1],[2527,15,2527,20,0,0,4,0],[2527,15,2527,20,0,0,4,1],[2528,9,2528,46,0,0,4,0],[2528,9,2528,46,0,0,4,1]],"jpeg_make_d_derived_tbl":[[201,9,201,44,0,0,4,0],[257,22,257,63,0,0,4,0]],"init_simd":[[60,31,60,53,0,0,4,0],[63,31,63,53,0,0,4,0],[101,7,101,29,0,0,4,0]],"jinit_merged_upsampler":[[472,7,472,47,0,0,4,1],[484,7,484,36,0,0,4,0],[487,9,487,46,0,0,4,0],[519,9,519,45,0,0,4,1]],"jsimd_set_h2v2_merged_upsample":[[803,7,803,23,0,0,4,1],[807,7,807,49,0,0,4,0],[46,30,46,50,6,0,4,0],[366,5,366,21,7,0,4,0],[370,5,370,22,7,0,4,1],[371,5,371,22,7,0,4,1],[375,5,375,21,7,0,4,1],[379,5,379,22,7,0,4,1],[380,5,380,22,7,0,4,1],[384,5,384,22,7,0,4,1],[385,5,385,22,7,0,4,1],[389,5,389,22,7,0,4,1],[390,5,390,22,7,0,4,1],[394,5,394,12,7,0,4,1]],"jinit_lossless_decompressor":[[285,7,285,46,0,0,4,1],[285,50,285,75,0,0,4,1]],"start_pass_lhuff_decoder":[[89,16,89,41,0,0,4,0],[89,16,89,41,0,0,4,1],[93,9,93,18,0,0,4,1],[93,22,93,44,0,0,4,1],[94,9,94,47,0,0,4,0],[103,29,103,57,0,0,4,1]],"master_selection":[[528,7,528,30,0,0,4,0],[528,7,528,30,0,0,4,1],[542,7,542,46,0,0,4,1],[553,7,553,30,0,0,4,0],[558,7,558,29,0,0,4,1],[610,7,610,27,0,0,4,0],[611,9,611,38,0,0,4,0],[611,9,611,38,0,0,4,1],[613,11,613,37,0,0,4,0],[623,11,623,37,0,0,4,0],[638,9,638,35,0,0,4,0],[650,7,650,30,0,0,4,0],[650,7,650,30,0,0,4,1],[655,9,655,35,0,0,4,0],[662,9,662,26,0,0,4,0],[662,9,662,26,0,0,4,1],[669,20,669,55,0,0,4,0],[669,20,669,55,0,0,4,1],[670,20,670,41,0,0,4,1],[671,9,671,35,0,0,4,0],[671,9,671,35,0,0,4,1],[673,14,673,41,0,0,4,1],[684,9,684,35,0,0,4,0],[691,9,691,26,0,0,4,0],[691,9,691,26,0,0,4,1],[698,11,698,34,0,0,4,1],[709,20,709,55,0,0,4,1],[710,20,710,41,0,0,4,1],[711,9,711,36,0,0,4,1],[720,7,720,27,0,0,4,0],[720,7,720,27,0,0,4,1],[721,9,721,35,0,0,4,0],[753,7,753,30,0,0,4,1]],"jinit_inverse_dct":[[340,7,340,47,0,0,4,1],[349,44,349,70,0,0,4,0],[349,44,349,70,0,0,4,1]],"prepare_range_limit_table":[[428,7,428,33,0,0,4,0],[437,17,437,32,0,0,4,0],[437,17,437,32,0,0,4,1],[441,29,441,53,0,0,4,0],[441,29,441,53,0,0,4,1]],"jsimd_set_h2v2_upsample":[[516,7,516,23,0,0,4,1],[520,7,520,47,0,0,4,0]],"default_decompress_parms":[[149,9,149,31,0,0,4,1],[151,16,151,39,0,0,4,1],[170,11,170,20,0,0,4,0],[170,24,170,33,0,0,4,0],[170,37,170,46,0,0,4,0],[172,13,172,36,0,0,4,0],[172,13,172,36,0,0,4,1]],"jpeg_core_output_dimensions":[[108,7,108,31,0,0,4,1],[241,46,241,72,0,0,4,0]],"jinit_color_deconverter":[[630,7,630,30,0,0,4,0],[632,9,632,48,0,0,4,1],[632,52,632,77,0,0,4,1],[653,3,653,21,0,0,4,1],[658,3,658,15,0,0,4,0],[659,3,659,17,0,0,4,1],[660,9,660,35,0,0,4,1],[664,3,664,16,0,0,4,1],[665,3,665,16,0,0,4,1],[670,3,670,10,0,0,4,1],[684,3,684,21,0,0,4,1],[704,3,704,15,0,0,4,1],[705,3,705,19,0,0,4,0],[706,3,706,20,0,0,4,1],[707,3,707,19,0,0,4,1],[708,3,708,20,0,0,4,1],[709,3,709,20,0,0,4,1],[710,3,710,20,0,0,4,1],[711,3,711,20,0,0,4,1],[712,3,712,20,0,0,4,1],[713,3,713,20,0,0,4,1],[714,3,714,20,0,0,4,1],[716,9,716,32,0,0,4,0],[716,36,716,70,0,0,4,1],[720,9,720,45,0,0,4,1],[755,16,755,56,0,0,4,1],[782,16,782,50,0,0,4,0],[783,11,783,47,0,0,4,0],[784,11,784,49,0,0,4,0],[785,11,785,48,0,0,4,0],[786,11,786,53,0,0,4,0],[820,3,820,18,0,0,4,1],[857,3,857,16,0,0,4,1],[873,3,873,10,0,0,4,1],[883,7,883,29,0,0,4,1]],"jinit_d_diff_controller":[[354,7,354,46,0,0,4,1],[354,50,354,75,0,0,4,1],[369,44,369,70,0,0,4,0],[369,44,369,70,0,0,4,1],[383,7,383,23,0,0,4,1]],"jpeg_add_quant_table":[[60,27,60,38,0,0,4,0]],"tj3DecodeYUVPlanes8":[[2565,15,2565,33,0,0,4,0],[2565,15,2565,33,0,0,4,1],[2569,7,2569,37,0,0,4,1],[2572,7,2572,17,0,0,4,1],[2572,21,2572,34,0,0,4,1],[2572,38,2572,52,0,0,4,1],[2572,56,2572,66,0,0,4,1],[2573,7,2573,16,0,0,4,1],[2573,20,2573,31,0,0,4,1],[2573,35,2573,50,0,0,4,1],[2573,54,2573,77,0,0,4,1],[2575,7,2575,35,0,0,4,0],[2575,40,2575,53,0,0,4,1],[2575,57,2575,70,0,0,4,1],[2578,7,2578,38,0,0,4,1],[2580,7,2580,31,0,0,4,1],[2583,7,2583,17,0,0,4,1],[2601,28,2601,41,0,0,4,0],[2601,28,2601,41,0,0,4,1],[2610,7,2610,17,0,0,4,1],[2612,7,2612,73,0,0,4,1],[2614,15,2614,25,0,0,4,0],[2614,15,2614,25,0,0,4,1],[2615,9,2615,23,0,0,4,1],[2620,7,2620,19,0,0,4,1],[2623,15,2623,40,0,0,4,0],[2623,15,2623,40,0,0,4,1],[2628,9,2628,20,0,0,4,1],[2631,9,2631,19,0,0,4,1],[2633,19,2633,47,0,0,4,0],[2633,19,2633,47,0,0,4,1],[2643,9,2643,18,0,0,4,1],[2646,19,2646,30,0,0,4,0],[2646,19,2646,30,0,0,4,1],[2648,15,2648,22,0,0,4,0],[2648,26,2648,41,0,0,4,0],[2654,17,2654,26,0,0,4,0],[2654,17,2654,26,0,0,4,1],[2657,45,2657,70,0,0,4,0],[2657,45,2657,70,0,0,4,1],[2669,7,2669,23,0,0,4,0],[2671,7,2671,30,0,0,4,0],[2673,7,2673,41,0,0,4,0],[2673,7,2673,41,0,0,4,1],[2675,15,2675,33,0,0,4,0],[2675,15,2675,33,0,0,4,1],[2680,7,2680,25,0,0,4,1],[329,7,329,12,1,0,4,1]],"jpeg_quality_scaling":[[139,7,139,19,0,0,4,0],[148,7,148,19,0,0,4,0]],"use_merged_upsample":[[42,7,42,30,0,0,4,0],[48,7,48,43,0,0,4,1],[48,47,48,73,0,0,4,1],[49,8,49,41,0,0,4,0],[50,8,50,44,0,0,4,0],[51,8,51,45,0,0,4,1],[62,8,62,44,0,0,4,1],[64,8,64,44,0,0,4,0],[65,8,65,76,0,0,4,1],[68,7,68,45,0,0,4,1],[69,7,69,45,0,0,4,1],[70,7,70,45,0,0,4,1],[71,7,71,45,0,0,4,1],[72,7,72,45,0,0,4,1],[73,7,73,45,0,0,4,1],[76,7,76,74,0,0,4,1],[77,7,77,74,0,0,4,1],[78,7,78,74,0,0,4,1]],"jinit_d_main_controller":[[435,7,435,30,0,0,4,0],[435,7,435,30,0,0,4,1],[437,9,437,48,0,0,4,1],[437,52,437,77,0,0,4,1],[446,9,446,49,0,0,4,1],[457,7,457,23,0,0,4,1],[463,7,463,41,0,0,4,1],[472,44,472,70,0,0,4,0],[472,44,472,70,0,0,4,1]],"jinit_arith_decoder":[[762,15,762,33,0,0,4,0],[762,15,762,33,0,0,4,1],[770,7,770,30,0,0,4,1]],"merged_2v_upsample":[[237,7,237,27,0,0,4,1],[250,9,250,40,0,0,4,1],[254,9,254,34,0,0,4,1],[258,9,258,21,0,0,4,0],[280,7,280,28,0,0,4,0]],"jinit_d_post_controller":[[271,7,271,30,0,0,4,0],[271,7,271,30,0,0,4,1],[273,9,273,48,0,0,4,1],[273,52,273,77,0,0,4,1],[282,9,282,49,0,0,4,1],[295,7,295,29,0,0,4,1]],"jpeg_calc_output_dimensions":[[284,7,284,31,0,0,4,1],[290,46,290,72,0,0,4,0],[293,14,293,29,0,0,4,1],[310,46,310,72,0,0,4,0],[370,7,370,33,0,0,4,0]],"jinit_lhuff_decoder":[[297,15,297,32,0,0,4,0],[297,15,297,32,0,0,4,1]],"jinit_d_coef_controller":[[887,7,887,23,0,0,4,1],[925,17,925,40,0,0,4,0],[925,17,925,40,0,0,4,1]]},"library_calls":["tjInitCompress","tjInitTransform","tjDestroy","tjDestroy","tjDestroy","tjDestroy","tjDecompressHeader3","TJBUFSIZE","tjAlloc","tj3JPEGBufSize","tj3Alloc","tjFree","tj3Free","tjDestroy","tjDestroy","tjCompress2","tjTransform","tjFree","tjBufSizeYUV2","tj3Alloc","tjEncodeYUV3","tj3Alloc","tj3DecodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3CompressFromYUVPlanes8","tj3Free","tj3Transform","tj3Free","tj3Free","tjFree","tjFree","tj3Free","tjDestroy","tjDestroy"],"critical_calls":["tjInitCompress","tjInitTransform","tjDecompressHeader3","TJBUFSIZE","tjAlloc","tj3JPEGBufSize","tj3Alloc","tjCompress2","tjTransform","tjFree","tjBufSizeYUV2","tj3Alloc","tjEncodeYUV3","tj3Alloc","tj3DecodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3CompressFromYUVPlanes8","tj3Free","tj3Transform","tj3Free","tj3Free","tjFree","tjFree","tj3Free","tjDestroy","tjDestroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize handles for old API
    tjhandle compress_handle = tjInitCompress();
    tjhandle transform_handle = tjInitTransform();
    
    if (!compress_handle || !transform_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (transform_handle) tjDestroy(transform_handle);
        return 0;
    }
    
    // Try to parse the input as a JPEG header
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = size;
    
    // Check if we have enough data for header parsing
    if (size < 2) {
        tjDestroy(compress_handle);
        tjDestroy(transform_handle);
        return 0;
    }
    
    // Try to decompress header using old API
    int header_result = tjDecompressHeader3(transform_handle, 
                                            reinterpret_cast<const unsigned char*>(data), 
                                            jpegSize,
                                            &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // If header decompression failed, use default values for fuzzing
    if (header_result != 0) {
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
        jpegColorspace = TJCS_RGB;
    }
    
    // Ensure reasonable dimensions for fuzzing
    if (width <= 0) width = 64;
    if (height <= 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Allocate buffers using both old and new APIs
    unsigned long buffer_size = TJBUFSIZE(width, height);
    unsigned char* buffer_old = tjAlloc(buffer_size);
    
    size_t buffer_size_new = tj3JPEGBufSize(width, height, jpegSubsamp);
    void* buffer_new = tj3Alloc(buffer_size_new);
    
    if (!buffer_old || !buffer_new) {
        if (buffer_old) tjFree(buffer_old);
        if (buffer_new) tj3Free(buffer_new);
        tjDestroy(compress_handle);
        tjDestroy(transform_handle);
        return 0;
    }
    
    // Create a simple source image for compression (RGB format)
    int pitch = width * 3; // RGB
    unsigned char* src_buffer = (unsigned char*)malloc(width * height * 3);
    if (src_buffer) {
        // Fill with some pattern
        for (int i = 0; i < width * height * 3; i++) {
            src_buffer[i] = (i % 256);
        }
        
        // Try compression using old API
        unsigned char* compressed_buffer = nullptr;
        unsigned long compressed_size = 0;
        int quality = 85;
        
        int compress_result = tjCompress2(compress_handle,
                                         src_buffer,
                                         width,
                                         pitch,
                                         height,
                                         TJPF_RGB,
                                         &compressed_buffer,
                                         &compressed_size,
                                         jpegSubsamp,
                                         quality,
                                         0);
        
        // If compression succeeded, try transformation
        if (compress_result == 0 && compressed_buffer && compressed_size > 0) {
            // Prepare transformation
            tjtransform transform;
            memset(&transform, 0, sizeof(transform));
            transform.r.x = 0;
            transform.r.y = 0;
            transform.r.w = width / 2;
            transform.r.h = height / 2;
            if (transform.r.w <= 0) transform.r.w = 1;
            if (transform.r.h <= 0) transform.r.h = 1;
            
            // Try transformation using old API
            unsigned char* dst_bufs[1] = {nullptr};
            unsigned long dst_sizes[1] = {0};
            
            tjTransform(transform_handle,
                       compressed_buffer,
                       compressed_size,
                       1,
                       dst_bufs,
                       dst_sizes,
                       &transform,
                       0);
            
            // Clean up transformation output
            if (dst_bufs[0]) {
                tjFree(dst_bufs[0]);
            }
            
            // Try YUV encoding/decoding using old API
            int align = 4;
            unsigned long yuv_size = tjBufSizeYUV2(width, align, height, jpegSubsamp);
            unsigned char* yuv_buffer = (unsigned char*)tj3Alloc(yuv_size);
            
            if (yuv_buffer) {
                // Encode to YUV
                tjEncodeYUV3(transform_handle,
                            src_buffer,
                            width,
                            pitch,
                            height,
                            TJPF_RGB,
                            yuv_buffer,
                            align,
                            jpegSubsamp,
                            0);
                
                // Decode from YUV using new API
                unsigned char* decoded_buffer = (unsigned char*)tj3Alloc(width * height * 3);
                if (decoded_buffer) {
                    tj3DecodeYUV8(transform_handle,
                                 yuv_buffer,
                                 align,
                                 decoded_buffer,
                                 width,
                                 pitch,
                                 height,
                                 TJPF_RGB);
                    
                    // Compress from YUV using new API
                    unsigned char* jpeg_buf_new = nullptr;
                    size_t jpeg_size_new = 0;
                    
                    tj3CompressFromYUV8(transform_handle,
                                       yuv_buffer,
                                       width,
                                       align,
                                       height,
                                       &jpeg_buf_new,
                                       &jpeg_size_new);
                    
                    // Free the new API compression output
                    if (jpeg_buf_new) {
                        tj3Free(jpeg_buf_new);
                    }
                    
                    tj3Free(decoded_buffer);
                }
                
                // Try YUV plane operations
                int plane_width = tj3YUVPlaneWidth(0, width, jpegSubsamp);
                int plane_height = tj3YUVPlaneHeight(0, height, jpegSubsamp);
                size_t plane_size = tj3YUVPlaneSize(0, width, plane_width, height, jpegSubsamp);
                
                // Try compressing from YUV planes
                const unsigned char* src_planes[3] = {yuv_buffer, 
                                                     yuv_buffer + plane_size, 
                                                     yuv_buffer + plane_size * 2};
                int strides[3] = {plane_width, plane_width/2, plane_width/2};
                
                unsigned char* jpeg_planes_buf = nullptr;
                size_t jpeg_planes_size = 0;
                
                tj3CompressFromYUVPlanes8(transform_handle,
                                         src_planes,
                                         width,
                                         strides,
                                         height,
                                         &jpeg_planes_buf,
                                         &jpeg_planes_size);
                
                if (jpeg_planes_buf) {
                    tj3Free(jpeg_planes_buf);
                }
                
                // Try transformation using new API
                unsigned char* dst_bufs_new[1] = {nullptr};
                size_t dst_sizes_new[1] = {0};
                
                if (compressed_size > 0) {
                    tj3Transform(transform_handle,
                                compressed_buffer,
                                compressed_size,
                                1,
                                dst_bufs_new,
                                dst_sizes_new,
                                &transform);
                    
                    if (dst_bufs_new[0]) {
                        tj3Free(dst_bufs_new[0]);
                    }
                }
                
                tj3Free(yuv_buffer);
            }
            
            // Free compression output
            tjFree(compressed_buffer);
        }
        
        free(src_buffer);
    }
    
    // Clean up
    if (buffer_old) tjFree(buffer_old);
    if (buffer_new) tj3Free(buffer_new);
    if (compress_handle) tjDestroy(compress_handle);
    if (transform_handle) tjDestroy(transform_handle);
    
    return 0;
}