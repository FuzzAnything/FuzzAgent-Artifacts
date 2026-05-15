#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_002 {
/* Comprehensive fuzzing harness for c-ares library targeting event processing
 * and channel management APIs identified as critical coverage gaps:
 * - ares_process (305 undiscovered branches)
 * - ares_dup (418 undiscovered branches)
 * - ares_reinit (261 undiscovered branches)
 * - ares_send (260 undiscovered branches)
 * - ares_process_pending_write (259 undiscovered branches)
 * - ares_cancel (257 undiscovered branches)
 * - ares_search_dnsrec (255 undiscovered branches)
 *
 * This harness implements the complete event processing lifecycle with semantic
 * diversity from harness_000 (getaddrinfo) and harness_001 (DNS processing pipeline).
 * Focus: Channel initialization, query creation, sending queries, processing
 * events/timeouts, channel duplication/reinitialization, query cancellation, cleanup.
 */



/* Callback function for DNS record operations */
static void dnsrec_callback(void *arg, ares_status_t status,
                           size_t timeouts, const ares_dns_record_t *dnsrec) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)dnsrec;
}

/* Callback function for legacy ares_send operations */
static void legacy_callback(void *arg, int status, int timeouts,
                           const unsigned char *abuf, int alen) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)abuf;
  (void)alen;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  /* Minimum size check - need enough data for operation selection and basic parameters */
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  /* Initialize c-ares library */
  ares_library_init(ARES_LIB_INIT_ALL);

  /* Create initial channel */
  ares_channel_t *channel = NULL;
  int init_result = ares_init(&channel);
  if (init_result != ARES_SUCCESS) {
    ares_library_cleanup();
    return 0;
  }

  /* Consume fixed-size data for operation selection and parameters */
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
  uint16_t dns_class_val = fdp.ConsumeIntegral<uint16_t>();
  uint16_t dns_type_val = fdp.ConsumeIntegral<uint16_t>();
  uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
  uint16_t port = fdp.ConsumeIntegral<uint16_t>();
  uint8_t use_modern_api = fdp.ConsumeBool();
  uint8_t process_mode = fdp.ConsumeIntegral<uint8_t>() % 3;

  /* Consume variable-size data for strings and buffers */
  std::string domain_name = fdp.ConsumeRandomLengthString(256);
  std::vector<uint8_t> query_buffer = fdp.ConsumeBytes<uint8_t>(128);
  
  /* Get remaining bytes for raw DNS data */
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();

  /* Main operation switch */
  switch (operation) {
    case 0: {
      /* Test ares_process_fds - modern event processing */
      ares_fd_events_t events[2];
      events[0].fd = ARES_SOCKET_BAD;
      events[0].events = ARES_FD_EVENT_NONE;
      events[1].fd = ARES_SOCKET_BAD;
      events[1].events = ARES_FD_EVENT_NONE;
      
      /* Create a simple DNS query first to have something to process */
      ares_dns_record_t *dnsrec = NULL;
      ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
      if (create_result == ARES_SUCCESS && dnsrec) {
        ares_status_t query_result = ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                 (ares_dns_rec_type_t)dns_type_val,
                                 (ares_dns_class_t)dns_class_val);
        if (query_result == ARES_SUCCESS) {
          unsigned short qid = 0;
          ares_send_dnsrec(channel, dnsrec, dnsrec_callback, NULL, &qid);
        }
        ares_dns_record_destroy(dnsrec);
      }
      
      /* Process events with different flags */
      unsigned int process_flags = 0;
      if (flags & 0x01) {
        process_flags |= ARES_PROCESS_FLAG_SKIP_NON_FD;
      }
      
      /* Call ares_process_fds with various parameters */
      ares_status_t process_result = ares_process_fds(channel, events, 0, process_flags);
      (void)process_result; /* Result may vary based on channel state */
      
      /* Also test ares_process_pending_write */
      ares_process_pending_write(channel);
      break;
    }

    case 1: {
      /* Test ares_dup - channel duplication */
      ares_channel_t *duplicated_channel = NULL;
      int dup_result = ares_dup(&duplicated_channel, channel);
      
      if (dup_result == ARES_SUCCESS && duplicated_channel) {
        /* Create and send a query on duplicated channel */
        if (use_modern_api) {
          ares_dns_record_t *dnsrec = NULL;
          ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
          if (create_result == ARES_SUCCESS && dnsrec) {
            ares_status_t query_result = ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                     (ares_dns_rec_type_t)dns_type_val,
                                     (ares_dns_class_t)dns_class_val);
            if (query_result == ARES_SUCCESS) {
              unsigned short qid = 0;
              ares_send_dnsrec(duplicated_channel, dnsrec, dnsrec_callback, NULL, &qid);
            }
            ares_dns_record_destroy(dnsrec);
          }
        } else {
          /* Legacy API */
          ares_send(duplicated_channel, query_buffer.data(), 
                   (int)query_buffer.size(), legacy_callback, NULL);
        }
        
        /* Clean up duplicated channel */
        ares_destroy(duplicated_channel);
      }
      break;
    }

    case 2: {
      /* Test ares_reinit - channel reinitialization */
      ares_status_t reinit_result = ares_reinit(channel);
      (void)reinit_result; /* Result may vary based on channel state */
      
      /* After reinit, try to use the channel */
      if (use_modern_api) {
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        if (create_result == ARES_SUCCESS && dnsrec) {
          ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                   (ares_dns_rec_type_t)dns_type_val,
                                   (ares_dns_class_t)dns_class_val);
          unsigned short qid = 0;
          ares_send_dnsrec(channel, dnsrec, dnsrec_callback, NULL, &qid);
          ares_dns_record_destroy(dnsrec);
        }
      }
      break;
    }

    case 3: {
      /* Test ares_send (both modern and legacy) */
      if (use_modern_api) {
        /* Modern ares_send_dnsrec */
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        if (create_result == ARES_SUCCESS && dnsrec) {
          ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                   (ares_dns_rec_type_t)dns_type_val,
                                   (ares_dns_class_t)dns_class_val);
          unsigned short qid = 0;
          ares_status_t send_result = ares_send_dnsrec(channel, dnsrec, 
                                                      dnsrec_callback, NULL, &qid);
          (void)send_result;
          ares_dns_record_destroy(dnsrec);
        }
      } else {
        /* Legacy ares_send */
        ares_send(channel, query_buffer.data(), (int)query_buffer.size(),
                 legacy_callback, NULL);
      }
      break;
    }

    case 4: {
      /* Test ares_search_dnsrec */
      ares_dns_record_t *dnsrec = NULL;
      ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
      if (create_result == ARES_SUCCESS && dnsrec) {
        ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                 (ares_dns_rec_type_t)dns_type_val,
                                 (ares_dns_class_t)dns_class_val);
        
        /* Add some additional sections to make it more interesting */
        if (remaining_data.size() > 10) {
          ares_dns_rr_t *rr = NULL;
          ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, domain_name.c_str(),
                                (ares_dns_rec_type_t)dns_type_val,
                                (ares_dns_class_t)dns_class_val, 3600);
        }
        
        ares_status_t search_result = ares_search_dnsrec(channel, dnsrec, 
                                                        dnsrec_callback, NULL);
        (void)search_result;
        ares_dns_record_destroy(dnsrec);
      }
      break;
    }

    case 5: {
      /* Test ares_cancel - create queries then cancel them */
      /* First create and send multiple queries */
      for (int i = 0; i < 3 && remaining_data.size() > 20; i++) {
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        if (create_result == ARES_SUCCESS && dnsrec) {
          std::string temp_name = domain_name + std::to_string(i);
          ares_status_t query_result = ares_dns_record_query_add(dnsrec, temp_name.c_str(),
                                   (ares_dns_rec_type_t)dns_type_val,
                                   (ares_dns_class_t)dns_class_val);
          if (query_result == ARES_SUCCESS) {
            unsigned short qid = 0;
            ares_send_dnsrec(channel, dnsrec, dnsrec_callback, NULL, &qid);
          }
          ares_dns_record_destroy(dnsrec);
        }
      }
      
      /* Now cancel all pending queries */
      ares_cancel(channel);
      break;
    }


    case 6: {
      /* Test ares_process_fd - single file descriptor processing */
      /* First create a query */
      ares_dns_record_t *dnsrec = NULL;
      ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
      if (create_result == ARES_SUCCESS && dnsrec) {
        ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                 (ares_dns_rec_type_t)dns_type_val,
                                 (ares_dns_class_t)dns_class_val);
        unsigned short qid = 0;
        ares_send_dnsrec(channel, dnsrec, dnsrec_callback, NULL, &qid);
        ares_dns_record_destroy(dnsrec);
      }
      
      /* Process with different fd combinations */
      ares_socket_t read_fd = ARES_SOCKET_BAD;
      ares_socket_t write_fd = ARES_SOCKET_BAD;
      
      if (flags & 0x01) {
        read_fd = 0; /* Simulate a read fd */
      }
      if (flags & 0x02) {
        write_fd = 1; /* Simulate a write fd */
      }
      
      ares_process_fd(channel, read_fd, write_fd);
      break;
    }

    case 7: {
      /* Test deprecated ares_process with fd_set */
      /* First create a query */
      ares_dns_record_t *dnsrec = NULL;
      ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
      if (create_result == ARES_SUCCESS && dnsrec) {
        ares_dns_record_query_add(dnsrec, domain_name.c_str(),
                                 (ares_dns_rec_type_t)dns_type_val,
                                 (ares_dns_class_t)dns_class_val);
        unsigned short qid = 0;
        ares_send_dnsrec(channel, dnsrec, dnsrec_callback, NULL, &qid);
        ares_dns_record_destroy(dnsrec);
      }
      
      /* Create fd_sets */
      fd_set read_fds, write_fds;
      FD_ZERO(&read_fds);
      FD_ZERO(&write_fds);
      
      if (flags & 0x01) {
        FD_SET(0, &read_fds); /* stdin */
      }
      if (flags & 0x02) {
        FD_SET(1, &write_fds); /* stdout */
      }
      
      /* Call deprecated ares_process */
      ares_process(channel, &read_fds, &write_fds);
      break;
    }

    default:
      /* Should never happen due to modulo operation */
      break;
  }

  /* Test ares_process_pending_write in all cases */
  ares_process_pending_write(channel);

  /* Clean up */
  ares_destroy(channel);
  ares_library_cleanup();

  return 0;
}

} // namespace harness_002
