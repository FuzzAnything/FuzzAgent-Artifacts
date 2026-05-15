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
#include <ares.h>
}
extern "C" {
#include <ares_dns.h>
}
extern "C" {
#include <ares_nameser.h>
}
//<ID> 1234
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 6
//<Quality> {"density":10,"unique_branches":{"legacycb_arecvfrom":[[527,7,527,41,0,0,4,1]],"legacycb_aclose":[[468,7,468,41,0,0,4,1]],"legacycb_asocket":[[482,7,482,41,0,0,4,1]],"ares_queue_notify_empty":[[825,7,825,43,0,0,4,0]],"legacycb_asendto":[[550,7,550,41,0,0,4,1]],"legacycb_aconnect":[[510,7,510,41,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_srv_reply","ares_free_data","ares_set_servers_ports","ares_expand_name","ares_free_string","ares_dns_rec_type_tostr","ares_set_socket_functions","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_destroy","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_srv_reply","ares_free_data","ares_set_servers_ports","ares_expand_name","ares_free_string","ares_dns_rec_type_tostr","ares_set_socket_functions","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_destroy","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback for ares_send
static void  id_000064_send_callback(void* arg, int status, int timeouts,
                         const unsigned char* abuf, int alen) {
  // Empty callback - just consume the parameters
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)abuf;
  (void)alen;
}

// Callback for ares_gethostbyaddr
static void  id_000064_host_callback(void* arg, int status, int timeouts,
                         const struct hostent* host) {
  // Empty callback - just consume the parameters
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)host;
}

// Callback for ares_query_dnsrec
static void  id_000064_dnsrec_callback(void* arg, ares_status_t status, size_t timeouts,
                           const ares_dns_record_t* dnsrec) {
  // Empty callback - just consume the parameters
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)dnsrec;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




  if (size < 4) {
	return 0;
	}
	

  // Step 1: Initialize library
  if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
    return 0;
  }

  ares_channel_t* channel = nullptr;
  struct ares_options options;
  int optmask = 0;

  // Initialize options with some defaults
  memset(&options, 0, sizeof(options));
  options.flags = ARES_FLAG_STAYOPEN;
  options.timeout = 2000;
  options.tries = 2;
  optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;

  // Step 2: Initialize channel with options
  if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
    ares_library_cleanup();
    return 0;
  }

  // Step 3: Try to parse input as SRV reply
  struct ares_srv_reply* srv_reply = nullptr;
  if (size > 0) {
    ares_parse_srv_reply(data, static_cast<int>(size), &srv_reply);
    if (srv_reply) {
      ares_free_data(srv_reply);
    }
  }

  // Step 4: Set servers using dummy data
  struct ares_addr_port_node server;
  memset(&server, 0, sizeof(server));
  server.family = AF_INET;
  server.next = nullptr;
  
  // Use first 4 bytes for IPv4 address if available
  if (size >= 4) {
    memcpy(&server.addr.addr4, data, 4);
    ares_set_servers_ports(channel, &server);
  }

  // Step 5: Try to expand a name from the data
  char* expanded_name = nullptr;
  long enclen = 0;
  if (size > 2) {
    // Use a portion of the data for expansion attempt
    size_t expansion_size = size > 100 ? 100 : size;
    ares_expand_name(data, data, static_cast<int>(expansion_size),
                     &expanded_name, &enclen);
    if (expanded_name) {
      ares_free_string(expanded_name);
    }
  }

  // Step 6: Convert a DNS record type to string
  const char* type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_SRV);
  (void)type_str; // Use variable to avoid unused warning

  // Step 7: Set socket functions (use default)
  ares_set_socket_functions(channel, nullptr, nullptr);

  // Step 8: Create and manipulate DNS record
  ares_dns_record_t* dnsrec = nullptr;
  if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, ARES_FLAG_QR,
                            ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
    // Try to set ID - use first 2 bytes of data if available
    unsigned short id = 0;
    if (size >= 2) {
      memcpy(&id, data, 2);
    } else {
      id = 12345;
    }
    ares_dns_record_set_id(dnsrec, fuzz_uint16_t_3);
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
  }

  // Step 9: Send a query using the input data
  if (size > 0) {
    // Use first part of data as query
    size_t query_len = size > 512 ? 512 : size;
    ares_send(channel, data, static_cast<int>(query_len),
               id_000064_send_callback, nullptr);
  }

  // Step 10: Try reverse lookup
  if (size >= 4) {
    // Use first 4 bytes as IPv4 address
    struct in_addr addr;
    memcpy(&addr, data, 4);
    ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_4,
                        id_000064_host_callback, nullptr);
  }

  // Step 11: Make a DNS query
  if (size > 1) {
    // Create a name from the data
    char name[256];
    size_t name_len = size > 255 ? 255 : size;
    memcpy(name, data, name_len);
    name[name_len] = '\0';
    
    unsigned short qid = 0;
    ares_query_dnsrec(channel, name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                       id_000064_dnsrec_callback, nullptr, &qid);
  }

  // Step 12: Process any pending events
  fd_set read_fds, write_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  
  int nfds = ares_fds(channel, &read_fds, &write_fds);
  if (nfds > 0) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000; // 1ms timeout
    
    // Create file descriptor set for select
    fd_set read_copy = read_fds;
    fd_set write_copy = write_fds;
    
    // Use select with zero timeout to check without blocking
    select(nfds, &read_copy, &write_copy, nullptr, &tv);
    
    ares_process(channel, &read_fds, &write_fds);
  }

  // Step 13: Cancel any pending operations
  ares_cancel(channel);

  // Step 14: Clean up channel
  ares_destroy(channel);

  // Step 15: Final library cleanup
  ares_library_cleanup();

  return 0;
}