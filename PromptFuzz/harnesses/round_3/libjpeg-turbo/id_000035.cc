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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"tj3InitVersion":[[589,23,589,45,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Set","tj3Set","TJBUFSIZE","tj3Alloc","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3YUVPlaneSize","tj3Alloc","tj3Decompress16","tj3Alloc","tj3SaveImage12","tj3Free","tj3Compress16","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tj3Alloc","tjCompress","tj3Free","tj3SetICCProfile","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Set","tj3Set","TJBUFSIZE","tj3Alloc","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3YUVPlaneSize","tj3Alloc","tj3Decompress16","tj3Alloc","tj3SaveImage12","tj3Free","tj3Compress16","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tj3Alloc","tjCompress","tj3Free","tj3SetICCProfile","tj3Free","tj3Free","tj3Destroy"],"visited":0}
/**/


// Include libjpeg-turbo headers

// Define constants that might be needed
#define TJINIT_DECOMPRESS 1
#define TJINIT_COMPRESS 2
#define TJPARAM_BOTTOMUP 0
#define TJPARAM_FASTDCT fuzz_int32_t_5
#define TJPARAM_FASTUPSAMPLE 2
#define TJPARAM_OPTIMIZE 3
#define TJPARAM_PROGRESSIVE 4
#define TJPARAM_SUBSAMP 5
#define TJPARAM_NOREALLOC 6
#define TJPARAM_QUALITY fuzz_int32_t_7
#define TJPARAM_STOPONWARNING 8
#define TJPARAM_ARITHMETIC 9
#define TJPARAM_COLORSPACE 10
#define TJPARAM_PRECISION 11
#define TJPARAM_SCANLIMIT 12
#define TJPARAM_RESTARTBLOCKS 13
#define TJPARAM_RESTARTROWS 14
#define TJPARAM_XOVERFLOW fuzz_int32_t_3
#define TJPARAM_YOVERFLOW fuzz_int32_t_4
#define TJSAMP_444 0
#define TJSAMP_422 1
#define TJSAMP_420 2
#define TJSAMP_GRAY 3
#define TJSAMP_440 4
#define TJSAMP_411 5
#define TJPF_RGB fuzz_int32_t_14
#define TJPF_BGR 1
#define TJPF_RGBX 2
#define TJPF_BGRX 3
#define TJPF_XRGB 4
#define TJPF_XBGR 5
#define TJPF_GRAY 6
#define TJPF_RGBA 7
#define TJPF_BGRA 8
#define TJPF_ABGR 9
#define TJPF_ARGB 10

// Define TJINIT_BOTH if not defined
#ifndef TJINIT_BOTH
#define TJINIT_BOTH fuzz_int32_t_1
#endif

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<116) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
	//fuzzer shim end}




    // Early return if input is too small for any meaningful processing
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle handle = nullptr;
    int width = 0, height = 0;
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    
    // 1. Initialize decompressor/compressor handle
    handle = tj3InitVersion(TJINIT_BOTH, fuzz_int32_t_2);
    if (!handle) {
	return 0;
	}
	
    
    // 2. Try to decompress header from input data
    if (tj3DecompressHeader(handle, data, size) == 0) {
        // Get image dimensions
        width = tj3Get(handle, TJPARAM_XOVERFLOW);
        height = tj3Get(handle, TJPARAM_YOVERFLOW);
        
        // If dimensions are invalid, set some defaults
        if (width <= 0) width = 64;
        if (height <= 0) height = 64;
    } else {
        // If header decompression fails, use default dimensions
        width = 64;
        height = 64;
    }
    
    // 3. Set some parameters on the handle
    tj3Set(handle, TJPARAM_FASTDCT, fuzz_int32_t_6);
    tj3Set(handle, TJPARAM_QUALITY, fuzz_int32_t_8);
    
    // 4. Allocate buffers for different operations
    size_t buf_size = TJBUFSIZE(width, height);
    void *temp_buffer = tj3Alloc(buf_size);
    
    // 5. Create a transform structure for tj3Transform
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width / 2;
    transform.r.h = height / 2;
    transform.op = TJXOP_NONE;
    transform.options = TJXOPT_CROP;
    
    // 6. Calculate transform buffer size
    size_t transform_buf_size = tj3TransformBufSize(handle, &transform);
    
    // 7. Allocate buffer for transform operation
    unsigned char *transform_buffer = nullptr;
    size_t transform_size = 0;
    
    if (transform_buf_size > 0) {
        transform_buffer = (unsigned char *)tj3Alloc(transform_buf_size);
        if (transform_buffer) {
            // 8. Perform transform operation
            tj3Transform(handle, data, size, 1, &transform_buffer, &transform_size, &transform);
        }
    }
    
    // 9. Calculate YUV plane size
    size_t yuv_plane_size = tj3YUVPlaneSize(fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11, fuzz_int32_t_12, fuzz_int32_t_13);
    
    // 10. Allocate buffer for 16-bit decompression
    size_t pitch = width * 3 * sizeof(unsigned short); // RGB, 16-bit per component
    unsigned short *decompressed_16bit = (unsigned short *)tj3Alloc(height * pitch);
    
    if (decompressed_16bit) {
        // 11. Try to decompress to 16-bit buffer
        tj3Decompress16(handle, data, size, decompressed_16bit, pitch, TJPF_RGB);
        
        // 12. Save the 16-bit image as 12-bit (tj3SaveImage12 expects 12-bit data)
        // We'll convert by shifting the 16-bit data to 12-bit
        if (width > 0 && height > 0) {
            // Create a buffer for 12-bit data
            size_t twelve_bit_size = width * height * 3 * sizeof(short);
            short *twelve_bit_buffer = (short *)tj3Alloc(twelve_bit_size);
            
            if (twelve_bit_buffer) {
                // Convert 16-bit to 12-bit by shifting
                for (size_t i = 0; i < (size_t)(width * height * 3); i++) {
                    twelve_bit_buffer[i] = (short)(decompressed_16bit[i] >> 4);
                }
                
                // Save as 12-bit image
                tj3SaveImage12(handle, "output_file_12bit.raw", 
                               twelve_bit_buffer, fuzz_int32_t_15, width * 3 * sizeof(short), 
                               fuzz_int32_t_16, TJPF_RGB);
                
                tj3Free(twelve_bit_buffer);
            }
            
            // 13. Compress the 16-bit data back to JPEG
            unsigned char *compressed_jpeg_16bit = nullptr;
            size_t compressed_size_16bit = 0;
            tj3Compress16(handle, decompressed_16bit, fuzz_int32_t_17, pitch, fuzz_int32_t_18, 
                         TJPF_RGB, &compressed_jpeg_16bit, &compressed_size_16bit);
            
            // 14. Also try to compress as 12-bit
            if (twelve_bit_size > 0) {
                // We need to allocate a buffer for 12-bit source data
                short *src_12bit = (short *)tj3Alloc(twelve_bit_size);
                if (src_12bit) {
                    // Fill with some data
                    for (size_t i = 0; i < twelve_bit_size / sizeof(short); i++) {
                        src_12bit[i] = (short)(i % 4096); // 12-bit range
                    }
                    
                    unsigned char *compressed_jpeg_12bit = nullptr;
                    size_t compressed_size_12bit = 0;
                    tj3Compress12(handle, src_12bit, fuzz_int32_t_19, width * 3 * sizeof(short), 
                                 fuzz_int32_t_20, TJPF_RGB, &compressed_jpeg_12bit, 
                                 &compressed_size_12bit);
                    
                    if (compressed_jpeg_12bit) {
                        tj3Free(compressed_jpeg_12bit);
                    }
                    tj3Free(src_12bit);
                }
            }
            
            if (compressed_jpeg_16bit) {
                tj3Free(compressed_jpeg_16bit);
            }
        }
        
        tj3Free(decompressed_16bit);
    }
    
    // 15. Use the legacy tjCompress API
    if (temp_buffer && width > 0 && height > 0) {
        // Fill buffer with some data
        memset(temp_buffer, 0x80, buf_size);
        
        unsigned char *dst_buf = (unsigned char *)tj3Alloc(buf_size);
        if (dst_buf) {
            unsigned long compressed_size = buf_size;
            tjCompress(handle, (unsigned char *)temp_buffer, fuzz_int32_t_21, 
                      width * 3, fuzz_int32_t_22, fuzz_int32_t_23, dst_buf, &compressed_size, 
                      fuzz_int32_t_24, fuzz_int32_t_25, fuzz_int32_t_26);
            tj3Free(dst_buf);
        }
    }
    
    // 16. Try to set an ICC profile (create a minimal ICC profile)
    unsigned char icc_data[] = {
        0x00, 0x00, 0x02, 0x30, 0x6C, 0x63, 0x6D, 0x73,
        0x02, 0x10, 0x00, 0x00, 0x6D, 0x6E, 0x74, 0x72,
        0x52, 0x47, 0x42, 0x20, 0x58, 0x59, 0x5A, 0x20
    };
    tj3SetICCProfile(handle, icc_data, sizeof(icc_data));
    
    // 17. Clean up
    if (temp_buffer) {
        tj3Free(temp_buffer);
    }
    
    if (transform_buffer) {
        tj3Free(transform_buffer);
    }
    
    tj3Destroy(handle);
    
    // 18. Clean up any output files we created
    remove("output_file_12bit.raw");
    
    return 0;
}