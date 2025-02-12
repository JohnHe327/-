# lab4 分支预测

### 实验目的
* 实现BTB（Branch Target Buffer）和BHT（Branch History Table）两种动态分支预测器
* 体会动态分支预测对流水线性能的影响

### 实验内容
* 阶段一：在Lab3阶段二的RV32I Core基础上，实现BTB
* 阶段二：实现BHT
* 阶段二**需要在阶段一的基础上实现**，不能仅实现阶段二

### 实验指导
* BTB
  * 我们要实现的BTB本质可以理解为是1bit预测器，如果上次这条分支指令跳转，那么这次它也跳转；如果上次不跳，那么这次也不跳
  * BTB实现一个buffer，保存当前地址高位、目标地址和有效位，类似于直接映射的cache，可以直接使用reg实现buffer
  * **buffer 放在取指阶段**，buffer内容读取一个周期内可以完成
  * BTB的命中：当前指令的低位用于寻址，对比指令的高位和buffer中是否相等并且有效位为1，表示命中，则下一条指令的地址不是pc+4，而是buffer中的内容
  * 在IF阶段是否命中信息会随着流水线段寄存器传递到EX阶段，根据实际是否跳转和IF阶段是否命中信息，**在EX阶段对buffer进行修改**
* BHT
  * BHT 首先要实现一个N\*2的buffer，N为大小，2表示2bit预测
  * 实现一个状态机，详见**lab4-分支预测-实验指导.pptx**
  * 用BHT来控制是否跳转（BTB不命中，BHT命中该如何处理？），BHT的根据状态机更新，BTB的更新与之前不同
* 实验说明
  * instruction和data的生成请参考Lab3（bht.s和btb.s无需生成data），四个测试样例的汇编指令在 Lab4\Test\ASM-Benchmark\generate_inst目录下。
  * 请将你的代码文件放在Lab4\Src\CPU_SrcCode目录下。Lab4\Src\Simulation目录下的cpu_tb.v是Lab3中的仿真代码，仿真时将其设置为Top文件。使用说明参考Lab3。
  * 本次实验仅要求**支持对branch指令的动态预测，不要求支持jump指令的动态预测**

### 实验检查 （60%）
> 阶段1（30%），阶段2（30%）
* 通过QuickSort测试和MatMul测试（和Lab3使用的QuickSort、MatMul测试一致，7号寄存器最后会按序读取最终结果）
* **两个阶段一起验收**

### 实验报告 （40%）
我们提供了btb.s、bht.s、QuickSort.s、MatMul.s四个测试样例，分别执行这四个测试样例，并在报告中分析以下内容：

* 分析分支收益和分支代价

* 统计未使用分支预测和使用分支预测的总周期数及差值

* 统计分支指令数目、动态分支预测正确次数和错误次数

* 对比不同策略并分析以上几点的关系

  

**注意**：由于QuickSort.s和MatMul.s执行完成后会陷入死循环，测试终止时间记为第一次执行到死循环的jal指令为止。
