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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"vpx_codec_peek_stream_info":[[71,38,71,41,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_img_free","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*Looking at the APIs provided, I can see they're focused on encoding, decoding, and image manipulation in libvpx. Based on the instruction to use a byte stream of libvpx's output data as input, I'll create a fuzzer that:

1. First decodes the input byte stream (VP8/VP9 encoded data)
2. Gets the decoded frames and processes them
3. Re-encodes the frames back to video
4. Uses all the specified APIs

Here's the implementation:

*/


extern "C" {
}

// Global callback counters for tracking API usage
static int  id_000079_put_frame_callback_count = 0;
static int  id_000079_put_slice_callback_count = 0;

// Callback function for put_frame
static void  id_000079_put_frame_callback(void* user_priv, const vpx_image_t* img) {
     id_000079_put_frame_callback_count++;
}

// Callback function for put_slice
static void  id_000079_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect,
                               const vpx_image_rect_t* rect2) {
     id_000079_put_slice_callback_count++;
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Reset callback counters
     id_000079_put_frame_callback_count = 0;
     id_000079_put_slice_callback_count = 0;

    // Create a FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Determine which codec to use based on input data
    vpx_codec_iface_t* decoder_iface = nullptr;
    vpx_codec_iface_t* encoder_iface = nullptr;
    
    // Try VP9 first, then VP8
    if (vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, 
                                   (unsigned int)size, nullptr) == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else if (vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, 
                                         (unsigned int)size, nullptr) == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get interface name (API 7)
    const char* iface_name = vpx_codec_iface_name(encoder_iface);
    if (!iface_name) {
        iface_name = "unknown";
    }

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get stream info (API 5)
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    // First use peek_stream_info (indirectly through our earlier check)
    // Then get_stream_info from decoder context
    vpx_codec_get_stream_info(&decoder, &stream_info);

    // Decode the input
    if (vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register callbacks (APIs 3 and 9)
    vpx_codec_register_put_frame_cb(&decoder,  id_000079_put_frame_callback, nullptr);
    vpx_codec_register_put_slice_cb(&decoder,  id_000079_put_slice_callback, nullptr);

    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img = nullptr;
    
    // We'll process up to 5 frames to keep fuzzing fast
    int frame_count = 0;
    const int max_frames = 5;
    
    // Allocate image for re-encoding (API 4)
    vpx_image_t* img_to_encode = nullptr;
    
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr && 
           frame_count < max_frames) {
        
        frame_count++;
        
        // If this is the first frame, allocate image for encoding
        if (frame_count == 1) {
            img_to_encode = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                                          decoded_img->d_w, decoded_img->d_h, 16);
            
            if (!img_to_encode) {
                vpx_codec_destroy(&decoder);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
            }
            
            // Copy the decoded image to our allocated image
            for (int plane = 0; plane < 3; plane++) {
                uint8_t* src_plane = decoded_img->planes[plane];
                uint8_t* dst_plane = img_to_encode->planes[plane];
                int src_stride = decoded_img->stride[plane];
                int dst_stride = img_to_encode->stride[plane];
                int height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
                
                for (int y = 0; y < height; y++) {
                    memcpy(dst_plane, src_plane, 
                           (plane == 0) ? decoded_img->d_w : decoded_img->d_w / 2);
                    src_plane += src_stride;
                    dst_plane += dst_stride;
                }
            }
        }
    }

    // Destroy decoder
    vpx_codec_destroy(&decoder);

    // If we got a frame, re-encode it
    if (img_to_encode) {
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration (API 5 related - though not in the list)
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
            vpx_img_free(img_to_encode);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder configuration based on decoded frame
        enc_cfg.g_w = img_to_encode->d_w;
        enc_cfg.g_h = img_to_encode->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000; // 1 Mbps
        enc_cfg.g_error_resilient = 0;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                   VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
            vpx_img_free(img_to_encode);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Set encoder configuration (API 6)
        if (vpx_codec_enc_config_set(&encoder, &enc_cfg) != VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder);
            vpx_img_free(img_to_encode);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Encode the frame (API 1)
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        
        if (vpx_codec_encode(&encoder, img_to_encode, pts, duration, 
                            flags, VPX_DL_REALTIME) != VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder);
            vpx_img_free(img_to_encode);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Get preview frame (API 8)
        const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&encoder);
        if (preview_frame) {
            // Preview frame available - we could process it if needed
        }

        // Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write encoded data to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                break; // Just get first packet for fuzzing
            }
        }

        // Destroy encoder
        vpx_codec_destroy(&encoder);
        
        // Free the allocated image
        vpx_img_free(img_to_encode);
    }

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file to avoid accumulating data
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}