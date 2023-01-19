#include "ap_int.h"
#include "hardware_structs.hpp"
#include "shared_parameters.hpp"

PinValue invert(const PinValue& x) {
#pragma HLS INLINE
    if (x == pin_value::kZero) {
        return pin_value::kOne;
    } else if (x == pin_value::kOne) {
        return pin_value::kZero;
    } else {
        return pin_value::kUnknown;
    }
}

/* Generates all nth varariable truth tables, as well as their inverse
 * for example (with N 3) mask_tables =
 *   10101010 01010101
 *   11001100 00110011
 *   11110000 00001111
 */
template <unsigned int N>
struct Masks {
    constexpr static uint32_t num_bits = 1 << N;
    constexpr Masks() : mask_tables() {
        for (unsigned int i = 0; i < N; i++) {
            uint32_t half_period = 1 << i;
            uint32_t period = 2 * half_period;
            uint32_t blocks = num_bits / period;

            for (unsigned int b = 0; b < blocks; b++) {
                for (unsigned int j = 0; j < half_period; j++) {
                    mask_tables[i][0][b * period + j] = 0;
                }
                for (unsigned int j = 0; j < half_period; j++) {
                    mask_tables[i][0][b * period + half_period + j] = 1;
                }
            }
            mask_tables[i][1] = ~mask_tables[i][0];
        }
    }
    ap_uint<num_bits> mask_tables[N][2];
};

const static Masks<LUT_SIZE> MASKS;

void imply(Pins pins, const TruthTable& tt, Pins& implied_pins) {
    TruthTable mask = -1;  // all 1s

// Mask out rows of the TT
imply_maskout_loop:
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
#pragma HLS unroll
        if (pins(2 * i + 1, 2 * i) == pin_value::kOne) {
            mask &= MASKS.mask_tables[i][0];
        } else if (pins(2 * i + 1, 2 * i) == pin_value::kZero) {
            mask &= MASKS.mask_tables[i][1];
        }
    }
    if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == pin_value::kOne) {
        mask &= tt;
    } else if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == pin_value::kZero) {
        mask &= ~tt;
    }

// If a column only contains 1 value, it can be implied
imply_isUnate_loop:
    for (unsigned int i = 0; i <= LUT_SIZE; i++) {
#pragma HLS unroll
        if (i == LUT_SIZE) {
            TruthTable remaining_ones = mask & tt;
            TruthTable remaining_zeros = mask & ~tt;
            if (remaining_ones.nor_reduce()) {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kZero;
            } else if (remaining_zeros.nor_reduce()) {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kOne;
            } else {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = pin_value::kUnknown;
            }
        } else {
            TruthTable remaining_ones = mask & MASKS.mask_tables[i][0];
            TruthTable remaining_zeros = mask & MASKS.mask_tables[i][1];
            if (remaining_ones.nor_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kZero;
            } else if (remaining_zeros.nor_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kOne;
            } else {
                implied_pins(2 * i + 1, 2 * i) = pin_value::kUnknown;
            }
        }
    }
}