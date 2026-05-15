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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"img_alloc_helper":[[75,5,75,26,0,0,4,0],[91,5,91,26,0,0,4,0],[104,5,104,12,0,0,4,0]],"set_encoder_config":[[662,32,662,54,0,0,4,1]],"is_valid_img_fmt":[[24,5,24,26,0,0,4,0]],"vp9_set_row_mt":[[6863,55,6863,71,0,0,4,0],[6864,7,6864,20,0,0,4,0],[6869,61,6869,77,0,0,4,0],[6873,7,6873,18,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback function for vpx_codec_register_put_slice_cb
static void  id_000069_put_slice_callback(void *user_priv, const vpx_image_t *img,
                              const vpx_image_rect_t *slice_rect,
                              const vpx_image_rect_t *render_rect) {
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)render_rect;
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse first few bytes to determine if this is VP8 or VP9
    uint8_t header[16];
    size_t read_bytes = fread(header, 1, sizeof(header), in_file);
    fseek(in_file, 0, SEEK_SET);

    if (read_bytes < 10) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;

    // Try to determine codec type from header
    // VP8: First 3 bytes are 0x9D 0x01 0x2A
    // VP9: First 2 bytes have specific patterns
    if (header[0] == 0x9D && header[1] == 0x01 && header[2] == 0x2A) {
        // Likely VP8
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else if ((header[0] & 0xE0) == 0xC0) {
        // Likely VP9
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        // Try both
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    }

    // Check decoder capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Use the result

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = vpx_codec_dec_cfg_t();
    dec_cfg.threads = 1;

    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Register put slice callback
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000069_put_slice_callback, nullptr);
    (void)res; // Result may fail if not supported

    // Read the rest of the data
    uint8_t *compressed_data = (uint8_t *)malloc(size);
    if (!compressed_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    size_t data_sz = fread(compressed_data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Decode the input data
    res = vpx_codec_decode(&decoder, compressed_data, data_sz, nullptr, 0);

    // Get stream info
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t info_res = vpx_codec_get_stream_info(&decoder, &si);
    (void)info_res; // May fail if decoder hasn't decoded a frame yet

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(compressed_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set some configuration based on input data
    // Use some bytes from input to determine encoder settings
    unsigned int width = 320 + (data[0] % 320);
    unsigned int height = 240 + (data[1] % 240);
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = 1 + (data[2] % 4);
    enc_cfg.rc_target_bitrate = 100 + (data[3] % 900);
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;

    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        free(compressed_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration again (test vpx_codec_enc_config_set)
    res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    (void)res; // Result may be ignored

    // Set some encoder controls based on input data
    // Use different control IDs based on codec type
    if (encoder_iface == vpx_codec_vp8_cx()) {
        // VP8 controls
        int ctrl_id = VP8E_SET_CPUUSED;
        int value = 8 + (data[4] % 8);
        vpx_codec_control_(&encoder, ctrl_id, value);
        
        ctrl_id = VP8E_SET_NOISE_SENSITIVITY;
        value = data[5] % 4;
        vpx_codec_control_(&encoder, ctrl_id, value);
    } else {
        // VP9 controls
        int ctrl_id = VP9E_SET_TILE_COLUMNS;
        int value = data[4] % 4;
        vpx_codec_control_(&encoder, ctrl_id, value);
        
        ctrl_id = VP9E_SET_ROW_MT;
        value = data[5] % 2;
        vpx_codec_control_(&encoder, ctrl_id, value);
    }

    // Create a dummy image for encoding
    vpx_image_t img;
    unsigned int align = 16;
    
    // Use input bytes to determine image format
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (data[6] % 2 == 0) {
        fmt = VPX_IMG_FMT_I422;
    }
    
    if (!vpx_img_alloc(&img, fmt, width, height, align)) {
        free(compressed_data);
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Fill image with some data from input
    size_t img_data_size = width * height * 3 / 2;
    size_t copy_size = img_data_size < (size - 16) ? img_data_size : (size - 16);
    
    if (copy_size > 0) {
        memcpy(img.planes[0], data + 16, copy_size);
    }

    // Encode the image
    vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    free(compressed_data);

    assert_file_closed(&in_file);
	return 0;
}