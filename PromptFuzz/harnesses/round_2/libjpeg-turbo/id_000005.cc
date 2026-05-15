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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":44,"unique_branches":{},"library_calls":["tjInitDecompress","tjGetScalingFactors","tjDestroy","tjAlloc","tjDestroy","tjDestroy","tjDecompressHeader2","tjFree","tjDestroy","tj3InitVersion","tj3DecompressHeader","tj3GetICCProfile","tj3Destroy","TJBUFSIZE","tjFree","tjDestroy","tjAlloc","tjAlloc","tjFree","tjFree","tjFree","tjDestroy","tjDecompress","tjDecompress2","tj3InitVersion","tj3Alloc","tj3DecompressHeader","tj3Decompress12","tj3Free","tj3Destroy","tj3InitVersion","tj3Alloc","tj3Alloc","tj3EncodeYUVPlanes8","tjAlloc","TJBUFSIZEYUV","tjDecodeYUV","tjFree","tj3Free","tj3Free","tj3Destroy","tjGetErrorStr2","tjFree","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjGetScalingFactors","tjAlloc","tjDecompressHeader2","tj3InitVersion","tj3DecompressHeader","tj3GetICCProfile","tj3Destroy","TJBUFSIZE","tjAlloc","tjAlloc","tjDecompress","tjDecompress2","tj3InitVersion","tj3Alloc","tj3DecompressHeader","tj3Decompress12","tj3Free","tj3Destroy","tj3InitVersion","tj3Alloc","tj3Alloc","tj3EncodeYUVPlanes8","tjAlloc","TJBUFSIZEYUV","tjDecodeYUV","tjFree","tj3Free","tj3Free","tj3Destroy","tjGetErrorStr2","tjFree","tjFree","tjFree","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Initialize decompressor handle using legacy API
  tjhandle handle = tjInitDecompress();
  if (!handle) {
    return 0;
  }

  // Get scaling factors
  int numScalingFactors;
  tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
  if (!scalingFactors || numScalingFactors <= 0) {
    tjDestroy(handle);
    return 0;
  }

  // Create a temporary buffer for JPEG data
  unsigned char *jpegBuf = nullptr;
  unsigned long jpegSize = size;
  
  // Allocate buffer and copy input data
  if (size > 0) {
    jpegBuf = tjAlloc(size);
    if (!jpegBuf) {
      tjDestroy(handle);
      return 0;
    }
    memcpy(jpegBuf, data, size);
  } else {
    tjDestroy(handle);
    return 0;
  }

  // Get header information using legacy API
  int width = 0, height = 0, jpegSubsamp = 0;
  int headerResult = tjDecompressHeader2(handle, jpegBuf, jpegSize, 
                                        &width, &height, &jpegSubsamp);
  
  if (headerResult != 0 || width <= 0 || height <= 0) {
    tjFree(jpegBuf);
    tjDestroy(handle);
    return 0;
  }

  // Try to get ICC profile using new API (need new handle)
  tjhandle new_handle = tj3InitVersion(1, 0); // TJINIT_DECOMPRESS
  if (new_handle) {
    unsigned char *iccBuf = nullptr;
    size_t iccSize = 0;
    // We need to decompress header with new handle first
    if (tj3DecompressHeader(new_handle, jpegBuf, jpegSize) == 0) {
      tj3GetICCProfile(new_handle, &iccBuf, &iccSize);
      // iccBuf is allocated by library, we don't need to free it
    }
    tj3Destroy(new_handle);
  }

  // Calculate buffer sizes
  unsigned long bufSize = TJBUFSIZE(width, height);
  if (bufSize == 0) {
    tjFree(jpegBuf);
    tjDestroy(handle);
    return 0;
  }

  // Allocate destination buffers
  unsigned char *dstBuf = (unsigned char *)tjAlloc(bufSize);
  unsigned char *dstBuf2 = (unsigned char *)tjAlloc(bufSize);
  if (!dstBuf || !dstBuf2) {
    if (dstBuf) tjFree(dstBuf);
    if (dstBuf2) tjFree(dstBuf2);
    tjFree(jpegBuf);
    tjDestroy(handle);
    return 0;
  }

  // Decompress using legacy tjDecompress (using pixelSize=3 for RGB)
  int pitch = width * 3;
  int decompResult = tjDecompress(handle, jpegBuf, jpegSize, dstBuf,
                                 width, pitch, height, 3, 0);
  
  // Also try with tjDecompress2 using TJPF_RGB
  int decomp2Result = tjDecompress2(handle, jpegBuf, jpegSize, dstBuf2,
                                   width, pitch, height, TJPF_RGB, 0);

  // Try 12-bit decompression with new API
  if (new_handle) {
    // Recreate new handle for 12-bit decompression
    tjhandle handle12 = tj3InitVersion(1, 0);
    if (handle12) {
      // We need to allocate 12-bit buffer (short type)
      short *dstBuf12 = (short *)tj3Alloc(width * height * 3 * sizeof(short));
      if (dstBuf12) {
        if (tj3DecompressHeader(handle12, jpegBuf, jpegSize) == 0) {
          tj3Decompress12(handle12, jpegBuf, jpegSize, dstBuf12, 
                         width * 3 * sizeof(short), TJPF_RGB);
        }
        tj3Free(dstBuf12);
      }
      tj3Destroy(handle12);
    }
  }

  // Try to encode to YUV planes using new API
  if (new_handle) {
    tjhandle handleYUV = tj3InitVersion(1, 0);
    if (handleYUV) {
      // Allocate source buffer (use our decompressed data)
      unsigned char *srcBuf = (unsigned char *)tj3Alloc(width * height * 3);
      if (srcBuf) {
        memcpy(srcBuf, dstBuf, width * height * 3);
        
        // Allocate destination planes
        unsigned char *dstPlanes[3];
        int strides[3];
        
        // Initialize strides
        strides[0] = width;
        strides[1] = width / 2;
        strides[2] = width / 2;
        
        for (int i = 0; i < 3; i++) {
          dstPlanes[i] = (unsigned char *)tj3Alloc(width * height);
        }
        
        // Try to encode
        tj3EncodeYUVPlanes8(handleYUV, srcBuf, width, width * 3, height,
                           TJPF_RGB, dstPlanes, strides);
        
        // Try to decode YUV back using legacy API
        if (dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
          unsigned char *yuvBuf = (unsigned char *)tjAlloc(TJBUFSIZEYUV(width, height, jpegSubsamp));
          if (yuvBuf) {
            // Combine planes into interleaved YUV buffer (simplified)
            // This is a simplified approach - in real code you'd need proper packing
            int align = 4;
            tjDecodeYUV(handle, dstPlanes[0], align, jpegSubsamp, yuvBuf,
                       width, pitch, height, TJPF_RGB, 0);
            tjFree(yuvBuf);
          }
        }
        
        // Free YUV planes
        for (int i = 0; i < 3; i++) {
          if (dstPlanes[i]) tj3Free(dstPlanes[i]);
        }
        
        tj3Free(srcBuf);
      }
      tj3Destroy(handleYUV);
    }
  }

  // Get error string if any
  char *errorStr = tjGetErrorStr2(handle);
  if (errorStr) {
    // Error string is static, don't free it
  }

  // Cleanup
  tjFree(dstBuf);
  tjFree(dstBuf2);
  tjFree(jpegBuf);
  tjDestroy(handle);
  
  return 0;
}