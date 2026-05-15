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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 26
//<Quality> {"density":21,"unique_branches":{"tj3SetICCProfile":[[1230,7,1230,35,0,0,4,1],[1233,7,1233,29,0,0,4,1],[1239,7,1239,13,0,0,4,0],[1239,17,1239,24,0,0,4,0],[1240,9,1240,66,0,0,4,1]],"tjCompress2":[[1283,7,1283,22,0,0,4,0]],"jpeg_write_m_header":[[267,7,267,32,0,0,4,1],[268,8,268,46,0,0,4,1]],"jpeg_write_icc_profile":[[56,7,56,27,0,0,4,1],[56,31,56,48,0,0,4,1],[58,7,58,44,0,0,4,1],[63,7,63,61,0,0,4,0],[66,10,66,26,0,0,4,0],[66,10,66,26,0,0,4,1],[69,9,69,42,0,0,4,1],[99,12,99,20,0,0,4,0],[99,12,99,20,0,0,4,1]],"write_marker_header":[[447,7,447,36,0,0,4,1]],"tj3Compress8":[[120,7,120,22,0,0,4,0],[124,7,124,27,0,0,4,0],[124,31,124,49,0,0,4,0]],"getPixelFormat":[[350,7,350,21,0,0,4,0],[351,9,351,23,0,0,4,1]],"tjCompress":[[1300,24,1300,32,0,0,4,0],[1302,7,1302,21,0,0,4,1]],"tj3Get":[[887,3,887,26,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3Destroy","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3Get","tj3Alloc","tj3SetICCProfile","tj3Free","tj3TransformBufSize","tj3Transform","tj3Alloc","tj3Decompress16","tj3Set","tj3Set","tj3Compress16","tj3Free","tj3Free","tj3Free","tj3Alloc","tj3Alloc","TJBUFSIZE","TJBUFSIZE","tjCompress","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3Get","tj3Alloc","tj3SetICCProfile","tj3Free","tj3TransformBufSize","tj3Transform","tj3Alloc","tj3Decompress16","tj3Set","tj3Set","tj3Compress16","tj3Free","tj3Free","tj3Free","tj3Alloc","tj3Alloc","TJBUFSIZE","TJBUFSIZE","tjCompress","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize handles
    tjhandle decompressor = tj3Init(TJINIT_DECOMPRESS);
    tjhandle compressor = tj3Init(TJINIT_COMPRESS);
    tjhandle transformer = tj3Init(TJINIT_TRANSFORM);
    
    if (!decompressor || !compressor || !transformer) {
        if (decompressor) tj3Destroy(decompressor);
        if (compressor) tj3Destroy(compressor);
        if (transformer) tj3Destroy(transformer);
        return 0;
    }
    
    // Step 1: Parse JPEG header using input data
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int headerResult = tj3DecompressHeader(decompressor, data, size);
    
    if (headerResult == 0) {
        width = tj3Get(decompressor, TJPARAM_JPEGWIDTH);
        height = tj3Get(decompressor, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(decompressor, TJPARAM_SUBSAMP);
        jpegColorspace = tj3Get(decompressor, TJPARAM_COLORSPACE);
    }
    
    // Step 2: Set ICC profile from input data (use first portion as ICC)
    if (size > 128) {
        size_t iccSize = size > 1024 ? 1024 : size / 2;
        unsigned char* iccBuf = (unsigned char*)tj3Alloc(iccSize);
        if (iccBuf) {
            memcpy(iccBuf, data, iccSize);
            tj3SetICCProfile(compressor, iccBuf, iccSize);
            tj3Free(iccBuf);
        }
    }
    
    // Step 3: Transform the JPEG (if header was valid)
    if (headerResult == 0 && width > 0 && height > 0) {
        // Calculate transform buffer size
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.op = TJXOP_NONE;
        transform.options = TJXOPT_CROP;
        
        size_t transformBufSize = tj3TransformBufSize(transformer, &transform);
        
        if (transformBufSize > 0) {
            unsigned char* dstBuf = nullptr;
            size_t dstSize = transformBufSize;
            
            // Perform the transform
            int transformResult = tj3Transform(transformer, data, size, 1, &dstBuf, &dstSize, &transform);
            
            if (transformResult == 0 && dstBuf && dstSize > 0) {
                // Step 4: Decompress the transformed image to 16-bit
                int pitch = tjPixelSize[TJPF_RGB] * (width / 2);
                unsigned short* decompressedBuf = (unsigned short*)tj3Alloc(pitch * (height / 2));
                
                if (decompressedBuf) {
                    int decompressResult = tj3Decompress16(transformer, dstBuf, dstSize, 
                                                          decompressedBuf, pitch, TJPF_RGB);
                    
                    if (decompressResult == 0) {
                        // Step 5: Recompress the 16-bit data
                        unsigned char* recompressedBuf = nullptr;
                        size_t recompressedSize = 0;
                        
                        tj3Set(compressor, TJPARAM_SUBSAMP, fuzz_int32_t_1);
                        tj3Set(compressor, TJPARAM_QUALITY, fuzz_int32_t_2);
                        
                        int compressResult = tj3Compress16(compressor, decompressedBuf, 
                                                          width / 2, pitch, height / 2, 
                                                          TJPF_RGB, &recompressedBuf, &recompressedSize);
                        
                        if (recompressedBuf) {
                            tj3Free(recompressedBuf);
                        }
                    }
                    
                    tj3Free(decompressedBuf);
                }
                
                tj3Free(dstBuf);
            }
        }
    }
    
    // Step 6: Also use the old tjCompress API with a simple image
    if (size > 100) {
        // Create a small test image
        int testWidth = 64;
        int testHeight = 64;
        int pixelSize = 3; // RGB
        int pitch = testWidth * pixelSize;
        unsigned char* testBuf = (unsigned char*)tj3Alloc(pitch * testHeight);
        
        if (testBuf) {
            // Fill with pattern from input data
            for (int i = 0; i < pitch * testHeight && i < (int)size; i++) {
                testBuf[i] = data[i % size];
            }
            
            unsigned char* dstBuf = (unsigned char*)tj3Alloc(TJBUFSIZE(testWidth, testHeight));
            unsigned long compressedSize = TJBUFSIZE(testWidth, testHeight);
            
            if (dstBuf) {
                // Use the old tjCompress API
                tjCompress(compressor, testBuf, testWidth, pitch, testHeight, 
                          fuzz_int32_t_3, dstBuf, &compressedSize, TJSAMP_444, fuzz_int32_t_4, fuzz_int32_t_5);
                
                tj3Free(dstBuf);
            }
            
            tj3Free(testBuf);
        }
    }
    
    // Clean up
    tj3Destroy(decompressor);
    tj3Destroy(compressor);
    tj3Destroy(transformer);
    
    return 0;
}