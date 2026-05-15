#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <sstream>

namespace harness_009 {

#ifndef PUGIXML_NO_STL
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content, XPath expressions, and save parameters
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: XML DOCUMENT LOADING ====================
    // TARGET: xml_document::load_string (Helper API for initialization)
    pugi::xml_document doc;
    
    // Consume XML content from fuzzer input
    std::string xml_content = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(64, 1024));
    
    // Parse XML using load_string
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str());
    
    // If parsing failed, create a simple valid XML document
    if (!parse_result) {
        // Create a minimal valid XML structure
        std::string simple_xml = "<?xml version=\"1.0\"?><root>";
        simple_xml += fdp.ConsumeRandomLengthString(128);
        simple_xml += "</root>";
        parse_result = doc.load_string(simple_xml.c_str());
    }
    
    // Get document root for context nodes
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // No valid root, return early
        return 0;
    }
    
    // Collect various context nodes for XPath evaluation
    std::vector<pugi::xpath_node> context_nodes;
    context_nodes.push_back(pugi::xpath_node(root));  // Root node
    context_nodes.push_back(pugi::xpath_node());       // Empty node
    
    // Try to get child nodes for additional contexts
    for (pugi::xml_node child = root.first_child(); child && context_nodes.size() < 5; child = child.next_sibling()) {
        context_nodes.push_back(pugi::xpath_node(child));
        // Also try to get an attribute if available
        for (pugi::xml_attribute attr = child.first_attribute(); attr && context_nodes.size() < 5; attr = attr.next_attribute()) {
            context_nodes.push_back(pugi::xpath_node(attr, child));
            break;
        }
        break; // Just get one child with its attribute for variety
    }

