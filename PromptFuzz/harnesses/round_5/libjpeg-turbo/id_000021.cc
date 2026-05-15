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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"tj3Compress16":[[87,7,87,35,0,0,4,0]],"jpeg_abort":[[41,7,41,25,0,0,4,0]]},"library_calls":["tj3InitVersion","tjInitCompress","tjInitDecompress","tj3Destroy","tjDestroy","tjDestroy","tj3Destroy","tjDestroy","tjDestroy","tj3Destroy","tjDestroy","tjDestroy","tjDecompressHeader2","tjAlloc","tjDecompress","tjAlloc","TJBUFSIZE","TJBUFSIZE","tjCompress","tjFree","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3DecompressToYUVPlanes8","tj3LoadImage12","tj3Compress16","tj3Free","tj3Free","tjFree","tjFree","tj3Free","tj3Destroy","tjDestroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitCompress","tjInitDecompress","tjDecompressHeader2","tjAlloc","tjDecompress","tjAlloc","TJBUFSIZE","TJBUFSIZE","tjCompress","tjFree","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3DecompressToYUVPlanes8","tj3LoadImage12","tj3Compress16","tj3Free","tj3Free","tjFree","tjFree","tj3Free","tj3Destroy","tjDestroy","tjDestroy"],"visited":0}
/*To create a fuzz driver using the specified libjpeg-turbo APIs, I'll implement a program that processes JPEG data through multiple encoding/decoding paths. Here's the complete solution:

*/


