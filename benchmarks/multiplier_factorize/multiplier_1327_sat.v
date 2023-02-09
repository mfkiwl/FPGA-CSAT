// Benchmark "multiplier_1327_sat" written by ABC on Fri Jan 27 17:09:49 2023

module multiplier_1327_sat ( 
    \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \a[7] , \a[8] ,
    \a[9] , \b[0] , \b[1] , \b[2] , \b[3] , \b[4] , \b[5] ,
    sat  );
  input  \a[0] , \a[1] , \a[2] , \a[3] , \a[4] , \a[5] , \a[6] , \a[7] ,
    \a[8] , \a[9] , \b[0] , \b[1] , \b[2] , \b[3] , \b[4] , \b[5] ;
  output sat;
  wire new_n20_, new_n21_, new_n22_, new_n23_, new_n24_, new_n25_, new_n26_,
    new_n27_, new_n28_, new_n29_, new_n30_, new_n31_, new_n32_, new_n33_,
    new_n34_, new_n35_, new_n36_, new_n37_, new_n38_, new_n39_, new_n40_,
    new_n41_, new_n42_, new_n43_, new_n44_, new_n45_, new_n46_, new_n47_,
    new_n48_, new_n49_, new_n50_, new_n51_, new_n52_, new_n53_, new_n54_,
    new_n55_, new_n56_, new_n57_, new_n58_, new_n59_, new_n60_, new_n61_,
    new_n62_, new_n63_, new_n64_, new_n65_, new_n66_, new_n67_, new_n68_,
    new_n69_, new_n70_, new_n71_, new_n72_, new_n73_, new_n74_, new_n75_,
    new_n76_, new_n77_, new_n78_, new_n79_, new_n80_, new_n81_, new_n82_,
    new_n83_, new_n84_, new_n85_, new_n86_, new_n87_, new_n88_, new_n89_,
    new_n90_;
  assign sat = (((\a[2]  & (new_n88_ | new_n89_ | new_n90_) & (~new_n88_ | (~new_n89_ & ~new_n90_))) | (((new_n74_ & \a[2] ) | (~new_n20_ & (~new_n74_ | ~\a[2] ) & (new_n74_ | \a[2] ))) & (~\a[2]  | (~new_n88_ & ~new_n89_ & ~new_n90_) | (new_n88_ & (new_n89_ | new_n90_))) & (\a[2]  | (~new_n88_ ^ (~new_n89_ & ~new_n90_))))) ^ (~new_n69_ ^ (~new_n90_ & (new_n88_ | new_n89_ | new_n90_)))) & new_n73_ & (((~new_n74_ | ~\a[2] ) & (new_n20_ | (new_n74_ & \a[2] ) | (~new_n74_ & ~\a[2] ))) ^ (\a[2]  ^ (~new_n88_ ^ (~new_n89_ & ~new_n90_))));
  assign new_n20_ = (~new_n21_ | ~\a[2] ) & ((new_n21_ & \a[2] ) | (~new_n21_ & ~\a[2] ) | ((~new_n52_ | ~\a[2] ) & (new_n53_ | (new_n52_ & \a[2] ) | (~new_n52_ & ~\a[2] ))));
  assign new_n21_ = (new_n25_ ^ \a[5] ) ^ ((\a[5]  & (new_n28_ | (~new_n22_ & ~new_n51_) | (new_n22_ & new_n51_)) & (~new_n28_ | (~new_n22_ ^ ~new_n51_))) | (~new_n36_ & (~\a[5]  | (~new_n28_ & (new_n22_ | new_n51_) & (~new_n22_ | ~new_n51_)) | (new_n28_ & (new_n22_ ^ ~new_n51_))) & (\a[5]  | (~new_n28_ ^ (~new_n22_ ^ ~new_n51_)))));
  assign new_n22_ = new_n23_ ^ \a[8] ;
  assign new_n23_ = (~new_n24_ | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] )) & (~\b[3]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ \a[7] )) & (~\b[4]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  & \a[7] ) | (~\a[6]  & ~\a[7] )) & (~\b[5]  | (\a[7]  ^ \a[8] ) | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ));
  assign new_n24_ = ((\b[3]  & \b[4] ) | ((~\b[3]  | ~\b[4] ) & (\b[3]  | \b[4] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] ))))) ^ (~\b[4]  ^ ~\b[5] );
  assign new_n25_ = (((~new_n23_ ^ \a[8] ) & ((\b[2]  & (~\a[8]  ^ ~\a[9] )) | (\b[1]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] )))) | (~new_n28_ & ((new_n23_ ^ \a[8] ) | ((~\b[2]  | (\a[8]  ^ ~\a[9] )) & (~\b[1]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )))) & ((new_n23_ & \a[8] ) | (~new_n23_ & ~\a[8] ) | (\b[2]  & (~\a[8]  ^ ~\a[9] )) | (\b[1]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))))) ^ ((~new_n26_ ^ \a[8] ) ^ ((\b[3]  & (~\a[8]  ^ ~\a[9] )) | (\b[2]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))));
  assign new_n26_ = ((\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ) | (~new_n27_ ^ ~\b[5] )) & (~\b[4]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ \a[7] )) & (~\b[5]  | (~\a[5]  ^ ~\a[6] ) | (\a[6]  & \a[7] ) | (~\a[6]  & ~\a[7] ));
  assign new_n27_ = (~\b[4]  | ~\b[5] ) & (((~\b[3]  | ~\b[4] ) & ((\b[3]  & \b[4] ) | (~\b[3]  & ~\b[4] ) | ((~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] ))))) | (~\b[4]  & ~\b[5] ) | (\b[4]  & \b[5] ));
  assign new_n28_ = (new_n32_ | (\a[8]  ^ (new_n31_ & (~new_n29_ | ~new_n30_)))) & ((~new_n33_ & (new_n34_ | ~new_n35_)) | (~new_n32_ & (~\a[8]  ^ (new_n31_ & (~new_n29_ | ~new_n30_)))) | (new_n32_ & (~\a[8]  | ~new_n31_ | (new_n29_ & new_n30_)) & (\a[8]  | (new_n31_ & (~new_n29_ | ~new_n30_)))));
  assign new_n29_ = (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ) & (~\a[7]  | ~\a[8] ) & (\a[7]  | \a[8] );
  assign new_n30_ = (\b[3]  ^ \b[4] ) ^ ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )));
  assign new_n31_ = (~\b[2]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\a[5]  ^ \a[6] ) | (~\a[6]  ^ ~\a[7] )) & (~\b[3]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )) & (~\b[4]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] ));
  assign new_n32_ = (~\b[0]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )) & (~\b[1]  | (\a[8]  ^ ~\a[9] ));
  assign new_n33_ = \b[0]  & (~\a[8]  ^ ~\a[9] ) & (~\b[0]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )) & (~\b[1]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\b[0]  ^ \b[1] )) & \a[8]  & (~\b[0]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] )) & (~\b[0]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\a[5]  ^ \a[6] ) | (~\a[6]  ^ ~\a[7] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[2]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & (~\b[1]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] ));
  assign new_n34_ = \a[8]  ^ (((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[1]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\a[5]  ^ \a[6] ) | (~\a[6]  ^ ~\a[7] )) & (~\b[3]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & (~\b[2]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )));
  assign new_n35_ = (\b[0]  & (~\a[8]  ^ ~\a[9] )) ^ ((~\b[0]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )) & (~\b[1]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\b[0]  ^ \b[1] )) & \a[8]  & (~\b[0]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] )) & (~\b[0]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\a[5]  ^ \a[6] ) | (~\a[6]  ^ ~\a[7] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[2]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & (~\b[1]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )));
  assign new_n36_ = (~new_n37_ | new_n38_) & ((new_n37_ & ~new_n38_) | (~new_n37_ & new_n38_) | ((~new_n39_ | new_n40_) & (((new_n41_ | ~new_n50_) & (new_n44_ | (~new_n41_ & new_n50_) | (new_n41_ & ~new_n50_))) | (new_n39_ & ~new_n40_) | (~new_n39_ & new_n40_))));
  assign new_n37_ = (new_n33_ | (~new_n34_ & new_n35_)) ^ (~new_n32_ ^ (~\a[8]  ^ (new_n31_ & (~new_n29_ | ~new_n30_))));
  assign new_n38_ = \a[5]  ^ (~\b[5]  | ((new_n27_ | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] )) & ((\a[3]  ^ \a[4] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  ^ ~\a[3] ))));
  assign new_n39_ = new_n34_ ^ ~new_n35_;
  assign new_n40_ = \a[5]  ^ (((\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  & ~\a[3] ) | (\a[2]  & \a[3] ) | (~new_n27_ ^ ~\b[5] )) & (~\b[5]  | (~\a[2]  ^ ~\a[3] ) | (\a[3]  & \a[4] ) | (~\a[3]  & ~\a[4] )) & (~\b[4]  | (\a[3]  ^ \a[4] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\a[2]  ^ ~\a[3] )));
  assign new_n41_ = \a[5]  ^ (new_n43_ & (~new_n24_ | ~new_n42_));
  assign new_n42_ = (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[4]  | ~\a[5] ) & (\a[4]  | \a[5] );
  assign new_n43_ = (~\b[4]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[5]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & (~\b[3]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ));
  assign new_n44_ = (~new_n46_ | (\a[5]  ^ (new_n45_ & (~new_n30_ | ~new_n42_)))) & ((~new_n47_ & (new_n48_ | ~new_n49_)) | (new_n46_ & (~\a[5]  ^ (new_n45_ & (~new_n30_ | ~new_n42_)))) | (~new_n46_ & (~\a[5]  | ~new_n45_ | (new_n30_ & new_n42_)) & (\a[5]  | (new_n45_ & (~new_n30_ | ~new_n42_)))));
  assign new_n45_ = (~\b[3]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[4]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & (~\b[2]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ));
  assign new_n46_ = ((\b[0]  & (~\a[5]  ^ \a[6] ) & (\a[6]  | \a[7] ) & (~\a[6]  | ~\a[7] )) | (\b[1]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ) & (~\a[7]  ^ \a[8] )) | ((~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ) & (~\a[7]  | ~\a[8] ) & (\a[7]  | \a[8] ) & (\b[0]  ^ \b[1] ))) ^ (\a[8]  & \b[0]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ));
  assign new_n47_ = \b[0]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] ) & (~\b[0]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\b[0]  ^ \b[1] )) & \a[5]  & (~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[2]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & (~\b[1]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ));
  assign new_n48_ = \a[5]  ^ (((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[3]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & (~\b[2]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )));
  assign new_n49_ = (\b[0]  & (~\a[5]  | ~\a[6] ) & (\a[5]  | \a[6] )) ^ ((~\b[0]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\b[0]  ^ \b[1] )) & \a[5]  & (~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))) & (~\b[2]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & (~\b[1]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )));
  assign new_n50_ = ((~\b[0]  | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\a[5]  ^ \a[6] ) | (~\a[6]  ^ ~\a[7] )) & (~\b[1]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )) & (~\b[2]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (\b[2]  ^ (\b[0]  | ~\b[1] )))) ^ (~\a[8]  | ((~\b[0]  | (\a[5]  ^ \a[6] ) | (~\a[6]  & ~\a[7] ) | (\a[6]  & \a[7] )) & (~\b[1]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  ^ \a[8] )) & ((\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ) | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\b[0]  ^ \b[1] )) & \a[8]  & (~\b[0]  | (\a[5]  & \a[6] ) | (~\a[5]  & ~\a[6] ))));
  assign new_n51_ = (~\b[1]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )) & (~\b[2]  | (\a[8]  ^ ~\a[9] ));
  assign new_n52_ = ~new_n36_ ^ (\a[5]  ^ (~new_n28_ ^ (new_n22_ ^ new_n51_)));
  assign new_n53_ = (~new_n54_ | ~\a[2] ) & ((~new_n54_ & ~\a[2] ) | (new_n54_ & \a[2] ) | ((~new_n55_ | ~\a[2] ) & (((~new_n56_ | ~\a[2] ) & (new_n57_ | (new_n56_ & \a[2] ) | (~new_n56_ & ~\a[2] ))) | (new_n55_ & \a[2] ) | (~new_n55_ & ~\a[2] ))));
  assign new_n54_ = (new_n37_ ^ ~new_n38_) ^ ((new_n39_ & ~new_n40_) | (((~new_n41_ & new_n50_) | (~new_n44_ & (new_n41_ | ~new_n50_) & (~new_n41_ | new_n50_))) & (~new_n39_ | new_n40_) & (new_n39_ | ~new_n40_)));
  assign new_n55_ = (new_n39_ ^ ~new_n40_) ^ ((~new_n41_ & new_n50_) | (~new_n44_ & (new_n41_ | ~new_n50_) & (~new_n41_ | new_n50_)));
  assign new_n56_ = ~new_n44_ ^ (~new_n41_ ^ new_n50_);
  assign new_n57_ = (new_n58_ | ~new_n59_) & ((new_n58_ & ~new_n59_) | (~new_n58_ & new_n59_) | (~new_n60_ & (new_n61_ | ~new_n68_)));
  assign new_n58_ = \a[2]  ^ (~\b[5]  | ((new_n27_ | ~\a[0]  | (\a[1]  & \a[2] ) | (~\a[1]  & ~\a[2] )) & (\a[0]  | \a[1]  | (\a[1]  & \a[2] ) | (~\a[1]  & ~\a[2] ))));
  assign new_n59_ = (new_n47_ | (~new_n48_ & new_n49_)) ^ (new_n46_ ^ (~\a[5]  ^ (new_n45_ & (~new_n30_ | ~new_n42_))));
  assign new_n60_ = (~\a[2]  ^ (((~new_n27_ ^ ~\b[5] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[4]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[5]  | \a[0]  | ~\a[1] ))) & (new_n48_ | ~new_n49_) & (~new_n48_ | new_n49_);
  assign new_n61_ = (~new_n64_ | (\a[2]  ^ (new_n63_ & (~new_n24_ | ~new_n62_)))) & ((new_n64_ & (~\a[2]  ^ (new_n63_ & (~new_n24_ | ~new_n62_)))) | (~new_n64_ & (~\a[2]  | ~new_n63_ | (new_n24_ & new_n62_)) & (\a[2]  | (new_n63_ & (~new_n24_ | ~new_n62_)))) | ((new_n65_ | ~new_n66_) & (~new_n67_ | (~new_n65_ & new_n66_) | (new_n65_ & ~new_n66_))));
  assign new_n62_ = \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] );
  assign new_n63_ = (~\b[3]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[5]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[4]  | \a[0]  | ~\a[1] );
  assign new_n64_ = (~\a[5]  | ((~\b[0]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[1]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (~\b[0]  ^ \b[1] )) & \a[5]  & (~\b[0]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] )))) ^ ((~\b[0]  | (~\a[3]  ^ ~\a[4] ) | (\a[2]  ^ \a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] )) & (~\b[1]  | (\a[2]  ^ \a[3] ) | (~\a[3]  & ~\a[4] ) | (\a[3]  & \a[4] )) & (~\b[2]  | (\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  ^ \a[5] )) & ((\a[2]  & \a[3] ) | (~\a[2]  & ~\a[3] ) | (\a[4]  & \a[5] ) | (~\a[4]  & ~\a[5] ) | (\b[2]  ^ (\b[0]  | ~\b[1] ))));
  assign new_n65_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[3]  | ~\b[4] ) & (\b[3]  | \b[4] ) & ((\b[2]  & \b[3] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )))) | ((~\b[3]  ^ \b[4] ) & (~\b[2]  | ~\b[3] ) & ((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[2]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[4]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[3]  | \a[0]  | ~\a[1] ));
  assign new_n66_ = ((\b[0]  & (~\a[2]  ^ \a[3] ) & (\a[3]  | \a[4] ) & (~\a[3]  | ~\a[4] )) | (\b[1]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[4]  ^ \a[5] )) | ((~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[4]  | ~\a[5] ) & (\a[4]  | \a[5] ) & (\b[0]  ^ \b[1] ))) ^ (\a[5]  & \b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ));
  assign new_n67_ = ((\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] )) | (\a[2]  & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ))) | (~\a[2]  & ((\b[1]  & ~\a[0]  & ~\a[1]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[3]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[2]  & ~\a[0]  & \a[1] ) | (((\b[2]  & \b[3] ) | (~\b[2]  & ~\b[3] ) | ~\b[1]  | (~\b[0]  & ~\b[2] )) & ((\b[2]  ^ \b[3] ) | (\b[1]  & (\b[0]  | \b[2] ))) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] ))))) & ((\b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] ) & (~\a[2]  ^ ((~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] ))) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ))))) | (\a[2]  & (~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | ~\a[1] ) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & ((\b[2]  ^ (\b[0]  | ~\b[1] )) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\a[0]  | ~\b[0] ) & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] ) & ((~\b[0]  ^ \b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ))));
  assign new_n68_ = (~\a[2]  ^ (((~new_n27_ ^ ~\b[5] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[4]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[5]  | \a[0]  | ~\a[1] ))) ^ (~new_n48_ ^ new_n49_);
  assign new_n69_ = ~new_n70_ ^ (\a[5]  ^ (\a[8]  & (~\a[9]  | ~\b[5] )));
  assign new_n70_ = \a[2]  ^ ((\a[8]  & ((\b[5]  & (~\a[8]  ^ ~\a[9] )) | (\a[9]  & \b[4] ))) | (((((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))) & (~\a[8]  ^ (new_n72_ | ~\b[5] ))) | (~new_n71_ & (((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))) | (\a[8]  ^ (new_n72_ | ~\b[5] ))) & ((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] )) | (\a[8]  & (new_n72_ | ~\b[5] )) | (~\a[8]  & ~new_n72_ & \b[5] )))) & (~\a[8]  | ((~\b[5]  | (\a[8]  ^ ~\a[9] )) & (~\a[9]  | ~\b[4] ))) & (\a[8]  | (\a[9]  & \b[5] ))));
  assign new_n71_ = ((new_n26_ ^ \a[8] ) | ((~\b[3]  | (\a[8]  ^ ~\a[9] )) & (~\b[2]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )))) & ((((new_n23_ ^ \a[8] ) | ((~\b[2]  | (\a[8]  ^ ~\a[9] )) & (~\b[1]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )))) & (new_n28_ | ((~new_n23_ ^ \a[8] ) & ((\b[2]  & (~\a[8]  ^ ~\a[9] )) | (\b[1]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] )))) | ((~new_n23_ | ~\a[8] ) & (new_n23_ | \a[8] ) & (~\b[2]  | (\a[8]  ^ ~\a[9] )) & (~\b[1]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))))) | ((~new_n26_ ^ \a[8] ) & ((\b[3]  & (~\a[8]  ^ ~\a[9] )) | (\b[2]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] )))) | ((~new_n26_ | ~\a[8] ) & (new_n26_ | \a[8] ) & (~\b[3]  | (\a[8]  ^ ~\a[9] )) & (~\b[2]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))));
  assign new_n72_ = ((\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  ^ ~\a[6] ) | (\a[6]  ^ \a[7] )) & (new_n27_ | (\a[7]  & \a[8] ) | (~\a[7]  & ~\a[8] ) | (~\a[5]  & ~\a[6] ) | (\a[5]  & \a[6] ));
  assign new_n73_ = (((~new_n21_ | ~\a[2] ) & ((new_n21_ & \a[2] ) | (~new_n21_ & ~\a[2] ) | ((~new_n52_ | ~\a[2] ) & (new_n53_ | (new_n52_ & \a[2] ) | (~new_n52_ & ~\a[2] ))))) ^ (new_n74_ ^ \a[2] )) & ((~new_n21_ ^ \a[2] ) ^ ((new_n52_ & \a[2] ) | (~new_n53_ & (~new_n52_ | ~\a[2] ) & (new_n52_ | \a[2] )))) & new_n76_ & (new_n53_ ^ (new_n52_ ^ \a[2] ));
  assign new_n74_ = ((new_n25_ & \a[5] ) | ((~new_n25_ | ~\a[5] ) & (new_n25_ | \a[5] ) & ((\a[5]  & (new_n28_ | (~new_n22_ & ~new_n51_) | (new_n22_ & new_n51_)) & (~new_n28_ | (~new_n22_ ^ ~new_n51_))) | (~new_n36_ & (~\a[5]  | (~new_n28_ & (new_n22_ | new_n51_) & (~new_n22_ | ~new_n51_)) | (new_n28_ & (new_n22_ ^ ~new_n51_))) & (\a[5]  | (~new_n28_ ^ (~new_n22_ ^ ~new_n51_))))))) ^ (\a[5]  ^ (~new_n71_ ^ new_n75_));
  assign new_n75_ = ((\b[4]  & (\a[8]  ^ \a[9] )) | (\b[3]  & \a[9]  & (\a[8]  | ~\a[9] ) & (~\a[8]  | \a[9] ))) ^ (~\a[8]  ^ (new_n72_ | ~\b[5] ));
  assign new_n76_ = ((~new_n54_ ^ ~\a[2] ) | (new_n55_ & \a[2] ) | (((new_n56_ & \a[2] ) | (~new_n57_ & (~new_n56_ | ~\a[2] ) & (new_n56_ | \a[2] ))) & (~new_n55_ | ~\a[2] ) & (new_n55_ | \a[2] ))) & ((~new_n54_ & ~\a[2] ) | (new_n54_ & \a[2] ) | ((~new_n55_ | ~\a[2] ) & (((~new_n56_ | ~\a[2] ) & (new_n57_ | (new_n56_ & \a[2] ) | (~new_n56_ & ~\a[2] ))) | (new_n55_ & \a[2] ) | (~new_n55_ & ~\a[2] )))) & (((~new_n56_ | ~\a[2] ) & (new_n57_ | (new_n56_ & \a[2] ) | (~new_n56_ & ~\a[2] ))) ^ (new_n55_ ^ \a[2] )) & (~new_n57_ | (new_n56_ ^ \a[2] )) & ~new_n77_ & new_n78_ & (new_n57_ | (new_n56_ & \a[2] ) | (~new_n56_ & ~\a[2] ));
  assign new_n77_ = (new_n58_ ^ ~new_n59_) ^ (new_n60_ | (~new_n61_ & new_n68_));
  assign new_n78_ = (new_n61_ ^ new_n68_) & (new_n79_ | (~new_n65_ & new_n66_) | (new_n67_ & (new_n65_ | ~new_n66_) & (~new_n65_ | new_n66_))) & (~new_n79_ | ((new_n65_ | ~new_n66_) & (~new_n67_ | (~new_n65_ & new_n66_) | (new_n65_ & ~new_n66_)))) & new_n80_ & (~new_n67_ ^ (~new_n65_ ^ new_n66_));
  assign new_n79_ = new_n64_ ^ (~\a[2]  ^ (new_n63_ & (~new_n24_ | ~new_n62_)));
  assign new_n80_ = (~new_n81_ | new_n82_) & (new_n81_ | ~new_n82_) & ~new_n83_ & ~new_n84_ & new_n87_ & (~new_n85_ | ~new_n86_);
  assign new_n81_ = \b[0]  & (~\a[2]  | ~\a[3] ) & (\a[2]  | \a[3] );
  assign new_n82_ = \a[2]  ^ ((~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | ((~\b[2]  | ~\b[3] ) & (\b[2]  | \b[3] ) & \b[1]  & (\b[0]  | \b[2] )) | ((~\b[2]  ^ \b[3] ) & (~\b[1]  | (~\b[0]  & ~\b[2] )))) & (~\b[3]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[2]  | \a[0]  | ~\a[1] ) & (~\b[1]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )));
  assign new_n83_ = ((~\b[2]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[1]  | \a[0]  | ~\a[1] ) & (~\b[0]  | \a[0]  | \a[1]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] ) | (\b[2]  ^ (\b[0]  | ~\b[1] )))) ? ~\a[2]  : (((\b[0]  ^ ~\b[1] ) | ~\a[0]  | (~\a[1]  & ~\a[2] ) | (\a[1]  & \a[2] )) & (~\b[1]  | ~\a[0]  | (~\a[1]  ^ ~\a[2] )) & (~\b[0]  | \a[0]  | ~\a[1] ));
  assign new_n84_ = \a[2]  & (((~\b[0]  ^ ~\b[1] ) & \a[0]  & (\a[1]  | \a[2] ) & (~\a[1]  | ~\a[2] )) | (\b[1]  & \a[0]  & (\a[1]  ^ ~\a[2] )) | (\b[0]  & ~\a[0]  & \a[1] ));
  assign new_n85_ = ~\a[9]  & ~\a[5]  & ~\a[6] ;
  assign new_n86_ = ~\a[1]  & ~\a[2]  & ~\a[7]  & ~\a[8]  & ~\a[3]  & ~\a[4] ;
  assign new_n87_ = \a[0]  & \b[0]  & (\b[3]  | \b[4]  | \b[5]  | \b[1]  | \b[2] );
  assign new_n88_ = (~\a[5]  | (~new_n71_ & new_n75_) | (new_n71_ & ~new_n75_)) & (((~new_n25_ | ~\a[5] ) & ((new_n25_ & \a[5] ) | (~new_n25_ & ~\a[5] ) | ((~\a[5]  | (~new_n28_ & (new_n22_ | new_n51_) & (~new_n22_ | ~new_n51_)) | (new_n28_ & (new_n22_ ^ ~new_n51_))) & (new_n36_ | (\a[5]  & (new_n28_ | (~new_n22_ & ~new_n51_) | (new_n22_ & new_n51_)) & (~new_n28_ | (~new_n22_ ^ ~new_n51_))) | (~\a[5]  & (new_n28_ ^ (~new_n22_ ^ ~new_n51_))))))) | (\a[5]  & (new_n71_ | ~new_n75_) & (~new_n71_ | new_n75_)) | (~\a[5]  & (new_n71_ ^ new_n75_)));
  assign new_n89_ = ~\a[5]  & (((((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))) | (\a[8]  ^ (new_n72_ | ~\b[5] ))) & (new_n71_ | (((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))) & (~\a[8]  ^ (new_n72_ | ~\b[5] ))) | ((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )) & (~\a[8]  | (~new_n72_ & \b[5] )) & (\a[8]  | new_n72_ | ~\b[5] )))) ^ (\a[8]  ? ((~\b[5]  | (\a[8]  ^ ~\a[9] )) & (~\a[9]  | ~\b[4] )) : (\a[9]  & \b[5] )));
  assign new_n90_ = \a[5]  & (((((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))) | (\a[8]  ^ (new_n72_ | ~\b[5] ))) & (new_n71_ | (((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))) & (~\a[8]  ^ (new_n72_ | ~\b[5] ))) | ((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] )) & (~\a[8]  | (~new_n72_ & \b[5] )) & (\a[8]  | new_n72_ | ~\b[5] )))) | (\a[8]  & ((\b[5]  & (~\a[8]  ^ ~\a[9] )) | (\a[9]  & \b[4] ))) | (~\a[8]  & (~\a[9]  | ~\b[5] ))) & ((((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] ))) & (~\a[8]  ^ (new_n72_ | ~\b[5] ))) | (~new_n71_ & (((~\b[4]  | (\a[8]  ^ ~\a[9] )) & (~\b[3]  | ~\a[9]  | (\a[8]  & ~\a[9] ) | (~\a[8]  & \a[9] ))) | (\a[8]  ^ (new_n72_ | ~\b[5] ))) & ((\b[4]  & (~\a[8]  ^ ~\a[9] )) | (\b[3]  & \a[9]  & (~\a[8]  | \a[9] ) & (\a[8]  | ~\a[9] )) | (\a[8]  & (new_n72_ | ~\b[5] )) | (~\a[8]  & ~new_n72_ & \b[5] ))) | (\a[8]  ? ((~\b[5]  | (\a[8]  ^ ~\a[9] )) & (~\a[9]  | ~\b[4] )) : (\a[9]  & \b[5] )));
endmodule

