#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_006 {
/* Comprehensive fuzzing harness for c-ares library targeting platform-specific,
 * Android, system configuration, metrics, and search functions identified as
 * uncovered areas by CoverageAnalyzerAgent:
 * 
 * - Android-specific APIs (ares_android.c): ares_library_init_android, ares_get_android_server_list
 * - System configuration functions (ares_sysconfig.c, ares_sysconfig_mac.c, ares_sysconfig_win.c)
 * - Metrics functions (ares_metrics.c) - internal metrics collection
 * - Search functions (ares_search.c): ares_search_dnsrec
 *
 * This harness implements platform-specific configuration testing, Android API
 * integration (when compiled for Android), comprehensive search functionality,
 * and internal metrics collection through channel operations with semantic
 * diversity from previous harnesses:
 * - harness_000: getaddrinfo and modern APIs
 * - harness_001: DNS processing pipeline
 * - harness_002: event processing and channel management
 * - harness_003: server configuration APIs
 * - harness_004: legacy DNS reply parsing
 * - harness_005: legacy DNS mapping functions and query creation APIs
 *
 * Focus: Platform-specific system configuration, Android integration, DNS search
 * domain resolution, internal metrics collection, testing conditional compilation
 * paths for different platforms.
 */


/* Forward declaration for Android functions if not available in header */
#if defined(ANDROID) || defined(__ANDROID__)
/* These functions are declared in ares.h when Android is defined */
#else
/* Provide dummy declarations for non-Android builds to avoid compilation errors */
typedef void* jobject;
typedef void* JavaVM;
extern "C" {
    void ares_library_init_jvm(JavaVM *jvm);
    int ares_library_init_android(jobject connectivity_manager);
    int ares_library_android_initialized(void);
    char **ares_get_android_server_list(size_t max_servers, size_t *num_servers);
    char *ares_get_android_search_domains_list(void);
}
#endif

