module Ejercicio_2 (A,B,C,D,X,Y,Z);
  input A,B,C,D;
  output X,Y,Z;
  
  wire w1,w2,w3,w4,w5,w6,w7;
  
  notG U1(B,w1);
  notG U2(D,w2);
  orG U3(A,w1,w3);
  nandG U4(w1,C,w4);
  and3G U5(C,B,w2,w5);
  andG U6(w1,w2,w6);
  nandG U7(w1,D,w7);
  norG U8(w3,w4,X);
  xorG U9(w5,w6,Y);
  orG U10(w6,w7,Z);
  
endmodule