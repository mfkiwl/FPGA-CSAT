// Benchmark "multiplier_23_sat" written by ABC on Mon Nov 14 17:44:35 2022

module multiplier_23_sat ( 
    \a[0] , \a[1] , \a[2] , \a[3] , \b[0] , \b[1] , \b[2] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \a[3] , \b[0] , \b[1] , \b[2] ;
  output sat;
  assign sat = (((((~\a[2]  | (\b[2]  & ((\b[1]  & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | ((\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & ~\a[0]  & ~\a[1] )))) & (\a[2]  | ~\b[2]  | ((~\b[1]  | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & ((~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] ))) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[0]  | ~\a[2]  | ~\a[3] )) | ((~\a[2]  | (\a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & (\b[2]  ? ~\b[1]  : (\b[0]  & \b[1] ))) | (\b[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & ~\a[0]  & ~\a[1] ) | (~\a[0]  & \a[1]  & \b[2] )) & (\a[2]  | ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ? \b[1]  : (~\b[0]  | ~\b[1] ))) & (~\b[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] ) & (\a[0]  | ~\a[1]  | ~\b[2] ))) & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )))) & ((\a[2]  & (~\b[2]  | ((~\b[1]  | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & ((~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] )))) | (~\a[2]  & \b[2]  & ((\b[1]  & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | ((\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & ~\a[0]  & ~\a[1] ))) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | (\b[0]  & \a[2]  & \a[3] ) | ((~\a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ? \b[1]  : (~\b[0]  | ~\b[1] ))) & (~\b[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] ) & (\a[0]  | ~\a[1]  | ~\b[2] ))) & \b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ))) & ((\a[2]  ^ (~\b[2]  | ((~\b[1]  | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & ((~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] )))) | ((~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[0]  | ~\a[2]  | ~\a[3] ))) & ((\a[3]  & \b[2] ) ^ ((\a[3]  & \b[1] ) | ~\a[2]  | \b[2] ))) | ((~\a[2]  ^ (~\b[2]  | ((~\b[1]  | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & ((~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] )))) & ((\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | (\b[0]  & \a[2]  & \a[3] )) & (~\a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ? \b[1]  : (~\b[0]  | ~\b[1] ))) & (~\b[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | \a[0]  | \a[1] ) & (\a[0]  | ~\a[1]  | ~\b[2] ))) & \b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[3]  | ~\b[1] ) & \a[2]  & ~\b[2] )) & (\a[2]  ^ (((\b[1]  ? (\b[0]  | ~\b[2] ) : \b[2] ) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] )) | (\b[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & ~\a[0]  & ~\a[1] ) | ((\a[1]  ^ ~\a[2] ) & \a[0]  & \b[2] ) | (\b[1]  & ~\a[0]  & \a[1] ))) & (~\b[1]  | (~\a[1]  & (\a[2]  | \a[3] ))) & (\b[1]  | (\a[1]  & \b[2] )) & \a[0]  & \b[0] ;
endmodule