/* Callback for ares_search_dnsrec operations */
static void search_dnsrec_callback(void *arg, ares_status_t status, size_t timeouts,
                                   const ares_dns_record_t *dnsrec) {
    /* Minimal callback - just satisfy API requirement */
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

/* Helper function to test system configuration through ares_init_options */
static void test_system_configuration(FuzzedDataProvider &fdp) {
    /* Test various system configuration options */
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    /* Configure timeout options */
    if (fdp.ConsumeBool()) {
        options.timeout = fdp.ConsumeIntegralInRange<int>(100, 5000);
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    /* Configure tries options */
    if (fdp.ConsumeBool()) {
        options.tries = fdp.ConsumeIntegralInRange<int>(1, 10);
        optmask |= ARES_OPT_TRIES;
    }
    
    /* Configure EDNS payload size */
    if (fdp.ConsumeBool()) {
        options.ednspsz = fdp.ConsumeIntegralInRange<int>(512, 4096);
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    /* Configure lookups order */
    if (fdp.ConsumeBool()) {
        const char *lookups_str = "bf";
        if (fdp.ConsumeBool()) {
            lookups_str = "fb";
        }
        options.lookups = const_cast<char*>(lookups_str);
        optmask |= ARES_OPT_LOOKUPS;
    }
    
    /* Test ares_init_options with various configuration combinations */
    int result = ares_init_options(&channel, &options, optmask);
    
    if (result == ARES_SUCCESS && channel != NULL) {
        /* Test additional configuration APIs */
        
        /* Test local IP binding configuration */
        if (fdp.ConsumeBool()) {
            unsigned int local_ip = fdp.ConsumeIntegral<unsigned int>();
            ares_set_local_ip4(channel, local_ip);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned char local_ip6[16];
            for (int i = 0; i < 16; i++) {
                local_ip6[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            ares_set_local_ip6(channel, local_ip6);
        }
        
        if (fdp.ConsumeBool()) {
            std::string dev_name = fdp.ConsumeRandomLengthString(64);
            ares_set_local_dev(channel, dev_name.c_str());
        }
        
        /* Test sortlist configuration */
        if (fdp.ConsumeBool()) {
            std::string sortstr = fdp.ConsumeRandomLengthString(128);
            ares_set_sortlist(channel, sortstr.c_str());
        }
        
        /* Clean up channel */
        ares_destroy(channel);
    }
}

/* Helper function to test Android-specific APIs */
static void test_android_apis(FuzzedDataProvider &fdp) {
#if defined(ANDROID) || defined(__ANDROID__)
    /* Test Android initialization APIs */
    
    /* Note: In real Android environment, we would need actual JVM and ConnectivityManager
     * objects. For fuzzing, we test the APIs with NULL/placeholder values to exercise
     * error paths and initialization logic */
    
    /* Test ares_library_android_initialized before initialization */
    int initialized = ares_library_android_initialized();
    (void)initialized; /* Result may be ARES_ENOTINITIALIZED */
    
    /* Test ares_library_init_android with NULL (error path) */
    int init_result = ares_library_init_android(NULL);
    (void)init_result; /* Result may be ARES_ENOTINITIALIZED */
    
    /* Test server list retrieval with various parameters */
    size_t num_servers = 0;
    size_t max_servers = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    char **servers = ares_get_android_server_list(max_servers, &num_servers);
    if (servers != NULL) {
        /* Free the returned server list */
        for (size_t i = 0; i < num_servers && servers[i] != NULL; i++) {
            free(servers[i]);
        }
        free(servers);
    }
    
    /* Test search domains list retrieval */
    char *search_domains = ares_get_android_search_domains_list();
    if (search_domains != NULL) {
        free(search_domains);
    }
#endif
}

/* Helper function to test search functionality */
static void test_search_functionality(FuzzedDataProvider &fdp, ares_channel_t *channel) {
    if (channel == NULL) {
        return;
    }
    
    /* Create a DNS record for search testing */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  0, /* id */
                                                  ARES_FLAG_QR,
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return;
    }
    
    /* Configure query section */
    std::string search_name = fdp.ConsumeRandomLengthString(256);
    
    /* Consume DNS class and type values, but validate they are within valid enum ranges */
    uint16_t dns_class_raw = fdp.ConsumeIntegral<uint16_t>();
    uint16_t dns_type_raw = fdp.ConsumeIntegral<uint16_t>();
    
    /* Define valid DNS class values according to ares_dns_class_t enum */
    static const uint16_t valid_dns_classes[] = {
        ARES_CLASS_IN,     /* 1 */
        ARES_CLASS_CHAOS,  /* 3 */
        ARES_CLASS_HESIOD, /* 4 */
        ARES_CLASS_HESOID, /* 4 - alias */
        ARES_CLASS_NONE,   /* 254 */
        ARES_CLASS_ANY     /* 255 */
    };
    
    /* Define valid DNS type values according to ares_dns_rec_type_t enum */
    static const uint16_t valid_dns_types[] = {
        ARES_REC_TYPE_A,       /* 1 */
        ARES_REC_TYPE_NS,      /* 2 */
        ARES_REC_TYPE_CNAME,   /* 5 */
        ARES_REC_TYPE_SOA,     /* 6 */
        ARES_REC_TYPE_PTR,     /* 12 */
        ARES_REC_TYPE_HINFO,   /* 13 */
        ARES_REC_TYPE_MX,      /* 15 */
        ARES_REC_TYPE_TXT,     /* 16 */
        ARES_REC_TYPE_SIG,     /* 24 */
        ARES_REC_TYPE_AAAA,    /* 28 */
        ARES_REC_TYPE_SRV,     /* 33 */
        ARES_REC_TYPE_NAPTR,   /* 35 */
        ARES_REC_TYPE_OPT,     /* 41 */
        ARES_REC_TYPE_TLSA,    /* 52 */
        ARES_REC_TYPE_SVCB,    /* 64 */
        ARES_REC_TYPE_HTTPS,   /* 65 */
        ARES_REC_TYPE_ANY,     /* 255 */
        ARES_REC_TYPE_URI,     /* 256 */
        ARES_REC_TYPE_CAA      /* 257 */
        /* Note: ARES_REC_TYPE_RAW_RR = 65536 is too large for uint16_t */
    };
    
    /* Helper function to check if value is in array */
    auto is_in_array = [](uint16_t value, const uint16_t* arr, size_t size) -> bool {
        for (size_t i = 0; i < size; i++) {
            if (arr[i] == value) {
                return true;
            }
        }
        return false;
    };
    
    /* Validate DNS class */
    bool valid_class = is_in_array(dns_class_raw, valid_dns_classes, sizeof(valid_dns_classes)/sizeof(valid_dns_classes[0]));
    
    /* Validate DNS type - also accept values in reasonable range for testing */
    bool valid_type = is_in_array(dns_type_raw, valid_dns_types, sizeof(valid_dns_types)/sizeof(valid_dns_types[0]));
    
    /* If either value is invalid, skip this test to avoid undefined behavior */
    if (!valid_class || !valid_type) {
        ares_dns_record_destroy(dnsrec);
        return;
    }
    
    /* Safe cast now that we've validated the values are valid enum values */
    ares_dns_class_t dns_class_enum = static_cast<ares_dns_class_t>(dns_class_raw);
    ares_dns_rec_type_t dns_type_enum = static_cast<ares_dns_rec_type_t>(dns_type_raw);
    
    status = ares_dns_record_query_add(dnsrec, search_name.c_str(), dns_type_enum, dns_class_enum);
    
    if (status == ARES_SUCCESS) {
        /* Test ares_search_dnsrec - searches with domain expansion */
        ares_search_dnsrec(channel, dnsrec, search_dnsrec_callback, NULL);
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
}
/* Helper function to test metrics-related operations */
static void test_metrics_operations(FuzzedDataProvider &fdp, ares_channel_t *channel) {
    if (channel == NULL) {
        return;
    }
    
    /* Metrics are internal to c-ares and not exposed through public API.
     * However, we can test operations that would trigger metrics collection:
     * - DNS queries with various timeouts and retries
     * - Server state changes
     * - Network configuration changes
     */
    
    /* Create a simple DNS query to trigger internal metrics */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  0, /* id */
                                                  ARES_FLAG_QR,
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return;
    }
    
    /* Configure query */
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    ares_dns_class_t dns_class = ARES_CLASS_IN;
    ares_dns_rec_type_t dns_type = fdp.ConsumeBool() ? ARES_REC_TYPE_A : ARES_REC_TYPE_AAAA;
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), dns_type, dns_class);
    
    if (status == ARES_SUCCESS) {
        /* Send query - this will trigger internal metrics collection */
        unsigned short query_id = 0;
        ares_send_dnsrec(channel, dnsrec, search_dnsrec_callback, NULL, &query_id);
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check */
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);
    
    /* Consume operation selection */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            /* Test system configuration and initialization */
            test_system_configuration(fdp);
            break;
        }
        
        case 1: {
            /* Test Android-specific APIs (if compiled for Android) */
            test_android_apis(fdp);
            
            /* Also test with a regular channel for other operations */
            ares_channel_t *channel = NULL;
            if (ares_init(&channel) == ARES_SUCCESS && channel != NULL) {
                test_search_functionality(fdp, channel);
                ares_destroy(channel);
            }
            break;
        }
        
        case 2: {
            /* Test search functionality with comprehensive configuration */
            ares_channel_t *channel = NULL;
            if (ares_init(&channel) == ARES_SUCCESS && channel != NULL) {
                test_search_functionality(fdp, channel);
                ares_destroy(channel);
            }
            break;
        }
        
        case 3: {
            /* Test metrics-related operations */
            ares_channel_t *channel = NULL;
            if (ares_init(&channel) == ARES_SUCCESS && channel != NULL) {
                test_metrics_operations(fdp, channel);
                
                /* Also test search to combine metrics with actual queries */
                test_search_functionality(fdp, channel);
                
                ares_destroy(channel);
            }
            break;
        }
    }
    
    /* Cleanup library */
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_006
