`timescale 100ps/100ps

module imply_tb();
    parameter LUT_SIZE = 8;
    parameter TRUTH_TABLE_BITS = 1 << LUT_SIZE;
    parameter [1:0] ZERO = 2'b00;
    parameter [1:0] ONE = 2'b01;
    parameter [1:0] UNKNOWN = 2'b11;

    reg [2*LUT_SIZE + 1:0] pins;
    reg [TRUTH_TABLE_BITS - 1:0] tt;
    wire [2*LUT_SIZE + 1:0] implied_pins;
    wire conflict;

    imply #(.LUT_SIZE(LUT_SIZE)) uut(.pins(pins), .tt(tt), .implied_pins(implied_pins), .conflict(conflict));

    localparam period = 20;

    function [TRUTH_TABLE_BITS-1 : 0] projection(input integer i);
            integer address;
        begin
            for(address = 0; address < TRUTH_TABLE_BITS; address = address + 1) begin
                if(address[i] == 1'b1) begin
                    projection[address] = 1;
                end else begin
                    projection[address] = 0;
                end
            end
        end
    endfunction

    initial
    begin
        // CONST
        tt = 0;
        pins = -1;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != ZERO)
            $display("Test failed for const 0 output");

        // SINGLE VAR PROJECTION
        tt = projection(0);
        pins = -1;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != UNKNOWN)
            $display("Test failed for var_0 projection output (U)");

        pins[1:0] = ZERO;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != ZERO)
            $display("Test failed for var_0 projection output (0)");

        pins[1:0] = ONE;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != ONE)
            $display("Test failed for var_0 projection output (1)");

        // AND
        tt = projection(0) & projection(1);
        pins = -1;
        pins[1:0] = ZERO;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != ZERO)
            $display("Test failed for AND output (0)");

        pins[1:0] = ONE;
        pins[3:2] = ONE;
        #period;
        if(implied_pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] != ONE)
            $display("Test failed for AND output (1)");

        pins[1:0] = UNKNOWN;
        pins[3:2] = UNKNOWN;
        pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] = ONE;
        #period;
        if(implied_pins[1:0] != ONE || implied_pins[3:2] != ONE)
            $display("Test failed for AND input (1, 1)");

        pins[1:0] = ONE;
        pins[3:2] = UNKNOWN;
        pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] = ZERO;
        #period;
        if(implied_pins[3:2] != ZERO)
            $display("Test failed for AND input (0)");

        // Conflict Detection (AND)
        pins[1:0] = ZERO;
        pins[3:2] = ZERO;
        pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] = ONE;
        #period;
        if(conflict != 1'b1)
            $display("Test failed for conflicting AND");

        pins[2*LUT_SIZE + 1 : 2*LUT_SIZE] = ZERO;
        #period;
        if(conflict != 1'b0)
            $display("Test failed for non-conflicting AND");
        
        $display("Test Finished!");
        $finish;
    end
endmodule
