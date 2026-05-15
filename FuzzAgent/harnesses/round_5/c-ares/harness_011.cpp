#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ares.h"

namespace harness_011 {
/* Comprehensive fuzzing harness for c-ares library targeting file-based host resolution
 * identified as critical coverage gap: ares_gethostbyname_file (0% coverage, 12 undiscovered branches).
 * 
 * Target local hosts file parsing functionality, representing distinct code path from
 * network DNS resolution missed by all 11 existing harnesses.
 * 
 * Focus on ares_gethostbyname_file and related file-based host resolution APIs.
 * Ensure semantic diversity from existing harnesses which cover network DNS resolution,
 * socket callbacks, event processing, etc.
 * 
 * This harness tests local hosts file parsing with various hostname inputs, edge cases,
 * and error conditions.
 */



/* Minimal callback for async operations if needed */
static void host_callback(void *arg, int status, int timeouts,
                         const struct hostent *hostent) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)hostent;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  /* Minimum size check - need enough data for operation selection and basic parameters */
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  /* Initialize c-ares library */
  ares_library_init(ARES_LIB_INIT_ALL);

  /* Create channel */
  ares_channel_t *channel = NULL;
  int init_result = ares_init(&channel);
  if (init_result != ARES_SUCCESS) {
    ares_library_cleanup();
    return 0;
  }

  /* Consume fixed-size data for operation parameters */
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
  int family = fdp.ConsumeIntegral<uint8_t>() % 3; /* 0=AF_INET, 1=AF_INET6, 2=AF_UNSPEC */
  uint8_t test_variant = fdp.ConsumeIntegral<uint8_t>() % 4;
  uint8_t use_async = fdp.ConsumeBool();
  uint8_t trigger_timeout = fdp.ConsumeBool();

  /* Map family values */
  int actual_family;
  switch (family) {
    case 0:
      actual_family = AF_INET;
      break;
    case 1:
      actual_family = AF_INET6;
      break;
    default:
      actual_family = AF_UNSPEC;
      break;
  }

  /* Consume variable-size data for hostnames */
  std::string hostname1 = fdp.ConsumeRandomLengthString(128);
  std::string hostname2 = fdp.ConsumeRandomLengthString(128);
  std::string hostname3 = fdp.ConsumeRandomLengthString(128);
  
  /* Get remaining bytes for additional test data */
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();

  /* Main operation switch - focus on file-based host resolution */
  switch (operation) {
    case 0: {
      /* Test 1: Direct ares_gethostbyname_file call with fuzzed hostname */
      struct hostent *host_result = NULL;
      
      /* Use different hostname based on test variant */
      const char *test_hostname = NULL;
      switch (test_variant % 3) {
        case 0:
          test_hostname = hostname1.c_str();
          break;
        case 1:
          test_hostname = hostname2.c_str();
          break;
        case 2:
          test_hostname = hostname3.c_str();
          break;
      }
      
      if (strlen(test_hostname) > 0) {
        int file_result = ares_gethostbyname_file(channel, test_hostname, 
                                                  actual_family, &host_result);
        (void)file_result;
        
        /* Clean up hostent if returned */
        if (host_result != NULL) {
          ares_free_hostent(host_result);
        }
      }
      break;
    }
    
    case 1: {
      /* Test 2: ares_gethostbyname (async) which may use hosts file internally */
      if (strlen(hostname1.c_str()) > 0) {
        ares_gethostbyname(channel, hostname1.c_str(), actual_family, 
                          host_callback, NULL);
        
        /* Process any pending events to trigger callback */
        fd_set read_fds, write_fds;
        struct timeval tv;
        
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        ares_fds(channel, &read_fds, &write_fds);
        ares_process(channel, &read_fds, &write_fds);
      }
      break;
    }
    
    case 2: {
      /* Test 3: Error path testing with NULL parameters */
      struct hostent *host_result = NULL;
      
      switch (test_variant % 3) {
        case 0:
          /* NULL channel */
          ares_gethostbyname_file(NULL, hostname1.c_str(), 
                                 actual_family, &host_result);
          break;
        case 1:
          /* NULL hostname */
          ares_gethostbyname_file(channel, NULL, 
                                 actual_family, &host_result);
          break;
        case 2:
          /* NULL host result pointer */
          ares_gethostbyname_file(channel, hostname1.c_str(), 
                                 actual_family, NULL);
          break;
      }
      
      /* Clean up if host_result was populated despite error */
      if (host_result != NULL) {
        ares_free_hostent(host_result);
      }
      break;
    }
    
    case 3: {
      /* Test 4: Test with empty string hostname */
      struct hostent *host_result = NULL;
      
      /* Empty string test */
      int result = ares_gethostbyname_file(channel, "", 
                                          actual_family, &host_result);
      (void)result;
      
      if (host_result != NULL) {
        ares_free_hostent(host_result);
      }
      break;
    }
    
    case 4: {
      /* Test 5: Test with hostnames containing special characters */
      struct hostent *host_result = NULL;
      
      /* Construct test hostname with special characters from fuzzed data */
      std::string special_hostname;
      if (remaining_data.size() > 0) {
        /* Use first part of remaining data to create a "special" hostname */
        size_t len = remaining_data.size() % 128;
        if (len > 0) {
          special_hostname.assign(reinterpret_cast<const char*>(remaining_data.data()), len);
        }
      }
      
      if (!special_hostname.empty()) {
        int result = ares_gethostbyname_file(channel, special_hostname.c_str(), 
                                            actual_family, &host_result);
        (void)result;
        
        if (host_result != NULL) {
          ares_free_hostent(host_result);
        }
      }
      break;
    }
    
    case 5: {
      /* Test 6: Multiple sequential calls to test caching behavior */
      struct hostent *host_result1 = NULL;
      struct hostent *host_result2 = NULL;
      struct hostent *host_result3 = NULL;
      
      /* First call */
      if (strlen(hostname1.c_str()) > 0) {
        int result1 = ares_gethostbyname_file(channel, hostname1.c_str(), 
                                             actual_family, &host_result1);
        (void)result1;
      }
      
      /* Second call with same hostname (should potentially use cache) */
      if (strlen(hostname1.c_str()) > 0) {
        int result2 = ares_gethostbyname_file(channel, hostname1.c_str(), 
                                             actual_family, &host_result2);
        (void)result2;
      }
      
      /* Third call with different hostname */
      if (strlen(hostname2.c_str()) > 0) {
        int result3 = ares_gethostbyname_file(channel, hostname2.c_str(), 
                                             actual_family, &host_result3);
        (void)result3;
      }
      
      /* Clean up all results */
      if (host_result1 != NULL) ares_free_hostent(host_result1);
      if (host_result2 != NULL) ares_free_hostent(host_result2);
      if (host_result3 != NULL) ares_free_hostent(host_result3);
      break;
    }
  }

  /* Clean up */
  ares_destroy(channel);
  ares_library_cleanup();
  
  return 0;
}

} // namespace harness_011
