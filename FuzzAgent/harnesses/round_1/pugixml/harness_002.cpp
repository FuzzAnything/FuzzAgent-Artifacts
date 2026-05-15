#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <sstream>

namespace harness_002 {

#ifndef PUGIXML_NO_STL
#endif

// Custom xml_writer implementation that writes to a vector
class VectorWriter : public pugi::xml_writer {
private:
    std::vector<char>& buffer;
    
public:
    VectorWriter(std::vector<char>& buf) : buffer(buf) {}
    
    virtual void write(const void* data, size_t size) {
        const char* ptr = static_cast<const char*>(data);
        buffer.insert(buffer.end(), ptr, ptr + size);
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Parse XML from fuzzer input
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size));
    pugi::xml_parse_result result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, try with minimal parsing options
    if (!result) {
        result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_minimal);
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
#ifndef PUGIXML_NO_XPATH
    try {
        // Create xpath_variable_set with complex variables
        pugi::xpath_variable_set vars;
        
        // Add boolean variable
        bool bool_var = fdp.ConsumeBool();
        vars.add("bool_var", pugi::xpath_type_boolean);
        vars.set("bool_var", bool_var);
        
        // Add number variables
        double num_var1 = fdp.ConsumeFloatingPoint<double>();
        double num_var2 = fdp.ConsumeFloatingPoint<double>();
        vars.add("num_var1", pugi::xpath_type_number);
        vars.add("num_var2", pugi::xpath_type_number);
        vars.set("num_var1", num_var1);
        vars.set("num_var2", num_var2);
        
        // Add string variables
        std::string str_var1 = fdp.ConsumeRandomLengthString(100);
        std::string str_var2 = fdp.ConsumeRandomLengthString(100);
        vars.add("str_var1", pugi::xpath_type_string);
        vars.add("str_var2", pugi::xpath_type_string);
        vars.set("str_var1", str_var1.c_str());
        vars.set("str_var2", str_var2.c_str());
        
        // Create complex XPath queries
        std::vector<std::string> xpath_expressions;
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(200));
        xpath_expressions.push_back("//*[@id='" + fdp.ConsumeRandomLengthString(50) + "']");
        xpath_expressions.push_back("/root/" + fdp.ConsumeRandomLengthString(50) + "[position()<" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10)) + "]");
        xpath_expressions.push_back("string(" + fdp.ConsumeRandomLengthString(100) + ")");
        xpath_expressions.push_back("number(" + fdp.ConsumeRandomLengthString(100) + ")");
        
        // TARGET: xpath_query::evaluate_string (59 undiscovered branches)
        for (const auto& expr : xpath_expressions) {
            if (expr.empty()) continue;
            
            pugi::xpath_query query(expr.c_str(), &vars);
            
            if (query) {
                // Evaluate as string - primary target
                std::string eval_result = query.evaluate_string(doc);
                
                // Also evaluate other types for completeness
                query.evaluate_boolean(doc);
                query.evaluate_number(doc);
                query.evaluate_node(doc);
                query.evaluate_node_set(doc);
                
                // TARGET: xml_node::select_nodes and select_single_node (51 branches each)
                pugi::xpath_node_set selected_nodes = root.select_nodes(expr.c_str(), &vars);
                pugi::xpath_node single_node = root.select_single_node(expr.c_str(), &vars);
                
                // Process selected nodes for manipulation
                for (pugi::xpath_node_set::const_iterator it = selected_nodes.begin(); it != selected_nodes.end(); ++it) {
                    pugi::xml_node node = it->node();
                    if (node) {
                        // TARGET: xml_node::insert_child_before/after (21-29 undiscovered branches)
                        // Create reference node for insertion operations
                        pugi::xml_node ref_child = node.first_child();
                        if (ref_child) {
                            // Insert before reference child
                            std::string insert_name_before = "inserted_before_" + fdp.ConsumeRandomLengthString(10);
                            pugi::xml_node new_before = node.insert_child_before(insert_name_before.c_str(), ref_child);
                            
                            // Insert after reference child  
                            std::string insert_name_after = "inserted_after_" + fdp.ConsumeRandomLengthString(10);
                            pugi::xml_node new_after = node.insert_child_after(insert_name_after.c_str(), ref_child);
                            
                            if (new_before) {
                                // TARGET: xml_text::set (19 undiscovered branches)
                                pugi::xml_text before_text = new_before.text();
                                before_text.set(fdp.ConsumeRandomLengthString(50).c_str());
                                
                                // Set with different data types
                                before_text.set(fdp.ConsumeIntegral<int>());
                                before_text.set(fdp.ConsumeFloatingPoint<double>());
                                before_text.set(fdp.ConsumeBool());
                            }
                            
                            if (new_after) {
                                pugi::xml_text after_text = new_after.text();
                                after_text.set(fdp.ConsumeRandomLengthString(50).c_str());
                            }
                        }
                        
                        // Also test append_child and prepend_child for comparison
                        std::string append_name = "appended_" + fdp.ConsumeRandomLengthString(10);
                        pugi::xml_node appended = node.append_child(append_name.c_str());
                        
                        std::string prepend_name = "prepended_" + fdp.ConsumeRandomLengthString(10);
                        pugi::xml_node prepended = node.prepend_child(prepend_name.c_str());
                    }
                }
            }
        }
        
        // TARGET: xml_document::save_file (18 undiscovered branches)
        // Create temporary file for saving
        char temp_filename[] = "/tmp/pugixml_fuzz_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd != -1) {
            close(fd);
            
            // Save with different formatting options
            bool save_success1 = doc.save_file(temp_filename, "\t", pugi::format_default);
            bool save_success2 = doc.save_file(temp_filename, "  ", pugi::format_indent | pugi::format_write_bom);
            bool save_success3 = doc.save_file(temp_filename, "", pugi::format_raw);
            
            // Also test save with wide character path if supported
            #ifdef _WIN32
            std::wstring wtemp_filename = L"/tmp/pugixml_fuzz_XXXXXX";
            _wmktemp(&wtemp_filename[0]);
            doc.save_file(wtemp_filename.c_str());
            #endif
            
            // Clean up temporary file
            remove(temp_filename);
        }
        
        // TARGET: xml_node::print (12 undiscovered branches)
        // Print nodes with different formatting using VectorWriter
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Print to string stream
            #ifndef PUGIXML_NO_STL
            std::stringstream ss;
            child.print(ss, "  ", pugi::format_default);
            std::string printed = ss.str();
            #endif
            
            // Print with VectorWriter
            std::vector<char> buffer1(4096);
            VectorWriter writer1(buffer1);
            child.print(writer1, "\t", pugi::format_indent);
            
            std::vector<char> buffer2(4096);
            VectorWriter writer2(buffer2);
            child.print(writer2, "", pugi::format_raw);
        }
        
        // Print entire document with VectorWriter
        std::vector<char> doc_buffer(8192);
        VectorWriter doc_writer(doc_buffer);
        doc.print(doc_writer, "  ", pugi::format_indent | pugi::format_no_declaration);
        
    } catch (...) {
        // Catch any exceptions from XPath evaluation
    }
#endif // PUGIXML_NO_XPATH
    
    // Reset document for cleanup
    doc.reset();
    
    return 0;
}

} // namespace harness_002
