#include "ap_int.h"
#include "hardware_structs.hpp"
#include "shared_parameters.hpp"

const PinValue ZERO = 0b00;
const PinValue ONE = 0b01;
const PinValue DONTCARE = 0b10;
const PinValue UNKNOWN = 0b11;

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
 *   01010101 10101010
 *   00110011 11001100
 *   00001111 11110000
 */
template <unsigned int N>
struct Masks {
    constexpr Masks() : mask_tables() {
        for (unsigned int i = 0; i < N; i++) {
            ap_uint<N> count = 0;
            do {
                for (size_t index; index < N; index++) {
                    mask_tables[index][0][count] = ~count[count];
                    mask_tables[index][1][count] = count[count];
                }
                count++;
            } while (count > 0);
        }
    }
    mask_tables[N][2];
};

constexpr static Masks MASKS;

Gate imply(Gate pins, const TruthTable& tt) {
    Gate implied_pins;
    TruthTable mask = -1;  // all 1s

    // Mask out rows of the TT
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
        if (pins.input(i) == ZERO) {
            mask &= MASKS.mask_tables[i][0];
        } else if (pins.input(i) == ONE) {
            mask &= MASKS.mask_tables[i][1];
        }
    }
    if (pins.output() == ZERO) {
        mask &= ~tt;
    } else if (pins.output() == ONE) {
        mask &= tt;
    }

    // If a column only contains 1 value, it can be implied
    for (unsigned int i = 0; i < LUT_SIZE; i++) {
        TruthTable remaining_zeros = mask & MASKS.mask_tables[i][0];
        TruthTable remaining_ones = mask & MASKS.mask_tables[i][1];
        if (remaining_ones.nor_reduce()) {
            implied_pins.input(i) = ZERO;
        } else if (remaining_zeros.nor_reduce()) {
            implied_pins.input(i) = ONE;
        } else {
            implied_pins.input(i) = UNKNOWN;
        }
    }
    TruthTable remaining_zeros = mask & ~tt;
    TruthTable remaining_ones = mask & tt;
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