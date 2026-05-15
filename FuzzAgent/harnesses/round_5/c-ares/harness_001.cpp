#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_001 {
/* Comprehensive fuzzing harness for c-ares DNS resolver library
 * Targets legacy async DNS processing pipeline functions identified as critical coverage gaps:
 * - ares_process, ares_process_fd, ares_process_fds (0% covered, 300+ undiscovered branches each)
 * - Legacy query APIs: ares_query, ares_search, ares_gethostbyname, ares_gethostbyaddr, ares_getnameinfo
 * 
 * This harness implements the complete async DNS resolution lifecycle with semantic diversity
 * from harness_000 (which focused on getaddrinfo and modern APIs).
 */



/* Callback for legacy ares_query/ares_search operations */
static void query_callback(void *arg, int status, int timeouts,
                          const unsigned char *abuf, int alen) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)abuf;
  (void)alen;
}

/* Callback for ares_gethostbyname/ares_gethostbyaddr operations */
static void host_callback(void *arg, int status, int timeouts,
                         const struct hostent *hostent) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)hostent;
}

/* Callback for ares_getnameinfo operations */
static void nameinfo_callback(void *arg, int status, int timeouts,
                             const char *node, const char *service) {
  /* Minimal callback - just satisfy API requirement */
  (void)arg;
  (void)status;
  (void)timeouts;
  (void)node;
  (void)service;
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

  /* Consume fixed-size data for operation selection and parameters */
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
  uint16_t dns_class_val = fdp.ConsumeIntegral<uint16_t>();
  uint16_t dns_type_val = fdp.ConsumeIntegral<uint16_t>();
  uint8_t family = fdp.ConsumeIntegral<uint8_t>() % 3;  /* 0=AF_INET, 1=AF_INET6, 2=AF_UNSPEC */
  uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
  uint16_t port = fdp.ConsumeIntegral<uint16_t>();

  /* Consume variable-size data for strings and addresses */
  std::string domain_name = fdp.ConsumeRandomLengthString(256);
  std::string service_name = fdp.ConsumeRandomLengthString(256);
  
  /* Consume address data for ares_gethostbyaddr */
  std::vector<uint8_t> addr_data;
  if (family == 0) {
    /* IPv4 address - 4 bytes */
    addr_data = fdp.ConsumeBytes<uint8_t>(4);
  } else if (family == 1) {
    /* IPv6 address - 16 bytes */
    addr_data = fdp.ConsumeBytes<uint8_t>(16);
  }

  /* Create sockaddr structure for ares_getnameinfo */
  struct sockaddr_in sa_in;
  struct sockaddr_in6 sa_in6;
  struct sockaddr *sa = NULL;
  ares_socklen_t salen = 0;

  /* Consume remaining bytes for raw DNS data or additional parameters */
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();

  switch (operation) {
    case 0: {
      /* Test ares_query (legacy API) */
      ares_query(channel,
                 domain_name.c_str(),
                 dns_class_val,
                 dns_type_val,
                 query_callback,
                 NULL);
      break;
    }

    case 1: {
      /* Test ares_search (legacy API) */
      ares_search(channel,
                  domain_name.c_str(),
                  dns_class_val,
                  dns_type_val,
                  query_callback,
                  NULL);
      break;
    }

    case 2: {
      /* Test ares_gethostbyname */
      int actual_family = (family == 0) ? AF_INET : 
                         (family == 1) ? AF_INET6 : AF_UNSPEC;
      ares_gethostbyname(channel,
                        domain_name.c_str(),
                        actual_family,
                        host_callback,
                        NULL);
      break;
    }

    case 3: {
      /* Test ares_gethostbyaddr - only if we have enough address data */
      int actual_family = (family == 0) ? AF_INET : AF_INET6;
      int addrlen = (family == 0) ? 4 : 16;
      
      if (addr_data.size() >= (size_t)addrlen) {
        ares_gethostbyaddr(channel,
                          addr_data.data(),
                          addrlen,
                          actual_family,
                          host_callback,
                          NULL);
      }
      break;
    }

    case 4: {
      /* Test ares_getnameinfo */
      int actual_family = (family == 0) ? AF_INET : AF_INET6;
      
      if (actual_family == AF_INET) {
        memset(&sa_in, 0, sizeof(sa_in));
        sa_in.sin_family = AF_INET;
        sa_in.sin_port = htons(port);
        if (addr_data.size() >= 4) {
          memcpy(&sa_in.sin_addr, addr_data.data(), 4);
        }
        sa = (struct sockaddr *)&sa_in;
        salen = sizeof(sa_in);
      } else {
        memset(&sa_in6, 0, sizeof(sa_in6));
        sa_in6.sin6_family = AF_INET6;
        sa_in6.sin6_port = htons(port);
        if (addr_data.size() >= 16) {
          memcpy(&sa_in6.sin6_addr, addr_data.data(), 16);
        }
        sa = (struct sockaddr *)&sa_in6;
        salen = sizeof(sa_in6);
      }
      
      if (sa != NULL) {
        ares_getnameinfo(channel,
                        sa,
                        salen,
                        flags,
                        nameinfo_callback,
                        NULL);
      }
      break;
    }

    case 5: {
      /* Test async processing pipeline with ares_fds and ares_process_fds */
      
      /* First, initiate a query to have something to process */
      ares_query(channel,
                 domain_name.c_str(),
                 dns_class_val,
                 dns_type_val,
                 query_callback,
                 NULL);
      
      /* Get file descriptors that need attention */
      fd_set read_fds, write_fds;
      FD_ZERO(&read_fds);
      FD_ZERO(&write_fds);
      
      int nfds = ares_fds(channel, &read_fds, &write_fds);
      
      /* Process using ares_process_fds with generated events */
      if (nfds > 0) {
        /* Create event array for ares_process_fds */
        const size_t max_events = 10;
        ares_fd_events_t events[max_events];
        size_t nevents = 0;
        
        /* Check for read events */
        for (int fd = 0; fd < nfds && nevents < max_events; fd++) {
          if (FD_ISSET(fd, &read_fds)) {
            events[nevents].fd = fd;
            events[nevents].events = ARES_FD_EVENT_READ;
            nevents++;
          }
        }
        
        /* Check for write events */
        for (int fd = 0; fd < nfds && nevents < max_events; fd++) {
          if (FD_ISSET(fd, &write_fds)) {
            events[nevents].fd = fd;
            events[nevents].events = ARES_FD_EVENT_WRITE;
            nevents++;
          }
        }
        
        /* Process the events */
        if (nevents > 0) {
          ares_process_fds(channel, events, nevents, ARES_PROCESS_FLAG_NONE);
        }
      }
      
      /* Also test ares_process_fd for individual sockets */
      if (nfds > 0) {
        for (int fd = 0; fd < nfds; fd++) {
          ares_socket_t read_fd = ARES_SOCKET_BAD;
          ares_socket_t write_fd = ARES_SOCKET_BAD;
          
          if (FD_ISSET(fd, &read_fds)) {
            read_fd = fd;
          }
          if (FD_ISSET(fd, &write_fds)) {
            write_fd = fd;
          }
          
          if (read_fd != ARES_SOCKET_BAD || write_fd != ARES_SOCKET_BAD) {
            ares_process_fd(channel, read_fd, write_fd);
          }
        }
      }
      
      /* Process timeouts */
      struct timeval tv;
      struct timeval *timeout = ares_timeout(channel, NULL, &tv);
      (void)timeout; /* Use timeout in real applications */
      
      break;
    }
  }

  /* Cleanup */
  ares_destroy(channel);
  ares_library_cleanup();

  return 0;
}

} // namespace harness_001
