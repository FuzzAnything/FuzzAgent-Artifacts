// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ares.h"

static void dummy_socket_callback(void *user_data, ares_socket_t sock,
                                  int read, int write) {
  /* Dummy callback */
  (void)user_data;
  (void)sock;
  (void)read;
  (void)write;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
  ares_channel_t *channel = NULL;
  
  // Call ares_destroy first (can be NULL-safe)
  ares_destroy(channel);
  
  // Now handle ares_expand_name
  if (Size > 0) {
    const unsigned char *encoded = Data;
    const unsigned char *abuf = Data;
    int alen = (int)(Size > INT_MAX ? INT_MAX : Size);
    char *s = NULL;
    long enclen = 0;
    
    // Try different offsets for encoded pointer
    size_t offset = 0;
    if (Size > 1) {
      offset = Data[0] % (Size - 1);
    }
    encoded = Data + offset;
    
    ares_expand_name(encoded, abuf, alen, &s, &enclen);
    if (s) {
      ares_free_string(s);
    }
  }
  
  // Now handle ares_expand_string
  if (Size > 0) {
    const unsigned char *encoded = Data;
    const unsigned char *abuf = Data;
    int alen = (int)(Size > INT_MAX ? INT_MAX : Size);
    unsigned char *s = NULL;
    long enclen = 0;
    
    // Try different offsets for encoded pointer
    size_t offset = 0;
    if (Size > 1) {
      offset = Data[Size - 1] % (Size - 1);
    }
    encoded = Data + offset;
    
    ares_expand_string(encoded, abuf, alen, &s, &enclen);
    if (s) {
      ares_free_string((char *)s);
    }
  }
  
  // Now handle ares_fds
  // Create a channel if possible
  struct ares_options options;
  int optmask = 0;
  
  // Initialize channel with minimal options
  int status = ares_init(&channel);
  if (status == ARES_SUCCESS && channel) {
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    ares_fds(channel, &read_fds, &write_fds);
    
    // Clean up
    ares_destroy(channel);
  }
  
  return 0;
}