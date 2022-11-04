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

Gate imply(Gate pins, const TruthTable& tt) {
    Gate implied_pins;
    TruthTable mask = -1;  // all 1s

    // Mask out rows of the TT
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
        if (pins.input(i) == ONE) {
            mask &= MASKS.mask_tables[i][0];
        } else if (pins.input(i) == ZERO) {
            mask &= MASKS.mask_tables[i][1];
        }
    }
    if (pins.output() == ONE) {
        mask &= tt;
    } else if (pins.output() == ZERO) {
        mask &= ~tt;
    }

    // If a column only contains 1 value, it can be implied
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
        TruthTable remaining_ones = mask & MASKS.mask_tables[i][0];
        TruthTable remaining_zeros = mask & MASKS.mask_tables[i][1];
        if (remaining_ones.nor_reduce()) {
            implied_pins.input(i) = ZERO;
        } else if (remaining_zeros.nor_reduce()) {
            implied_pins.input(i) = ONE;
        } else {
            implied_pins.input(i) = UNKNOWN;
        }
    }
    TruthTable remaining_ones = mask & tt;
    TruthTable remaining_zeros = mask & ~tt;
    if (remaining_ones.nor_reduce()) {
        implied_pins.output() = ZERO;
    } else if (remaining_zeros.nor_reduce()) {
        implied_pins.output() = ONE;
    } else {
        implied_pins.output() = UNKNOWN;
    }

    return implied_pins;
}

bool conflictingAssign(const PinValue& old_value, const PinValue& new_value) {
    if ((old_value == ZERO && new_value == ONE) || (old_value == ONE && new_value == ZERO)) {
        return true;
    }
    return false;
}