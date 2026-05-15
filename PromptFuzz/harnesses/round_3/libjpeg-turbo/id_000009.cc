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
//<score> 136, nr_unique_branch: 3
//<Quality> {"density":34,"unique_branches":{"tj3DecompressToYUV8":[[2438,7,2438,25,0,0,4,0]],"tj3YUVBufSize":[[1035,39,1035,50,0,0,4,0]],"tj3Decompress8":[[168,61,168,70,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3Alloc","tj3Alloc","tj3YUVBufSize","tj3Alloc","tj3Decompress8","tj3Decompress16","tj3DecompressToYUV8","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tjCompress2","tj3Alloc","tj3Decompress8","tj3Free","tj3SaveImage8","tj3SaveImage16","tjFree","tj3Free","tjCompressFromYUV","tj3Alloc","tj3Decompress8","tj3Free","tjFree","tjBufSizeYUV2","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3Alloc","tj3Alloc","tj3YUVBufSize","tj3Alloc","tj3Decompress8","tj3Decompress16","tj3DecompressToYUV8","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tjCompress2","tj3Alloc","tj3Decompress8","tj3Free","tj3SaveImage8","tj3SaveImage16","tjFree","tj3Free","tjCompressFromYUV","tj3Alloc","tj3Decompress8","tj3Free","tjFree","tjBufSizeYUV2","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<69) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Create handles for compression and decompression
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }

    // Try to decompress the input as JPEG
    int width, height, jpegSubsamp, jpegColorspace;
    int header_result = tj3DecompressHeader(decompress_handle, data, size);
    
    if (header_result == 0) {
        // Get JPEG header information
        width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(decompress_handle, TJPARAM_SUBSAMP);
        
        // Allocate buffer for decompressed image (8-bit)
        int pitch = width * tjPixelSize[TJPF_RGB];
        unsigned char *dstBuf8 = (unsigned char *)tj3Alloc(pitch * height);
        
        // Allocate buffer for decompressed image (16-bit)
        unsigned short *dstBuf16 = (unsigned short *)tj3Alloc(pitch * height * sizeof(unsigned short));
        
        // Allocate buffer for YUV decompression
        int align = 4;
        size_t yuvBufSize = tj3YUVBufSize(width, fuzz_int32_t_1, height, jpegSubsamp);
        unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
        
        if (dstBuf8 && dstBuf16 && yuvBuf) {
            // Decompress to 8-bit RGB
            int result8 = tj3Decompress8(decompress_handle, data, size, dstBuf8, pitch, TJPF_RGB);
            
            // Decompress to 16-bit RGB
            int result16 = tj3Decompress16(decompress_handle, data, size, dstBuf16, pitch, TJPF_RGB);
            
            // Decompress to YUV
            int resultYUV = tj3DecompressToYUV8(decompress_handle, data, size, yuvBuf, fuzz_int32_t_2);
            
            if (result8 == 0 && resultYUV == 0) {
                // Set cropping region (crop to center quarter of image)
                tjregion cropRegion;
                cropRegion.x = width / 4;
                cropRegion.y = height / 4;
                cropRegion.w = width / 2;
                cropRegion.h = height / 2;
                
                if (cropRegion.w > 0 && cropRegion.h > 0) {
                    tj3SetCroppingRegion(decompress_handle, cropRegion);
                    
                    // Allocate buffer for cropped decompression
                    int croppedPitch = cropRegion.w * tjPixelSize[TJPF_RGB];
                    unsigned char *croppedBuf = (unsigned char *)tj3Alloc(croppedPitch * cropRegion.h);
                    
                    if (croppedBuf) {
                        // Decompress cropped region
                        tj3Decompress8(decompress_handle, data, size, croppedBuf, croppedPitch, TJPF_RGB);
                        
                        // Compress the cropped image using tjCompress2
                        unsigned char *jpegBuf1 = nullptr;
                        unsigned long jpegSize1 = 0;
                        
                        if (tjCompress2(compress_handle, croppedBuf, cropRegion.w, croppedPitch, 
                                        cropRegion.h, TJPF_RGB, &jpegBuf1, &jpegSize1, 
                                        TJSAMP_444, fuzz_int32_t_3, fuzz_int32_t_4) == 0) {
                            // Decompress the newly compressed image
                            unsigned char *reconBuf = (unsigned char *)tj3Alloc(croppedPitch * cropRegion.h);
                            if (reconBuf) {
                                tj3Decompress8(decompress_handle, jpegBuf1, jpegSize1, 
                                              reconBuf, croppedPitch, TJPF_RGB);
                                tj3Free(reconBuf);
                            }
                            
                            // Save the cropped image to file (8-bit)
                            tj3SaveImage8(compress_handle, fuzz_str_5, 
                                         croppedBuf, cropRegion.w, croppedPitch, 
                                         cropRegion.h, TJPF_RGB);
                            
                            // Save the 16-bit decompressed image to file
                            tj3SaveImage16(compress_handle, fuzz_str_6, 
                                          dstBuf16, width, pitch, height, TJPF_RGB);
                            
                            tjFree(jpegBuf1);
                        }
                        
                        tj3Free(croppedBuf);
                    }
                }
                
                // Compress from YUV using tjCompressFromYUV
                unsigned char *jpegBuf2 = nullptr;
                unsigned long jpegSize2 = 0;
                
                if (tjCompressFromYUV(compress_handle, yuvBuf, width, fuzz_int32_t_7, height, 
                                     jpegSubsamp, &jpegBuf2, &jpegSize2, fuzz_int32_t_8, fuzz_int32_t_9) == 0) {
                    // Decompress the YUV-compressed image
                    unsigned char *reconBuf2 = (unsigned char *)tj3Alloc(pitch * height);
                    if (reconBuf2) {
                        tj3Decompress8(decompress_handle, jpegBuf2, jpegSize2, 
                                      reconBuf2, pitch, TJPF_RGB);
                        tj3Free(reconBuf2);
                    }
                    
                    tjFree(jpegBuf2);
                }
                
                // Calculate YUV buffer size using tjBufSizeYUV2
                unsigned long yuvSizeCalc = tjBufSizeYUV2(width, align, height, jpegSubsamp);
                (void)yuvSizeCalc; // Use the result to avoid unused variable warning
            }
        }
        
        // Clean up allocated buffers
        if (dstBuf8) tj3Free(dstBuf8);
        if (dstBuf16) tj3Free(dstBuf16);
        if (yuvBuf) tj3Free(yuvBuf);
    }
    
    // Clean up handles
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}