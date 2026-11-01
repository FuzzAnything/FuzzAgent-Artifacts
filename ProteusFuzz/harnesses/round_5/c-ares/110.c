#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <ares.h>

/* Helper to create a simple domain list */
static char* create_domain_list(int count) {
    if (count <= 0) return NULL;
    char **domains = (char **)malloc(count * sizeof(char *));
    if (!domains) return NULL;
    for (int i = 0; i < count; i++) {
        domains[i] = strdup("example.com");
        if (!domains[i]) {
            for (int j = 0; j < i; j++) {
                free(domains[j]);
            }
            free(domains);
            return NULL;
        }
    }
    return (char *)domains;
}

/* Helper to create servers */
static struct in_addr* create_servers(int count) {
    if (count <= 0) return NULL;
    struct in_addr *servers = (struct in_addr *)malloc(count * sizeof(struct in_addr));
    if (!servers) return NULL;
    for (int i = 0; i < count; i++) {
        servers[i].s_addr = 0x7f000001; /* 127.0.0.1 */
    }
    return servers;
}

static void cleanup_options(struct ares_options *opts) {
    if (opts->servers) {
        free(opts->servers);
        opts->servers = NULL;
    }
    if (opts->domains) {
        for (int i = 0; i < opts->ndomains; i++) {
            free(opts->domains[i]);
        }
        free(opts->domains);
        opts->domains = NULL;
    }
    if (opts->sortlist) {
        free(opts->sortlist);
        opts->sortlist = NULL;
    }
    if (opts->lookups) {
        free(opts->lookups);
        opts->lookups = NULL;
    }
    if (opts->resolvconf_path) {
        free(opts->resolvconf_path);
        opts->resolvconf_path = NULL;
    }
    if (opts->hosts_path) {
        free(opts->hosts_path);
        opts->hosts_path = NULL;
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    ares_channel_t *channel = NULL;
    struct ares_options opts;
    int optmask = 0;
    int rc;

    memset(&opts, 0, sizeof(opts));

    /* Use input data to determine options */
    uint32_t flags = (uint32_t)data[0];
    int timeout = (int)data[1];
    int tries = (int)data[2];
    int ndots = (int)data[3];
    int maxtimeout = (int)data[4];
    uint16_t udp_port = (uint16_t)data[5] | ((uint16_t)data[6] << 8);
    uint16_t tcp_port = (uint16_t)data[7] | ((uint16_t)data[8] << 8);
    int sock_send_buf = (int)data[9];
    int sock_recv_buf = (int)data[10];
    int ednspsz = (int)data[11];
    int udp_max_queries = (int)data[12];
    int qcache_max_ttl = (int)data[13];
    int server_retry_chance = (int)data[14];
    int server_retry_delay = (int)data[15];

    /* Set flags based on input to trigger various branches */
    if (data[0] & 0x01) { optmask |= ARES_OPT_FLAGS; opts.flags = flags; }
    if (data[0] & 0x02) { optmask |= ARES_OPT_TIMEOUTMS; opts.timeout = timeout; }
    if (data[0] & 0x04) { optmask |= ARES_OPT_TRIES; opts.tries = tries; }
    if (data[0] & 0x08) { optmask |= ARES_OPT_NDOTS; opts.ndots = ndots; }
    if (data[0] & 0x10) { optmask |= ARES_OPT_MAXTIMEOUTMS; opts.maxtimeout = maxtimeout; }
    if (data[0] & 0x20) { optmask |= ARES_OPT_UDP_PORT; opts.udp_port = udp_port; }
    if (data[0] & 0x40) { optmask |= ARES_OPT_TCP_PORT; opts.tcp_port = tcp_port; }
    
    /* Buffer sizes */
    if (data[0] & 0x80) { 
        optmask |= ARES_OPT_SOCK_SNDBUF; 
        opts.socket_send_buffer_size = sock_send_buf; 
    }
    if (data[1] & 0x01) { 
        optmask |= ARES_OPT_SOCK_RCVBUF; 
        opts.socket_receive_buffer_size = sock_recv_buf; 
    }

    /* Servers */
    int nservers = (data[2] % 5) + 1;
    opts.servers = create_servers(nservers);
    opts.nservers = nservers;
    if (opts.servers) {
        optmask |= ARES_OPT_SERVERS;
    }

    /* Domains */
    int ndomains = (data[3] % 5) + 1;
    opts.domains = (char **)create_domain_list(ndomains);
    opts.ndomains = ndomains;
    if (opts.domains) {
        optmask |= ARES_OPT_DOMAINS;
    }

    /* Lookups */
    if (data[4] & 0x01) {
        opts.lookups = strdup("files,system");
        if (opts.lookups) {
            optmask |= ARES_OPT_LOOKUPS;
        }
    }

    /* Sortlist: REMOVED because struct apattern is opaque/incomplete in public API.
       We cannot safely allocate or access members of struct apattern. */

    /* Resolvconf and Hosts */
    if (data[6] & 0x01) {
        opts.resolvconf_path = strdup("./dummy_resolv.conf");
        if (opts.resolvconf_path) {
            optmask |= ARES_OPT_RESOLVCONF;
        }
    }
    if (data[7] & 0x01) {
        opts.hosts_path = strdup("./dummy_hosts");
        if (opts.hosts_path) {
            optmask |= ARES_OPT_HOSTS_FILE;
        }
    }

    /* Other options */
    if (data[8] & 0x01) { optmask |= ARES_OPT_EDNSPSZ; opts.ednspsz = ednspsz; }
    if (data[9] & 0x01) { optmask |= ARES_OPT_UDP_MAX_QUERIES; opts.udp_max_queries = udp_max_queries; }
    if (data[10] & 0x01) { optmask |= ARES_OPT_QUERY_CACHE; opts.qcache_max_ttl = qcache_max_ttl; }
    if (data[11] & 0x01) { optmask |= ARES_OPT_SERVER_FAILOVER; 
        opts.server_failover_opts.retry_chance = server_retry_chance;
        opts.server_failover_opts.retry_delay = server_retry_delay;
    }

    /* Initialize channel */
    rc = ares_init_options(&channel, &opts, optmask);
    if (rc != ARES_SUCCESS || channel == NULL) {
        cleanup_options(&opts);
        return 0;
    }

    /* Save options - this is the target function */
    struct ares_options saved_opts;
    int saved_optmask = 0;
    memset(&saved_opts, 0, sizeof(saved_opts));
    
    int save_rc = ares_save_options(channel, &saved_opts, &saved_optmask);
    
    /* Cleanup saved options */
    cleanup_options(&saved_opts);

    /* Cleanup channel */
    ares_destroy(channel);

    /* Cleanup initial options */
    cleanup_options(&opts);

    return 0;
}