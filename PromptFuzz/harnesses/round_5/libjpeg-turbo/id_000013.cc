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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 7
//<Quality> {"density":26,"unique_branches":{"save_marker":[[775,11,775,45,0,0,4,0],[841,3,841,15,0,0,4,0]],"tjBufSize":[[996,7,996,22,0,0,4,1],[1002,10,1002,23,0,0,4,1]],"tjTransform":[[3141,7,3141,25,0,0,4,0]],"tj3Transform":[[3078,7,3078,25,0,0,4,0]],"jpeg_read_header":[[289,9,289,22,0,0,4,0]]},"library_calls":["tjInitDecompress","tjInitCompress","tjDestroy","tjInitTransform","tjDestroy","tjDestroy","tjDecompressHeader3","tjGetErrorStr2","tjBufSize","tj3InitVersion","tj3Decompress8","tj3Destroy","tjCompress2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjEncodeYUVPlanes","tjDecodeYUV","tjTransform","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjInitCompress","tjInitTransform","tjDecompressHeader3","tjGetErrorStr2","tjBufSize","tj3InitVersion","tj3Decompress8","tj3Destroy","tjCompress2","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjPlaneWidth","tjEncodeYUVPlanes","tjDecodeYUV","tjTransform","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libjpeg-turbo APIs to achieve a comprehensive JPEG processing workflow:

*/


