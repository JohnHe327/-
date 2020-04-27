`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
//           John He (hechunwang2000327@hotmail.com)
// 
// Design Name: RV32I Core
// Module Name: Write-back Data seg reg
// Tool Versions: Vivado 2017.4.1
// Description: Write-back data seg reg for MEM\WB
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    // MEM\WB的写回寄存器内容
    // 为了数据同步，Data Extension和Data Cache集成在其中
// 输入
    // clk               时钟信号
    // rst               CPU的rst信号
    // wb_select         选择写回寄存器的数据：如果为0，写回ALU计算结果，如果为1，写回Memory读取的内容
    // load_type         load指令类型
    // write_en          Data Cache写使能
    // debug_write_en    Data Cache debug写使能
    // addr              Data Cache的写地址，也是ALU的计算结果
    // debug_addr        Data Cache的debug写地址
    // in_data           Data Cache的写入数据
    // debug_in_data     Data Cache的debug写入数据
    // bubbleW           WB阶段的bubble信号
    // flushW            WB阶段的flush信号
// 输出
    // debug_out_data    Data Cache的debug读出数据
    // data_WB           传给下一流水段的写回寄存器内容
    // miss              Cache的miss信号
// 实验要求  
    // 添加CSR，将DataCache换为Cache

module WB_Data_WB(
    input wire clk, rst, bubbleW, flushW,
    output wire miss,
    input wire [1:0] wb_select,
    input wire [2:0] load_type,
    input  [3:0] write_en, debug_write_en,
    input  [31:0] addr,
    input  [31:0] csr_data,
    input  [31:0] debug_addr,
    input  [31:0] in_data, debug_in_data,
    output wire [31:0] debug_out_data,
    output wire [31:0] data_WB
    );

    wire [31:0] data_raw;
    wire [31:0] data_WB_raw;


    /*DataCache DataCache1(
        .clk(clk),
        .write_en(write_en << addr[1:0]),
        .debug_write_en(debug_write_en),
        .addr(addr[31:2]),
        .debug_addr(debug_addr[31:2]),
        .in_data(in_data << (8 * addr[1:0])),
        .debug_in_data(debug_in_data),
        .out_data(data_raw),
        .debug_out_data(debug_out_data)
    );*/

    cache #(
        .LINE_ADDR_LEN  ( 3             ),
        .SET_ADDR_LEN   ( 2             ),
        .TAG_ADDR_LEN   ( 12            ),
        .WAY_CNT        ( 3             )
    ) cache1 (
        .clk            ( clk           ),
        .rst            ( rst           ),
        .miss           ( miss          ),
        .addr           ( addr          ),
        .rd_req         ( |load_type    ),
        .rd_data        ( data_raw      ),
        .wr_req         ( |write_en     ),
        .wr_data        ( in_data       )
    );

    reg [31:0] hit_cnt, miss_cnt; // 命中/缺失计数

    always@(posedge clk or posedge rst) begin
        if (rst) begin
            hit_cnt <= 0;
        end else begin
            if (|load_type && ~miss)
                hit_cnt <= hit_cnt + 1;
        end
    end

    always@(posedge miss or posedge rst) begin
        if (rst) begin
            miss_cnt <= 0;
        end else begin
            miss_cnt <= miss_cnt + 1;
        end
    end


    // Add flush and bubble support
    // if chip not enabled, output output last read result
    // else if chip clear, output 0
    // else output values from cache

    reg bubble_ff = 1'b0;
    reg flush_ff = 1'b0;
    reg [1:0] wb_select_old = 2'h0;
    reg [31:0] data_WB_old = 32'b0;
    reg [31:0] addr_old;
    reg [31:0] csr_data_old;
    reg [2:0] load_type_old;

    DataExtend DataExtend1(
        .data(data_raw),
        .addr(addr_old[1:0]),
        .load_type(load_type_old),
        .dealt_data(data_WB_raw)
    );

    always@(posedge clk)
    begin
        bubble_ff <= bubbleW;
        flush_ff <= flushW;
        data_WB_old <= data_WB;
        addr_old <= addr;
        csr_data_old <= csr_data;
        wb_select_old <= wb_select;
        load_type_old <= load_type;
    end

    assign data_WB = bubble_ff ? data_WB_old :
                                 (flush_ff ? 32'b0 : 
                                             ((wb_select_old == 2'h0) ? addr_old :
                                                                         ((wb_select_old == 2'h1) ? data_WB_raw :
                                                                                                     csr_data_old)));

endmodule
