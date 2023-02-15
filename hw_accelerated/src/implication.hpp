#include "ap_int.h"
#include "hardware_structs.hpp"
#include "shared_parameters.hpp"

/* Generates all nth variable truth tables
 * for example (with N 3) mask_tables =
 *   10101010
 *   11001100
 *   11110000
 */
template <unsigned int N>
struct Masks {
    constexpr static uint32_t num_bits = 1 << N;
    constexpr Masks() {
        for (unsigned int i = 0; i < N; i++) {
            uint32_t half_period = 1 << i;
            uint32_t period = 2 * half_period;
            uint32_t blocks = num_bits / period;

            for (unsigned int b = 0; b < blocks; b++) {
                for (unsigned int j = 0; j < half_period; j++) {
                    mask_tables[i][b * period + j] = ap_uint<1>(0);
                }
                for (unsigned int j = 0; j < half_period; j++) {
                    mask_tables[i][b * period + half_period + j] = ap_uint<1>(1);
                }
            }
        }
    }
    ap_uint<num_bits> mask_tables[N];
};

const static Masks<LUT_SIZE> MASKS;

/*
 * If all rows are masked out, conflict will be true, and the implied_pins are undefined
 */

void imply(const Pins pins, const TruthTable tt, Pins& implied_pins, bool& conflict) {
    TruthTable mask = 0;

// Mask out rows of the TT
imply_maskout_loop:
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
        if (pins(2 * i + 1, 2 * i) == pin_value::kZero) {
            mask |= MASKS.mask_tables[i];
        } else if (pins(2 * i + 1, 2 * i) == pin_value::kOne) {
            mask |= ~MASKS.mask_tables[i];
        }
    }
    if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == pin_value::kZero) {
        mask |= tt;
    } else if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == pin_value::kOne) {
        mask |= ~tt;
    }

    conflict = (mask == -1) ? true : false;

// If a column only contains 1 value, it can be implied
imply_isUnate_loop:
    for (unsigned int i = 0; i <= LUT_SIZE; i++) {
        if (i == LUT_SIZE) {
            TruthTable remaining_zeros = mask | ~tt;
            TruthTable remaining_ones = mask | tt;
            if (remaining_zeros.and_reduce()) {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kZero;
            } else if (remaining_ones.and_reduce()) {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kOne;
            } else {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kUnknownPS1;
            }
        } else {
            TruthTable remaining_zeros = mask | ~MASKS.mask_tables[i];
            TruthTable remaining_ones = mask | MASKS.mask_tables[i];
            if (remaining_zeros.and_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kZero;
            } else if (remaining_ones.and_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kOne;
            } else {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kUnknownPS1;
            }
        }
    }
}