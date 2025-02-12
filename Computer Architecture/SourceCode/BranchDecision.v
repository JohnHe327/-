`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
//           John He (hechunwang2000327@hotmail.com)
// Design Name: RV32I Core
// Module Name: Branch Decision
// Tool Versions: Vivado 2017.4.1
// Description: Decide whether to branch
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    //  判断是否branch
// 输入
    // reg1               寄存器1
    // reg2               寄存器2
    // br_type            branch类型
// 输出
    // br                 是否branch
// 实验要求
    // 补全模块

`include "Parameters.v"   
module BranchDecision(
    input wire [31:0] reg1, reg2,
    input wire [2:0] br_type,
    output reg br
    );

    // DONE: Complete this module
    always@(*)
    begin
        case (br_type)
            `NOBRANCH: br <= 0;
            `BEQ: br <= (reg1 == reg2) ? 1 : 0;
            `BNE: br <= (reg1 != reg2) ? 1 : 0;
            `BLT: br <= (reg1[31] & ~reg2[31]) ? 1 :
                            (~reg1[31] & reg2[31]) ? 0 :
                                (reg1 < reg2) ? 1 : 0;
            `BLTU: br <= (reg1 < reg2) ? 1 : 0;
            `BGE: br <= (reg1[31] & ~reg2[31]) ? 0 :
                            (~reg1[31] & reg2[31]) ? 1 :
                                (reg1 >= reg2) ? 1 : 0;
            `BGEU: br <= (reg1 >= reg2) ? 1 : 0;
            default: br <= 0;
        endcase
    end
endmodule
