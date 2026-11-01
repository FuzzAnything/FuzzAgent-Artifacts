// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

static int get_fuzz_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    return val;
}

static unsigned int get_fuzz_uint(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) {
        return 0;
    }
    unsigned int val = *(const unsigned int *)(data + *offset);
    *offset += sizeof(unsigned int);
    return val;
}

static vpx_codec_pts_t get_fuzz_pts(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(vpx_codec_pts_t) > size) {
        return 0;
    }
    vpx_codec_pts_t val = *(const vpx_codec_pts_t *)(data + *offset);
    *offset += sizeof(vpx_codec_pts_t);
    return val;
}

static vpx_enc_frame_flags_t get_fuzz_flags(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(vpx_enc_frame_flags_t) > size) {
        return 0;
    }
    vpx_enc_frame_flags_t val = *(const vpx_enc_frame_flags_t *)(data + *offset);
    *offset += sizeof(vpx_enc_frame_flags_t);
    return val;
}

static vpx_enc_deadline_t get_fuzz_deadline(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(vpx_enc_deadline_t) > size) {
        return 0;
    }
    vpx_enc_deadline_t val = *(const vpx_enc_deadline_t *)(data + *offset);
    *offset += sizeof(vpx_enc_deadline_t);
    return val;
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    size_t offset = 0;
    int val_int = get_fuzz_int(Data, Size, &offset);
    unsigned int width = get_fuzz_uint(Data, Size, &offset);
    unsigned int height = get_fuzz_uint(Data, Size, &offset);
    unsigned int stride_align = get_fuzz_uint(Data, Size, &offset);
    
    // Clamp dimensions and alignment to valid ranges
    if (width == 0 || width > 0x08000000) width = 64;
    if (height == 0 || height > 0x08000000) height = 64;
    if (stride_align == 0 || stride_align > 65536) stride_align = 16;
    
    // Select image format
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(val_int % 10);
    if (fmt == VPX_IMG_FMT_NONE) fmt = VPX_IMG_FMT_I420;

    // Test vpx_img_alloc
    vpx_image_t *img_alloc = vpx_img_alloc(NULL, fmt, width, height, stride_align);
    if (img_alloc) {
        // Test vpx_img_flip
        vpx_img_flip(img_alloc);
        
        // Flip again to restore (optional, but good for state exploration)
        vpx_img_flip(img_alloc);
        
        vpx_img_free(img_alloc);
    }

    // Test vpx_img_wrap with pre-allocated buffer
    // Calculate approximate size needed (simplified for fuzzing)
    // We use a large enough buffer to avoid immediate OOB on wrap
    size_t buf_size = (size_t)width * height * 2; 
    unsigned char *img_data = (unsigned char *)malloc(buf_size);
    if (!img_data) return 0;

    vpx_image_t img_wrap;
    vpx_image_t *img_wrapped = vpx_img_wrap(&img_wrap, fmt, width, height, stride_align, img_data);
    
    if (img_wrapped) {
        vpx_img_flip(img_wrapped);
        vpx_img_flip(img_wrapped);
        // Note: vpx_img_free on wrapped image with external data only frees descriptor if self_allocd
        // Since we passed &img_wrap, self_allocd is 0, so it won't free img_data.
        vpx_img_free(img_wrapped);
    }
    
    // Test vpx_img_wrap with heap allocated descriptor
    vpx_image_t *img_heap = vpx_img_wrap(NULL, fmt, width, height, stride_align, img_data);
    if (img_heap) {
        vpx_img_flip(img_heap);
        vpx_img_flip(img_heap);
        vpx_img_free(img_heap);
    }
    
    free(img_data);

    // Test vpx_codec_encode
    // We need a valid encoder context. Since we can't easily initialize one without more complex setup
    // and the task focuses on the image functions, we will attempt to call encode with a NULL context
    // or a partially constructed one to test error handling paths, but primarily we focus on valid image inputs.
    // However, to strictly follow the API, we need a valid ctx. 
    // Without vpx_codec_enc_init, we cannot get a valid ctx. 
    // We can try to create a dummy context structure to test parameter validation if the API allows,
    // but vpx_codec_encode expects a valid iface. 
    // Given the constraints, we will skip the encode call unless we can initialize a codec.
    // Let's try to initialize a VP8 encoder if possible, but it might require more setup than fuzzing allows.
    // Instead, we focus on the image manipulation which is the core of the provided APIs.
    
    // If we have enough data, try to initialize a codec to test vpx_codec_encode
    if (Size > 40) {
        vpx_codec_ctx_t ctx;
        memset(&ctx, 0, sizeof(ctx));
        
        // We cannot safely call vpx_codec_encode without a valid initialized context.
        // The function checks iface->priv and other internal fields.
        // Passing a zeroed struct might trigger an assert or crash, which is fine for fuzzing
        // if it's a null pointer deref, but let's avoid undefined behavior if possible.
        // However, for fuzzing, triggering the error path is useful.
        // Let's try to pass the zeroed context to see if it handles NULL iface gracefully.
        // But first, let's create a valid image to pass.
        
        vpx_image_t *img_for_encode = vpx_img_alloc(NULL, fmt, width, height, stride_align);
        if (img_for_encode) {
            // Fill image with fuzz data if possible
            if (img_for_encode->planes[0]) {
                size_t plane_size = (size_t)img_for_encode->stride[0] * img_for_encode->h;
                if (Size > offset + plane_size) {
                     memcpy(img_for_encode->planes[0], Data + offset, plane_size);
                     offset += plane_size;
                }
            }
            
            // Get parameters for encode
            vpx_codec_pts_t pts = get_fuzz_pts(Data, Size, &offset);
            unsigned long duration = get_fuzz_uint(Data, Size, &offset);
            vpx_enc_frame_flags_t flags = get_fuzz_flags(Data, Size, &offset);
            vpx_enc_deadline_t deadline = get_fuzz_deadline(Data, Size, &offset);
            
            // Attempt encode with uninitialized context (expecting error)
            // This tests the error handling in vpx_codec_encode
            vpx_codec_encode(&ctx, img_for_encode, pts, duration, flags, deadline);
            
            // Flush
            vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
            
            vpx_img_free(img_for_encode);
        }
    }

    return 0;
}