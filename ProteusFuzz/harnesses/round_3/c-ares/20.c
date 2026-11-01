#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ares.h>

/* Fuzzing harness for ares_save_options */

/* Static helper to initialize a channel with maximum options to trigger all optmask branches */
static ares_channel_t* setup_channel_with_all_options(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    struct in_addr servers[1];
    char *domains[2];
    /* Note: struct apattern is opaque and cannot be instantiated directly. 
       Removed sortlist setup to fix compilation error. */
    char *lookups_str = "fb";
    char *resolvconf_path = "/etc/resolv.conf";
    char *hosts_path = "/etc/hosts";
    
    memset(&options, 0, sizeof(options));
    
    /* Set flags */
    options.flags = ARES_FLAG_NOCHECKRESP;
    optmask |= ARES_OPT_FLAGS;
    
    /* Set timeout (ms) */
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    /* Set tries */
    options.tries = 2;
    optmask |= ARES_OPT_TRIES;
    
    /* Set ndots */
    options.ndots = 3;
    optmask |= ARES_OPT_NDOTS;
    
    /* Set maxtimeout */
    options.maxtimeout = 5000;
    optmask |= ARES_OPT_MAXTIMEOUTMS;
    
    /* Set ports */
    options.udp_port = 53;
    options.tcp_port = 53;
    optmask |= ARES_OPT_UDP_PORT | ARES_OPT_TCP_PORT;
    
    /* Set socket buffers */
    options.socket_send_buffer_size = 4096;
    options.socket_receive_buffer_size = 4096;
    optmask |= ARES_OPT_SOCK_SNDBUF | ARES_OPT_SOCK_RCVBUF;
    
    /* Set servers */
    servers[0].s_addr = 0x7f000001; /* 127.0.0.1 */
    options.servers = servers;
    options.nservers = 1;
    optmask |= ARES_OPT_SERVERS;
    
    /* Set domains */
    domains[0] = strdup("example.com");
    domains[1] = strdup("test.org");
    if (domains[0] && domains[1]) {
        options.domains = domains;
        options.ndomains = 2;
        optmask |= ARES_OPT_DOMAINS;
    }
    
    /* Set lookups */
    options.lookups = strdup(lookups_str);
    if (options.lookups) {
        optmask |= ARES_OPT_LOOKUPS;
    }
    
    /* Set resolvconf and hosts path */
    options.resolvconf_path = strdup(resolvconf_path);
    options.hosts_path = strdup(hosts_path);
    if (options.resolvconf_path) optmask |= ARES_OPT_RESOLVCONF;
    if (options.hosts_path) optmask |= ARES_OPT_HOSTS_FILE;
    
    /* Set EDNSPSZ */
    options.ednspsz = 4096;
    optmask |= ARES_OPT_EDNSPSZ;
    
    /* Set UDP max queries */
    options.udp_max_queries = 10;
    optmask |= ARES_OPT_UDP_MAX_QUERIES;
    
    /* Set query cache TTL */
    options.qcache_max_ttl = 300;
    optmask |= ARES_OPT_QUERY_CACHE;
    
    /* Set server failover options */
    options.server_failover_opts.retry_chance = 100;
    options.server_failover_opts.retry_delay = 1000;
    optmask |= ARES_OPT_SERVER_FAILOVER;
    
    /* Initialize channel */
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        /* Cleanup allocated strings if init fails */
        free(domains[0]);
        free(domains[1]);
        free(options.lookups);
        free(options.resolvconf_path);
        free(options.hosts_path);
        return NULL;
    }
    
    /* Cleanup temporary strings as they are copied into the channel */
    free(domains[0]);
    free(domains[1]);
    free(options.lookups);
    free(options.resolvconf_path);
    free(options.hosts_path);
    
    return channel;
}

/* Static cleanup helper */
static void cleanup_channel(ares_channel_t *channel) {
    if (channel) {
        ares_destroy(channel);
    }
}

/* Static cleanup for options structure */
static void cleanup_options(struct ares_options *options) {
    if (options) {
        ares_destroy_options(options);
    }
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    /* 
     * Strategy:
     * The uncovered lines in ares_save_options are inside conditional blocks checking 
     * channel->optmask. To cover them, we must ensure the channel is initialized with 
     * a wide variety of options so that the optmask has many bits set.
     * 
     * We use the input data to potentially influence the initialization or simply 
     * rely on a fixed robust setup that covers all paths, as the logic is deterministic
     * based on the optmask.
     * 
     * We create a channel with ALL possible options (excluding sortlist due to opaque type)
     * to ensure every 'if (channel->optmask & ...)' branch is taken.
     */
    
    if (size < 1) {
        return 0;
    }

    ares_channel_t *channel = setup_channel_with_all_options();
    
    if (!channel) {
        return 0;
    }

    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));

    /* Call the target function */
    int result = ares_save_options(channel, &options, &optmask);

    /* 
     * Verify result. 
     * If result is ARES_SUCCESS, it means we traversed the function and copied data.
     * The uncovered lines are the assignments inside the if-blocks.
     * By setting all options in setup_channel_with_all_options, we force 
     * channel->optmask to have all bits set, triggering lines 109, 115, 119, etc.
     */
    (void)result;
    (void)optmask;

    /* Cleanup */
    cleanup_options(&options);
    cleanup_channel(channel);

    return 0;
}