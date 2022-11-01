module test5 (a, b, c, d, e, z);
  input  a, b, c, d, e;
  output z;
  wire w0, w1, w2;
  assign w0 = a ^ d ^ e;
  assign w1 = ~(b ^ c ^ d);
  assign w2 = d ^ e;
  assign z = w0 & w1 & w2;
endmodule
