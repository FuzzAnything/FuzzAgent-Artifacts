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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":30,"unique_branches":{"tj3TransformBufSize":[[2877,7,2877,24,0,0,4,1],[2880,7,2880,26,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3Destroy","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3YUVPlaneSize","tj3Alloc","tjInitDecompress","tjDecodeYUVPlanes","tjDestroy","tj3CompressFromYUVPlanes8","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tjInitCompress","tjCompressFromYUVPlanes","tjDestroy","tjFree","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","TJBUFSIZE","tj3TransformBufSize","tj3Transform","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3YUVPlaneSize","tj3Alloc","tjInitDecompress","tjDecodeYUVPlanes","tjDestroy","tj3CompressFromYUVPlanes8","tj3YUVBufSize","tj3Alloc","tj3EncodeYUV8","tj3CompressFromYUV8","tj3Free","tj3Free","tjInitCompress","tjCompressFromYUVPlanes","tjDestroy","tjFree","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","TJBUFSIZE","tj3TransformBufSize","tj3Transform","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified libjpeg-turbo APIs to perform a comprehensive JPEG processing workflow:

*/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing
    
    tjhandle handle_compress = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_decompress = tj3Init(TJINIT_DECOMPRESS);
    tjhandle handle_transform = tj3Init(TJINIT_TRANSFORM);
    
    if (!handle_compress || !handle_decompress || !handle_transform) {
        if (handle_compress) tj3Destroy(handle_compress);
        if (handle_decompress) tj3Destroy(handle_decompress);
        if (handle_transform) tj3Destroy(handle_transform);
        return 0;
    }
    
    // 1. First try to decompress input as JPEG
    unsigned char* jpeg_buffer = const_cast<unsigned char*>(data);
    size_t jpeg_size = size;
    
    // Check if it's a valid JPEG
    int width = 0, height = 0;
    int subsamp = TJSAMP_444;
    if (tj3DecompressHeader(handle_decompress, jpeg_buffer, jpeg_size) == 0) {
        width = tj3Get(handle_decompress, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle_decompress, TJPARAM_JPEGHEIGHT);
        subsamp = tj3Get(handle_decompress, TJPARAM_SUBSAMP);
        
        if (width > 0 && height > 0) {
            // 2. Decompress to YUV planes using tj3DecompressToYUVPlanes8
            int strides[3];
            unsigned char* yuv_planes[3];
            
            // Allocate YUV planes
            for (int i = 0; i < 3; i++) {
                int plane_width = tj3YUVPlaneWidth(i, width, subsamp);
                int plane_height = tj3YUVPlaneHeight(i, height, subsamp);
                strides[i] = plane_width;
                size_t plane_size = tj3YUVPlaneSize(i, plane_width, strides[i], plane_height, subsamp);
                yuv_planes[i] = static_cast<unsigned char*>(tj3Alloc(plane_size));
            }
            
            if (tj3DecompressToYUVPlanes8(handle_decompress, jpeg_buffer, jpeg_size, 
                                         yuv_planes, strides) == 0) {
                
                // 3. Use tj3YUVPlaneSize
                size_t y_plane_size = tj3YUVPlaneSize(0, width, strides[0], height, subsamp);
                
                // 4. Decode YUV planes to RGB using tjDecodeYUVPlanes (old API)
                unsigned long rgb_size = width * height * 3;
                unsigned char* rgb_buffer = static_cast<unsigned char*>(tj3Alloc(rgb_size));
                if (rgb_buffer) {
                    // Need old API handle for tjDecodeYUVPlanes
                    tjhandle old_handle = tjInitDecompress();
                    if (old_handle) {
                        tjDecodeYUVPlanes(old_handle, 
                                         const_cast<const unsigned char**>(yuv_planes), 
                                         strides, subsamp, rgb_buffer, 
                                         width, width * 3, height, TJPF_RGB, 0);
                        tjDestroy(old_handle);
                    }
                    
                    // 5. Compress from YUV planes using tj3CompressFromYUVPlanes8
                    unsigned char* compressed_jpeg1 = nullptr;
                    size_t compressed_size1 = 0;
                    
                    tj3CompressFromYUVPlanes8(handle_compress, 
                                            const_cast<const unsigned char**>(yuv_planes), 
                                            width, strides, height, 
                                            &compressed_jpeg1, &compressed_size1);
                    
                    // 6. Create packed YUV buffer and compress using tj3CompressFromYUV8
                    size_t yuv_buf_size = tj3YUVBufSize(width, 4, height, subsamp);
                    unsigned char* yuv_buffer = static_cast<unsigned char*>(tj3Alloc(yuv_buf_size));
                    
                    if (yuv_buffer) {
                        // Encode RGB to YUV (simplified - in reality we'd need proper conversion)
                        tj3EncodeYUV8(handle_compress, rgb_buffer, width, width * 3, 
                                     height, TJPF_RGB, yuv_buffer, 4);
                        
                        unsigned char* compressed_jpeg2 = nullptr;
                        size_t compressed_size2 = 0;
                        
                        tj3CompressFromYUV8(handle_compress, yuv_buffer, width, 4, 
                                          height, &compressed_jpeg2, &compressed_size2);
                        
                        tj3Free(yuv_buffer);
                        if (compressed_jpeg2) tj3Free(compressed_jpeg2);
                    }
                    
                    // 7. Use old API tjCompressFromYUVPlanes
                    unsigned char* compressed_jpeg3 = nullptr;
                    unsigned long compressed_size3 = 0;
                    tjhandle old_compress = tjInitCompress();
                    
                    if (old_compress) {
                        tjCompressFromYUVPlanes(old_compress, 
                                              const_cast<const unsigned char**>(yuv_planes), 
                                              width, strides, height, subsamp, 
                                              &compressed_jpeg3, &compressed_size3, 
                                              85, 0);
                        tjDestroy(old_compress);
                        if (compressed_jpeg3) tjFree(compressed_jpeg3);
                    }
                    
                    // 8. Try 12-bit compression (dummy data since we don't have 12-bit source)
                    int small_width = width > 100 ? 100 : width;
                    int small_height = height > 100 ? 100 : height;
                    size_t short_buffer_size = small_width * small_height * 3;
                    short* twelve_bit_buffer = static_cast<short*>(
                        malloc(short_buffer_size * sizeof(short)));
                    
                    if (twelve_bit_buffer) {
                        // Fill with dummy 12-bit data
                        for (size_t i = 0; i < short_buffer_size; i++) {
                            twelve_bit_buffer[i] = static_cast<short>(i % 4096);
                        }
                        
                        unsigned char* compressed_jpeg4 = nullptr;
                        size_t compressed_size4 = 0;
                        
                        tj3Compress12(handle_compress, twelve_bit_buffer, 
                                     small_width, small_width * 3 * sizeof(short), 
                                     small_height, TJPF_RGB, 
                                     &compressed_jpeg4, &compressed_size4);
                        
                        free(twelve_bit_buffer);
                        if (compressed_jpeg4) tj3Free(compressed_jpeg4);
                    }
                    
                    tj3Free(rgb_buffer);
                    if (compressed_jpeg1) tj3Free(compressed_jpeg1);
                }
                
                // Clean up YUV planes
                for (int i = 0; i < 3; i++) {
                    if (yuv_planes[i]) tj3Free(yuv_planes[i]);
                }
            }
        }
    }
    
    // 9. Use TJBUFSIZE (old macro)
    unsigned long bufsize = TJBUFSIZE(1024, 768);
    
    // 10. Use tj3TransformBufSize
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.w = 100;
    transform.r.h = 100;
    transform.op = TJXOP_NONE;
    
    size_t transform_buf_size = tj3TransformBufSize(handle_transform, &transform);
    
    // 11. Try to transform the input JPEG if it was valid
    if (width > 0 && height > 0) {
        unsigned char* dst_buf = nullptr;
        size_t dst_size = 0;
        
        if (tj3Transform(handle_transform, jpeg_buffer, jpeg_size, 
                        1, &dst_buf, &dst_size, &transform) == 0) {
            if (dst_buf) tj3Free(dst_buf);
        }
    }
    
    // Cleanup
    tj3Destroy(handle_compress);
    tj3Destroy(handle_decompress);
    tj3Destroy(handle_transform);
    
    return 0;
}