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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"tjDecompressHeader3":[[1932,42,1932,61,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3LoadImage12","tjDecompressHeader2","tjAlloc","tjDecompress2","tjFree","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tj3Alloc","tj3DecompressToYUVPlanes8","tjPlaneHeight","tjPlaneHeight","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3Free","tj3Alloc","tj3YUVPlaneWidth","tj3Alloc","tj3YUVPlaneHeight","tj3EncodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjInitDecompress","tjDestroy"],"critical_calls":["tj3InitVersion","tj3LoadImage12","tjDecompressHeader2","tjAlloc","tjDecompress2","tjFree","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tj3Alloc","tj3DecompressToYUVPlanes8","tjPlaneHeight","tjPlaneHeight","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3Free","tj3Alloc","tj3YUVPlaneWidth","tj3Alloc","tj3YUVPlaneHeight","tj3EncodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjInitDecompress","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
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
    unsigned char* jpegBuf = nullptr;
    unsigned short* dstBuf16 = nullptr;
    unsigned char** yuvPlanes = nullptr;
    unsigned char* yuvBuffer = nullptr;
    short* img12 = nullptr;
    int* strides = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Create temporary input file with JPEG data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Load 12-bit image (may fail since input is JPEG, but we try anyway)
    int width = 0, height = 0, pixelFormat = 0;
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle) {
        img12 = tj3LoadImage12(handle, "input_file", &width, 1, &height, &pixelFormat);
        // Note: This will likely fail since input is JPEG, not raw 12-bit image
        // We proceed with other operations regardless
    }
    
    // 3. Decompress input JPEG using old API (tjDecompress2)
    if (handle) {
        unsigned char* dstBuf8 = nullptr;
        unsigned long jpegSize = size;
        
        // First get dimensions
        int width2 = 0, height2 = 0;
        if (tjDecompressHeader2(handle, (unsigned char*)data, jpegSize, &width2, &height2, nullptr) == 0) {
            if (width2 > 0 && height2 > 0) {
                int pitch = width2 * tjPixelSize[TJPF_RGB];
                dstBuf8 = (unsigned char*)tjAlloc(pitch * height2);
                if (dstBuf8) {
                    // Use tjDecompress2
                    tjDecompress2(handle, (unsigned char*)data, jpegSize, 
                                 dstBuf8, width2, pitch, height2, TJPF_RGB, 0);
                    tjFree(dstBuf8);
                }
            }
        }
    }
    
    // 4. Decompress to YUV planes using new API
    if (handle) {
        // Try to decompress header first
        if (tj3DecompressHeader(handle, data, size) == 0) {
            width = tj3Get(handle, TJPARAM_JPEGWIDTH);
            height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
            
            if (width > 0 && height > 0) {
                int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
                
                // Allocate YUV planes
                yuvPlanes = new unsigned char*[3];
                strides = new int[3];
                
                for (int i = 0; i < 3; i++) {
                    strides[i] = tj3YUVPlaneWidth(i, width, subsamp);
                    int planeHeight = tjPlaneHeight(i, height, subsamp);
                    size_t planeSize = tjPlaneSizeYUV(i, width, strides[i], planeHeight, subsamp);
                    yuvPlanes[i] = (unsigned char*)tj3Alloc(planeSize);
                }
                
                if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
                    tj3DecompressToYUVPlanes8(handle, data, size, yuvPlanes, strides);
                    
                    // Get plane height using tjPlaneHeight
                    int yHeight = tjPlaneHeight(0, height, subsamp);
                    int uvHeight = tjPlaneHeight(1, height, subsamp);
                }
            }
        }
    }
    
    // 5. Compress from YUV planes back to JPEG
    if (yuvPlanes && yuvPlanes[0] && width > 0 && height > 0) {
        unsigned char* compressedBuf = nullptr;
        size_t compressedSize = 0;
        
        if (tj3CompressFromYUVPlanes8(handle, (const unsigned char* const*)yuvPlanes, 
                                     width, strides, height, &compressedBuf, &compressedSize) == 0) {
            // 6. Decompress the newly compressed JPEG to 16-bit
            if (compressedBuf && compressedSize > 0) {
                // Get dimensions
                if (tj3DecompressHeader(handle, compressedBuf, compressedSize) == 0) {
                    int newWidth = tj3Get(handle, TJPARAM_JPEGWIDTH);
                    int newHeight = tj3Get(handle, TJPARAM_JPEGHEIGHT);
                    
                    if (newWidth > 0 && newHeight > 0) {
                        int pitch = newWidth * tjPixelSize[TJPF_RGB];
                        dstBuf16 = (unsigned short*)tj3Alloc(pitch * newHeight * sizeof(unsigned short));
                        if (dstBuf16) {
                            tj3Decompress16(handle, compressedBuf, compressedSize, 
                                           dstBuf16, pitch, TJPF_RGB);
                        }
                    }
                }
                tj3Free(compressedBuf);
            }
        }
    }
    
    // 7. Encode RGB to YUV planes (need RGB source, so create dummy one)
    if (handle && width > 0 && height > 0) {
        unsigned char* srcRGB = (unsigned char*)tj3Alloc(width * height * 3);
        if (srcRGB) {
            // Fill with pattern
            for (size_t i = 0; i < width * height * 3; i++) {
                srcRGB[i] = (i % 256);
            }
            
            // Allocate destination planes
            unsigned char** dstPlanes2 = new unsigned char*[3];
            int* strides2 = new int[3];
            int subsamp = TJSAMP_420;
            
            for (int i = 0; i < 3; i++) {
                strides2[i] = tj3YUVPlaneWidth(i, width, subsamp);
                dstPlanes2[i] = (unsigned char*)tj3Alloc(
                    strides2[i] * tj3YUVPlaneHeight(i, height, subsamp));
            }
            
            if (dstPlanes2[0] && dstPlanes2[1] && dstPlanes2[2]) {
                tj3EncodeYUVPlanes8(handle, srcRGB, width, width * 3, height, 
                                   TJPF_RGB, dstPlanes2, strides2);
            }
            
            // Cleanup
            for (int i = 0; i < 3; i++) {
                if (dstPlanes2[i]) tj3Free(dstPlanes2[i]);
            }
            delete[] dstPlanes2;
            delete[] strides2;
            tj3Free(srcRGB);
        }
    }
    
    // 8. Create output file
    out_file = fopen("output_file", "wb");
    if (out_file) {
        if (dstBuf16 && width > 0 && height > 0) {
            // Write some data to output file
            fwrite(dstBuf16, sizeof(unsigned short), 
                   width * height * 3, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (yuvPlanes) {
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) tj3Free(yuvPlanes[i]);
        }
        delete[] yuvPlanes;
    }
    
    if (strides) delete[] strides;
    if (dstBuf16) tj3Free(dstBuf16);
    if (img12) tj3Free(img12);
    if (in_file) assert_file_closed(&in_file);;
    
    // Destroy handles
    if (handle) {
        tj3Destroy(handle);
    }
    
    // Also create and destroy old API handle
    tjhandle old_handle = tjInitDecompress();
    if (old_handle) {
        tjDestroy(old_handle);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}