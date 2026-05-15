#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"

namespace harness_003 {
/* Comprehensive fuzzing harness for c-ares library targeting server configuration APIs
 * identified as critical coverage gaps by CoverageAnalyzerAgent:
 * - ares_set_servers (238 undiscovered branches)
 * - ares_set_servers_ports (238 undiscovered branches)  
 * - ares_get_servers (38 undiscovered branches)
 * - ares_get_servers_ports (38 undiscovered branches)
 * - ares_set_servers_csv (234 undiscovered branches)
 * - ares_set_servers_ports_csv (234 undiscovered branches)
 *
 * This harness implements the complete server configuration lifecycle with semantic
 * diversity from previous harnesses:
 * - harness_000: getaddrinfo and modern APIs
 * - harness_001: DNS processing pipeline
 * - harness_002: event processing and channel management
 *
 * Focus: Library initialization, channel creation, server configuration (set servers),
 * server retrieval (get servers), CSV variant testing, proper cleanup.
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  /* Minimum size check - need enough data for operation selection and basic server configurations */
  if (size < 64) {
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
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
  uint8_t num_servers = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
  uint8_t family_choice = fdp.ConsumeIntegral<uint8_t>() % 3; /* 0=AF_INET, 1=AF_INET6, 2=mixed */
  uint16_t base_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
  uint8_t use_ports = fdp.ConsumeBool(); /* Whether to use port-specific variants */

  /* Consume variable-size data for CSV strings */
  std::string csv_servers = fdp.ConsumeRandomLengthString(512);

  /* Create server node structures from fuzzer input */
  struct ares_addr_node *servers = NULL;
  struct ares_addr_node *server_tail = NULL;
  
  struct ares_addr_port_node *port_servers = NULL;
  struct ares_addr_port_node *port_server_tail = NULL;

  /* Build server lists based on fuzzer input */
  for (uint8_t i = 0; i < num_servers && fdp.remaining_bytes() >= 20; i++) {
    /* Determine address family for this server */
    int family = AF_INET;
    if (family_choice == 1) {
      family = AF_INET6;
    } else if (family_choice == 2) {
      family = (fdp.ConsumeBool() ? AF_INET : AF_INET6);
    }

    if (use_ports) {
      /* Create port-specific server node */
      struct ares_addr_port_node *new_port_server = 
          (struct ares_addr_port_node *)malloc(sizeof(struct ares_addr_port_node));
      if (!new_port_server) break;
      
      memset(new_port_server, 0, sizeof(struct ares_addr_port_node));
      new_port_server->family = family;
      
      /* Consume port values */
      new_port_server->udp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
      new_port_server->tcp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
      
      /* Consume address data */
      if (family == AF_INET) {
        /* IPv4 address - 4 bytes */
        if (fdp.remaining_bytes() < 4) break;
        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
        memcpy(&new_port_server->addr.addr4, addr_bytes.data(), 4);
      } else {
        /* IPv6 address - 16 bytes */
        if (fdp.remaining_bytes() < 16) break;
        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
        memcpy(&new_port_server->addr.addr6, addr_bytes.data(), 16);
      }
      
      /* Link into list */
      new_port_server->next = NULL;
      if (!port_servers) {
        port_servers = new_port_server;
        port_server_tail = new_port_server;
      } else {
        port_server_tail->next = new_port_server;
        port_server_tail = new_port_server;
      }
    } else {
      /* Create basic server node */
      struct ares_addr_node *new_server = 
          (struct ares_addr_node *)malloc(sizeof(struct ares_addr_node));
      if (!new_server) break;
      
      memset(new_server, 0, sizeof(struct ares_addr_node));
      new_server->family = family;
      
      /* Consume address data */
      if (family == AF_INET) {
        /* IPv4 address - 4 bytes */
        if (fdp.remaining_bytes() < 4) break;
        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
        memcpy(&new_server->addr.addr4, addr_bytes.data(), 4);
      } else {
        /* IPv6 address - 16 bytes */
        if (fdp.remaining_bytes() < 16) break;
        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
        memcpy(&new_server->addr.addr6, addr_bytes.data(), 16);
      }
      
      /* Link into list */
      new_server->next = NULL;
      if (!servers) {
        servers = new_server;
        server_tail = new_server;
      } else {
        server_tail->next = new_server;
        server_tail = new_server;
      }
    }
  }

  /* Main operation switch - test different server configuration APIs */
  switch (operation) {
    case 0: {
      /* Test ares_set_servers - basic server configuration */
      int set_result = ares_set_servers(channel, servers);
      (void)set_result; /* Result may vary based on input validity */
      
      /* Retrieve and verify configuration */
      struct ares_addr_node *retrieved_servers = NULL;
      int get_result = ares_get_servers(channel, &retrieved_servers);
      (void)get_result;
      
      /* Clean up retrieved servers */
      if (retrieved_servers) {
        ares_free_data(retrieved_servers);
      }
      break;
    }
    
    case 1: {
      /* Test ares_set_servers_ports - port-specific server configuration */
      int set_result = ares_set_servers_ports(channel, port_servers);
      (void)set_result;
      
      /* Retrieve and verify configuration */
      struct ares_addr_port_node *retrieved_port_servers = NULL;
      int get_result = ares_get_servers_ports(channel, &retrieved_port_servers);
      (void)get_result;
      
      /* Clean up retrieved servers */
      if (retrieved_port_servers) {
        ares_free_data(retrieved_port_servers);
      }
      break;
    }
    
    case 2: {
      /* Test ares_set_servers_csv - CSV format server configuration */
      int set_result = ares_set_servers_csv(channel, csv_servers.c_str());
      (void)set_result;
      
      /* Test retrieval via CSV */
      char *csv_output = ares_get_servers_csv(channel);
      if (csv_output) {
        free(csv_output);
      }
      break;
    }
    
    case 3: {
      /* Test ares_set_servers_ports_csv - CSV format with ports */
      int set_result = ares_set_servers_ports_csv(channel, csv_servers.c_str());
      (void)set_result;
      
      /* Test retrieval via CSV */
      char *csv_output = ares_get_servers_csv(channel);
      if (csv_output) {
        free(csv_output);
      }
      break;
    }
    
    case 4: {
      /* Test mixed operations: set basic, then ports, then CSV */
      if (servers) {
        (void)ares_set_servers(channel, servers);
      }
      
      if (port_servers) {
        (void)ares_set_servers_ports(channel, port_servers);
      }
      
      if (!csv_servers.empty()) {
        (void)ares_set_servers_csv(channel, csv_servers.c_str());
      }
      
      /* Retrieve via CSV to see final configuration */
      char *csv_output = ares_get_servers_csv(channel);
      if (csv_output) {
        free(csv_output);
      }
      break;
    }
    
    case 5: {
      /* Test edge cases: null inputs, empty configurations */
      /* Set null servers (clears configuration) */
      (void)ares_set_servers(channel, NULL);
      
      /* Set empty CSV */
      (void)ares_set_servers_csv(channel, "");
      
      /* Try to get servers from cleared configuration */
      struct ares_addr_node *retrieved = NULL;
      (void)ares_get_servers(channel, &retrieved);
      if (retrieved) {
        ares_free_data(retrieved);
      }
      
      /* Restore some configuration */
      if (servers) {
        (void)ares_set_servers(channel, servers);
      }
      break;
    }
  }

  /* Clean up allocated server structures */
  struct ares_addr_node *curr_server = servers;
  while (curr_server) {
    struct ares_addr_node *next = curr_server->next;
    free(curr_server);
    curr_server = next;
  }
  
  struct ares_addr_port_node *curr_port_server = port_servers;
  while (curr_port_server) {
    struct ares_addr_port_node *next = curr_port_server->next;
    free(curr_port_server);
    curr_port_server = next;
  }

  /* Clean up channel and library */
  ares_destroy(channel);
  ares_library_cleanup();

  return 0;
}

} // namespace harness_003
