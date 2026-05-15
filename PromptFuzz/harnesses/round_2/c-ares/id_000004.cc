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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 487.5, nr_unique_branch: 354
//<Quality> {"density":13,"unique_branches":{"ares_server_find":[[595,9,595,50,0,0,4,1],[599,9,599,73,0,0,4,1],[603,9,603,74,0,0,4,1]],"ares_htable_asvp_get":[[181,7,181,10,0,0,4,0],[185,7,185,21,0,0,4,1],[190,7,190,21,0,0,4,1],[194,7,194,10,0,0,4,0]],"ares_buf_reclaim":[[123,7,123,23,0,0,4,1],[133,7,133,34,0,0,4,1]],"ares_destroy":[[74,10,74,22,0,0,4,0]],"ares_metrics_server_timeout":[[216,15,216,36,0,0,4,0],[216,15,216,36,0,0,4,1],[221,9,221,36,0,0,4,0],[224,11,224,48,0,0,4,0],[224,11,224,48,0,0,4,1],[225,11,225,70,0,0,4,0],[245,7,245,22,0,0,4,0],[250,7,250,34,0,0,4,1],[255,20,255,39,0,0,4,1],[256,7,256,34,0,0,4,1]],"ares_check_cleanup_conns":[[99,12,99,25,0,0,4,0],[106,11,106,48,0,0,4,0]],"ares_send_dnsrec":[[234,7,234,22,0,0,4,1]],"ares_socket_connect":[[377,9,377,15,0,0,4,1]],"ares_set_servers":[[1228,7,1228,22,0,0,4,1],[1233,7,1233,29,0,0,4,1]],"ares_send_nolock":[[221,7,221,29,0,0,4,0],[221,33,221,36,0,0,4,0],[221,33,221,36,0,0,4,1]],"ares_sockaddr_to_ares_addr":[[306,7,306,37,0,0,4,0],[316,9,316,13,0,0,4,1]],"invoke_server_state_cb":[[89,7,89,39,0,0,4,0]],"ares_conn_write":[[185,7,185,39,0,0,4,1],[192,7,192,47,0,0,4,1],[205,7,205,35,0,0,4,0],[205,7,205,35,0,0,4,1],[209,7,209,13,0,0,4,1],[218,7,218,35,0,0,4,0],[218,7,218,35,0,0,4,1],[218,39,218,54,0,0,4,0],[224,16,224,22,0,0,4,1],[225,14,225,45,0,0,4,1]],"timeadd":[[187,7,187,27,0,0,4,0]],"ares_conn_sock_state_cb_update":[[34,7,34,61,0,0,4,0],[34,7,34,61,0,0,4,1],[35,7,35,29,0,0,4,1]],"ares_parse_ptr_reply_dnsrec":[[54,7,54,29,0,0,4,1],[59,7,59,19,0,0,4,0],[59,7,59,19,0,0,4,1],[66,7,66,22,0,0,4,1],[74,7,74,35,0,0,4,1],[79,7,79,19,0,0,4,0],[79,23,79,34,0,0,4,0],[81,9,81,40,0,0,4,1],[92,7,92,33,0,0,4,1],[100,15,100,26,0,0,4,0],[100,15,100,26,0,0,4,1],[104,9,104,19,0,0,4,1],[110,9,110,51,0,0,4,0],[110,9,110,51,0,0,4,1],[115,9,115,56,0,0,4,0],[115,9,115,56,0,0,4,1],[117,11,117,26,0,0,4,1],[124,9,124,54,0,0,4,0],[124,9,124,54,0,0,4,1],[145,9,145,25,0,0,4,1],[152,9,152,45,0,0,4,1],[159,7,159,20,0,0,4,0],[159,7,159,20,0,0,4,1],[168,7,168,30,0,0,4,1],[174,7,174,29,0,0,4,0],[174,7,174,29,0,0,4,1],[177,9,177,32,0,0,4,1]],"ares_requeue_queries":[[39,10,39,71,0,0,4,0],[39,10,39,71,0,0,4,1]],"ares_timedout":[[168,7,168,15,0,0,4,1],[171,7,171,15,0,0,4,0]],"ares_slist_node_last":[[391,7,391,19,0,0,4,1]],"ares_conn_query_write":[[1237,7,1237,29,0,0,4,1],[1244,7,1244,29,0,0,4,1],[1250,7,1250,39,0,0,4,1],[1258,7,1258,39,0,0,4,1]],"ares_nameoffset_find":[[98,7,98,19,0,0,4,1],[98,23,98,35,0,0,4,1],[98,39,98,52,0,0,4,0]],"ares_llist_attach_at":[[90,11,90,21,0,0,4,0],[98,11,98,21,0,0,4,0]],"ares_queue_active_queries":[[289,7,289,22,0,0,4,1]],"ares_dns_rr_get_u8":[[796,7,796,56,0,0,4,1],[801,7,801,17,0,0,4,1]],"ares_process":[[355,7,355,18,0,0,4,0],[357,9,357,23,0,0,4,1],[362,15,362,30,0,0,4,0],[364,9,364,17,0,0,4,0],[370,9,370,18,0,0,4,0]],"bucket_free":[[67,7,67,28,0,0,4,1]],"ares_dns_write_rr_opt":[[617,7,617,15,0,0,4,1],[626,7,626,29,0,0,4,1],[632,7,632,29,0,0,4,1],[642,7,642,29,0,0,4,1],[648,7,648,29,0,0,4,1],[653,15,653,67,0,0,4,0],[653,15,653,67,0,0,4,1],[662,9,662,31,0,0,4,1],[668,9,668,31,0,0,4,1],[673,9,673,12,0,0,4,0],[673,16,673,23,0,0,4,0],[675,11,675,33,0,0,4,1]],"ares_llist_node_detach":[[279,7,279,17,0,0,4,0],[287,7,287,25,0,0,4,1]],"ares_dns_rr_set_opt":[[1411,7,1411,18,0,0,4,0],[1413,9,1413,21,0,0,4,1],[1421,7,1421,29,0,0,4,1]],"ares_query_nolock":[[72,7,72,22,0,0,4,1],[72,26,72,38,0,0,4,1],[72,42,72,58,0,0,4,1],[82,7,82,46,0,0,4,0],[88,5,88,46,0,0,4,0],[89,7,89,29,0,0,4,1],[95,7,95,21,0,0,4,1]],"ares_dns_write_buf_tcp":[[1152,7,1152,21,0,0,4,1],[1152,25,1152,36,0,0,4,1],[1160,7,1160,29,0,0,4,1],[1166,7,1166,29,0,0,4,1],[1172,7,1172,22,0,0,4,1],[1181,7,1181,29,0,0,4,1],[1187,7,1187,29,0,0,4,1]],"ares_htable_insert":[[349,7,349,35,0,0,4,1]],"ares_socket_deref_error":[[73,5,73,21,0,0,4,1],[79,5,79,22,0,0,4,0]],"ares_dns_rcode_tostr":[[912,11,912,16,0,0,4,1],[913,5,913,28,0,0,4,0],[915,5,915,28,0,0,4,1],[917,5,917,29,0,0,4,1],[919,5,919,29,0,0,4,1],[921,5,921,27,0,0,4,1],[923,5,923,28,0,0,4,1],[925,5,925,29,0,0,4,1],[927,5,927,28,0,0,4,1],[929,5,929,28,0,0,4,1],[931,5,931,28,0,0,4,1],[933,5,933,28,0,0,4,1],[935,5,935,29,0,0,4,1],[937,5,937,27,0,0,4,1],[939,5,939,27,0,0,4,1],[941,5,941,28,0,0,4,1],[943,5,943,28,0,0,4,1],[945,5,945,28,0,0,4,1],[947,5,947,27,0,0,4,1],[949,5,949,29,0,0,4,1],[951,5,951,30,0,0,4,1]],"ares_fetch_connection":[[1203,7,1203,19,0,0,4,1],[1209,7,1209,39,0,0,4,1],[1215,7,1215,40,0,0,4,0],[1215,7,1215,40,0,0,4,1],[1220,7,1220,35,0,0,4,1]],"ares_socket_write":[[145,7,145,14,0,0,4,0],[145,7,145,14,0,0,4,1]],"ares_close_connection":[[54,7,54,39,0,0,4,1]],"ares_addr_equal":[[266,7,266,37,0,0,4,1],[271,5,271,17,0,0,4,0],[272,11,273,49,0,0,4,0],[277,5,277,18,0,0,4,1],[287,5,287,12,0,0,4,1]],"handle_conn_error":[[958,7,958,23,0,0,4,0],[960,15,960,49,0,0,4,1]],"channel_socket_list":[[309,61,309,73,0,0,4,0],[315,11,315,38,0,0,4,1],[320,11,320,33,0,0,4,1]],"ares_buf_ensure_space":[[170,7,170,36,0,0,4,0]],"ares_process_fds_nolock":[[201,27,201,41,0,0,4,1]],"ares_dns_write_rr_be16":[[304,7,304,57,0,0,4,1]],"ares_requeue_query":[[980,7,980,26,0,0,4,0]],"ares_conn_set_self_ip":[[139,7,139,13,0,0,4,1],[145,7,145,47,0,0,4,1],[153,7,153,14,0,0,4,1],[164,7,165,74,0,0,4,1]],"key_eq":[[79,7,79,17,0,0,4,0]],"process_timeouts":[[704,10,704,77,0,0,4,0],[710,9,710,45,0,0,4,0]],"ares_servers_update":[[781,9,781,22,0,0,4,0],[786,11,786,41,0,0,4,1],[792,11,792,29,0,0,4,1],[826,7,826,19,0,0,4,1]],"ares_htable_asvp_insert":[[150,7,150,21,0,0,4,1],[155,7,155,21,0,0,4,1],[163,7,163,48,0,0,4,1]],"ares_dns_write_rr":[[944,15,944,58,0,0,4,0],[956,9,956,19,0,0,4,1],[962,9,962,23,0,0,4,1],[969,9,969,31,0,0,4,1],[975,9,975,31,0,0,4,1],[982,9,982,31,0,0,4,1],[988,9,988,40,0,0,4,1],[994,9,994,31,0,0,4,1],[1001,9,1001,31,0,0,4,1],[1006,13,1006,17,0,0,4,1],[1007,7,1007,27,0,0,4,1],[1010,7,1010,28,0,0,4,1],[1013,7,1013,31,0,0,4,1],[1016,7,1016,29,0,0,4,1],[1019,7,1019,29,0,0,4,1],[1022,7,1022,31,0,0,4,1],[1025,7,1025,28,0,0,4,1],[1028,7,1028,29,0,0,4,1],[1031,7,1031,29,0,0,4,1],[1034,7,1034,30,0,0,4,1],[1037,7,1037,29,0,0,4,1],[1040,7,1040,31,0,0,4,1],[1043,7,1043,29,0,0,4,1],[1046,7,1046,29,0,0,4,0],[1049,7,1049,30,0,0,4,1],[1052,7,1052,30,0,0,4,1],[1055,7,1055,31,0,0,4,1],[1058,7,1058,29,0,0,4,1],[1061,7,1061,29,0,0,4,1],[1064,7,1064,32,0,0,4,1],[1069,9,1069,31,0,0,4,1],[1079,9,1079,31,0,0,4,1],[1084,9,1084,31,0,0,4,1],[1089,9,1089,31,0,0,4,1]],"ares_probe_failed_server":[[1094,8,1094,27,0,0,4,0],[1094,31,1094,64,0,0,4,0]],"default_asendto":[[390,7,390,22,0,0,4,1]],"ares_get_servers_csv":[[1324,7,1324,18,0,0,4,1],[1328,56,1328,68,0,0,4,0],[1328,56,1328,68,0,0,4,1],[1333,9,1333,26,0,0,4,1],[1341,9,1341,31,0,0,4,1]],"ares_dns_record_create_query":[[1600,7,1600,23,0,0,4,0],[1604,9,1604,29,0,0,4,1],[1611,9,1611,31,0,0,4,1],[1617,9,1617,31,0,0,4,1],[1622,9,1622,31,0,0,4,1],[1627,9,1627,31,0,0,4,1]],"ares_dns_rr_get_opt":[[1023,7,1023,10,0,0,4,0],[1026,7,1026,14,0,0,4,0],[1030,7,1030,57,0,0,4,1],[1035,7,1035,19,0,0,4,1],[1035,23,1035,36,0,0,4,1],[1040,7,1040,18,0,0,4,1],[1044,7,1044,10,0,0,4,0],[1047,7,1047,14,0,0,4,0]],"ares_buf_append_be32":[[266,7,266,29,0,0,4,1],[271,7,271,29,0,0,4,1],[276,7,276,29,0,0,4,1],[281,7,281,29,0,0,4,1]],"ares_query_dnsrec_cb":[[45,7,45,29,0,0,4,0]],"ares_close_sockets":[[77,10,77,69,0,0,4,0]],"ares_query_timeout_cmp_cb":[[77,7,77,40,0,0,4,1],[80,7,80,40,0,0,4,0],[80,7,80,40,0,0,4,1],[84,7,84,42,0,0,4,1],[87,7,87,42,0,0,4,0]],"ares_conn_connect":[[334,42,334,70,0,0,4,1]],"ares_calc_query_timeout":[[1148,7,1148,23,0,0,4,1],[1155,7,1155,17,0,0,4,0],[1155,7,1155,17,0,0,4,1],[1159,7,1159,26,0,0,4,1],[1172,7,1172,17,0,0,4,0],[1172,7,1172,17,0,0,4,1],[1183,7,1183,25,0,0,4,1]],"ares_parse_ptr_reply":[[194,7,194,19,0,0,4,1],[201,7,201,29,0,0,4,0],[201,7,201,29,0,0,4,1],[209,7,209,30,0,0,4,0],[209,7,209,30,0,0,4,1]],"ares_slist_node_destroy":[[460,22,460,38,0,0,4,1]],"ares_open_connection":[[420,7,420,29,0,0,4,1],[424,7,424,30,0,0,4,1],[434,7,434,39,0,0,4,1],[440,7,440,29,0,0,4,1],[447,7,447,13,0,0,4,1],[452,7,452,19,0,0,4,1],[461,7,461,76,0,0,4,1],[471,7,471,39,0,0,4,1],[479,7,479,50,0,0,4,0],[483,7,483,13,0,0,4,1],[488,7,488,29,0,0,4,1]],"ares_dns_get_opt_rr":[[1540,15,1540,71,0,0,4,0],[1540,15,1540,71,0,0,4,1],[1543,9,1543,54,0,0,4,0]],"ares_metric_timestamp":[[119,11,119,17,0,0,4,1],[120,5,120,29,0,0,4,0],[120,5,120,29,0,0,4,1],[123,5,123,31,0,0,4,0],[123,5,123,31,0,0,4,1],[126,5,126,27,0,0,4,0],[126,5,126,27,0,0,4,1],[129,5,129,26,0,0,4,0],[129,5,129,26,0,0,4,1],[132,5,132,31,0,0,4,0],[132,5,132,31,0,0,4,1],[133,14,133,25,0,0,4,0],[133,14,133,25,0,0,4,1],[134,5,134,27,0,0,4,1],[138,7,138,18,0,0,4,0],[138,7,138,18,0,0,4,1],[139,9,139,28,0,0,4,1]],"ares_conn_flush":[[243,7,243,19,0,0,4,1],[247,7,247,47,0,0,4,1],[252,9,252,41,0,0,4,0],[252,9,252,41,0,0,4,1],[257,9,257,41,0,0,4,1],[265,11,265,33,0,0,4,1],[271,11,271,43,0,0,4,1],[280,9,280,37,0,0,4,0],[280,9,280,37,0,0,4,1],[281,11,281,42,0,0,4,0],[290,9,290,44,0,0,4,0],[300,12,300,47,0,0,4,0],[303,7,303,29,0,0,4,0],[303,7,303,29,0,0,4,1],[308,9,308,12,0,0,4,1],[314,9,314,41,0,0,4,1]],"ares_addr_node_to_sconfig_llist":[[846,7,846,16,0,0,4,1],[850,24,850,36,0,0,4,0],[850,24,850,36,0,0,4,1],[854,9,854,32,0,0,4,1],[859,9,859,24,0,0,4,1],[864,9,864,32,0,0,4,0],[872,9,872,51,0,0,4,1]],"ares_query":[[138,7,138,22,0,0,4,1],[143,7,143,19,0,0,4,1]],"ares_cookie_apply":[[305,7,305,17,0,0,4,0],[305,7,305,17,0,0,4,1],[310,7,310,39,0,0,4,1],[316,7,316,45,0,0,4,1],[324,7,324,47,0,0,4,1],[337,7,337,43,0,0,4,0],[337,7,337,43,0,0,4,1],[344,8,344,46,0,0,4,0],[346,7,346,59,0,0,4,1],[352,7,352,45,0,0,4,1],[361,7,361,25,0,0,4,1]],"ares_free_hostent":[[37,7,37,12,0,0,4,0]],"ares_query_dnsrec":[[123,7,123,22,0,0,4,1]],"ares_htable_asvp_remove":[[211,7,211,21,0,0,4,1]],"ares_send_query":[[1306,7,1306,19,0,0,4,1],[1310,7,1310,24,0,0,4,0],[1315,7,1315,29,0,0,4,1],[1331,5,1331,22,0,0,4,0],[1331,5,1331,22,0,0,4,1],[1334,5,1334,21,0,0,4,1],[1341,5,1341,27,0,0,4,0],[1341,5,1341,27,0,0,4,1],[1342,5,1342,25,0,0,4,1],[1345,11,1345,34,0,0,4,1],[1350,5,1350,12,0,0,4,1],[1366,7,1366,38,0,0,4,1],[1379,7,1379,42,0,0,4,1],[1391,7,1391,26,0,0,4,0],[1391,7,1391,26,0,0,4,1],[1395,7,1395,32,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers","ares_get_servers_csv","ares_free_string","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_queue_active_queries","ares_process","ares_parse_ptr_reply","ares_free_hostent","ares_dns_rcode_tostr","ares_query","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers","ares_get_servers_csv","ares_free_string","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_queue_active_queries","ares_process","ares_parse_ptr_reply","ares_free_hostent","ares_dns_rcode_tostr","ares_query","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set up a dummy server using the input data
    struct ares_addr_node *servers = nullptr;
    if (size >= sizeof(struct in_addr)) {
        servers = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (servers) {
            memset(servers, 0, sizeof(struct ares_addr_node));
            servers->family = AF_INET;
            // Use first 4 bytes of input as IP address
            memcpy(&servers->addr.addr4, data, sizeof(struct in_addr));
            servers->next = nullptr;
            
            // Set the server
            ares_set_servers(channel, servers);
        }
    }
    
    // Get servers in CSV format
    char *csv_servers = ares_get_servers_csv(channel);
    if (csv_servers) {
        // Just using the API - we don't actually need the result
        ares_free_string(csv_servers);
    }
    
    // Create a DNS record for sending
    ares_dns_record_t *dnsrec = nullptr;
    unsigned short id = 1234;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (ares_dns_record_create(&dnsrec, id, 0, opcode, rcode) == ARES_SUCCESS) {
        // Add a query to the DNS record
        const char *query_name = "example.com";
        ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
        ares_dns_class_t qclass = ARES_CLASS_IN;
        
        ares_dns_record_query_add(dnsrec, query_name, qtype, qclass);
        
        // Send the DNS record
        unsigned short qid = 0;
        ares_status_t send_status = ares_send_dnsrec(channel, dnsrec, 
            [](void *arg, ares_status_t status, size_t timeout_ms, const ares_dns_record_t *rec) {
                // Callback - just a placeholder since we're fuzzing
            }, 
            nullptr, &qid);
        
        // Check queue of active queries
        size_t active_queries = ares_queue_active_queries(channel);
        
        // Process any pending operations
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        ares_process(channel, &read_fds, &write_fds);
        
        // Parse PTR reply from input data
        struct hostent *host = nullptr;
        if (size > 0) {
            struct in_addr addr;
            memset(&addr, 0, sizeof(addr));
            
            // Parse the input as a PTR reply
            int parse_result = ares_parse_ptr_reply(data, (int)size, 
                                                    &addr, sizeof(addr), 
                                                    AF_INET, &host);
            
            if (host) {
                ares_free_hostent(host);
            }
        }
        
        // Convert rcode to string (just using the API)
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
        
        // Make a regular query
        ares_query(channel, "test.com", C_IN, T_A, 
            [](void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
                // Callback - just a placeholder
            }, 
            nullptr);
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up
    if (servers) {
        free(servers);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}