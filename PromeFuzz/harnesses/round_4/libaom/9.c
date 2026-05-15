// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
  // Test aom_obu_type_to_string with various enum values
  if (Size > 0) {
    // Use first byte to generate OBU_TYPE value
    OBU_TYPE obu_type = (OBU_TYPE)(Data[0]);
    const char *obu_str = aom_obu_type_to_string(obu_type);
    (void)obu_str; // Use result to avoid unused variable warning
  }

  // Test aom_codec_version
  int version = aom_codec_version();
  (void)version; // Use result

  // Test aom_codec_version_str
  const char *version_str = aom_codec_version_str();
  (void)version_str; // Use result

  // Test aom_codec_version_extra_str
  const char *extra_str = aom_codec_version_extra_str();
  (void)extra_str; // Use result

  // Test aom_codec_build_config
  const char *build_config = aom_codec_build_config();
  (void)build_config; // Use result

  // Test aom_codec_error_detail
  // Create a dummy codec context structure
  static aom_codec_ctx_t dummy_ctx;
  memset(&dummy_ctx, 0, sizeof(dummy_ctx));
  
  // Initialize some fields with fuzzer data
  if (Size > 0) {
    // Use first few bytes to set error code if available
    if (Size >= sizeof(dummy_ctx.err)) {
      memcpy(&dummy_ctx.err, Data, sizeof(dummy_ctx.err));
    }
    
    // Set error detail pointer if we have enough data
    if (Size >= sizeof(dummy_ctx.err) + sizeof(char *)) {
      // Note: We're just testing the function call, not actually using
      // the pointer value for anything
      dummy_ctx.err_detail = (const char *)(Data + sizeof(dummy_ctx.err));
    }
  }
  
  const char *error_detail = aom_codec_error_detail(&dummy_ctx);
  (void)error_detail; // Use result

  // Test with different OBU_TYPE values using more fuzzer data
  if (Size > 1) {
    // Use second byte for another OBU_TYPE test
    OBU_TYPE obu_type2 = (OBU_TYPE)(Data[1]);
    const char *obu_str2 = aom_obu_type_to_string(obu_type2);
    (void)obu_str2; // Use result
  }

  // Test aom_codec_error_detail with NULL context
  if (Size > 2 && (Data[2] & 0x01)) {
    const char *null_error = aom_codec_error_detail(NULL);
    (void)null_error; // Use result
  }

  return 0;
}