module test5 (a, b, c, d, e, sat);
  input  a, b, c, d, e;
  output sat;
  wire w0, w1, w2, w3;
  assign w0 = a ^ b ^ c;
  assign w1 = b ^ c;
  assign w2 = a ^ d ^ e;
  assign w3 = d ^ e;
  assign sat = w0 & w1 & w2 & w3;
endmodule
