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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"vp8_init_ctx":[[78,7,78,22,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000003_put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    // This callback can be empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // May be unused
    (void)out_fd; // May be unused
    
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_image_t *image = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    vpx_codec_err_t res;
    
    // 1. Use vpx_codec_peek_stream_info to get stream information
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 interfaces
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    bool stream_info_success = false;
    for (auto iface : decoder_ifaces) {
        res = vpx_codec_peek_stream_info(iface, data, (unsigned int)size, &stream_info);
        if (res == VPX_CODEC_OK) {
            stream_info_success = true;
            break;
        }
    }
    
    if (!stream_info_success) {
        // If we can't peek stream info, we can't proceed meaningfully
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Use vpx_codec_get_caps to check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(vpx_codec_vp8_dx());
    (void)caps; // May be unused in fuzzing context
    
    // 3. Initialize decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    if (vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0) != VPX_CODEC_OK) {
        // Try VP9 if VP8 fails
        decoder_iface = vpx_codec_vp9_dx();
        if (vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0) != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }
    
    // 4. Use vpx_codec_register_put_slice_cb to register callback
    // Note: This is deprecated in newer libvpx but we use it as requested
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000003_put_slice_callback, nullptr);
    // We continue even if this fails since it's deprecated
    
    // 5. Decode the input data
    res = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_1);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Use vpx_img_alloc to allocate an image
    // Allocate with dimensions from stream info or default values
    unsigned int width = stream_info.w > 0 ? stream_info.w : 640;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 480;
    
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, fuzz_uint32_t_2);
    if (!image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Initialize encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(&encoder, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 8. Encode the allocated image
    // Fill image with some data for encoding
    // For fuzzing, we use minimal data
    if (image->fmt == VPX_IMG_FMT_I420) {
        // Fill Y plane
        for (unsigned int i = 0; i < height && i < image->h; ++i) {
            for (unsigned int j = 0; j < width && j < image->w; ++j) {
                image->planes[0][i * image->stride[0] + j] = (i + j) % 256;
            }
        }
        
        // Fill U and V planes (subsampled)
        unsigned int uv_height = height / 2;
        unsigned int uv_width = width / 2;
        for (unsigned int i = 0; i < uv_height && i < image->h/2; ++i) {
            for (unsigned int j = 0; j < uv_width && j < image->w/2; ++j) {
                image->planes[1][i * image->stride[1] + j] = (i * 2 + j * 2) % 256;
                image->planes[2][i * image->stride[2] + j] = (i * 2 + j * 2 + 128) % 256;
            }
        }
    }
    
    // Encode the image
    res = vpx_codec_encode(&encoder, image, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // 9. Use vpx_codec_get_cx_data to get encoded packets
    if (res == VPX_CODEC_OK) {
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Clean up
    vpx_img_free(image);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}