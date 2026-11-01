#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ares.h>

/* 
 * Forward declaration for ares_addrinfo2hostent if missing from the included ares.h
 * This resolves the "call to undeclared function" error.
 */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Check if the function is already declared, if not, declare it */
#ifdef __cplusplus
extern "C" {
#endif

/* Attempt to declare the function if the header is missing it. 
   We use a weak check or just declare it. Since we can't easily check macro version here,
   we assume the symbol exists in the library but is missing from the header.
   The signature is: int ares_addrinfo2hostent(const struct ares_addrinfo *ai, int family, struct hostent **host);
*/
/* Note: In a real build, we might use #ifdef to check version, but for a fuzz driver fix,
   a direct declaration is often sufficient if the library supports it. */
/* To avoid redefinition errors if the header *does* have it, we rely on the fact that 
   if the header has it, the compiler is happy. If the header doesn't, we need it.
   Since the error occurred, the header definitely doesn't have it in this context.
   So adding it is safe. */

/* Forward declaration */
CARES_EXTERN int ares_addrinfo2hostent(const struct ares_addrinfo *ai, int family, struct hostent **host);

#ifdef __cplusplus
}
#endif

/* 
 * Helper to create a minimal ares_addrinfo structure.
 * We must manually manage the linked lists for nodes and cnames.
 * Note: Replaced internal ares_ memory functions with standard libc equivalents
 * to resolve compilation errors regarding undeclared functions.
 */

static void cleanup_addrinfo(struct ares_addrinfo *ai) {
    if (!ai) return;
    
    struct ares_addrinfo_node *next_node = ai->nodes;
    while (next_node) {
        struct ares_addrinfo_node *tmp = next_node;
        next_node = next_node->ai_next;
        free(tmp->ai_addr);
        free(tmp);
    }
    
    struct ares_addrinfo_cname *next_cname = ai->cnames;
    while (next_cname) {
        struct ares_addrinfo_cname *tmp = next_cname;
        next_cname = next_cname->next;
        free(tmp->name);
        free(tmp->alias);
        free(tmp);
    }
    
    free(ai->name);
    free(ai);
}

