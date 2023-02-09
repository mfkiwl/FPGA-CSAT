module test4 (a, b, c, d, e, sat);
  input  a, b, c, d, e;
  output sat;
  wire w0, w1, w2;
  assign w0 = a ^ d ^ e;
  assign w1 = ~(b ^ c ^ d);
  assign w2 = d ^ e;
  assign sat = w0 & w1 & w2;
endmodule
