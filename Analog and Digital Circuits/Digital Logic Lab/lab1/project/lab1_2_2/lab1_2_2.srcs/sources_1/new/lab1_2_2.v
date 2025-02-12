`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/10/12 15:29:30
// Design Name: 
// Module Name: lab1_2_2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module lab1_2_2(
    input [1:0] x,
    input [1:0] y,
    input s,
    output [1:0] m
    );
    
    assign #3 m[0] = (~s & x[0]) | (s & y[0]); 
    assign #3 m[1] = (~s & x[1]) | (s & y[1]); 
endmodule
