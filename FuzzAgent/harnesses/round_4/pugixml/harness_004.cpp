#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_004 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful XPath evaluation testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t evaluation_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    uint8_t var_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    bool use_variables = fdp.ConsumeBool();
    bool test_buffer_version = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume XPath expressions - generate multiple diverse XPath expressions
    std::vector<std::string> xpath_expressions;
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 20; i++) {
        xpath_expressions.push_back(fdp.ConsumeRandomLengthString(512));
    }
    
    // Consume variable names and values for parameterized queries
    std::vector<std::string> var_names;
    std::vector<std::string> var_values;
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 10; i++) {
        var_names.push_back(fdp.ConsumeRandomLengthString(64));
        var_values.push_back(fdp.ConsumeRandomLengthString(128));
    }
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Parse XML with different methods based on test_mode
    switch (test_mode % 3) {
        case 0:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2: {
            // Use remaining bytes as buffer
            std::vector<uint8_t> buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!buffer.empty()) {
                parse_result = doc.load_buffer(buffer.data(), buffer.size(), parse_options);
            }
            break;
        }
    }
    
    // If parsing failed, create a structured document for comprehensive XPath testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        
        // Create diverse XML structure for XPath testing
        for (int i = 0; i < 10; i++) {
            pugi::xml_node child = root.append_child(("node" + std::to_string(i)).c_str());
            child.append_attribute(("id" + std::to_string(i)).c_str()) = std::to_string(i * 100).c_str();
            std::string type_value = (i % 3 == 0 ? "even" : "odd");
            child.append_attribute(("type" + std::to_string(i % 3)).c_str()) = type_value.c_str();
            child.text().set(("text_content_" + std::to_string(i)).c_str());
            
            // Add nested children for more complex XPath queries
            if (i % 3 == 0) {
                pugi::xml_node nested = child.append_child("nested");
                nested.append_attribute("level") = "2";
                nested.text().set("nested_text");
                
                // Add attributes to nested nodes
                for (int j = 0; j < 3; j++) {
                    nested.append_attribute(("attr" + std::to_string(j)).c_str()) = 
                        ("value" + std::to_string(j)).c_str();
                }
            }
        }
        
        // Add some elements with specific names for targeted XPath queries
        pugi::xml_node special = root.append_child("special");
        special.append_attribute("category") = "test";
        special.append_attribute("count") = "42";
        special.text().set("special_content");
        
        // Add mixed content nodes
        pugi::xml_node mixed = root.append_child("mixed");
        mixed.append_child(pugi::node_pcdata).set_value("text before ");
        pugi::xml_node child = mixed.append_child("child");
        child.text().set("child text");
        mixed.append_child(pugi::node_pcdata).set_value(" text after");
    }
    
    // Get document root for node selection
    pugi::xml_node root_node = doc.document_element();
    if (!root_node) {
        root_node = doc.first_child();
    }
    
    // Test the primary target: xpath_query::evaluate_string (56 undiscovered branches)
    for (const auto& xpath_expr : xpath_expressions) {
        if (xpath_expr.empty()) continue;
        
        try {
            // Create XPath variable set if variables are enabled
            pugi::xpath_variable_set vars;
            bool has_vars = false;
            
            if (use_variables && !var_names.empty()) {
                for (size_t i = 0; i < var_names.size() && i < var_values.size(); i++) {
                    // Randomly assign variable types to test different evaluation paths
                    uint8_t var_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    switch (var_type) {
                        case 0: // Boolean variable
                            if (vars.add(var_names[i].c_str(), pugi::xpath_type_boolean)) {
                                vars.set(var_names[i].c_str(), fdp.ConsumeBool());
                                has_vars = true;
                            }
                            break;
                        case 1: // Number variable
                            if (vars.add(var_names[i].c_str(), pugi::xpath_type_number)) {
                                vars.set(var_names[i].c_str(), fdp.ConsumeFloatingPoint<double>());
                                has_vars = true;
                            }
                            break;
                        case 2: // String variable
                            if (vars.add(var_names[i].c_str(), pugi::xpath_type_string)) {
                                vars.set(var_names[i].c_str(), var_values[i].c_str());
                                has_vars = true;
                            }
                            break;
                        case 3: // Node set variable (requires actual node set)
                            // Create a simple node set from document
                            pugi::xpath_node_set sample_set;
                            if (root_node) {
                                sample_set = root_node.select_nodes("*");
                                if (vars.add(var_names[i].c_str(), pugi::xpath_type_node_set)) {
                                    vars.set(var_names[i].c_str(), sample_set);
                                    has_vars = true;
                                }
                            }
                            break;
                    }
                }
            }
            
            // Create XPath query with optional variables
            pugi::xpath_query query(xpath_expr.c_str(), has_vars ? &vars : nullptr);
            
            // Check if query compilation was successful
            if (!query) {
                // Even if query compilation failed, we can test the result() method
                const pugi::xpath_parse_result& compile_result = query.result();
                (void)compile_result;
                continue;
            }
            
            // Get query return type
            pugi::xpath_value_type return_type = query.return_type();
            
            // TARGET 1: xpath_query::evaluate_string (56 undiscovered branches)
            // Test both string evaluation methods
            if (test_buffer_version && fdp.remaining_bytes() > 100) {
                // Test buffer version of evaluate_string
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 1024);
                std::vector<char> buffer(buffer_size);
                size_t required_size = query.evaluate_string(buffer.data(), buffer.size(), doc);
                (void)required_size;
            } else {
                // Test STL string version of evaluate_string
#ifndef PUGIXML_NO_STL
                std::string string_result = query.evaluate_string(doc);
                (void)string_result;
#endif
            }
            
            // TARGET 4: xpath_query::evaluate_number
            double number_result = query.evaluate_number(doc);
            (void)number_result;
            
            // TARGET 5: xpath_query::evaluate_boolean
            bool bool_result = query.evaluate_boolean(doc);
            (void)bool_result;
            
            // Test different evaluation types based on evaluation_type
            switch (evaluation_type) {
                case 0:
                    // Comprehensive evaluation of all types
                    query.evaluate_boolean(doc);
                    query.evaluate_number(doc);
#ifndef PUGIXML_NO_STL
                    query.evaluate_string(doc);
#endif
                    query.evaluate_node(doc);
                    query.evaluate_node_set(doc);
                    break;
                    
                case 1:
                    // Focus on string evaluation variations
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
#ifndef PUGIXML_NO_STL
                        std::string str = query.evaluate_string(doc);
                        (void)str;
#endif
                        
                        // Test buffer version with different sizes
                        size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                        std::vector<char> buf(buf_size);
                        query.evaluate_string(buf.data(), buf.size(), doc);
                    }
                    break;
                    
                case 2:
                    // Focus on numeric evaluation
                    for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; i++) {
                        double num = query.evaluate_number(doc);
                        (void)num;
                        
                        // Also test boolean evaluation
                        bool b = query.evaluate_boolean(doc);
                        (void)b;
                    }
                    break;
                    
                case 3: {
                    // Test node and node_set evaluation
                    pugi::xpath_node node_result = query.evaluate_node(doc);
                    (void)node_result;
                    
                    pugi::xpath_node_set set_result = query.evaluate_node_set(doc);
                    (void)set_result;
                    break;
                }
                    
                case 4:
                    // Test with different context nodes
                    if (root_node) {
                        // Evaluate against document root
                        query.evaluate_string(root_node);
                        query.evaluate_number(root_node);
                        query.evaluate_boolean(root_node);
                        
                        // Evaluate against child nodes
                        for (pugi::xml_node child : root_node.children()) {
                            if (fdp.ConsumeBool()) {
                                query.evaluate_string(child);
                                query.evaluate_number(child);
                            }
                        }
                    }
                    break;
                    
                case 5:
                    // Test query properties and re-evaluation
                    for (int i = 0; i < 3; i++) {
                        // Re-evaluate multiple times to hit different code paths
                        query.evaluate_boolean(doc);
                        query.evaluate_number(doc);
#ifndef PUGIXML_NO_STL
                        query.evaluate_string(doc);
#endif
                    }
                    break;
            }
            
            // TARGET 2 & 3: xml_node::select_node and xml_node::select_nodes
            // Test node selection methods if we have a valid root node
            if (root_node) {
                // Test select_node (48 undiscovered branches)
                pugi::xpath_node selected_node = root_node.select_node(query);
                (void)selected_node;
                
                // Test select_nodes (48 undiscovered branches)
                pugi::xpath_node_set selected_nodes = root_node.select_nodes(query);
                (void)selected_nodes;
                
                // Test selection on nested nodes
                for (pugi::xml_node child : root_node.children()) {
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                        pugi::xpath_node child_selected = child.select_node(query);
                        (void)child_selected;
                        
                        pugi::xpath_node_set child_selected_set = child.select_nodes(query);
                        (void)child_selected_set;
                    }
                }
                
                // Test deprecated select_single_node for completeness
                pugi::xpath_node single_node = root_node.select_single_node(query);
                (void)single_node;
            }
            
            // Test variable set operations after query evaluation
            if (has_vars) {
                // Get and test variable retrieval
                for (const auto& var_name : var_names) {
                    const pugi::xpath_variable* var = vars.get(var_name.c_str());
                    (void)var;
                }
            }
            
        } catch (pugi::xpath_exception& e) {
            // XPath parsing or evaluation error - catch and continue
            const char* what = e.what();
            (void)what;
        } catch (std::bad_alloc&) {
            // Out of memory error - continue with next expression
        } catch (...) {
            // Catch any other exceptions
        }
    }
    
    // Additional comprehensive XPath testing with built-in XPath expressions
    try {
        if (root_node) {
            // Test common XPath patterns that exercise different parts of the XPath engine
            const char* common_xpaths[] = {
                "//*",
                "//@*",
                "//text()",
                "//node()",
                "//*[@*]",
                "//*[text()]",
                "//*[position() < 3]",
                "//*[last()]",
                "//*[name() = 'node']",
                "//*[contains(text(), 'text')]",
                "//*[@id > 50]",
                "//*[@type = 'even']",
                "//special[@category = 'test']",
                "//mixed/child",
                "//*[../@*]",
                "/root/*[1]",
                "/root/*[position() mod 2 = 0]",
                "//nested[@level = '2']",
                "//*[@id and @type]",
                "//*[string-length(name()) > 3]"
            };
            
            for (const char* xpath : common_xpaths) {
                pugi::xpath_query common_query(xpath);
                if (common_query) {
                    // Test all evaluation methods for each common XPath
                    common_query.evaluate_boolean(doc);
                    common_query.evaluate_number(doc);
#ifndef PUGIXML_NO_STL
                    common_query.evaluate_string(doc);
#endif
                    
                    // Test node selection methods
                    if (root_node) {
                        root_node.select_node(common_query);
                        root_node.select_nodes(common_query);
                    }
                }
            }
            
            // Test XPath queries with functions and operators
            const char* complex_xpaths[] = {
                "count(//*)",
                "sum(//@id)",
                "string(//special)",
                "number(//special/@count)",
                "boolean(//mixed)",
                "//*[position() = last()]",
                "//*[@id > 100 and @type = 'odd']",
                "//*[@id = 200 or @id = 400]",
                "//*[starts-with(name(), 'node')]",
                "//*[substring(text(), 1, 4) = 'text']",
                "//*[string-length(@type) = 3]",
                "//*[normalize-space(text()) != '']",
                "//*[translate(@type, 'eo', 'EO') = 'Odd']",
                "//*[concat('prefix-', name()) = 'prefix-node0']"
            };
            
            for (const char* xpath : complex_xpaths) {
                try {
                    pugi::xpath_query complex_query(xpath);
                    if (complex_query) {
                        // Focus on string and number evaluation for complex queries
                        complex_query.evaluate_string(doc);
                        complex_query.evaluate_number(doc);
                        complex_query.evaluate_boolean(doc);
                    }
                } catch (...) {
                    // Ignore exceptions from complex XPath evaluation
                }
            }
        }
    } catch (...) {
        // Ignore exceptions in additional testing
    }
    
    return 0;
}

} // namespace harness_004
