// Benchmark "multiplier_107_sat" written by ABC on Fri Jan 27 14:56:40 2023

module multiplier_107_sat ( 
    \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \b[0] , \b[1] , \b[2] ,
    \b[3] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \b[0] , \b[1] ,
    \b[2] , \b[3] ;
  output sat;
  wire new_n14_, new_n15_, new_n16_, new_n17_, new_n18_, new_n19_, new_n20_,
    new_n21_, new_n22_, new_n23_, new_n24_, new_n25_, new_n26_, new_n27_,
    new_n28_, new_n29_, new_n30_, new_n31_, new_n32_;
  assign sat = (new_n32_ ^ ((~new_n31_ | ~\a[2] ) & (((~new_n18_ | ~\a[2] ) & (((~new_n16_ | ~\a[2] ) & (new_n30_ | (new_n16_ & \a[2] ) | (~new_n16_ & ~\a[2] ))) | (new_n18_ & \a[2] ) | (~new_n18_ & ~\a[2] ))) | (new_n31_ & \a[2] ) | (~new_n31_ & ~\a[2] )))) & new_n14_ & (((~new_n18_ | ~\a[2] ) & (((~new_n16_ | ~\a[2] ) & (new_n30_ | (new_n16_ & \a[2] ) | (~new_n16_ & ~\a[2] ))) | (new_n18_ & \a[2] ) | (~new_n18_ & ~\a[2] ))) ^ (new_n31_ ^ \a[2] ));
  assign new_n14_ = ~new_n17_ & (new_n15_ | (~new_n19_ & new_n20_) | (((~new_n21_ & new_n22_) | (new_n23_ & (new_n21_ | ~new_n22_) & (~new_n21_ | new_n22_))) & (new_n19_ | ~new_n20_) & (~new_n19_ | new_n20_))) & (~new_n15_ | ((new_n19_ | ~new_n20_) & (((new_n21_ | ~new_n22_) & (~new_n23_ | (~new_n21_ & new_n22_) | (new_n21_ & ~new_n22_))) | (~new_n19_ & new_n20_) | (new_n19_ & ~new_n20_)))) & ((~new_n21_ & new_n22_) | (new_n23_ & (new_n21_ | ~new_n22_) & (~new_n21_ | new_n22_)) | (~new_n19_ ^ new_n20_)) & (((new_n21_ | ~new_n22_) & (~new_n23_ | (~new_n21_ & new_n22_) | (new_n21_ & ~new_n22_))) | (~new_n19_ & new_n20_) | (new_n19_ & ~new_n20_)) & new_n24_ & (~new_n23_ ^ (~new_n21_ ^ new_n22_));
  assign new_n15_ = ~new_n16_ ^ ~\a[2] ;
  assign new_n16_ = (~\a[5]  ^ (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) ^ ((\a[5]  & \b[0] ) ^ ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))));
  assign new_n17_ = ((new_n16_ & \a[2] ) | (((~new_n19_ & new_n20_) | (((~new_n21_ & new_n22_) | (new_n23_ & (new_n21_ | ~new_n22_) & (~new_n21_ | new_n22_))) & (new_n19_ | ~new_n20_) & (~new_n19_ | new_n20_))) & (~new_n16_ | ~\a[2] ) & (new_n16_ | \a[2] ))) ^ (new_n18_ ^ \a[2] );
  assign new_n18_ = ((\a[5]  & \b[0]  & (~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))) | ((~\a[5]  ^ (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) & (~\a[5]  | ~\b[0]  | (\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[0]  | ~\b[1] ) & (\b[0]  | \b[1] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((~\b[1]  & \b[2] ) | ((\b[2]  | ~\b[0]  | ~\b[1] ) & \b[1]  & (\b[0]  | ~\b[2] )))) | (\b[0]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[2]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[1]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] ))) & ((\a[5]  & \b[0] ) | ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))))) ^ ((\a[5]  & ~\b[1] ) ^ (((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & (\b[1]  | ~\b[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] )) | (\b[2]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[3]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] ))));
  assign new_n19_ = \a[2]  ^ (~\b[3]  | ((\a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ))));
  assign new_n20_ = (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))) ^ (~\a[5]  | ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ))));
  assign new_n21_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[1]  & \b[2] ) | (~\b[2]  & \b[0]  & \b[1] )) & (~\b[2]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | \a[0]  | ~\a[1] ));
  assign new_n22_ = ((\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[0]  | ~\b[1] ) & (\b[0]  | \b[1] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] ))) ^ ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & \a[5]  & \b[0] );
  assign new_n23_ = ((\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | (\a[2]  & (((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))) | (~\a[2]  & ((((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & ~\a[0]  & \a[1] ) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] ))))) & ((\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))))) | ((((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | ~\a[1] ) & \a[2]  & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & ((\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] ) & (~\a[0]  | ~\b[0] )));
  assign new_n24_ = (new_n25_ | ~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )) & (~new_n25_ | (\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ))) & ~new_n26_ & ~new_n27_ & ~new_n28_ & new_n29_;
  assign new_n25_ = \a[2]  ^ ((~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))));
  assign new_n26_ = \a[2]  ^ ((\a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & ((~\b[1]  & \b[2] ) | ((\b[2]  | ~\b[0]  | ~\b[1] ) & \b[1]  & (\b[0]  | ~\b[2] )))) | (\b[2]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[1]  & ~\a[0]  & \a[1] ) | (\b[0]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )));
  assign new_n27_ = ((~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[0]  | \a[0]  | ~\a[1] )) ? ~\a[2]  : (\a[2]  & \a[0]  & \b[0] );
  assign new_n28_ = ~\a[1]  & ~\a[2]  & ~\a[5]  & ~\a[3]  & ~\a[4] ;
  assign new_n29_ = \a[0]  & \b[0]  & (\b[1]  | \b[2]  | \b[3] );
  assign new_n30_ = (new_n19_ | ~new_n20_) & (((new_n21_ | ~new_n22_) & (~new_n23_ | (~new_n21_ & new_n22_) | (new_n21_ & ~new_n22_))) | (~new_n19_ & new_n20_) | (new_n19_ & ~new_n20_));
  assign new_n31_ = ((\b[3]  & (((\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )))))) ^ (\a[5]  & ~\b[2] )) ^ ((((\a[5]  & \b[0]  & (~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))) | ((~\a[5]  ^ (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) & (~\a[5]  | ~\b[0]  | (\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[0]  | ~\b[1] ) & (\b[0]  | \b[1] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((~\b[1]  & \b[2] ) | ((\b[2]  | ~\b[0]  | ~\b[1] ) & \b[1]  & (\b[0]  | ~\b[2] )))) | (\b[0]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[2]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[1]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] ))) & ((\a[5]  & \b[0] ) | ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))))) & (~\a[5]  | \b[1]  | (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[1]  & \b[2] ) | (~\b[2]  & \b[0]  & \b[1] )) & (~\b[2]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) & ((\a[5]  & ~\b[1] ) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & (\b[1]  | ~\b[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] )) | (\b[2]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[3]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )))) | (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[1]  & \b[2] ) | (~\b[2]  & \b[0]  & \b[1] )) & (~\b[2]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & \a[5]  & \b[1] ));
  assign new_n32_ = ((((~\b[3]  | (((~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )))))) ^ (\a[5]  & ~\b[2] )) | ((((~\a[5]  | ~\b[0]  | (\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[0]  | ~\b[1] ) & (\b[0]  | \b[1] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((~\b[1]  & \b[2] ) | ((\b[2]  | ~\b[0]  | ~\b[1] ) & \b[1]  & (\b[0]  | ~\b[2] )))) | (\b[0]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[2]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[1]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] ))) & ((\a[5]  ^ (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) | (\a[5]  & \b[0]  & (~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | ((\b[1]  | ~\b[2] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ~\b[1]  | (~\b[0]  & \b[2] )))) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  ^ ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))) | ((~\a[5]  | ~\b[0] ) & ((\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[0]  | ~\b[1] ) & (\b[0]  | \b[1] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | ~\a[5]  | (\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((~\b[1]  & \b[2] ) | ((\b[2]  | ~\b[0]  | ~\b[1] ) & \b[1]  & (\b[0]  | ~\b[2] )))) | (\b[0]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[2]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  ^ ~\a[5] )) | (\b[1]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )))))) | (\a[5]  & ~\b[1]  & (((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & (\b[1]  | ~\b[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] )) | (\b[2]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[3]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )))) | ((~\a[5]  | \b[1] ) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[1]  & \b[2] ) | (~\b[2]  & \b[0]  & \b[1] )) & (~\b[2]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & (~\b[3]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )))) & (((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & (\b[1]  | ~\b[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] )) | (\b[2]  & (\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | (\b[3]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ~\a[5]  | ~\b[1] ))) & (~\a[5]  | ~\b[2]  | (\b[3]  & (((\a[3]  ^ ~\a[4] ) & (\a[2]  ^ ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )))))))) ^ (\a[2]  ^ (~\a[5]  | \b[3] ));
endmodule


