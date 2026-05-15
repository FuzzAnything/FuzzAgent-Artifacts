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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_control","aom_codec_enc_config_set","aom_img_wrap","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_enc_config_set","aom_img_wrap","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 64) {
    // Need minimum data for header parsing
    return 0;
  }

  // Use fmemopen to create FILE* from input data
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

  // Get build configuration (API requirement)
  const char *build_config = aom_codec_build_config();
  (void)build_config; // Prevent unused variable warning

  // Decode the input bitstream
  aom_codec_ctx_t decoder;
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  
  aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
  dec_cfg.threads = 1;
  
  aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                               0, AOM_DECODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Read and decode the bitstream
  std::vector<uint8_t> buffer(size);
  fread(buffer.data(), 1, size, in_file);
  
  res = aom_codec_decode(&decoder, buffer.data(), size, nullptr);
  
  // Get error details if decode failed (API requirement)
  if (res != AOM_CODEC_OK) {
    const char *error_detail = aom_codec_error_detail(&decoder);
    (void)error_detail; // Prevent unused variable warning
  }

  // Get decoded frame
  aom_codec_iter_t iter = nullptr;
  aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
  
  if (decoded_img) {
    // Create encoder configuration
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
      aom_codec_destroy(&decoder);
      assert_file_closed(&in_file);;
      assert_file_closed(&out_file);;
      assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on decoded image (API requirement)
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_profile = 0;
    
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
      aom_codec_destroy(&decoder);
      assert_file_closed(&in_file);;
      assert_file_closed(&out_file);;
      assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use control API (API requirement)
    res = aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
    (void)res; // Prevent unused variable warning
    
    // Set encoder configuration (API requirement)
    res = aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Create wrapped image from decoded data (API requirement)
    // We need to allocate buffer for image data
    size_t image_size = decoded_img->d_w * decoded_img->d_h * 3 / 2; // For I420
    unsigned char *img_data = (unsigned char *)malloc(image_size);
    if (img_data) {
      // Copy Y plane
      size_t y_size = decoded_img->d_w * decoded_img->d_h;
      memcpy(img_data, decoded_img->planes[0], y_size);
      
      // Copy U plane
      size_t uv_size = (decoded_img->d_w * decoded_img->d_h) / 4;
      memcpy(img_data + y_size, decoded_img->planes[1], uv_size);
      
      // Copy V plane
      memcpy(img_data + y_size + uv_size, decoded_img->planes[2], uv_size);
      
      // Wrap the image data (API requirement)
      aom_image_t wrapped_img;
      aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
      unsigned int stride_align = 1;
      aom_image_t *wrapped = aom_img_wrap(&wrapped_img, fmt, 
                                          decoded_img->d_w, decoded_img->d_h,
                                          stride_align, img_data);
      
      if (wrapped) {
        // Get plane height (API requirement)
        int plane_height = aom_img_plane_height(wrapped, 0);
        (void)plane_height; // Prevent unused variable warning
        
        // Encode the wrapped image
        aom_codec_encode(&encoder, wrapped, 0, 1, 0);
        
        // Get compressed data
        iter = nullptr;
        const aom_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
          if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
          }
        }
      }
      
      free(img_data);
    }
    
    aom_codec_destroy(&encoder);
  }
  
  // Clean up
  aom_codec_destroy(&decoder);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}