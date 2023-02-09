module test6 (a, b, c, d, e, f, sat);
  input  a, b, c, d, e, f;
  output sat;
  wire w0, w1, w2, w3;
  assign w0 = !(a ^ b ^ c ^ d);
  assign w1 = a ^ e ^ f;
  assign w2 = c ^ d;
  assign w3 = e ^ f;
  assign sat = w0 & w1 & w2 & w3;
endmodule
