// Benchmark "test0" written by ABC on Tue Sep 27 23:25:21 2022

module test0 ( 
    a, b, c, d, e, f, g, h, i, j, k, l,
    sat  );
  input  a, b, c, d, e, f, g, h, i, j, k, l;
  output sat;
  wire new_n0_, new_n1_;
  assign new_n0_ = h | ~g | f | e | d | ~c | a | ~b;
  assign new_n1_ = l & k & ~j & i & h & g & ~e & f;
  assign sat = new_n0_ & new_n1_;
endmodule


