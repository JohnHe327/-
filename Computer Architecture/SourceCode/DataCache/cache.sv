`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: John He (hechunwang2000327@hotmail.com)
// 
// Design Name: RV32I Core
// Module Name: cache
// Tool Versions: Vivado 2017.4.1
// Description: RV32I Data Cache
// 
//////////////////////////////////////////////////////////////////////////////////


// 功能说明
    // 同步读写 Cache
    // 暂未实现独热码非整字读写功能
// 输入
    // clk               输入时钟
    // rst               CPU的rst信号
    // addr              读写地址
    // rd_req            读请求
    // wr_req            写请求
    // wr_data           写入数据
    // 
    // REPLACE_POLICY    替换策略，0表示FIFO，1表示LRU
    // LINE_ADDR_LEN     line内地址长度，决定了每个line具有2^n个word
    // SET_ADDR_LEN      组地址长度，决定了一共有2^n个组
    // TAG_ADDR_LEN      tag长度
    // WAY_CNT           组相连度，决定了每组中有2^n路line
// 输出
    // miss              cache未命中信号
    // rd_data           输出数据
// 实验要求
    // 修改为组相联cache，实现FIFO、LRU两种替换算法

module cache #(
    parameter  REPLACE_POLICY= 1, // 0表示FIFO，1表示LRU
    parameter  LINE_ADDR_LEN = 3, // line内地址长度，决定了每个line具有2^3个word
    parameter  SET_ADDR_LEN  = 3, // 组地址长度，决定了一共有2^3=8组
    parameter  TAG_ADDR_LEN  = 6, // tag长度
    parameter  WAY_CNT       = 3  // 组相连度，决定了每组中有多少路line，这里是直接映射型cache，因此该参数没用到
)(
    input  clk, rst,
    output miss,               // 对CPU发出的miss信号
    input  [31:0] addr,        // 读写请求地址
    input  rd_req,             // 读请求信号
    output reg [31:0] rd_data, // 读出的数据，一次读一个word
    input  wr_req,             // 写请求信号
    input  [31:0] wr_data      // 要写入的数据，一次写一个word
);

localparam MEM_ADDR_LEN    = TAG_ADDR_LEN + SET_ADDR_LEN ; // 计算主存地址长度 MEM_ADDR_LEN，主存大小=2^MEM_ADDR_LEN个line
localparam UNUSED_ADDR_LEN = 32 - TAG_ADDR_LEN - SET_ADDR_LEN - LINE_ADDR_LEN - 2 ;       // 计算未使用的地址的长度

localparam LINE_SIZE       = 1 << LINE_ADDR_LEN  ;         // 计算 line 中 word 的数量，即 2^LINE_ADDR_LEN 个word 每 line
localparam SET_SIZE        = 1 << SET_ADDR_LEN   ;         // 计算一共有多少组，即 2^SET_ADDR_LEN 个组

reg [            31:0] cache_mem    [SET_SIZE][WAY_CNT + 1][LINE_SIZE]; // SET_SIZE个line，每个line有LINE_SIZE个word
reg [TAG_ADDR_LEN-1:0] cache_tags   [SET_SIZE][WAY_CNT + 1];            // SET_SIZE个TAG
reg                    valid        [SET_SIZE][WAY_CNT + 1];            // SET_SIZE个valid(有效位)
reg                    dirty        [SET_SIZE][WAY_CNT + 1];            // SET_SIZE个dirty(脏位)
reg [            31:0] usecnt       [SET_SIZE][WAY_CNT + 1];            // 使用情况记录，LRU时表示距上次使用时间，FIFO时表示距换入时间

wire [              2-1:0]   word_addr;                   // 将输入地址addr拆分成这5个部分
wire [  LINE_ADDR_LEN-1:0]   line_addr;
wire [   SET_ADDR_LEN-1:0]    set_addr;
wire [   TAG_ADDR_LEN-1:0]    tag_addr;
wire [UNUSED_ADDR_LEN-1:0] unused_addr;

enum  {IDLE, SWAP_OUT, SWAP_IN, SWAP_IN_OK} cache_stat;    // cache 状态机的状态定义
                                                           // IDLE代表就绪，SWAP_OUT代表正在换出，SWAP_IN代表正在换入，SWAP_IN_OK代表换入后进行一周期的写入cache操作。

reg  [   SET_ADDR_LEN-1:0] mem_rd_set_addr = 0;
reg  [   TAG_ADDR_LEN-1:0] mem_rd_tag_addr = 0;
wire [   MEM_ADDR_LEN-1:0] mem_rd_addr = {mem_rd_tag_addr, mem_rd_set_addr};
reg  [   MEM_ADDR_LEN-1:0] mem_wr_addr = 0;

reg  [31:0] mem_wr_line [LINE_SIZE];
wire [31:0] mem_rd_line [LINE_SIZE];

wire mem_gnt;      // 主存响应读写的握手信号

assign {unused_addr, tag_addr, set_addr, line_addr, word_addr} = addr;  // 拆分 32bit ADDR

reg [WAY_CNT:0] cache_hit = 0;
always @ (*) begin              // 判断 输入的address 是否在 cache 中命中
    for (integer i = 0; i <= WAY_CNT; i++)
        if(valid[set_addr][i] && cache_tags[set_addr][i] == tag_addr)   // 如果 cache line有效，并且tag与输入地址中的tag相等，则命中
            cache_hit[i] = 1'b1;
        else
            cache_hit[i] = 1'b0;
end

integer swapi;
always@(*) begin
    if(|cache_hit) begin
        for (integer i = 0; i <= WAY_CNT; i++)
            if (cache_hit[i])
                swapi = i;
    end else begin
        integer i, j, m, n;
        for (i = 0; i <= WAY_CNT; i++)                      // find !valid
            if (valid[set_addr][i] == 0)
                break;
        for (j = 0; j <= WAY_CNT; j++)                      // find !dirty
            if (valid[set_addr][j] && !dirty[set_addr][j])
                break;
        for (m = 0, n = 1; n <= WAY_CNT; n++)               // find max usecnt
            if (usecnt[set_addr][n] > usecnt[set_addr][m])
                m = n;
        swapi = (i <= WAY_CNT) ? i :
                                ((j <= WAY_CNT) ? j : m);
    end
end

always @ (posedge clk or posedge rst) begin     // ?? cache ???
    if(rst) begin
        cache_stat <= IDLE;
        for(integer i = 0; i < SET_SIZE; i++)
            for(integer j = 0; j <= WAY_CNT; j++) begin
                dirty[i][j] = 1'b0;
                valid[i][j] = 1'b0;
                usecnt[i][j] = 0;
            end
        for(integer k = 0; k < LINE_SIZE; k++)
            mem_wr_line[k] <= 0;
        mem_wr_addr <= 0;
        {mem_rd_tag_addr, mem_rd_set_addr} <= 0;
        rd_data <= 0;
    end else begin
        case(cache_stat)
        IDLE:       begin
                        for (integer i = 0; i <= WAY_CNT; i++) usecnt[set_addr][i] <= usecnt[set_addr][i] + 1; // 时钟+1
                        if(|cache_hit) begin
                            if (REPLACE_POLICY == 1) usecnt[set_addr][swapi] <= 0; // >LRU
                            if(rd_req) begin    // 如果cache命中，并且是读请求，
                                rd_data <= cache_mem[set_addr][swapi][line_addr];   //则直接从cache中取出要读的数据
                            end else if(wr_req) begin // 如果cache命中，并且是写请求，
                                cache_mem[set_addr][swapi][line_addr] <= wr_data;   // 则直接向cache中写入数据
                                dirty[set_addr][swapi] <= 1'b1;                     // 写数据的同时置脏位
                            end 
                        end else begin
                            if(wr_req | rd_req) begin   // 如果 cache 未命中，并且有读写请求，则需要进行换入
                                if(valid[set_addr][swapi] & dirty[set_addr][swapi]) begin    // 如果 要换入的cache line 本来有效，且脏，则需要先将它换出
                                    cache_stat  <= SWAP_OUT;
                                    mem_wr_addr <= {cache_tags[set_addr][swapi], set_addr};
                                    mem_wr_line <= cache_mem[set_addr][swapi];
                                end else begin                                   // 反之，不需要换出，直接换入
                                    cache_stat  <= SWAP_IN;
                                end
                                {mem_rd_tag_addr, mem_rd_set_addr} <= {tag_addr, set_addr};
                            end
                        end
                    end
        SWAP_OUT:   begin
                        if(mem_gnt) begin           // 如果主存握手信号有效，说明换出成功，跳到下一状态
                            cache_stat <= SWAP_IN;
                        end
                    end
        SWAP_IN:    begin
                        if(mem_gnt) begin           // 如果主存握手信号有效，说明换入成功，跳到下一状态
                            cache_stat <= SWAP_IN_OK;
                        end
                    end
        SWAP_IN_OK: begin           // 上一个周期换入成功，这周期将主存读出的line写入cache，并更新tag，置高valid，置低dirty
                        for(integer i=0; i<LINE_SIZE; i++)  cache_mem[mem_rd_set_addr][swapi][i] <= mem_rd_line[i];
                        cache_tags[mem_rd_set_addr][swapi] <= mem_rd_tag_addr;
                        valid     [mem_rd_set_addr][swapi] <= 1'b1;
                        dirty     [mem_rd_set_addr][swapi] <= 1'b0;
                        usecnt    [mem_rd_set_addr][swapi] <= 0;
                        cache_stat <= IDLE;        // 回到就绪状态
                    end
        endcase
    end
end

wire mem_rd_req = (cache_stat == SWAP_IN );
wire mem_wr_req = (cache_stat == SWAP_OUT);
wire [   MEM_ADDR_LEN-1 :0] mem_addr = mem_rd_req ? mem_rd_addr : ( mem_wr_req ? mem_wr_addr : 0);

assign miss = (rd_req | wr_req) & ~(cache_hit && cache_stat==IDLE) ;     // 当 有读写请求时，如果cache不处于就绪(IDLE)状态，或者未命中，则miss=1

main_mem #(     // 主存，每次读写以line 为单位
    .LINE_ADDR_LEN  ( LINE_ADDR_LEN          ),
    .ADDR_LEN       ( MEM_ADDR_LEN           )
) main_mem_instance (
    .clk            ( clk                    ),
    .rst            ( rst                    ),
    .gnt            ( mem_gnt                ),
    .addr           ( mem_addr               ),
    .rd_req         ( mem_rd_req             ),
    .rd_line        ( mem_rd_line            ),
    .wr_req         ( mem_wr_req             ),
    .wr_line        ( mem_wr_line            )
);

endmodule
