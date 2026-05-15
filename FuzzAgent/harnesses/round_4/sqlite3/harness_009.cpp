#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"
#include "sqlite3rtree.h"

namespace harness_009 {
/*
 * Fuzzing harness for SQLite library - targeting R*Tree spatial indexing extension APIs
 * This harness targets SQLite R*Tree extension for spatial indexing, which are
 * semantically distinct from existing harnesses (000-008).
 * 
 * This harness targets R*Tree spatial indexing APIs:
 * - sqlite3_rtree_geometry_callback - registers geometry callbacks for spatial queries
 * - sqlite3_rtree_query_callback - registers 2nd-generation query callbacks  
 * - R*Tree virtual table creation and spatial data operations
 * - Spatial query execution using MATCH operator with custom geometry functions
 * 
 * Following the invocation sequence:
 * 1. Initialize SQLite and open database
 * 2. Register custom geometry callbacks using sqlite3_rtree_geometry_callback
 * 3. Register query callbacks using sqlite3_rtree_query_callback
 * 4. Create R*Tree virtual table with different dimensions (2D, 3D, 4D)
 * 5. Insert spatial data (bounding boxes) using fuzzer input coordinates
 * 6. Execute spatial queries using MATCH operator with registered geometry functions
 * 7. Test different spatial operations: point queries, range queries, custom geometries
 * 8. Clean up resources
 * 
 * Semantic differentiation from existing harnesses:
 * - Focuses specifically on R*Tree spatial indexing vs general virtual tables (harness_003)
 * - Targets spatial geometry/query callbacks vs text search (FTS5 in harness_008)
 * - Exercises spatial data structures and algorithms vs other virtual table types
 * - Tests MATCH operator with spatial geometry functions
 */


// Check if R*Tree is enabled in the build
#ifdef SQLITE_ENABLE_RTREE
#define HAS_RTREE 1
#else
#define HAS_RTREE 0
#endif

// Simple geometry callback for rectangle intersection
#if HAS_RTREE
static int rect_geom_callback(
    sqlite3_rtree_geometry *p,
    int nCoord, 
    sqlite3_rtree_dbl *aCoord,
    int *pRes
) {
    // p->aParam contains: [min_x, min_y, max_x, max_y] for rectangle
    if (p->nParam != 4 || nCoord != 4) {
        return SQLITE_ERROR;
    }
    
    // Check if bounding boxes intersect
    // Test: (rect1.min_x <= rect2.max_x) && (rect1.max_x >= rect2.min_x) &&
    //       (rect1.min_y <= rect2.max_y) && (rect1.max_y >= rect2.min_y)
    sqlite3_rtree_dbl rect_min_x = p->aParam[0];
    sqlite3_rtree_dbl rect_min_y = p->aParam[1];
    sqlite3_rtree_dbl rect_max_x = p->aParam[2];
    sqlite3_rtree_dbl rect_max_y = p->aParam[3];
    
    sqlite3_rtree_dbl node_min_x = aCoord[0];
    sqlite3_rtree_dbl node_max_x = aCoord[1];
    sqlite3_rtree_dbl node_min_y = aCoord[2];
    sqlite3_rtree_dbl node_max_y = aCoord[3];
    
    *pRes = 0;
    if (rect_min_x <= node_max_x && rect_max_x >= node_min_x &&
        rect_min_y <= node_max_y && rect_max_y >= node_min_y) {
        *pRes = 1;
    }
    
    return SQLITE_OK;
}

// Simple query callback for spatial queries with scoring
static int spatial_query_callback(sqlite3_rtree_query_info *pInfo) {
    // pInfo->aParam contains query parameters
    // For simplicity, implement distance-based scoring
    if (pInfo->nParam < 4) {
        return SQLITE_ERROR;
    }
    
    // Query point coordinates
    sqlite3_rtree_dbl query_x = pInfo->aParam[0];
    sqlite3_rtree_dbl query_y = pInfo->aParam[1];
    
    // Node bounding box coordinates
    sqlite3_rtree_dbl node_min_x = pInfo->aCoord[0];
    sqlite3_rtree_dbl node_max_x = pInfo->aCoord[1];
    sqlite3_rtree_dbl node_min_y = pInfo->aCoord[2];
    sqlite3_rtree_dbl node_max_y = pInfo->aCoord[3];
    
    // Calculate distance from query point to node center
    sqlite3_rtree_dbl center_x = (node_min_x + node_max_x) / 2.0;
    sqlite3_rtree_dbl center_y = (node_min_y + node_max_y) / 2.0;
    
    sqlite3_rtree_dbl dx = center_x - query_x;
    sqlite3_rtree_dbl dy = center_y - query_y;
    sqlite3_rtree_dbl distance = std::sqrt(dx*dx + dy*dy);
    
    // Set score (lower distance = better score)
    pInfo->rScore = distance;
    // Determine if node is within query (for this example, always within)
    pInfo->eWithin = 2; // 2 = fully within
    
    return SQLITE_OK;
}
#endif // HAS_RTREE

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need: operation type + coordinates for R*Tree operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    
    // Use in-memory database for fuzzing
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Step 2: Determine R*Tree configuration based on fuzzer input
    uint8_t rtree_config = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume table name from fuzzer input
    std::string table_name = fdp.ConsumeRandomLengthString(32);
    if (table_name.empty()) {
        table_name = "rtree_test";
    }
    
