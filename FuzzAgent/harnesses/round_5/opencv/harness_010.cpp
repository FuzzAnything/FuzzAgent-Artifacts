#include <opencv2/core.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need enough for matrix dimensions and data
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Step 1: Consume matrix dimension (must be square for both LU and Cholesky)
        int m = fdp.ConsumeIntegralInRange(2, 20); // Matrix dimension (at least 2x2)
        
        // Step 2: Create matrices for LU and Cholesky decompositions
        // For LU: general square matrix (should be non-singular)
        // For Cholesky: symmetric positive definite matrix
        
        // Create general square matrix for LU decomposition
        std::vector<double> A_lu(m * m);
        std::vector<double> b_lu(m); // Right-hand side vector
        
        // Fill LU matrix with random values
        for (int i = 0; i < m * m; ++i) {
            A_lu[i] = fdp.ConsumeFloatingPointInRange<double>(-10.0, 10.0);
        }
        
        // Fill b vector with random values
        for (int i = 0; i < m; ++i) {
            b_lu[i] = fdp.ConsumeFloatingPointInRange<double>(-10.0, 10.0);
        }
        
        // Step 3: Create symmetric positive definite matrix for Cholesky
        std::vector<double> A_chol(m * m);
        std::vector<double> b_chol(m);
        
        // Create a random matrix M
        std::vector<double> M(m * m);
        for (int i = 0; i < m * m; ++i) {
            M[i] = fdp.ConsumeFloatingPointInRange<double>(-5.0, 5.0);
        }
        
        // Compute A_chol = M*M^T + epsilon*I to ensure positive definiteness
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                double sum = 0.0;
                for (int k = 0; k < m; ++k) {
                    sum += M[i * m + k] * M[j * m + k];
                }
                A_chol[i * m + j] = sum;
            }
        }
        
        // Add epsilon*I to ensure positive definiteness (diagonal dominance)
        double epsilon = fdp.ConsumeFloatingPointInRange<double>(0.1, 10.0);
        for (int i = 0; i < m; ++i) {
            A_chol[i * m + i] += epsilon;
        }
        
        // Fill Cholesky b vector with random values
        for (int i = 0; i < m; ++i) {
            b_chol[i] = fdp.ConsumeFloatingPointInRange<double>(-10.0, 10.0);
        }
        
        // Step 4: Test LU decomposition (double precision)
        // Signature: int LU(double* A, size_t astep, int m, double* b, size_t bstep, int n)
        size_t astep = m * sizeof(double); // Assuming row-major, contiguous storage
        size_t bstep = sizeof(double);     // For vector b
        
        // Test LU with b = NULL (just decomposition)
        int lu_result1 = cv::LU(A_lu.data(), astep, m, nullptr, 0, 0);
        
        // Test LU with b vector (solving linear system)
        int lu_result2 = cv::LU(A_lu.data(), astep, m, b_lu.data(), bstep, 1);
        
        // Step 5: Test Cholesky decomposition (double precision)
        // Signature: bool Cholesky(double* A, size_t astep, int m, double* b, size_t bstep, int n)
        
        // Test Cholesky with b = NULL (just decomposition)
        bool chol_result1 = cv::Cholesky(A_chol.data(), astep, m, nullptr, 0, 0);
        
        // Test Cholesky with b vector (solving linear system)
        bool chol_result2 = cv::Cholesky(A_chol.data(), astep, m, b_chol.data(), bstep, 1);
        
        // Step 6: Test edge cases with remaining input
        if (fdp.remaining_bytes() > 0) {
            // Test with different right-hand side dimensions
            int n_rhs = fdp.ConsumeIntegralInRange(1, 5);
            if (n_rhs > 0) {
                std::vector<double> b_multi(m * n_rhs);
                for (int i = 0; i < m * n_rhs; ++i) {
                    b_multi[i] = fdp.ConsumeFloatingPointInRange<double>(-5.0, 5.0);
                }
                size_t bstep_multi = sizeof(double);
                
                // Test LU with multiple right-hand sides
                std::vector<double> A_lu_copy = A_lu; // Copy to preserve original
                int lu_result3 = cv::LU(A_lu_copy.data(), astep, m, b_multi.data(), bstep_multi, n_rhs);
                
                // Test Cholesky with multiple right-hand sides
                std::vector<double> A_chol_copy = A_chol; // Copy to preserve original
                bool chol_result3 = cv::Cholesky(A_chol_copy.data(), astep, m, b_multi.data(), bstep_multi, n_rhs);
            }
            
            // Test with potentially singular/ill-conditioned matrices
            if (fdp.ConsumeBool() && m > 2) {
                std::vector<double> A_singular(m * m, 0.0);
                // Create a singular matrix (rank-deficient)
                for (int i = 0; i < m - 1; ++i) {
                    for (int j = 0; j < m; ++j) {
                        A_singular[i * m + j] = fdp.ConsumeFloatingPointInRange<double>(-5.0, 5.0);
                    }
                }
                // Make last row linearly dependent
                for (int j = 0; j < m; ++j) {
                    A_singular[(m-1) * m + j] = A_singular[j] + A_singular[m + j]; // Linear combination
                }
                
                // Test LU with singular matrix
                std::vector<double> b_singular(m);
                for (int i = 0; i < m; ++i) {
                    b_singular[i] = fdp.ConsumeFloatingPointInRange<double>(-5.0, 5.0);
                }
                
                int lu_result_singular = cv::LU(A_singular.data(), astep, m, b_singular.data(), bstep, 1);
            }
        }
        
    } catch (...) {
        // Ignore any exceptions - they are expected for invalid inputs
    }
    
    return 0;
}

} // namespace harness_010
