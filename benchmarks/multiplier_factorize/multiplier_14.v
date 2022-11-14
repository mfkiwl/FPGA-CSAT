// Benchmark "multiplier_14_sat" written by ABC on Mon Nov 14 18:21:03 2022

module multiplier_14_sat ( 
    \a[0] , \a[1] , \a[2] , \b[0] , \b[1] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \b[0] , \b[1] ;
  output sat;
  assign sat = \a[0]  & \a[1]  & \b[1]  & \a[2]  & ~\b[0] ;
endmodule


