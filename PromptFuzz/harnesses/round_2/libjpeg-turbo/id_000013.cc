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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"tj3DecompressHeader":[[1873,7,1873,37,0,0,4,0]]},"library_calls":["tjInitCompress","tjDecompressHeader","tjDestroy","tj3InitVersion","tj3Alloc","tj3Decompress12","tj3Alloc","tj3Decompress16","tj3GetScalingFactors","tj3TransformBufSize","tj3Destroy","tjBufSizeYUV2","tjAlloc","tjDecompressToYUV2","tj3Free","tj3Free","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitCompress","tjDecompressHeader","tj3InitVersion","tj3Alloc","tj3Decompress12","tj3Alloc","tj3Decompress16","tj3GetScalingFactors","tj3TransformBufSize","tj3Destroy","tjBufSizeYUV2","tjAlloc","tjDecompressToYUV2","tj3Free","tj3Free","tjFree","tjFree","tjDestroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 1024) {
	return 0;
	}
	
    
    tjhandle handle_compress = nullptr;
    tjhandle handle_decompress = nullptr;
    unsigned char* jpeg_buf = nullptr;
    short* dst_buf_12bit = nullptr;
    unsigned short* dst_buf_16bit = nullptr;
    unsigned char* yuv_buf = nullptr;
    unsigned char* transform_buf = nullptr;
    FILE* in_file = nullptr;
    int width = 0, height = 0;
    
    // 1. Create compressor handle (old API)
    handle_compress = tjInitCompress();
    if (!handle_compress) {
	return 0;
	}
	
    
    // 2. Create decompressor handle implicitly through decompression functions
    // We'll use handle_compress for decompression header (old API allows this)
    
    // 3. Parse JPEG header to get dimensions
    unsigned long jpeg_size = static_cast<unsigned long>(std::min(size, (size_t)UINT32_MAX));
    int header_result = tjDecompressHeader(handle_compress, 
                                           const_cast<unsigned char*>(data), 
                                           jpeg_size, 
                                           &width, 
                                           &height);
    
    if (header_result != 0 || width <= 0 || height <= 0) {
        tjDestroy(handle_compress);
        return 0;
    }
    
    // Limit dimensions to avoid excessive memory usage
    width = std::min(width, 1024);
    height = std::min(height, 1024);
    
    // 4. Decompress to 12-bit buffer (new API - requires tj3 handle)
    // Note: tj3Decompress12 requires a tj3 handle, not the old API handle
    // We'll need to create a proper tj3 handle for this
    tjhandle handle3 = tj3InitVersion(0, 0);
    if (handle3) {
        // First try to decompress with 12-bit
        int pitch = width * 3 * sizeof(short); // 3 components, 12-bit each
        size_t dst_size_12bit = pitch * height;
        dst_buf_12bit = (short*)tj3Alloc(dst_size_12bit);
        if (dst_buf_12bit) {
            // Note: This may fail if JPEG is not 12-bit, but we call the API anyway
            tj3Decompress12(handle3, data, size, dst_buf_12bit, pitch, TJPF_RGB);
        }
        
        // 5. Decompress to 16-bit buffer (new API)
        int pitch16 = width * 3 * sizeof(unsigned short); // 3 components, 16-bit each
        size_t dst_size_16bit = pitch16 * height;
        dst_buf_16bit = (unsigned short*)tj3Alloc(dst_size_16bit);
        if (dst_buf_16bit) {
            tj3Decompress16(handle3, data, size, dst_buf_16bit, pitch16, TJPF_RGB);
        }
        
        // 6. Get scaling factors
        int num_scaling_factors = 0;
        tjscalingfactor* scaling_factors = tj3GetScalingFactors(&num_scaling_factors);
        (void)scaling_factors; // Use variable to avoid compiler warning
        (void)num_scaling_factors;
        
        // 7. Calculate transform buffer size
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.op = TJXOP_NONE;
        transform.options = 0;
        
        size_t transform_buf_size = tj3TransformBufSize(handle3, &transform);
        
        // 8. Clean up tj3 handle
        tj3Destroy(handle3);
    }
    
    // 9. Decompress to YUV using old API (tjDecompressToYUV2)
    // Calculate aligned width for YUV
    int align = 4;
    int yuv_width = ((width + align - 1) / align) * align;
    size_t yuv_buf_size = tjBufSizeYUV2(yuv_width, align, height, TJSAMP_420);
    
    if (yuv_buf_size > 0 && yuv_buf_size < 100 * 1024 * 1024) { // Limit to 100MB
        yuv_buf = (unsigned char*)tjAlloc(yuv_buf_size);
        if (yuv_buf) {
            tjDecompressToYUV2(handle_compress, 
                              const_cast<unsigned char*>(data), 
                              jpeg_size,
                              yuv_buf,
                              yuv_width,
                              align,
                              height,
                              0);
        }
    }
    
    // 10. Create a file handle from input data using fmemopen
    in_file = fmemopen((void*)data, std::min(size, (size_t)10*1024*1024), "rb");
    if (in_file) {
        // Get file descriptor if needed (though not used directly in these APIs)
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use variable to avoid compiler warning
        
        // Read some data from the file to simulate usage
        unsigned char tmp_buf[1024];
        fread(tmp_buf, 1, sizeof(tmp_buf), in_file);
        
        assert_file_closed(&in_file);;
    }
    
    // 11. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use variable to avoid compiler warning
        
        // Write some data to output file
        if (yuv_buf && yuv_buf_size > 0 && yuv_buf_size < 1024 * 1024) {
            fwrite(yuv_buf, 1, std::min(yuv_buf_size, (size_t)1024), out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (dst_buf_12bit) tj3Free(dst_buf_12bit);
    if (dst_buf_16bit) tj3Free(dst_buf_16bit);
    if (yuv_buf) tjFree(yuv_buf);
    if (transform_buf) tjFree(transform_buf);
    if (handle_compress) tjDestroy(handle_compress);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}