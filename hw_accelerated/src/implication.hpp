#include "ap_int.h"
#include "hardware_structs.hpp"
#include "shared_parameters.hpp"

PinValue invert(const PinValue& x) {
    if (x == ZERO) {
        return ONE;
    } else if (x == ONE) {
        return ZERO;
    } else {
        return UNKNOWN;
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

void imply(Gate pins, const TruthTable& tt, Gate& implied_pins) {
    TruthTable mask = -1;  // all 1s

// Mask out rows of the TT
imply_maskout_loop:
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
#pragma HLS unroll
        if (pins(2 * i + 1, 2 * i) == ONE) {
            mask &= MASKS.mask_tables[i][0];
        } else if (pins(2 * i + 1, 2 * i) == ZERO) {
            mask &= MASKS.mask_tables[i][1];
        }
    }
    if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == ONE) {
        mask &= tt;
    } else if (pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) == ZERO) {
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
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = ZERO;
            } else if (remaining_zeros.nor_reduce()) {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = ONE;
            } else {
                implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = UNKNOWN;
            }
        } else {
            TruthTable remaining_ones = mask & MASKS.mask_tables[i][0];
            TruthTable remaining_zeros = mask & MASKS.mask_tables[i][1];
            if (remaining_ones.nor_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = ZERO;
            } else if (remaining_zeros.nor_reduce()) {
                implied_pins(2 * i + 1, 2 * i) = ONE;
            } else {
                implied_pins(2 * i + 1, 2 * i) = UNKNOWN;
            }
        }
    }
}

Gate imply_new(Gate pins, const TruthTable& tt) {
    Gate implied_pins;
    TruthTable mask = 0;

// build the mask single addresses at a time (maybe Vitis will synthesize this better)
imply_maskout_loop:
    for (unsigned int address_index = 0; address_index < TRUTH_TABLE_BITS; address_index++) {
#pragma HLS unroll
#pragma HLS dependence variable = mask type = inter false
        const ap_uint<LUT_SIZE> address = ap_uint<LUT_SIZE>(address_index);
        // For an address: if a bit differs from the pins in ANY place, it is masked
        for (unsigned int i = 0; i < LUT_SIZE; i++) {
#pragma HLS unroll
#pragma HLS dependence variable = mask type = inter false
            const PinValue pv = pins(2 * i + 1, 2 * i);
            if ((pv == ZERO && address.test(i)) || (pv == ONE && !address.test(i))) {
                mask.set_bit(address, 1);
            }
        }
        const PinValue pv = pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE);
        if ((pv == ZERO && tt.test(address)) || (pv == ONE && !tt.test(address))) {
            mask.set_bit(address, 1);
        }
    }

    // For each pin:
    // Assume both imply0 and imply1 are true (at least one will be false by the end of the loop)
    // If a truth table cell contains 1 and isn't masked out, 0 cannot be implied (and vice versa)
    // There will always be at least one cell that isn't masked out, and the opposing imply flag will become false
imply_isUnate_loop:
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
#pragma HLS unroll
#pragma HLS dependence variable = implied_pins type = inter false
        bool imply0 = true;
        bool imply1 = true;
        for (unsigned int address_index = 0; address_index < TRUTH_TABLE_BITS; address_index++) {
#pragma HLS unroll
            const ap_uint<LUT_SIZE> address = ap_uint<LUT_SIZE>(address_index);
            if (address.test(i)) {
                // Cell contains 1
                if (!mask.test(address)) {
                    imply0 = false;
                }
            } else {
                // Cell contains 0
                if (!mask.test(address)) {
                    imply1 = false;
                }
            }
        }
        if (imply0) {
            implied_pins(2 * i + 1, 2 * i) = ZERO;
        } else if (imply1) {
            implied_pins(2 * i + 1, 2 * i) = ONE;
        } else {
            implied_pins(2 * i + 1, 2 * i) = UNKNOWN;
        }
    }

    // The output pin requires more resources because the cells are not fixed, tt[address] can be 0 or 1
    bool imply0 = true;
    bool imply1 = true;
    for (unsigned int address_index = 0; address_index < TRUTH_TABLE_BITS; address_index++) {
#pragma HLS unroll
        const ap_uint<LUT_SIZE> address = ap_uint<LUT_SIZE>(address_index);
        if (tt.test(address)) {
            // Cell contains 1
            if (!mask.test(address)) {
                imply0 = false;
            }
        } else {
            // Cell contains 0
            if (!mask.test(address)) {
                imply1 = false;
            }
        }
    }
    if (imply0) {
        implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = ZERO;
    } else if (imply1) {
        implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = ONE;
    } else {
        implied_pins(2 * LUT_SIZE + 1, 2 * LUT_SIZE) = UNKNOWN;
    }

    return implied_pins;
}

bool conflictingAssign(const PinValue& old_value, const PinValue& new_value) {
    if ((old_value == ZERO && new_value == ONE) || (old_value == ONE && new_value == ZERO)) {
        return true;
    }
    return false;
}