// Include libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have minimum data
    if (size < 100) {
	return 0;
	}
	
    
    tjhandle decompress_handle = nullptr;
    tjhandle compress_handle = nullptr;
    tjhandle transform_handle = nullptr;
    unsigned char *jpeg_buf = nullptr;
    unsigned char *decompressed_buf = nullptr;
    unsigned char *compressed_buf = nullptr;
    unsigned char *transform_buf = nullptr;
    unsigned char **yuv_planes = nullptr;
    int *strides = nullptr;
    tjtransform *transforms = nullptr;
    unsigned char **transform_bufs = nullptr;
    unsigned long *dst_sizes = nullptr;
    
    // Initialize handles
    decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
	return 0;
	}
	
    
    compress_handle = tjInitCompress();
    if (!compress_handle) {
        tjDestroy(decompress_handle);
        return 0;
    }
    
    transform_handle = tjInitTransform();
    if (!transform_handle) {
        tjDestroy(decompress_handle);
        tjDestroy(compress_handle);
        return 0;
    }
    
    // Use input data as JPEG buffer
    jpeg_buf = (unsigned char *)data;
    unsigned long jpeg_size = (unsigned long)size;
    
    // 1. Decompress header to get image info
    int width, height, jpegSubsamp, jpegColorspace;
    int header_result = tjDecompressHeader3(decompress_handle, jpeg_buf, jpeg_size, 
                                            &width, &height, &jpegSubsamp, &jpegColorspace);
    
    if (header_result != 0) {
        // Get error string using tjGetErrorStr2
        char *error_str = tjGetErrorStr2(decompress_handle);
        // We'll just continue with default values if header fails
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_444;
    }
    
    // Ensure reasonable dimensions for buffer allocation
    if (width <= 0) width = 64;
    if (height <= 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // 2. Use tjBufSize to calculate buffer sizes
    unsigned long buf_size = tjBufSize(width, height, jpegSubsamp);
    if (buf_size == 0) buf_size = 100 * 1024; // Default buffer size
    
    // 3. Decompress JPEG to RGB using new API (tj3Decompress8)
    tjhandle new_decompress_handle = tj3InitVersion(TJINIT_DECOMPRESS, 0);
    if (new_decompress_handle) {
        int pitch = width * 3; // RGB
        decompressed_buf = (unsigned char *)malloc(width * height * 3);
        if (decompressed_buf) {
            int decompress_result = tj3Decompress8(new_decompress_handle, 
                                                   jpeg_buf, size, 
                                                   decompressed_buf, pitch, 
                                                   TJPF_RGB);
            // Ignore result for fuzzing purposes
        }
        tj3Destroy(new_decompress_handle);
    }
    
    // 4. If we have decompressed buffer, compress it back
    if (decompressed_buf) {
        unsigned long compressed_size = 0;
        int compress_result = tjCompress2(compress_handle, decompressed_buf, 
                                          width, width * 3, height, 
                                          TJPF_RGB, &compressed_buf, 
                                          &compressed_size, jpegSubsamp, 
                                          85, 0);
        
        // 5. Encode to YUV planes
        if (compress_result == 0 && compressed_buf) {
            // Allocate YUV planes
            yuv_planes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
            strides = (int *)malloc(3 * sizeof(int));
            
            if (yuv_planes && strides) {
                // Calculate plane sizes
                strides[0] = tjPlaneWidth(0, width, jpegSubsamp);
                strides[1] = tjPlaneWidth(1, width, jpegSubsamp);
                strides[2] = tjPlaneWidth(2, width, jpegSubsamp);
                
                // Allocate planes
                for (int i = 0; i < 3; i++) {
                    int plane_height = tjPlaneHeight(i, height, jpegSubsamp);
                    int plane_width = tjPlaneWidth(i, width, jpegSubsamp);
                    yuv_planes[i] = (unsigned char *)malloc(plane_width * plane_height);
                }
                
                // Encode to YUV planes
                int encode_result = tjEncodeYUVPlanes(compress_handle, 
                                                      decompressed_buf, 
                                                      width, width * 3, height, 
                                                      TJPF_RGB, yuv_planes, 
                                                      strides, jpegSubsamp, 0);
                
                // 6. Decode YUV back to RGB
                if (encode_result == 0) {
                    unsigned char *yuv_decoded_buf = (unsigned char *)malloc(width * height * 3);
                    if (yuv_decoded_buf) {
                        int decode_result = tjDecodeYUV(compress_handle, 
                                                       yuv_planes[0], // Using first plane as source
                                                       strides[0], jpegSubsamp,
                                                       yuv_decoded_buf, width, 
                                                       width * 3, height, 
                                                       TJPF_RGB, 0);
                        free(yuv_decoded_buf);
                    }
                }
            }
        }
    }
    
    // 7. Transform the original JPEG
    transforms = (tjtransform *)malloc(sizeof(tjtransform));
    transform_bufs = (unsigned char **)malloc(sizeof(unsigned char *));
    dst_sizes = (unsigned long *)malloc(sizeof(unsigned long));
    
    if (transforms && transform_bufs && dst_sizes) {
        // Set up a simple transformation
        transforms[0].r.x = 0;
        transforms[0].r.y = 0;
        transforms[0].r.w = width / 2;
        transforms[0].r.h = height / 2;
        transforms[0].op = TJXOP_NONE;
        transforms[0].options = 0;
        transforms[0].data = nullptr;
        transforms[0].customFilter = nullptr;
        
        transform_bufs[0] = nullptr;
        dst_sizes[0] = 0;
        
        int transform_result = tjTransform(transform_handle, jpeg_buf, jpeg_size,
                                           1, transform_bufs, dst_sizes,
                                           transforms, 0);
        
        if (transform_result == 0 && transform_bufs[0]) {
            tjFree(transform_bufs[0]);
        }
    }
    
    // Cleanup
    if (yuv_planes) {
        for (int i = 0; i < 3; i++) {
            if (yuv_planes[i]) free(yuv_planes[i]);
        }
        free(yuv_planes);
    }
    
    if (strides) free(strides);
    if (decompressed_buf) free(decompressed_buf);
    if (compressed_buf) tjFree(compressed_buf);
    if (transforms) free(transforms);
    if (transform_bufs) free(transform_bufs);
    if (dst_sizes) free(dst_sizes);
    
    tjDestroy(decompress_handle);
    tjDestroy(compress_handle);
    tjDestroy(transform_handle);
    
    return 0;
}