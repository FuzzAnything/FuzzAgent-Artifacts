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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_uleb_size_in_bytes","aom_img_add_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_alloc_with_border","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_uleb_size_in_bytes","aom_img_add_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_alloc_with_border","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
	return 0;
	}
	  // Need minimal data for meaningful operations

  // 1. Get version string (API usage)
  const char *version_str = aom_codec_version_str();
  (void)version_str;  // Use to avoid compiler warnings

  // 2. Create a decoder for processing input stream
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t cfg = {0};
  cfg.threads = 1;
  
  if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &cfg, 0, AOM_DECODER_ABI_VERSION)) {
    return 0;  // Decoder initialization failed
  }

  // 3. Get interface name and capabilities (API usage)
  const char *iface_name = aom_codec_iface_name(aom_codec_av1_dx());
  aom_codec_caps_t caps = aom_codec_get_caps(aom_codec_av1_dx());
  (void)iface_name;
  (void)caps;

  // 4. Decode the input data
  aom_codec_decode(&decoder, data, size, nullptr);
  
  // 5. Try to get decoded frame
  aom_codec_iter_t iter = nullptr;
  aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
  
  // 6. If we got an image, add metadata to it
  if (img) {
    // Calculate ULEB size for a value derived from input (API usage)
    uint64_t uleb_value = size;
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
    
    // Prepare metadata from part of input
    size_t metadata_size = size > 50 ? 50 : size/2;
    uint32_t metadata_type = 1;  // Arbitrary type
    
    // Add metadata to image (API usage)
    aom_img_add_metadata(img, metadata_type, data, metadata_size, AOM_MIF_ANY_FRAME);
    
    // Create and free standalone metadata object (API usage)
    aom_metadata_t *standalone_metadata = aom_img_metadata_alloc(
        metadata_type, data + 10, metadata_size > 10 ? metadata_size - 10 : 1, 
        AOM_MIF_KEY_FRAME);
    
    if (standalone_metadata) {
      aom_img_metadata_free(standalone_metadata);  // API usage
    }
  }

  // 7. Create an output image for encoding test
  aom_image_t output_img;
  unsigned int width = 64;
  unsigned int height = 64;
  
  // Allocate image with border
  aom_image_t *wrapped_img = aom_img_alloc_with_border(
      &output_img, AOM_IMG_FMT_I420, width, height, 1, 1, 0);
  
  if (wrapped_img) {
    // Fill image with some data from input
    size_t copy_size = wrapped_img->sz < size ? wrapped_img->sz : size;
    memcpy(wrapped_img->img_data, data, copy_size);
    
    // Test ULEB encoding with input-derived value
    uint8_t coded_buffer[10];
    size_t coded_size = 0;
    uint64_t encode_value = data[0] + (data[1] << 8);
    aom_uleb_encode(encode_value, sizeof(coded_buffer), coded_buffer, &coded_size);
    
    // Test fixed-size ULEB encoding
    size_t fixed_coded_size = 0;
    aom_uleb_encode_fixed_size(encode_value, sizeof(coded_buffer), 4, 
                               coded_buffer, &fixed_coded_size);
    
    // Test ULEB decoding
    uint64_t decoded_value = 0;
    size_t decoded_length = 0;
    aom_uleb_decode(data, size > 10 ? 10 : size, &decoded_value, &decoded_length);
    
    aom_img_free(wrapped_img);
  }

  // 8. Cleanup
  aom_codec_destroy(&decoder);
  
  return 0;
}