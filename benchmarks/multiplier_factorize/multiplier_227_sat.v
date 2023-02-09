// Benchmark "multiplier_227_sat" written by ABC on Fri Jan 27 15:01:22 2023

module multiplier_227_sat ( 
    \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \b[0] , \b[1] ,
    \b[2] , \b[3] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \b[0] ,
    \b[1] , \b[2] , \b[3] ;
  output sat;
  wire new_n15_, new_n16_, new_n17_, new_n18_, new_n19_, new_n20_, new_n21_,
    new_n22_, new_n23_, new_n24_, new_n25_, new_n26_, new_n27_, new_n28_,
    new_n29_, new_n30_, new_n31_, new_n32_, new_n33_, new_n34_, new_n35_,
    new_n36_, new_n37_, new_n38_, new_n39_, new_n40_, new_n41_, new_n42_,
    new_n43_, new_n44_, new_n45_, new_n46_;
  assign sat = (new_n15_ ^ new_n33_) & ~new_n34_ & ~new_n35_ & new_n36_;
  assign new_n15_ = (~\a[2]  | (~new_n32_ & (new_n30_ | new_n31_) & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_))) | (new_n32_ & ((~new_n30_ & ~new_n31_) | (~new_n16_ & (new_n30_ | new_n31_) & (~new_n30_ | ~new_n31_))))) & (((~\a[2]  | (~new_n16_ & (new_n30_ | new_n31_) & (~new_n30_ | ~new_n31_)) | (new_n16_ & (new_n30_ ^ ~new_n31_))) & ((~new_n20_ & (new_n21_ | ~new_n29_)) | (\a[2]  & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_)) & (~new_n16_ | (~new_n30_ ^ ~new_n31_))) | (~\a[2]  & (new_n16_ ^ (~new_n30_ ^ ~new_n31_))))) | (\a[2]  & (new_n32_ | (~new_n30_ & ~new_n31_) | (~new_n16_ & (new_n30_ | new_n31_) & (~new_n30_ | ~new_n31_))) & (~new_n32_ | ((new_n30_ | new_n31_) & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_))))) | (~\a[2]  & (new_n32_ ^ ((new_n30_ | new_n31_) & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_))))));
  assign new_n16_ = ((new_n17_ ^ \a[5] ) | ((~\b[1]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[0]  | ~\a[5]  | ~\a[6] ))) & (((~new_n18_ | ~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & ((~new_n19_ ^ ~\a[5] ) | (new_n18_ & \b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (~new_n18_ & (~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ))))) | ((~new_n17_ ^ \a[5] ) & ((\b[1]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[0]  & \a[5]  & \a[6] ))) | ((~new_n17_ | ~\a[5] ) & (new_n17_ | \a[5] ) & (~\b[1]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[0]  | ~\a[5]  | ~\a[6] )));
  assign new_n17_ = ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[2]  & \b[0]  & \b[1] ) | (~\b[1]  & \b[2] )) & (~\b[2]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[3]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ));
  assign new_n18_ = (~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[1]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\b[2]  & \b[0]  & \b[1] ) | ((~\b[1]  | \b[2] ) & (~\b[0]  | ~\b[1] ) & (\b[1]  | ~\b[2] ))) & (~\b[0]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[2]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ));
  assign new_n19_ = ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[3]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ));
  assign new_n20_ = \a[2]  & (((~new_n18_ | ~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & ((new_n19_ ^ \a[5] ) | (new_n18_ & \b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (~new_n18_ & (~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ))))) | ((~new_n17_ ^ \a[5] ) & ((\b[1]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[0]  & \a[5]  & \a[6] ))) | ((~new_n17_ | ~\a[5] ) & (new_n17_ | \a[5] ) & (~\b[1]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[0]  | ~\a[5]  | ~\a[6] ))) & ((new_n18_ & \b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | ((~new_n19_ ^ \a[5] ) & (~new_n18_ | ~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (new_n18_ | (\b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )))) | ((~new_n17_ ^ \a[5] ) ^ ((\b[1]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[0]  & \a[5]  & \a[6] ))));
  assign new_n21_ = ((~new_n22_ & new_n23_) | (new_n22_ & ~new_n23_) | ~\a[2] ) & (((new_n24_ | ~new_n25_) & (((new_n26_ | ~new_n27_) & (~new_n28_ | (~new_n26_ & new_n27_) | (new_n26_ & ~new_n27_))) | (~new_n24_ & new_n25_) | (new_n24_ & ~new_n25_))) | ((new_n22_ | ~new_n23_) & (~new_n22_ | new_n23_) & \a[2] ) | ((new_n22_ ^ new_n23_) & ~\a[2] ));
  assign new_n22_ = \a[5]  ^ (((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[3]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )));
  assign new_n23_ = (\b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) ^ ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[0]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[2]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\b[2]  & \b[0]  & \b[1] ) | ((~\b[1]  | \b[2] ) & (~\b[0]  | ~\b[1] ) & (\b[1]  | ~\b[2] ))));
  assign new_n24_ = \a[2]  ^ (~\b[3]  | ((\a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))))));
  assign new_n25_ = (((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~\b[2]  & \b[0]  & \b[1] ) | ((~\b[1]  | \b[2] ) & (~\b[0]  | ~\b[1] ) & (\b[1]  | ~\b[2] ))) & (~\b[0]  | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & (~\b[2]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] ))) ^ (~\a[5]  | ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & ((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[1]  | (~\a[4]  ^ ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ))));
  assign new_n26_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | (~\b[1]  & \b[2] ) | (~\b[2]  & \b[0]  & \b[1] )) & (~\b[2]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (\a[0]  | ~\a[1]  | ~\b[3] ));
  assign new_n27_ = ((\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | ((\b[0]  | \b[1] ) & (~\b[0]  | ~\b[1] ) & (\a[4]  | \a[5] ) & (~\a[4]  | ~\a[5] ) & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | (\b[1]  & (\a[4]  ^ ~\a[5] ) & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ))) ^ (\a[5]  & \b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ));
  assign new_n28_ = ((\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] )) | (\a[2]  & (((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (\a[0]  | ~\a[1]  | ~\b[2] ) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))) | (~\a[2]  & ((((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (~\a[0]  & \a[1]  & \b[2] ) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] ))))) & ((\b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (\a[0]  | ~\a[1]  | ~\b[2] ) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))))) | (\a[2]  & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & ((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] ) & (~\a[0]  | ~\b[0] ) & ((~\b[2]  & \b[0]  & \b[1] ) | ((~\b[1]  | \b[2] ) & (~\b[0]  | ~\b[1] ) & (\b[1]  | ~\b[2] )) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (\a[0]  | ~\a[1]  | ~\b[1] )));
  assign new_n29_ = \a[2]  ^ (((new_n18_ & \b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | ((~new_n19_ ^ \a[5] ) & (~new_n18_ | ~\b[0]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (new_n18_ | (\b[0]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] ))))) ^ ((~new_n17_ ^ \a[5] ) ^ ((\b[1]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[0]  & \a[5]  & \a[6] ))));
  assign new_n30_ = \a[5]  ^ (~\b[3]  | (((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  ^ ~\a[4] )) & ((~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))))));
  assign new_n31_ = (~\b[2]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[1]  | ~\a[5]  | ~\a[6] );
  assign new_n32_ = \a[5]  ? ((~\a[6]  | ~\b[2] ) & (~\b[3]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ))) : (\a[6]  & \b[3] );
  assign new_n33_ = (\a[2]  ^ ((\a[5]  & ((\b[3]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\a[6]  & \b[2] ))) | (((~new_n30_ & ((\b[2]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[1]  & \a[5]  & \a[6] ))) | (~new_n16_ & (new_n30_ | ((~\b[2]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[1]  | ~\a[5]  | ~\a[6] ))) & (~new_n30_ | (\b[2]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[1]  & \a[5]  & \a[6] )))) & (~\a[5]  | ((~\b[3]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\a[6]  | ~\b[2] ))) & (\a[5]  | (\a[6]  & \b[3] ))))) ^ (\a[5]  & (~\a[6]  | ~\b[3] ));
  assign new_n34_ = ((\a[2]  & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_)) & (~new_n16_ | (~new_n30_ ^ ~new_n31_))) | ((new_n20_ | (~new_n21_ & new_n29_)) & (~\a[2]  | (~new_n16_ & (new_n30_ | new_n31_) & (~new_n30_ | ~new_n31_)) | (new_n16_ & (new_n30_ ^ ~new_n31_))) & (\a[2]  | (~new_n16_ ^ (~new_n30_ ^ ~new_n31_))))) ^ (\a[2]  ^ (~new_n32_ ^ ((new_n30_ | new_n31_) & (new_n16_ | (~new_n30_ & ~new_n31_) | (new_n30_ & new_n31_)))));
  assign new_n35_ = (new_n20_ | (~new_n21_ & new_n29_)) ^ (\a[2]  ^ (~new_n16_ ^ (~new_n30_ ^ ~new_n31_)));
  assign new_n36_ = (new_n29_ | new_n37_ | ((new_n38_ | (~new_n24_ & new_n25_)) & ~new_n37_ & ~new_n39_)) & (~new_n29_ | (~new_n37_ & ((~new_n38_ & (new_n24_ | ~new_n25_)) | new_n37_ | new_n39_))) & (new_n38_ | (~new_n24_ & new_n25_) | (~new_n37_ & ~new_n39_)) & ((~new_n38_ & (new_n24_ | ~new_n25_)) | new_n37_ | new_n39_) & ~new_n40_ & ~new_n38_ & new_n41_;
  assign new_n37_ = \a[2]  & (new_n22_ | ~new_n23_) & (~new_n22_ | new_n23_);
  assign new_n38_ = (new_n24_ | ~new_n25_) & (~new_n24_ | new_n25_) & ((~new_n26_ & new_n27_) | (new_n28_ & (new_n26_ | ~new_n27_) & (~new_n26_ | new_n27_)));
  assign new_n39_ = ~\a[2]  & (new_n22_ ^ new_n23_);
  assign new_n40_ = (new_n24_ ^ new_n25_) & (new_n26_ | ~new_n27_) & (~new_n28_ | (~new_n26_ & new_n27_) | (new_n26_ & ~new_n27_));
  assign new_n41_ = (~new_n28_ ^ (~new_n26_ ^ new_n27_)) & ~new_n42_ & ~new_n43_ & ~new_n44_ & ~new_n45_ & new_n46_;
  assign new_n42_ = (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) ^ (\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (\a[0]  | ~\a[1]  | ~\b[2] ) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))));
  assign new_n43_ = \a[2]  ^ ((\a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & (\b[2]  | ~\b[0]  | ~\b[1] ) & ((\b[1]  & ~\b[2] ) | (\b[0]  & \b[1] ) | (~\b[1]  & \b[2] ))) | (\b[2]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[1]  & ~\a[0]  & \a[1] ) | (\b[0]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )));
  assign new_n44_ = ((~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & ((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] )) ? ~\a[2]  : (\a[2]  & \a[0]  & \b[0] );
  assign new_n45_ = ~\a[5]  & ~\a[6]  & ~\a[3]  & ~\a[4]  & ~\a[1]  & ~\a[2] ;
  assign new_n46_ = \a[0]  & \b[0]  & (\b[1]  | \b[2]  | \b[3] );
endmodule