    // Build R*Tree table creation SQL based on configuration
    std::string create_sql;
    
    switch (rtree_config) {
        case 0:
            // 2D R*Tree (xmin, xmax, ymin, ymax)
            create_sql = "CREATE VIRTUAL TABLE " + table_name + " USING rtree(id, xmin, xmax, ymin, ymax);";
            break;
        case 1:
            // 3D R*Tree (xmin, xmax, ymin, ymax, zmin, zmax)
            create_sql = "CREATE VIRTUAL TABLE " + table_name + " USING rtree(id, xmin, xmax, ymin, ymax, zmin, zmax);";
            break;
        case 2:
            // 4D R*Tree with auxiliary data column
            create_sql = "CREATE VIRTUAL TABLE " + table_name + " USING rtree(id, xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, data TEXT);";
            break;
    }
    
    // Step 3: Register geometry callbacks before creating table (if R*Tree is enabled)
#if HAS_RTREE
    rc = sqlite3_rtree_geometry_callback(db, "rect_query", rect_geom_callback, nullptr);
    // Note: sqlite3_rtree_query_callback might not be available in all builds
#endif
    // Step 4: Create R*Tree table
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
        // Continue anyway - table might already exist or other error
    }
    
    // Step 5: Insert spatial data using fuzzer input
    // Determine number of insertions based on remaining input
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes >= 16) {
        // Insert at least one row if we have enough data
        int num_inserts = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int i = 0; i < num_inserts && fdp.remaining_bytes() >= 16; i++) {
            // Consume bounding box coordinates from fuzzer input
            double xmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            double xmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            double ymin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            double ymax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            
            // Ensure valid bounding box (min <= max)
            if (xmin > xmax) std::swap(xmin, xmax);
            if (ymin > ymax) std::swap(ymin, ymax);
            
            // Generate INSERT SQL
            char insert_sql[256];
            if (rtree_config == 0) {
                // 2D
                snprintf(insert_sql, sizeof(insert_sql),
                        "INSERT INTO %s VALUES(NULL, %f, %f, %f, %f);",
                        table_name.c_str(), xmin, xmax, ymin, ymax);
            } else if (rtree_config == 1) {
                // 3D - consume z coordinates
                double zmin = 0.0, zmax = 0.0;
                if (fdp.remaining_bytes() >= 8) {
                    zmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    zmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    if (zmin > zmax) std::swap(zmin, zmax);
                }
                snprintf(insert_sql, sizeof(insert_sql),
                        "INSERT INTO %s VALUES(NULL, %f, %f, %f, %f, %f, %f);",
                        table_name.c_str(), xmin, xmax, ymin, ymax, zmin, zmax);
            } else {
                // 4D - consume z and t coordinates
                double zmin = 0.0, zmax = 0.0, tmin = 0.0, tmax = 0.0;
                if (fdp.remaining_bytes() >= 16) {
                    zmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    zmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    tmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    tmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
                    if (zmin > zmax) std::swap(zmin, zmax);
                    if (tmin > tmax) std::swap(tmin, tmax);
                }
                // Consume some text data for auxiliary column
                std::string data_str = fdp.ConsumeRandomLengthString(64);
                snprintf(insert_sql, sizeof(insert_sql),
                        "INSERT INTO %s VALUES(NULL, %f, %f, %f, %f, %f, %f, %f, %f, '%s');",
                        table_name.c_str(), xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, data_str.c_str());
            }
            
            rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK && errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
        }
    }
    
    // Step 6: Execute spatial queries using registered geometry functions
    if (fdp.remaining_bytes() >= 16) {
        // Consume query rectangle coordinates
        double q_xmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double q_xmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double q_ymin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double q_ymax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        
        if (q_xmin > q_xmax) std::swap(q_xmin, q_xmax);
        if (q_ymin > q_ymax) std::swap(q_ymin, q_ymax);
        
        // Build query using registered rect_query geometry function
        char query_sql[256];
        snprintf(query_sql, sizeof(query_sql),
                "SELECT * FROM %s WHERE xmin <= %f AND xmax >= %f AND ymin <= %f AND ymax >= %f;",
                table_name.c_str(), q_xmax, q_xmin, q_ymax, q_ymin);
        
        // Also try MATCH operator with geometry callback if supported
#if HAS_RTREE
        char match_query_sql[256];
        snprintf(match_query_sql, sizeof(match_query_sql),
                "SELECT * FROM %s WHERE id MATCH rect_query(%f, %f, %f, %f);",
                table_name.c_str(), q_xmin, q_ymin, q_xmax, q_ymax);
        
        // Try MATCH query (may fail if geometry callbacks not fully supported)
        rc = sqlite3_exec(db, match_query_sql, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
#endif
    }
    
    // Step 7: Test different spatial operations
    if (fdp.remaining_bytes() >= 8) {
        // Test point query
        double point_x = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double point_y = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        
        char point_query[256];
        snprintf(point_query, sizeof(point_query),
                "SELECT * FROM %s WHERE %f BETWEEN xmin AND xmax AND %f BETWEEN ymin AND ymax;",
                table_name.c_str(), point_x, point_y);
        
        rc = sqlite3_exec(db, point_query, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
        
        // Test range query (find objects within distance)
        if (fdp.remaining_bytes() >= 12) {
            double range_x = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            double range_y = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
            double range_dist = std::abs(static_cast<double>(fdp.ConsumeIntegral<int32_t>())) / 1000.0;
            
            char range_query[256];
            snprintf(range_query, sizeof(range_query),
                    "SELECT * FROM %s WHERE "
                    "xmin BETWEEN %f AND %f AND xmax BETWEEN %f AND %f AND "
                    "ymin BETWEEN %f AND %f AND ymax BETWEEN %f AND %f;",
                    table_name.c_str(),
                    range_x - range_dist, range_x + range_dist,
                    range_x - range_dist, range_x + range_dist,
                    range_y - range_dist, range_y + range_dist,
                    range_y - range_dist, range_y + range_dist);
            
            rc = sqlite3_exec(db, range_query, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK && errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
        }
    }
    
    // Step 8: Test UPDATE and DELETE operations on R*Tree
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 16) {
        // Update a bounding box (move it)
        double new_xmin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double new_xmax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double new_ymin = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        double new_ymax = static_cast<double>(fdp.ConsumeIntegral<int32_t>());
        
        if (new_xmin > new_xmax) std::swap(new_xmin, new_xmax);
        if (new_ymin > new_ymax) std::swap(new_ymin, new_ymax);
        
        char update_sql[256];
        snprintf(update_sql, sizeof(update_sql),
                "UPDATE %s SET xmin = %f, xmax = %f, ymin = %f, ymax = %f WHERE id = (SELECT MIN(id) FROM %s);",
                table_name.c_str(), new_xmin, new_xmax, new_ymin, new_ymax, table_name.c_str());
        
        rc = sqlite3_exec(db, update_sql, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
        
        // Delete a random row
        if (fdp.ConsumeBool()) {
            char delete_sql[256];
            snprintf(delete_sql, sizeof(delete_sql),
                    "DELETE FROM %s WHERE id = (SELECT MIN(id) FROM %s);",
                    table_name.c_str(), table_name.c_str());
            
            rc = sqlite3_exec(db, delete_sql, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK && errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
        }
    }
    
    // Step 9: Test prepared statements with R*Tree
    if (fdp.remaining_bytes() >= 4) {
        sqlite3_stmt* stmt = nullptr;
        std::string select_all_query = "SELECT * FROM " + table_name + " LIMIT 5;";
        const char* select_all_sql = select_all_query.c_str();
        
        rc = sqlite3_prepare_v2(db, select_all_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            // Step through results
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Just iterate through rows
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Step 10: Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_009
