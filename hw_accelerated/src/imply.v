`timescale 100ps/100ps

module imply(pins, tt, implied_pins, ap_clk, ap_rst, ap_ce, ap_start, ap_continue, ap_idle, ap_done, ap_ready);
    parameter LUT_SIZE = 12;
    parameter TRUTH_TABLE_BITS = 1 << LUT_SIZE;
    parameter [1:0] ZERO = 2'b00;
    parameter [1:0] ONE = 2'b10;
    parameter [1:0] UNKNOWN = 2'b11;

    input [2*LUT_SIZE + 1:0] pins;
    input [TRUTH_TABLE_BITS - 1:0] tt;
    output [2*LUT_SIZE + 1:0] implied_pins;

    input ap_clk, ap_rst, ap_ce, ap_start, ap_continue;
    output ap_idle, ap_done, ap_ready;

    reg dly1;

    wire [TRUTH_TABLE_BITS - 1:0] mask;

    genvar address;
    genvar i;
    
    generate
        for (address = 0; address < TRUTH_TABLE_BITS; address = address + 1) begin
            computeMask #(.address(address), .LUT_SIZE(LUT_SIZE), .ZERO(ZERO), .ONE(ONE)) m (.pins(pins), .tt_bit(tt[address]), .mask_bit(mask[address]));
        end
    endgenerate

    for (i = 0; i < LUT_SIZE; i = i + 1) begin
        computeInputImpliedPin #(.pin_index(i), .TRUTH_TABLE_BITS(TRUTH_TABLE_BITS), .ZERO(ZERO), .ONE(ONE), .UNKNOWN(UNKNOWN)) ip (.mask(mask), .input_implied_pin(implied_pins[2*i + 1 : 2*i]));
    end

    computeOutputImpliedPin #(.TRUTH_TABLE_BITS(TRUTH_TABLE_BITS), .ZERO(ZERO), .ONE(ONE), .UNKNOWN(UNKNOWN)) op (.mask(mask), .tt(tt), .output_implied_pin(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE]));

    assign ap_ready  = dly1;
    assign ap_done   = dly1;
    assign ap_idle   = ~ap_start;

endmodule

module computeMask(pins, tt_bit, mask_bit);
    parameter LUT_SIZE = 8;
    parameter [1:0] ZERO = 2'b00;
    parameter [1:0] ONE = 2'b10;

    parameter [LUT_SIZE - 1 : 0] address = 0;

    input [2*LUT_SIZE + 1:0] pins;
    input tt_bit;
    output reg mask_bit;

    reg [LUT_SIZE : 0] mismatch_pins;
    genvar i;

    // Compute which input pins are mismatched
    for (i = 0; i < LUT_SIZE; i=i+1) begin
        always @* begin
            if (((pins[2*i + 1 : 2*i] == ZERO) && address[i]) || ((pins[2*i + 1 : 2*i] == ONE) && !address[i])) begin
                mismatch_pins[i] <= 1;
            end else begin
                mismatch_pins[i] <= 0;
            end
        end
    end

    always @* begin
        // Compute if output pin is mismatched
        if (((pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] == ZERO) && tt_bit) || ((pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] == ONE) && !tt_bit)) begin
            mismatch_pins[LUT_SIZE] <= 1;
        end else begin
            mismatch_pins[LUT_SIZE] <= 0;
        end

        // Mask bit is set if any pins are mismatched
        mask_bit <= (|mismatch_pins);
    end
endmodule

module computeInputImpliedPin(mask, input_implied_pin);
    parameter TRUTH_TABLE_BITS = 256;
    parameter [1:0] ZERO = 2'b00;
    parameter [1:0] ONE = 2'b10;
    parameter [1:0] UNKNOWN = 2'b11;

    parameter integer pin_index = 0;

    input [TRUTH_TABLE_BITS - 1 : 0] mask;
    output reg [1:0] input_implied_pin;

    reg [TRUTH_TABLE_BITS - 1:0] imply0;
    reg [TRUTH_TABLE_BITS - 1:0] imply1;
    genvar address;

    for (address = 0; address < TRUTH_TABLE_BITS; address = address + 1) begin
        always @* begin
            if (mask[address] == 1'b0) begin
                if (address[pin_index] == 1'b1) begin
                    imply0[address] <= 0;
                    imply1[address] <= 1;
                end else begin
                    imply0[address] <= 1;
                    imply1[address] <= 0;
                end
            end else begin
                imply0[address] <= 1;
                imply1[address] <= 1;
            end
        end
    end
    always @* begin
        if(&imply0) begin
            input_implied_pin <= ZERO;
        end else if(&imply1) begin
            input_implied_pin <= ONE;
        end else begin
            input_implied_pin <= UNKNOWN;
        end
    end
endmodule

module computeOutputImpliedPin(mask, tt, output_implied_pin);
    parameter TRUTH_TABLE_BITS = 256;
    parameter [1:0] ZERO = 2'b00;
    parameter [1:0] ONE = 2'b10;
    parameter [1:0] UNKNOWN = 2'b11;

    input [TRUTH_TABLE_BITS - 1 : 0] mask;
    input [TRUTH_TABLE_BITS - 1 : 0] tt;
    output reg [1:0] output_implied_pin;

    reg [TRUTH_TABLE_BITS - 1:0] imply0;
    reg [TRUTH_TABLE_BITS - 1:0] imply1;
    genvar address;

    for (address = 0; address < TRUTH_TABLE_BITS; address = address + 1) begin
        always @* begin
            if (mask[address] == 1'b0) begin
                if (tt[address] == 1'b1) begin
                    imply0[address] <= 0;
                    imply1[address] <= 1;
                end else begin
                    imply0[address] <= 1;
                    imply1[address] <= 0;
                end
            end else begin
                imply0[address] <= 1;
                imply1[address] <= 1;
            end
        end
    end
    always @* begin
        if(&imply0) begin
            output_implied_pin <= ZERO;
        end else if(&imply1) begin
            output_implied_pin <= ONE;
        end else begin
            output_implied_pin <= UNKNOWN;
        end
    end
endmodule