// Include libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data

    // Initialize handles
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    tjhandle handle_old_compress = tjInitCompress();
    tjhandle handle_old_decompress = tjInitDecompress();
    
    if (!handle_new || !handle_old_compress || !handle_old_decompress) {
        if (handle_new) tj3Destroy(handle_new);
        if (handle_old_compress) tjDestroy(handle_old_compress);
        if (handle_old_decompress) tjDestroy(handle_old_decompress);
        return 0;
    }

    // Write input data to file for tj3LoadImage12
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old_compress);
        tjDestroy(handle_old_decompress);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for saving images
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        tj3Destroy(handle_new);
        tjDestroy(handle_old_compress);
        tjDestroy(handle_old_decompress);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int ret = 0;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *dstBuf = nullptr;
    unsigned char *srcBuf = nullptr;
    unsigned char **dstPlanes = nullptr;
    int *strides = nullptr;
    short *img12 = nullptr;
    unsigned short *src16 = nullptr;
    
    // 1. First try to decompress using old API (tjDecompress)
    {
        // Get image dimensions using old API
        int width, height, jpegSubsamp;
        if (tjDecompressHeader2(handle_old_decompress, 
                               const_cast<unsigned char*>(data), 
                               static_cast<unsigned long>(size), 
                               &width, &height, &jpegSubsamp) == 0) {
            
            if (width > 0 && height > 0) {
                int pixelSize = 3; // RGB
                int pitch = width * pixelSize;
                dstBuf = static_cast<unsigned char*>(tjAlloc(pitch * height));
                
                if (dstBuf) {
                    // Call tjDecompress
                    ret = tjDecompress(handle_old_decompress,
                                      const_cast<unsigned char*>(data),
                                      static_cast<unsigned long>(size),
                                      dstBuf,
                                      width,
                                      pitch,
                                      height,
                                      pixelSize,
                                      0);
                    
                    // Now compress with old API (tjCompress)
                    if (ret == 0 && dstBuf) {
                        unsigned char *compressedBuf = static_cast<unsigned char*>(
                            tjAlloc(TJBUFSIZE(width, height)));
                        
                        if (compressedBuf) {
                            unsigned long compressedSize = TJBUFSIZE(width, height);
                            
                            // Call tjCompress
                            ret = tjCompress(handle_old_compress,
                                            dstBuf,
                                            width,
                                            pitch,
                                            height,
                                            pixelSize,
                                            compressedBuf,
                                            &compressedSize,
                                            jpegSubsamp,
                                            85,  // quality
                                            0);
                            
                            tjFree(compressedBuf);
                        }
                    }
                }
            }
        }
    }
    
    // 2. Try to decompress to YUV planes using new API
    {
        // First get dimensions
        if (tj3DecompressHeader(handle_new, data, size) == 0) {
            int width = tj3Get(handle_new, TJPARAM_JPEGWIDTH);
            int height = tj3Get(handle_new, TJPARAM_JPEGHEIGHT);
            int subsamp = tj3Get(handle_new, TJPARAM_SUBSAMP);
            
            if (width > 0 && height > 0) {
                // Allocate planes
                dstPlanes = new unsigned char*[3];
                strides = new int[3];
                
                for (int i = 0; i < 3; i++) {
                    int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
                    int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
                    strides[i] = 0; // Let library compute stride
                    
                    size_t planeSize = tj3YUVPlaneSize(i, planeWidth, 0, planeHeight, subsamp);
                    dstPlanes[i] = static_cast<unsigned char*>(tj3Alloc(planeSize));
                    
                    if (!dstPlanes[i]) {
                        for (int j = 0; j <= i; j++) {
                            if (dstPlanes[j]) tj3Free(dstPlanes[j]);
                        }
                        delete[] dstPlanes;
                        delete[] strides;
                        dstPlanes = nullptr;
                        strides = nullptr;
                        break;
                    }
                }
                
                if (dstPlanes && dstPlanes[0]) {
                    // Call tj3DecompressToYUVPlanes8
                    ret = tj3DecompressToYUVPlanes8(handle_new,
                                                   data,
                                                   size,
                                                   dstPlanes,
                                                   strides);
                }
            }
        }
    }
    
    // 3. Try to load as 12-bit image (requires file input)
    {
        fseek(in_file, 0, SEEK_SET);
        
        int width = 0, height = 0, pixelFormat = 0;
        int align = 1; // Default alignment
        
        // Call tj3LoadImage12
        img12 = tj3LoadImage12(handle_new, "input_file", &width, align, &height, &pixelFormat);
        
        if (img12 && width > 0 && height > 0) {
            // Try to compress as 16-bit (upscale 12-bit to 16-bit)
            int pixelSize = 0;
            switch (pixelFormat) {
                case TJPF_GRAY: pixelSize = 1; break;
                case TJPF_RGB: pixelSize = 3; break;
                case TJPF_BGR: pixelSize = 3; break;
                case TJPF_RGBX: pixelSize = 4; break;
                case TJPF_BGRX: pixelSize = 4; break;
                case TJPF_XRGB: pixelSize = 4; break;
                case TJPF_XBGR: pixelSize = 4; break;
                default: pixelSize = 3; break;
            }
            
            // Convert 12-bit to 16-bit by scaling
            size_t pixelCount = static_cast<size_t>(width) * height * pixelSize;
            src16 = new unsigned short[pixelCount];
            
            for (size_t i = 0; i < pixelCount; i++) {
                // Scale 12-bit (0-4095) to 16-bit (0-65535)
                src16[i] = static_cast<unsigned short>(img12[i] * 16);
            }
            
            unsigned char *jpegBuf16 = nullptr;
            size_t jpegSize16 = 0;
            
            // Call tj3Compress16
            ret = tj3Compress16(handle_new,
                               src16,
                               width,
                               width * pixelSize * sizeof(unsigned short), // pitch
                               height,
                               pixelFormat,
                               &jpegBuf16,
                               &jpegSize16);
            
            if (jpegBuf16) {
                tj3Free(jpegBuf16);
            }
        }
    }
    
    // Clean up
    if (dstPlanes) {
        for (int i = 0; i < 3; i++) {
            if (dstPlanes[i]) tj3Free(dstPlanes[i]);
        }
        delete[] dstPlanes;
    }
    
    if (strides) delete[] strides;
    if (dstBuf) tjFree(dstBuf);
    if (srcBuf) tjFree(srcBuf);
    
    if (img12) {
        // tj3LoadImage12 allocates with tj3Alloc, so use tj3Free
        tj3Free(img12);
    }
    
    if (src16) delete[] src16;
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary file
    remove("input_file");
    remove("output_file");
    
    // Destroy handles
    tj3Destroy(handle_new);
    tjDestroy(handle_old_compress);
    tjDestroy(handle_old_decompress);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}