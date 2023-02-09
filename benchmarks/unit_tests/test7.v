module test7 (a, b, c, d, e, f, sat);
  input  a, b, c, d, e, f, g;
  output sat;
  wire w0, w1, w2, w3, w4;
  assign w0 = a ^ b ^ c;
  assign w1 = c ^ d ^ e;
  assign w2 = d ^ e;
  assign w3 = a ^ f ^ g;
  assign w4 = f ^ g;
  assign sat = w0 & w1 & w2 & w3 & w4;
endmodule
