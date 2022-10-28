#pragma once

#include <array>

#include "kitty/include/kitty/kitty.hpp"
#include "shared_parameters.hpp"

using namespace std;

enum struct PinValue { unknown_ps0 = 0b00,
                       unknown_ps1 = 0b01,
                       zero = 0b10,
                       one = 0b11
};

bool isUnknown(const PinValue& pv) {
    if (pv == PinValue::unknown_ps0 || pv == PinValue::unknown_ps1) {
        return true;
    }
    return false;
}

bool isKnown(const PinValue& pv) {
    return !isUnknown(pv);
}

PinValue savePhase(const PinValue& pv) {
    if (pv == PinValue::zero) {
        return PinValue::unknown_ps0;
    } else if (pv == PinValue::one) {
        return PinValue::unknown_ps1;
    }
    return PinValue::unknown_ps1;
}

PinValue restorePhase(const PinValue& pv) {
    if (pv == PinValue::unknown_ps0) {
        return PinValue::zero;
    } else if (pv == PinValue::unknown_ps1) {
        return PinValue::one;
    }
    assert(false);
}

ostream& operator<<(ostream& os, const PinValue& pv) {
    switch (pv) {
        case PinValue::zero:
            os << "zero";
            break;
        case PinValue::one:
            os << "one";
            break;
        case PinValue::unknown_ps0:
            os << "unknown";
            break;
        case PinValue::unknown_ps1:
            os << "unknown";
            break;
    }
    return os;
};

/*! Struct containing the ephemeral pin values used during solve
 */
struct Gate {
    Gate() : output_pin(PinValue::unknown_ps1) {
        input_pins.fill(PinValue::unknown_ps1);
    }
    array<PinValue, LUT_SIZE> input_pins;
    PinValue output_pin;
    kitty::static_truth_table<LUT_SIZE> truth_table;

    void print() {
        cout << "{ input_pins = { ";
        for (const auto& input_pin : input_pins) {
            cout << input_pin << " ";
        }
        cout << "}, output_pin = { " << output_pin << "}, tt = ";
        kitty::print_binary(truth_table);
        cout << " }";
    }
};

array<array<kitty::static_truth_table<LUT_SIZE>, 2>, LUT_SIZE> generateMaskTables() {
    array<array<kitty::static_truth_table<LUT_SIZE>, 2>, LUT_SIZE> mask_tables;
    kitty::static_truth_table<LUT_SIZE> mask;
    for (size_t i = 0; i < LUT_SIZE; i++) {
        kitty::create_nth_var(mask, i);
        mask_tables[i][0] = ~mask;
        mask_tables[i][1] = mask;
    }
    return mask_tables;
}

kitty::static_truth_table<LUT_SIZE> generateConst1Table() {
    kitty::static_truth_table<LUT_SIZE> const1_table;
    const1_table = unary_not(const1_table);
    return const1_table;
}

const static array<array<kitty::static_truth_table<LUT_SIZE>, 2>, LUT_SIZE> mask_tables = generateMaskTables();
const static kitty::static_truth_table<LUT_SIZE> const1_table = generateConst1Table();
const static kitty::static_truth_table<LUT_SIZE> const0_table;

PinValue calculateInputImplication(const Gate& gate, size_t input_index) {
    kitty::static_truth_table<LUT_SIZE> mask = const1_table;

    for (size_t pin_index = 0; pin_index < LUT_SIZE; pin_index++) {
        if (pin_index == input_index) {
            continue;
        }

        if (gate.input_pins[pin_index] == PinValue::zero) {
            mask &= mask_tables[pin_index][0];
        } else if (gate.input_pins[pin_index] == PinValue::one) {
            mask &= mask_tables[pin_index][1];
        }
    }

    if (gate.output_pin == PinValue::zero) {
        mask &= ~gate.truth_table;
    } else if (gate.output_pin == PinValue::one) {
        mask &= gate.truth_table;
    }

    if (kitty::is_const0(mask & mask_tables[input_index][1])) {
        return PinValue::zero;
    } else if (kitty::is_const0(mask & mask_tables[input_index][0])) {
        return PinValue::one;
    } else {
        return gate.input_pins[input_index];
    }
}

PinValue calculateOutputImplication(const Gate& gate) {
    kitty::static_truth_table<LUT_SIZE> mask = const1_table;

    for (size_t pin_index = 0; pin_index < LUT_SIZE; pin_index++) {
        if (gate.input_pins[pin_index] == PinValue::zero) {
            mask &= mask_tables[pin_index][0];
        } else if (gate.input_pins[pin_index] == PinValue::one) {
            mask &= mask_tables[pin_index][1];
        }
    }

    if (kitty::is_const0(mask & gate.truth_table)) {
        return PinValue::zero;
    } else if (kitty::is_const0(mask & ~gate.truth_table)) {
        return PinValue::one;
    } else {
        return gate.output_pin;
    }
}