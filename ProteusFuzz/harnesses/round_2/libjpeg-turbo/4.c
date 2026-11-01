// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int get_safe_scaling_factor(const uint8_t *Data, size_t Size, int *numFactors) {
    int *factors = tj3GetScalingFactors(numFactors);
    if (!factors || *numFactors <= 0) {
        return 0;
    }
    
    // Use data to select a valid scaling factor index
    int index = (Size > 0) ? (Data[0] % *numFactors) : 0;
    tjscalingfactor sf = { factors[index], factors[*numFactors - 1 - index] };
    
    // Ensure valid fraction (numerator <= denominator)
    if (sf.num > sf.denom) {
        sf.num = sf.denom;
    }
    if (sf.denom == 0) {
        sf.denom = 1;
    }
    
    // Store the factor in a static buffer for the caller to use
    static tjscalingfactor static_sf;
    static_sf = sf;
    
    return 1;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *yuvBuf = NULL;
    unsigned char *rgbBuf = NULL;
    int width, height, subsamp, pixelFormat, align;
    size_t yuvSize, jpegSize;
    int numFactors = 0;

    // 1. Prepare the environment
    // Initialize handle for decompression
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Determine dimensions from input data, clamping to reasonable limits
    // Using first 4 bytes for width, next 4 for height
    int rawWidth = (Size > 4) ? ((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) : 0;
    int rawHeight = (Size > 8) ? ((Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]) : 0;

    // Clamp dimensions to avoid huge allocations or invalid states
    if (rawWidth <= 0 || rawWidth > 4096) rawWidth = 64;
    if (rawHeight <= 0 || rawHeight > 4096) rawHeight = 64;
    width = rawWidth;
    height = rawHeight;

    // Determine subsampling (TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY)
    subsamp = (Size > 12) ? (Data[12] % 4) : 0;
    // Ensure subsamp is valid
    if (subsamp < 0 || subsamp > 3) subsamp = 0;

    // Determine pixel format (TJPF_RGB, TJPF_BGR, etc.)
    pixelFormat = (Size > 13) ? (Data[13] % 6) : 0;
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = 0;

    // Determine alignment (must be power of 2)
    align = 1 << ((Size > 14) ? (Data[14] % 5) : 0); // 1, 2, 4, 8, 16

    // 2. Invoke tj3SetScalingFactor
    tjscalingfactor sf;
    if (get_safe_scaling_factor(Data, Size, &numFactors)) {
        tj3SetScalingFactor(handle, sf);
    } else {
        // Fallback to unscaled if factors unavailable
        sf.num = 1; sf.denom = 1;
        tj3SetScalingFactor(handle, sf);
    }

    // 3. Invoke tj3Alloc (first time) for JPEG buffer
    // Estimate JPEG size roughly (could be larger, but we just need a buffer)
    jpegSize = (size_t)width * height * 2; 
    if (Size > 0 && Size < jpegSize) jpegSize = Size;
    
    // Create a dummy JPEG file if needed, or use the fuzz data directly
    // For this fuzz driver, we will use the fuzz data as the JPEG source
    // but ensure we have a valid pointer.
    jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Copy fuzz data to jpegBuf
    size_t copyLen = (Size < jpegSize) ? Size : jpegSize;
    memcpy(jpegBuf, Data, copyLen);

    // 4. Invoke tj3Alloc (second time) for YUV buffer
    // Calculate size
    yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // 5. Invoke tj3DecompressToYUV8
    // This decompresses the JPEG (or garbage data) into YUV
    // We expect this to potentially fail if Data is not valid JPEG, which is fine for fuzzing
    tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, align);

    // 6. Invoke tj3DecodeYUV8
    // Allocate RGB buffer
    int pixelSize = tjPixelSize[pixelFormat];
    size_t rgbSize = (size_t)width * height * pixelSize;
    rgbBuf = (unsigned char *)tj3Alloc(rgbSize);
    if (!rgbBuf) {
        tj3Free(jpegBuf);
        tj3Free(yuvBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Decode YUV to RGB
    // pitch = 0 means width * pixelSize
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, 0, height, pixelFormat);

    // 7. Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    tj3Free(rgbBuf);
    tj3Destroy(handle);

    return 0;
}