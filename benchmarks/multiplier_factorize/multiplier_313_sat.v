// Benchmark "multiplier_313_sat" written by ABC on Fri Jan 27 14:37:07 2023

module multiplier_313_sat ( 
    \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \a[7] , \b[0] ,
    \b[1] , \b[2] , \b[3] , \b[4] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \a[7] ,
    \b[0] , \b[1] , \b[2] , \b[3] , \b[4] ;
  output sat;
  wire new_n17_, new_n18_, new_n19_, new_n20_, new_n21_, new_n22_, new_n23_,
    new_n24_, new_n25_, new_n26_, new_n27_, new_n28_, new_n29_, new_n30_,
    new_n31_, new_n32_, new_n33_, new_n34_, new_n35_, new_n36_, new_n37_,
    new_n38_, new_n39_, new_n40_, new_n41_, new_n42_, new_n43_, new_n44_,
    new_n45_, new_n46_, new_n47_, new_n48_, new_n49_, new_n50_, new_n51_,
    new_n52_, new_n53_, new_n54_, new_n55_, new_n56_, new_n57_, new_n58_,
    new_n59_;
  assign sat = (((~new_n55_ & \a[2] ) | (((new_n43_ & \a[2] ) | (~new_n17_ & (~new_n43_ | ~\a[2] ) & (new_n43_ | \a[2] ))) & (new_n55_ | ~\a[2] ) & (~new_n55_ | \a[2] ))) ^ (~new_n59_ ^ (new_n58_ ^ \a[2] ))) & new_n42_ & (((~new_n43_ | ~\a[2] ) & (new_n17_ | (new_n43_ & \a[2] ) | (~new_n43_ & ~\a[2] ))) ^ (~new_n55_ ^ \a[2] ));
  assign new_n17_ = (~new_n22_ | ~\a[2] ) & (((~\a[2]  | (new_n18_ & ~new_n23_) | (~new_n18_ & new_n23_)) & (new_n33_ | (\a[2]  & (~new_n18_ | new_n23_) & (new_n18_ | ~new_n23_)) | (~\a[2]  & (~new_n18_ ^ ~new_n23_)))) | (new_n22_ & \a[2] ) | (~new_n22_ & ~\a[2] ));
  assign new_n18_ = ~new_n19_ ^ ~new_n21_;
  assign new_n19_ = \a[5]  ^ (((\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~new_n20_ ^ ~\b[4] )) & (~\b[3]  | (\a[3]  ^ \a[4] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  ^ ~\a[3] )) & (~\b[4]  | (~\a[2]  ^ ~\a[3] ) | (\a[3]  & \a[4] ) | (~\a[3]  & ~\a[4] )));
  assign new_n20_ = (~\b[3]  | ~\b[4] ) & ((\b[3]  & \b[4] ) | (~\b[3]  & ~\b[4] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))));
  assign new_n21_ = (~\b[1]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ ~\a[7] )) & (~\b[2]  | \a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & ((\b[2]  ^ (\b[0]  | ~\b[1] )) | ~\a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[0]  | (~\a[5]  ^ ~\a[6] ) | ~\a[6]  | ~\a[7] );
  assign new_n22_ = new_n31_ ^ ((~new_n19_ & ~new_n21_) | (~new_n23_ & (new_n19_ | new_n21_) & (~new_n19_ | ~new_n21_)));
  assign new_n23_ = (new_n27_ | (\a[5]  ^ (new_n26_ & (~new_n24_ | ~new_n25_)))) & ((~new_n28_ & (new_n29_ | ~new_n30_)) | (~new_n27_ & (~\a[5]  ^ (new_n26_ & (~new_n24_ | ~new_n25_)))) | (new_n27_ & (~\a[5]  | ~new_n26_ | (new_n24_ & new_n25_)) & (\a[5]  | (new_n26_ & (~new_n24_ | ~new_n25_)))));
  assign new_n24_ = (\b[3]  ^ \b[4] ) ^ ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )));
  assign new_n25_ = (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[4]  | ~\a[5] ) & (\a[4]  | \a[5] );
  assign new_n26_ = (~\b[2]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & (~\b[3]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[4]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] ));
  assign new_n27_ = (~\b[0]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ ~\a[7] )) & (~\b[1]  | \a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & ((\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] ) | ~\a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ));
  assign new_n28_ = \b[0]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ) & (~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] ));
  assign new_n29_ = \a[5]  ^ (((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & (~\b[2]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[3]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )));
  assign new_n30_ = (\b[0]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] )) ^ ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[0]  & \b[1] ) | (~\b[0]  & ~\b[1] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )));
  assign new_n31_ = ~new_n32_ ^ (~\a[5]  ^ (~\b[4]  | ((new_n20_ | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & ((\a[3]  ^ \a[4] ) | (\a[2]  ^ \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )))));
  assign new_n32_ = (~\a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[3]  | \a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[1]  | (~\a[5]  ^ ~\a[6] ) | ~\a[6]  | ~\a[7] ) & (~\b[2]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ ~\a[7] ));
  assign new_n33_ = (~new_n34_ | ~\a[2] ) & (new_n35_ | (~new_n34_ & ~\a[2] ) | (new_n34_ & \a[2] ));
  assign new_n34_ = (new_n28_ | (~new_n29_ & new_n30_)) ^ (~new_n27_ ^ (~\a[5]  ^ (new_n26_ & (~new_n24_ | ~new_n25_))));
  assign new_n35_ = (new_n36_ | (~new_n29_ & new_n30_) | (new_n29_ & ~new_n30_)) & (((new_n37_ | ~new_n38_) & (((new_n39_ | ~new_n40_) & (new_n41_ | (~new_n39_ & new_n40_) | (new_n39_ & ~new_n40_))) | (~new_n37_ & new_n38_) | (new_n37_ & ~new_n38_))) | (~new_n36_ & (new_n29_ | ~new_n30_) & (~new_n29_ | new_n30_)) | (new_n36_ & (new_n29_ ^ new_n30_)));
  assign new_n36_ = \a[2]  ^ (~\b[4]  | ((\a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[3]  | ~\b[4] ) & ((\b[3]  & \b[4] ) | (~\b[3]  & ~\b[4] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))))))));
  assign new_n37_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (~\b[4]  ^ ((\b[3]  & \b[4] ) | ((~\b[3]  | ~\b[4] ) & (\b[3]  | \b[4] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] ))))))) & (~\b[3]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[4]  | \a[0]  | ~\a[1] ));
  assign new_n38_ = ((~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (~\a[2]  ^ ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & (~\b[1]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[2]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] )))) ^ (~\a[5]  | ((~\b[0]  | (~\a[2]  ^ ~\a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  ^ \a[5] )) & ((~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] )) & \a[5]  & (~\b[0]  | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ))));
  assign new_n39_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[3]  | ~\b[4] ) & (\b[3]  | \b[4] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )))) | ((~\b[3]  ^ \b[4] ) & (~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[2]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[4]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[3]  | \a[0]  | ~\a[1] ));
  assign new_n40_ = ((\b[0]  & (\a[2]  ^ ~\a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | (\b[1]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[4]  ^ \a[5] )) | ((\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ) & (~\a[4]  | ~\a[5] ) & (\a[4]  | \a[5] ) & (\b[0]  | \b[1] ) & (~\b[0]  | ~\b[1] ))) ^ (\a[5]  & \b[0]  & (\a[2]  | \a[3] ) & (~\a[2]  | ~\a[3] ));
  assign new_n41_ = (~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))) & ((\a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & (~\b[2]  ^ (\b[0]  | ~\b[1] ))) | (\b[1]  & ~\a[0]  & \a[1] ) | (\b[0]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[0]  & ~\a[0]  & \a[1] ) | ((\b[0]  | \b[1] ) & (~\b[0]  | ~\b[1] ) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | ~\a[2]  | (\a[0]  & \b[0] ) | (\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))) | ((~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )) & (~\a[2]  | (((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & ~\a[0]  & \a[1] )) & (\a[2]  | ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))));
  assign new_n42_ = (((~new_n22_ | ~\a[2] ) & (((~\a[2]  | (new_n18_ & ~new_n23_) | (~new_n18_ & new_n23_)) & (new_n33_ | (\a[2]  & (~new_n18_ | new_n23_) & (new_n18_ | ~new_n23_)) | (~\a[2]  & (~new_n18_ ^ ~new_n23_)))) | (new_n22_ & \a[2] ) | (~new_n22_ & ~\a[2] ))) ^ (new_n43_ ^ \a[2] )) & (((~\a[2]  | (new_n18_ & ~new_n23_) | (~new_n18_ & new_n23_)) & (new_n33_ | (\a[2]  & (~new_n18_ | new_n23_) & (new_n18_ | ~new_n23_)) | (~\a[2]  & (~new_n18_ ^ ~new_n23_)))) ^ (new_n22_ ^ \a[2] )) & (~new_n33_ | (\a[2]  ^ (new_n18_ ^ ~new_n23_))) & new_n46_ & (new_n33_ | (\a[2]  & (~new_n18_ | new_n23_) & (new_n18_ | ~new_n23_)) | (~\a[2]  & (~new_n18_ ^ ~new_n23_)));
  assign new_n43_ = new_n45_ ^ (new_n44_ | (new_n31_ & ((~new_n19_ & ~new_n21_) | (~new_n23_ & (new_n19_ | new_n21_) & (~new_n19_ | ~new_n21_)))));
  assign new_n44_ = ~new_n32_ & (~\a[5]  ^ (~\b[4]  | ((new_n20_ | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )) & ((\a[3]  ^ \a[4] ) | (\a[2]  ^ \a[3] ) | (~\a[4]  & ~\a[5] ) | (\a[4]  & \a[5] )))));
  assign new_n45_ = \a[5]  ^ ((new_n24_ & \a[7]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[4]  & ~\a[7]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )) | (\b[2]  & (\a[5]  ^ ~\a[6] ) & \a[6]  & \a[7] ) | (\b[3]  & (\a[5]  ^ ~\a[6] ) & (~\a[6]  ^ ~\a[7] )));
  assign new_n46_ = ((~new_n47_ & (((new_n37_ | ~new_n38_) & (new_n48_ | (~new_n37_ & new_n38_) | (new_n37_ & ~new_n38_))) | new_n47_ | new_n49_)) ^ (new_n34_ ^ \a[2] )) & (((new_n37_ | ~new_n38_) & (new_n48_ | (~new_n37_ & new_n38_) | (new_n37_ & ~new_n38_))) ^ (~new_n47_ & ~new_n49_)) & (~new_n48_ | (~new_n37_ ^ new_n38_)) & new_n50_ & (new_n48_ | (~new_n37_ & new_n38_) | (new_n37_ & ~new_n38_));
  assign new_n47_ = ~new_n36_ & (new_n29_ | ~new_n30_) & (~new_n29_ | new_n30_);
  assign new_n48_ = (new_n39_ | ~new_n40_) & (new_n41_ | (~new_n39_ & new_n40_) | (new_n39_ & ~new_n40_));
  assign new_n49_ = new_n36_ & (new_n29_ ^ new_n30_);
  assign new_n50_ = (~new_n41_ | (~new_n39_ ^ new_n40_)) & (new_n41_ | (~new_n39_ & new_n40_) | (new_n39_ & ~new_n40_)) & ~new_n52_ & ~new_n51_ & ~new_n53_ & new_n54_;
  assign new_n51_ = (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[1]  | \a[0]  | ~\a[1] ) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] ) & ((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & \a[2]  & (~\a[0]  | ~\b[0] ) & (~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))) & ((\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] )) | (\a[2]  & (((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )) | (~\a[2]  & ((((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & ~\a[0]  & \a[1] ))));
  assign new_n52_ = ((\a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ) & (~\b[2]  ^ (\b[0]  | ~\b[1] ))) | (\b[1]  & ~\a[0]  & \a[1] ) | (\b[0]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[0]  & ~\a[0]  & \a[1] ) | ((\b[0]  | \b[1] ) & (~\b[0]  | ~\b[1] ) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | ~\a[2]  | (\a[0]  & \b[0] )) & ((\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[2]  ^ ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))) | ((~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )) & (~\a[2]  | (((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[2]  & ~\a[0]  & \a[1] )) & (\a[2]  | ((((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] )))) | (\a[2]  & ((((~\b[0]  & ~\b[1] ) | (\b[0]  & \b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))) | ~\a[0]  | ~\b[0] )) | (((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[1]  | \a[0]  | ~\a[1] ) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] ))) ? ((\b[0]  & ~\a[0]  & \a[1] ) | ((\b[0]  | \b[1] ) & (~\b[0]  | ~\b[1] ) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & \a[0]  & (\a[1]  ^ ~\a[2] ))) : ~\a[2] ));
  assign new_n53_ = ~\a[7]  & ~\a[1]  & ~\a[2]  & ~\a[3]  & ~\a[4]  & ~\a[5]  & ~\a[6] ;
  assign new_n54_ = \a[0]  & \b[0]  & (\b[1]  | \b[2]  | \b[3]  | \b[4] );
  assign new_n55_ = ((new_n56_ | ~\a[5] ) & ((~new_n44_ & (~new_n31_ | ((new_n19_ | new_n21_) & (new_n23_ | (new_n19_ & new_n21_) | (~new_n19_ & ~new_n21_))))) | (~new_n56_ & \a[5] ) | (new_n56_ & ~\a[5] ))) ^ (~new_n57_ ^ \a[5] );
  assign new_n56_ = (~new_n24_ | ~\a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[4]  | \a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[2]  | (~\a[5]  ^ ~\a[6] ) | ~\a[6]  | ~\a[7] ) & (~\b[3]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ ~\a[7] ));
  assign new_n57_ = (~\b[3]  | (~\a[5]  ^ ~\a[6] ) | ~\a[6]  | ~\a[7] ) & (~\b[4]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ ~\a[7] )) & (~\a[7]  | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ) | (~new_n20_ ^ ~\b[4] ));
  assign new_n58_ = (((new_n56_ | ~\a[5] ) & ((~new_n44_ & (~new_n31_ | ((new_n19_ | new_n21_) & (new_n23_ | (new_n19_ & new_n21_) | (~new_n19_ & ~new_n21_))))) | (~new_n56_ & \a[5] ) | (new_n56_ & ~\a[5] ))) | (~new_n57_ & ~\a[5] )) & (((new_n44_ | (new_n31_ & ((~new_n19_ & ~new_n21_) | (~new_n23_ & (~new_n19_ | ~new_n21_) & (new_n19_ | new_n21_))))) & (new_n56_ | ~\a[5] ) & (~new_n56_ | \a[5] )) | (new_n57_ & \a[5] ));
  assign new_n59_ = \b[4]  & (((\a[5]  ^ ~\a[6] ) & \a[6]  & \a[7] ) | (~new_n20_ & \a[7]  & (\a[5]  | \a[6] ) & (~\a[5]  | ~\a[6] )));
endmodule


