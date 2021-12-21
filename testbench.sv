`timescale 1ns/1ns
`include "and3G.sv"
`include "andG.sv"
`include "nandG.sv"
`include "norG.sv"
`include "notG.sv"
`include "orG.sv"
`include "xorG.sv"

module Ejercicio_2_TB;
  reg A_TB, B_TB, C_TB, D_TB;
  wire X_TB, Y_TB, Z_TB;

Ejercicio_2 DUT(.A(A_TB), .B(B_TB), .C(C_TB), .D(D_TB),
            .X(X_TB), .Y(Y_TB), .Z(Z_TB));

  initial begin
    $dumpfile("Ejercicio_2.vcd");
    $dumpvars(1,Ejercicio_2_TB);

    {A_TB,B_TB,C_TB,D_TB} = 4'b0000; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1001; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1000; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b0011; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b0100; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1110; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b0111; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1000; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1110; #1
    {A_TB,B_TB,C_TB,D_TB} = 4'b1111; #1

    $finish;
  end
endmodule