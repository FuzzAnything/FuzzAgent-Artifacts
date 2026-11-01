#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ares.h>

/* Helper to create a dummy server address */
static void setup_server(struct in_addr *server) {
    server->s_addr = htonl(0x7F000001); /* 127.0.0.1 */
}

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_65(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    int rc;
    struct ares_options saved_options;
    int saved_optmask;

    /* Initialize options struct to zero */
    memset(&options, 0, sizeof(options));
    memset(&saved_options, 0, sizeof(saved_options));

    /* Use input data to determine which options to set */
    uint8_t flags = data[0];
    uint8_t has_flags = (flags & 0x01);
    uint8_t has_servers = (flags & 0x02);
    uint8_t has_domains = (flags & 0x04);
    uint8_t has_sortlist = (flags & 0x08);
    uint8_t has_lookups = (flags & 0x10);
    uint8_t has_resolvconf = (flags & 0x20);
    uint8_t has_hosts_file = (flags & 0x40);
    uint8_t has_sock_state_cb = (flags & 0x80);
    
    /* Additional flags for other options if size permits */
    uint8_t has_timeout = (size > 1) ? (data[1] & 0x01) : 0;
    uint8_t has_tries = (size > 1) ? (data[1] & 0x02) : 0;
    uint8_t has_ndots = (size > 1) ? (data[1] & 0x04) : 0;
    uint8_t has_maxtimeout = (size > 1) ? (data[1] & 0x08) : 0;
    uint8_t has_udp_port = (size > 1) ? (data[1] & 0x10) : 0;
    uint8_t has_tcp_port = (size > 1) ? (data[1] & 0x20) : 0;
    uint8_t has_sock_sndbuf = (size > 1) ? (data[1] & 0x40) : 0;
    uint8_t has_sock_rcvbuf = (size > 1) ? (data[1] & 0x80) : 0;
    uint8_t has_udp_max_queries = (size > 2) ? (data[2] & 0x01) : 0;
    uint8_t has_query_cache = (size > 2) ? (data[2] & 0x02) : 0;
    uint8_t has_event_thread = (size > 2) ? (data[2] & 0x04) : 0;
    uint8_t has_server_failover = (size > 2) ? (data[2] & 0x08) : 0;
    
    /* Fixed: Added missing declaration for has_ednspsz */
    uint8_t has_ednspsz = (size > 2) ? (data[2] & 0x10) : 0;

    /* Set basic options */
    if (has_flags) {
        optmask |= ARES_OPT_FLAGS;
        options.flags = (flags >> 4) & 0x0F; /* Use part of flags byte */
    }
    if (has_timeout) {
        optmask |= ARES_OPT_TIMEOUTMS;
        options.timeout = 1000;
    }
    if (has_tries) {
        optmask |= ARES_OPT_TRIES;
        options.tries = 5;
    }
    if (has_ndots) {
        optmask |= ARES_OPT_NDOTS;
        options.ndots = 3;
    }
    if (has_maxtimeout) {
        optmask |= ARES_OPT_MAXTIMEOUTMS;
        options.maxtimeout = 5000;
    }
    if (has_udp_port) {
        optmask |= ARES_OPT_UDP_PORT;
        options.udp_port = 53;
    }
    if (has_tcp_port) {
        optmask |= ARES_OPT_TCP_PORT;
        options.tcp_port = 53;
    }
    if (has_sock_sndbuf) {
        optmask |= ARES_OPT_SOCK_SNDBUF;
        options.socket_send_buffer_size = 4096;
    }
    if (has_sock_rcvbuf) {
        optmask |= ARES_OPT_SOCK_RCVBUF;
        options.socket_receive_buffer_size = 4096;
    }
    if (has_ednspsz) {
        optmask |= ARES_OPT_EDNSPSZ;
        options.ednspsz = 4096;
    }
    if (has_udp_max_queries) {
        optmask |= ARES_OPT_UDP_MAX_QUERIES;
        options.udp_max_queries = 10;
    }
    if (has_query_cache) {
        optmask |= ARES_OPT_QUERY_CACHE;
        options.qcache_max_ttl = 300;
    }
    if (has_event_thread) {
        optmask |= ARES_OPT_EVENT_THREAD;
        /* Fixed: Cast NULL to ares_evsys_t to resolve type mismatch */
        options.evsys = (ares_evsys_t)NULL; 
    }
    if (has_server_failover) {
        optmask |= ARES_OPT_SERVER_FAILOVER;
        options.server_failover_opts.retry_chance = 100;
        options.server_failover_opts.retry_delay = 5000;
    }

    /* Set Servers */
    if (has_servers) {
        optmask |= ARES_OPT_SERVERS;
        struct in_addr server;
        setup_server(&server);
        options.servers = &server;
        options.nservers = 1;
    }

    /* Set Domains */
    if (has_domains) {
        optmask |= ARES_OPT_DOMAINS;
        char *domain = "example.com";
        char *domains[] = { domain, NULL };
        options.domains = domains;
        options.ndomains = 1;
    }

    /* 
     * Set Sortlist 
     * Note: Removed because struct apattern is opaque/internal.
     * We cannot instantiate it without internal headers.
     * The flag 'has_sortlist' is still parsed but ignored to prevent errors.
     */
    if (has_sortlist) {
        /* 
         * We cannot set options.sortlist directly because struct apattern is incomplete.
         * Skipping this option to ensure compilation.
         */
    }

    /* Set Lookups */
    if (has_lookups) {
        optmask |= ARES_OPT_LOOKUPS;
        options.lookups = "fb";
    }

    /* Set Resolvconf */
    if (has_resolvconf) {
        optmask |= ARES_OPT_RESOLVCONF;
        options.resolvconf_path = "/etc/resolv.conf";
    }

    /* Set Hosts File */
    if (has_hosts_file) {
        optmask |= ARES_OPT_HOSTS_FILE;
        options.hosts_path = "/etc/hosts";
    }

    /* Set Socket State Callback */
    if (has_sock_state_cb) {
        optmask |= ARES_OPT_SOCK_STATE_CB;
        options.sock_state_cb = NULL; /* Dummy callback */
        options.sock_state_cb_data = NULL;
    }

    /* Initialize channel */
    rc = ares_init_options(&channel, &options, optmask);
    if (rc != ARES_SUCCESS) {
        /* If init fails, we can't test save_options effectively, but we return 0 */
        return 0;
    }

    /* Call the target function */
    rc = ares_save_options(channel, &saved_options, &saved_optmask);

    /* Cleanup */
    ares_destroy_options(&saved_options);
    ares_destroy(channel);

    return 0;
}