// Benchmark "test2" written by ABC on Mon Oct  3 15:37:06 2022

module test2 ( 
    a, b, c,
    z  );
  input  a, b, c;
  output z;
  wire new_new_n0__, new_new_n1__;
  assign new_new_n0__ = ~c ^ (~a ^ b);
  assign new_new_n1__ = b ^ c;
  assign z = new_new_n0__ & new_new_n1__;
endmodule


