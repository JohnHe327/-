// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
// Date        : Thu Apr 18 11:49:48 2019
// Host        : JohnHe_Laptop running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               j:/lab4/project/lab4_VGA/lab4_VGA.srcs/sources_1/ip/dist_mem_gen_0/dist_mem_gen_0_stub.v
// Design      : dist_mem_gen_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "dist_mem_gen_v8_0_12,Vivado 2018.2" *)
module dist_mem_gen_0(a, d, dpra, clk, we, dpo)
/* synthesis syn_black_box black_box_pad_pin="a[15:0],d[11:0],dpra[15:0],clk,we,dpo[11:0]" */;
  input [15:0]a;
  input [11:0]d;
  input [15:0]dpra;
  input clk;
  input we;
  output [11:0]dpo;
endmodule
