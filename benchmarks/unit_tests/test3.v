// Benchmark "test3" written by ABC on Mon Oct  3 15:39:35 2022

module test3 ( 
    a, b, c, d, e,
    sat  );
  input  a, b, c, d, e;
  output sat;
  wire new_n0_, new_n1_, new_n2_;
  assign new_n0_ = (e & (a | ~d) & (~a | d)) | (~e & (a ^ d));
  assign new_n1_ = d ^ e;
  assign new_n2_ = b | c;
  assign sat = new_n2_ & new_n0_ & new_n1_;
endmodule


