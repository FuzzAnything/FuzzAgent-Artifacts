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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 27
//<Quality> {"density":15,"unique_branches":{"encode_frame_to_data_rate":[[3983,9,3983,40,0,0,4,0],[4036,9,4036,41,0,0,4,0],[4037,9,4037,57,0,0,4,1]],"vp8_change_config":[[1582,7,1582,51,0,0,4,0]],"vp8_alloc_compressor_data":[[1221,7,1221,18,0,0,4,1],[1223,14,1223,27,0,0,4,0],[1223,14,1223,27,0,0,4,1],[1225,14,1225,27,0,0,4,0]],"validate_config":[[125,12,125,29,1,0,4,0],[125,12,125,29,2,0,4,0]],"vp8_compute_skin_block":[[38,9,38,28,0,0,4,0]],"tokenize2nd_order_b":[[120,7,120,11,0,0,4,0]],"vp8cx_pick_filter_level":[[310,17,310,32,0,0,4,1]],"vp8_regulate_q":[[1233,14,1233,46,0,0,4,1],[1239,9,1239,18,0,0,4,0],[1245,11,1245,46,0,0,4,0],[1279,14,1279,50,0,0,4,1]],"vp8_initialize_rd_consts":[[211,7,211,25,0,0,4,0],[215,17,215,30,0,0,4,0],[215,17,215,30,0,0,4,1],[216,11,216,43,0,0,4,0],[216,11,216,43,0,0,4,1]],"compute_skin_map":[[617,7,617,44,0,0,4,1]],"get_min_filter_level":[[112,16,112,33,0,0,4,1]],"vp8_compute_frame_size_bounds":[[1458,9,1458,30,0,0,4,0]],"calc_iframe_target_size":[[319,9,319,52,0,0,4,1]],"write_kfmodes":[[577,9,577,29,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_free","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000004_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *update) {
    // This callback can be empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Variables for cleanup tracking
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t *allocated_image = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;

    // 1. Use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 codecs
    vpx_codec_err_t err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, 
                                                     (unsigned int)size, &stream_info);
    if (err != VPX_CODEC_OK) {
        err = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, 
                                         (unsigned int)size, &stream_info);
        if (err != VPX_CODEC_OK) {
            // Input is not valid VP8/VP9 data
            return 0;
        }
    }

    // 2. Create input file from data using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use vpx_codec_get_caps to check capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(vpx_codec_vp8_cx());
    (void)caps; // Use caps to avoid unused variable warning
    
    caps = vpx_codec_get_caps(vpx_codec_vp9_cx());
    caps = vpx_codec_get_caps(vpx_codec_vp8_dx());
    caps = vpx_codec_get_caps(vpx_codec_vp9_dx());

    // 5. Initialize decoder (try VP9 first, then VP8)
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // 6. Register put_slice callback
    err = vpx_codec_register_put_slice_cb(&decoder,  id_000004_put_slice_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        // Continue anyway, callback registration might fail for some codecs
    }

    // 7. Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_3);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Get decoded frames
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 9. Allocate an image using vpx_img_alloc for re-encoding
        // Use the decoded image's dimensions and format
        allocated_image = vpx_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, fuzz_uint32_t_4);
        if (allocated_image) {
            // Copy image data (simplified - in reality would need to copy planes)
            // For fuzzing, we just create the allocated image
            vpx_img_free(allocated_image);
            allocated_image = nullptr;
        }
    }

    // 10. Initialize encoder for re-encoding
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_5);
    
    if (err == VPX_CODEC_OK) {
        // Set basic configuration
        if (stream_info.w > 0 && stream_info.h > 0) {
            cfg.g_w = stream_info.w;
            cfg.g_h = stream_info.h;
        } else {
            cfg.g_w = 640;
            cfg.g_h = 480;
        }
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256;

        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_6, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // 11. Create a dummy image for encoding
            vpx_image_t dummy_img;
            allocated_image = vpx_img_alloc(&dummy_img, VPX_IMG_FMT_I420, 
                                           cfg.g_w, cfg.g_h, fuzz_uint32_t_7);
            
            if (allocated_image) {
                // Fill with some data (simplified for fuzzing)
                // In real usage, you would fill the image planes with actual data
                
                // 12. Encode the image
                err = vpx_codec_encode(&encoder, allocated_image, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                
                if (err == VPX_CODEC_OK) {
                    // 13. Use vpx_codec_get_cx_data to get encoded packets
                    iter = nullptr;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                }
                
                // Free the allocated image
                vpx_img_free(allocated_image);
            }
        }
    }

    // 14. Cleanup
    if (encoder.priv) {
        vpx_codec_destroy(&encoder);
    }
    if (decoder.priv) {
        vpx_codec_destroy(&decoder);
    }
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}