static int build_addrinfo_from_input(const uint8_t *data, size_t len, 
                                     struct ares_addrinfo **out_ai, int family_hint) {
    if (len < 2) return 0;
    
    int scenario = data[0];
    struct ares_addrinfo *ai = calloc(1, sizeof(*ai));
    if (!ai) return 0;
    
    ai->name = NULL;
    ai->cnames = NULL;
    ai->nodes = NULL;
    
    /* Scenario 0: NULL checks (handled by passing NULL directly in main logic) */
    if (scenario == 0) {
        *out_ai = ai;
        return 1;
    }
    
    /* Scenario 1: Invalid Family (handled by passing AF_UNIX directly) */
    if (scenario == 1) {
        *out_ai = ai;
        return 1;
    }
    
    /* Scenario 2: Empty data (triggers line 214) */
    if (scenario == 2) {
        ai->name = strdup("empty.test");
        if (!ai->name) { free(ai); return 0; }
        *out_ai = ai;
        return 1;
    }
    
    /* Scenario 3: AF_INET */
    if (scenario == 3) {
        ai->name = strdup("host.ipv4.test");
        if (!ai->name) { free(ai); return 0; }
        
        /* Create a node */
        struct ares_addrinfo_node *node = calloc(1, sizeof(*node));
        if (!node) { free(ai->name); free(ai); return 0; }
        
        node->ai_family = AF_INET;
        node->ai_addr = calloc(1, sizeof(struct sockaddr_in));
        if (!node->ai_addr) { free(node); free(ai->name); free(ai); return 0; }
        
        struct sockaddr_in *sin = (struct sockaddr_in *)node->ai_addr;
        sin->sin_family = AF_INET;
        /* Use input data for IP if available */
        if (len > 5) {
            sin->sin_addr.s_addr = htonl((data[1] << 24) | (data[2] << 16) | (data[3] << 8) | data[4]);
        } else {
            sin->sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
        }
        
        ai->nodes = node;
        
        /* Create an alias */
        struct ares_addrinfo_cname *cname = calloc(1, sizeof(*cname));
        if (cname) {
            cname->name = strdup("alias1");
            cname->alias = strdup("alias1");
            if (!cname->name || !cname->alias) {
                free(cname->name);
                free(cname->alias);
                free(cname);
                /* Cleanup node and ai on partial failure */
                free(node->ai_addr);
                free(node);
                free(ai->name);
                free(ai);
                return 0;
            }
            ai->cnames = cname;
        }
        
        *out_ai = ai;
        return 1;
    }
    
    /* Scenario 4: AF_INET6 */
    if (scenario == 4) {
        ai->name = strdup("host.ipv6.test");
        if (!ai->name) { free(ai); return 0; }
        
        struct ares_addrinfo_node *node = calloc(1, sizeof(*node));
        if (!node) { free(ai->name); free(ai); return 0; }
        
        node->ai_family = AF_INET6;
        node->ai_addr = calloc(1, sizeof(struct sockaddr_in6));
        if (!node->ai_addr) { free(node); free(ai->name); free(ai); return 0; }
        
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)node->ai_addr;
        sin6->sin6_family = AF_INET6;
        /* Use input data for IPv6 */
        if (len > 18) {
            memcpy(&sin6->sin6_addr, data + 1, 16);
        } else {
            sin6->sin6_addr.s6_addr[15] = 1; /* ::1 */
        }
        
        ai->nodes = node;
        
        /* Create an alias */
        struct ares_addrinfo_cname *cname = calloc(1, sizeof(*cname));
        if (cname) {
            cname->name = strdup("alias6");
            cname->alias = strdup("alias6");
            if (!cname->name || !cname->alias) {
                free(cname->name);
                free(cname->alias);
                free(cname);
                free(node->ai_addr);
                free(node);
                free(ai->name);
                free(ai);
                return 0;
            }
            ai->cnames = cname;
        }
        
        *out_ai = ai;
        return 1;
    }
    
    /* Scenario 5: AF_UNSPEC logic (lines 114-117) */
    if (scenario == 5) {
        ai->name = strdup("host.unspec.test");
        if (!ai->name) { free(ai); return 0; }
        
        /* Create a node with AF_INET to trigger line 117-118 */
        struct ares_addrinfo_node *node = calloc(1, sizeof(*node));
        if (!node) { free(ai->name); free(ai); return 0; }
        node->ai_family = AF_INET;
        node->ai_addr = calloc(1, sizeof(struct sockaddr_in));
        if (!node->ai_addr) { free(node); free(ai->name); free(ai); return 0; }
        ((struct sockaddr_in *)node->ai_addr)->sin_family = AF_INET;
        ai->nodes = node;
        
        *out_ai = ai;
        return 1;
    }
    
    /* Default fallback */
    *out_ai = ai;
    return 1;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
    struct ares_addrinfo *ai = NULL;
    struct hostent *host = NULL;
    int family = AF_INET;
    int scenario = 0;
    
    if (size > 0) {
        scenario = data[0];
    }
    
    /* Test Case 0: NULL ai or NULL host (Line 106) */
    if (scenario == 0) {
        ares_addrinfo2hostent(NULL, AF_INET, &host);
        ares_addrinfo2hostent(NULL, AF_INET, NULL);
        if (host) ares_free_hostent(host);
        return 0;
    }
    
    /* Test Case 1: Invalid Family (Line 122) */
    if (scenario == 1) {
        if (build_addrinfo_from_input(data, size, &ai, AF_INET)) {
            ares_addrinfo2hostent(ai, AF_UNIX, &host);
            cleanup_addrinfo(ai);
            if (host) ares_free_hostent(host);
        }
        return 0;
    }
    
    /* Test Case 2: Empty data (Line 214) */
    if (scenario == 2) {
        if (build_addrinfo_from_input(data, size, &ai, AF_INET)) {
            ares_addrinfo2hostent(ai, AF_INET, &host);
            cleanup_addrinfo(ai);
            if (host) ares_free_hostent(host);
        }
        return 0;
    }
    
    /* Test Case 3: AF_INET full path */
    if (scenario == 3) {
        family = AF_INET;
        if (build_addrinfo_from_input(data, size, &ai, AF_INET)) {
            ares_addrinfo2hostent(ai, family, &host);
            cleanup_addrinfo(ai);
            if (host) ares_free_hostent(host);
        }
        return 0;
    }
    
    /* Test Case 4: AF_INET6 full path */
    if (scenario == 4) {
        family = AF_INET6;
        if (build_addrinfo_from_input(data, size, &ai, AF_INET6)) {
            ares_addrinfo2hostent(ai, family, &host);
            cleanup_addrinfo(ai);
            if (host) ares_free_hostent(host);
        }
        return 0;
    }
    
    /* Test Case 5: AF_UNSPEC logic (Lines 114-117) */
    if (scenario == 5) {
        family = AF_UNSPEC;
        if (build_addrinfo_from_input(data, size, &ai, AF_UNSPEC)) {
            ares_addrinfo2hostent(ai, family, &host);
            cleanup_addrinfo(ai);
            if (host) ares_free_hostent(host);
        }
        return 0;
    }
    
    /* Fallback: Try to build and run with default family */
    if (build_addrinfo_from_input(data, size, &ai, AF_INET)) {
        ares_addrinfo2hostent(ai, AF_INET, &host);
        cleanup_addrinfo(ai);
        if (host) ares_free_hostent(host);
    }
    
    return 0;
}