#ifndef PUGIXML_NO_XPATH
    try {
        // ==================== PHASE 2: XPath QUERY CONSTRUCTION ====================
        // TARGET: xpath_query constructor (Required setup)
        std::vector<std::string> xpath_expressions;
        
        // Generate a variety of XPath expressions targeting different evaluation types
        int expr_count = fdp.ConsumeIntegralInRange<int>(3, 10);
        
        for (int i = 0; i < expr_count && fdp.remaining_bytes() > 10; ++i) {
            // Consume expression length
            size_t expr_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
            std::string expr = fdp.ConsumeRandomLengthString(expr_len);
            if (!expr.empty()) {
                xpath_expressions.push_back(expr);
            }
        }
        
        // Add some specific expression patterns to target different evaluation types
        // Boolean expressions
        xpath_expressions.push_back("true()");
        xpath_expressions.push_back("false()");
        xpath_expressions.push_back("1 = 1");
        xpath_expressions.push_back("1 != 2");
        xpath_expressions.push_back("count(//*) > 0");
        
        // Numeric expressions
        xpath_expressions.push_back("1");
        xpath_expressions.push_back("1 + 2");
        xpath_expressions.push_back("count(//*)");
        xpath_expressions.push_back("position()");
        xpath_expressions.push_back("last()");
        
        // String expressions
        xpath_expressions.push_back("\"test\"");
        xpath_expressions.push_back("name()");
        xpath_expressions.push_back("string(1)");
        xpath_expressions.push_back("concat(\"a\", \"b\")");
        xpath_expressions.push_back("substring(\"hello\", 1, 3)");
        
        // Node-set expressions
        xpath_expressions.push_back("//*");
        xpath_expressions.push_back("child::*");
        xpath_expressions.push_back("//*[@*]");
        xpath_expressions.push_back("//*[text()]");
        xpath_expressions.push_back("@*");
        
        // ==================== PHASE 3: XPath QUERY EVALUATION ====================
        // Primary target: XPath evaluation APIs with 30-32 undiscovered branches each
        
        for (const auto& expr : xpath_expressions) {
            if (expr.empty() || fdp.remaining_bytes() < 10) {
                continue;
            }
            
            try {
                // Create XPath query object
                pugi::xpath_query query(expr.c_str());
                
                if (!query) {
                    // Query compilation failed, skip this expression
                    continue;
                }
                
                // Test evaluation against different context nodes
                for (const auto& context : context_nodes) {
                    if (fdp.remaining_bytes() < 5) {
                        break;
                    }
                    
                    // TARGET: xpath_query::evaluate_boolean (30 undiscovered branches)
                    try {
                        bool bool_result = query.evaluate_boolean(context);
                        (void)bool_result;
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                    
                    // TARGET: xpath_query::evaluate_number (30 undiscovered branches)
                    try {
                        double num_result = query.evaluate_number(context);
                        (void)num_result;
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                    
                    // TARGET: xpath_query::evaluate_string (string version) (30 undiscovered branches)
                    try {
#ifndef PUGIXML_NO_STL
                        std::string string_result = query.evaluate_string(context);
                        (void)string_result;
#endif
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                    
                    // TARGET: xpath_query::evaluate_string (buffer version) (32 undiscovered branches)
                    // This is the buffer version mentioned in coverage guidance
                    try {
                        char buffer[1024];
                        size_t result_len = query.evaluate_string(buffer, sizeof(buffer), context);
                        (void)result_len;
                        (void)buffer;
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                    
                    // TARGET: xpath_query::evaluate_node (30 undiscovered branches)
                    try {
                        pugi::xpath_node node_result = query.evaluate_node(context);
                        (void)node_result;
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                    
                    // TARGET: xpath_query::evaluate_node_set (30 undiscovered branches)
                    try {
                        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
                        (void)node_set_result;
                        
                        // Process node set if not empty
                        if (!node_set_result.empty()) {
                            // Test node set operations
                            size_t set_size = node_set_result.size();
                            pugi::xpath_node_set::const_iterator it = node_set_result.begin();
                            pugi::xpath_node_set::const_iterator end = node_set_result.end();
                            
                            // Access a few nodes from the set
                            for (size_t i = 0; i < std::min(set_size, size_t(3)); ++i) {
                                pugi::xpath_node xnode = node_set_result[i];
                                (void)xnode;
                            }
                        }
                    } catch (...) {
                        // Ignore exceptions during evaluation
                    }
                }
                
            } catch (...) {
                // Ignore exceptions during query construction or evaluation
                continue;
            }
        }
        
        // ==================== PHASE 4: DOCUMENT MODIFICATION ====================
        // Modify document to create interesting content for saving
        // Add some nodes/attributes based on fuzzer input
        int mod_count = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < mod_count && fdp.remaining_bytes() > 20; ++i) {
            std::string node_name = "node_" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(8);
            std::string node_value = fdp.ConsumeRandomLengthString(32);
            
            pugi::xml_node new_node = root.append_child(node_name.c_str());
            new_node.append_child(pugi::node_pcdata).set_value(node_value.c_str());
            
            // Add some attributes
            std::string attr_name = "attr_" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(6);
            std::string attr_value = fdp.ConsumeRandomLengthString(24);
            new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
        }
        
    } catch (...) {
        // Catch any unhandled exceptions in XPath processing
        // Continue to document save operations
    }
#endif // PUGIXML_NO_XPATH
    
    // ==================== PHASE 5: DOCUMENT SAVE OPERATIONS ====================
    // Secondary target: Document save APIs with 12-18 undiscovered branches each
    
    // Create temporary files for save_file testing
    char temp_filename[] = "/tmp/pugixml_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd != -1) {
        close(temp_fd);
        
        // TARGET: xml_document::save_file (char* version) (18 undiscovered branches)
        // Test with different formatting options from fuzzer input
        unsigned int save_flags = fdp.ConsumeIntegral<unsigned int>();
        int encoding_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
        pugi::xml_encoding encoding = pugi::encoding_auto;
        
        switch (encoding_choice) {
            case 0: encoding = pugi::encoding_auto; break;
            case 1: encoding = pugi::encoding_utf8; break;
            case 2: encoding = pugi::encoding_utf16_le; break;
            case 3: encoding = pugi::encoding_utf16_be; break;
            case 4: encoding = pugi::encoding_utf32; break;
        }
        
        // Consume indentation string
        std::string indent_str;
        int indent_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        switch (indent_type) {
            case 0: indent_str = "\t"; break;
            case 1: indent_str = "  "; break;
            case 2: indent_str = "    "; break;
            case 3: indent_str = ""; break;
        }
        
        // Call save_file with consumed parameters
        bool save_success = doc.save_file(temp_filename, indent_str.c_str(), save_flags, encoding);
        (void)save_success;
        
        // Test with different flag combinations
        bool save_success2 = doc.save_file(temp_filename, "\t", pugi::format_default);
        (void)save_success2;
        
        bool save_success3 = doc.save_file(temp_filename, "  ", pugi::format_indent | pugi::format_write_bom);
        (void)save_success3;
        
        bool save_success4 = doc.save_file(temp_filename, "", pugi::format_raw);
        (void)save_success4;
        
        // Clean up temporary file
        unlink(temp_filename);
    }
    
#ifndef PUGIXML_NO_STL
    // TARGET: xml_document::save (ostream version) (12 undiscovered branches)
    // Test saving to string stream
    try {
        std::stringstream ss;
        
        // Consume parameters for stream save
        unsigned int stream_flags = fdp.ConsumeIntegral<unsigned int>();
        std::string stream_indent;
        int stream_indent_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        switch (stream_indent_type) {
            case 0: stream_indent = "\t"; break;
            case 1: stream_indent = "  "; break;
            case 2: stream_indent = "    "; break;
            case 3: stream_indent = ""; break;
        }
        
        pugi::xml_encoding stream_encoding = pugi::encoding_auto;
        int stream_encoding_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
        switch (stream_encoding_choice) {
            case 0: stream_encoding = pugi::encoding_auto; break;
            case 1: stream_encoding = pugi::encoding_utf8; break;
            case 2: stream_encoding = pugi::encoding_utf16_le; break;
            case 3: stream_encoding = pugi::encoding_utf16_be; break;
            case 4: stream_encoding = pugi::encoding_utf32; break;
        }
        
        // Call save with ostream
        doc.save(ss, stream_indent.c_str(), stream_flags, stream_encoding);
        
        // Also test with different flag combinations
        std::stringstream ss2;
        doc.save(ss2, "\t", pugi::format_default);
        
        std::stringstream ss3;
        doc.save(ss3, "  ", pugi::format_indent);
        
        std::stringstream ss4;
        doc.save(ss4, "", pugi::format_raw);
        
    } catch (...) {
        // Ignore exceptions during stream save
    }
#endif // PUGIXML_NO_STL
    
    // ==================== PHASE 6: CLEANUP ====================
    // TARGET: xml_document::reset (Required cleanup)
    doc.reset();
    
    return 0;
}

} // namespace harness_009
