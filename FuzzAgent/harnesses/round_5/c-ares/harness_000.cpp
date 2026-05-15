#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_000 {
/* Comprehensive fuzzing harness for c-ares DNS resolver library
 * Targets core DNS resolution functions: ares_init, ares_getaddrinfo, ares_query
 */



/* Simple callback function for async operations */
static void dummy_callback(void *arg, int status, int timeouts,
                           const unsigned char *abuf, int alen) {
  /* Do nothing - just satisfy callback requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)abuf;
  (void)alen;
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status,
                                  size_t timeouts,
                                  const ares_dns_record_t *dnsrec) {
  /* Do nothing - just satisfy callback requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)dnsrec;
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts,
                                    struct ares_addrinfo *res) {
  /* Do nothing - just satisfy callback requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)res;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  /* Minimum size check */
  if (size < 16) {
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

  /* Consume fixed-size data for operation selection and parameters */
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
  uint16_t dns_class_val = fdp.ConsumeIntegral<uint16_t>();
  uint16_t dns_type_val = fdp.ConsumeIntegral<uint16_t>();
  uint16_t flags = fdp.ConsumeIntegral<uint16_t>();
  uint16_t port = fdp.ConsumeIntegral<uint16_t>();

  /* Consume variable-size data for strings and buffers */
  std::string node_name = fdp.ConsumeRandomLengthString(256);
  std::string service_name = fdp.ConsumeRandomLengthString(256);
  std::string csv_servers = fdp.ConsumeRandomLengthString(512);
  
  /* Get remaining bytes for raw DNS data */
  std::vector<uint8_t> dns_data = fdp.ConsumeRemainingBytes<uint8_t>();

  switch (operation) {
    case 0: {
      /* Test ares_getaddrinfo */
      struct ares_addrinfo_hints hints;
      memset(&hints, 0, sizeof(hints));
      hints.ai_flags = flags;
      hints.ai_family = (fdp.ConsumeBool() ? AF_INET : AF_INET6);
      hints.ai_socktype = (fdp.ConsumeBool() ? SOCK_STREAM : SOCK_DGRAM);
      hints.ai_protocol = (fdp.ConsumeBool() ? IPPROTO_TCP : IPPROTO_UDP);

      ares_getaddrinfo(channel,
                       node_name.c_str(),
                       service_name.c_str(),
                       &hints,
                       dummy_addrinfo_callback,
                       NULL);
      break;
    }

    case 1: {
      /* Test ares_query_dnsrec (modern API) */
      unsigned short query_id = 0;
      ares_dns_class_t dns_class = static_cast<ares_dns_class_t>(
          dns_class_val % (ARES_CLASS_ANY + 1));
      ares_dns_rec_type_t dns_type = static_cast<ares_dns_rec_type_t>(
          dns_type_val % (ARES_REC_TYPE_URI + 1));

      ares_query_dnsrec(channel,
                        node_name.c_str(),
                        dns_class,
                        dns_type,
                        dummy_callback_dnsrec,
                        NULL,
                        &query_id);
      break;
    }

    case 2: {
      /* Test ares_send_dnsrec with a simple constructed DNS record */
      ares_dns_record_t *dnsrec = NULL;
      ares_status_t rec_status = ares_dns_record_create(&dnsrec, 
                                                        0, /* id */
                                                        ARES_FLAG_QR,
                                                        ARES_OPCODE_QUERY,
                                                        ARES_RCODE_NOERROR);
      if (rec_status == ARES_SUCCESS && dnsrec != NULL) {
        /* Add a simple question */
        ares_dns_record_query_add(dnsrec, 
                                  node_name.c_str(),
                                  ARES_REC_TYPE_A,
                                  ARES_CLASS_IN);
        
        unsigned short send_id = 0;
        ares_send_dnsrec(channel, dnsrec, dummy_callback_dnsrec, NULL, &send_id);
        ares_dns_record_destroy(dnsrec);
      }
      break;
    }

    case 3: {
      /* Test server configuration with CSV */
      ares_set_servers_csv(channel, csv_servers.c_str());

      /* Also test some channel configuration functions */
      if (fdp.ConsumeBool()) {
        unsigned int local_ip = fdp.ConsumeIntegral<uint32_t>();
        ares_set_local_ip4(channel, local_ip);
      }

      if (fdp.ConsumeBool() && !node_name.empty()) {
        ares_set_local_dev(channel, node_name.c_str());
      }
      break;
    }
  }

  /* Process any pending operations (non-blocking check) */
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  
  fd_set read_fds, write_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  
  int nfds = ares_fds(channel, &read_fds, &write_fds);
  if (nfds > 0) {
    ares_timeout(channel, NULL, &tv);
  }

  /* Clean up */
  ares_destroy(channel);
  ares_library_cleanup();

  return 0;
}

} // namespace harness_000
