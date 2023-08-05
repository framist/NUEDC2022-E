![show](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/show.png)



fork from: 

[framist/STemWinForHAL: 移植emWin与HAL库结合。(github.com)](https://github.com/framist/STemWinForHAL)

# NUEDC 2022 E - 声源定位跟踪系统

省级大学生电子设计竞赛 一等奖作品



*2023年8月4日：因今年的赛题 NUEDC 2023 F - 基于声传播的智能定位系统（F 题） 与本题相近，故重新整理此仓库之*



## 赛题

> 参见： [2022年TI杯大学生电子设计竞赛参考赛题](https://www.ti.com.cn/cn/lit/ug/zhcn005/zhcn005.pdf)
>
> 2022年TI杯大学生电子设计竞赛 - 声源定位跟踪系统（E题）
>
> **一、任务** 
> 设计制作一个声源定位跟踪系统，能够实时显示及指示声源的位置，当声源移动时能够用激光笔动态跟踪声源。声源检测系统测量区域分布俯视如图1所示。
>
> ![图1 系统测量区域俯视图](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804195330067.png)
>
> **二、要求** 
>
> 1．设计并制作声音发生装置——“声源”，装置能独立工作，声音音量手动可调，装置最大边长或直径不超过10cm，装置可用支架安装，并可在地面移动；声源中心点B用红色或其他醒目颜色标识，并在B点所在的平面以B点为圆心，直径为5cm画圆圈，用醒目线条标识，该平面面向检测指示装置（图中A点）。（4分） 
>
> 2．设计并制作一个声源定位检测装置，传感器安装在图1的 C区范围内，高度不超过1m，系统采用的拾音器或麦克风传感器数量不超过10个；在装置上标记测试参考点A，作为位置坐标的原点；装置上有显示电路，实时显示D区域内声源的位置，显示A、B两点直线距离γ和以A点为原点，AB在地面的投影与图1中心线的夹角θ，测量时间不超过5s，距离γ和角度θ的测值误差越小越好。（36分）   
>
> 3．设计并制作一个声源指示控制装置，此装置和上述声源定位检测装置可以合为一体。也放置在图1的 C区，安装有激光笔和二维电动云台，能控制激光笔指向声源，定位计算过程中时，激光笔关闭，定位运算完成时激光笔开启。定位指示声源时，动作反应时间不超过10s，光点与B点偏差越小越好。（30分）
>
> 4．声源移动动态追踪：当声源摆放在地面，用细绳牵引，以0.2m/s左右的速度在D区移动时，激光笔光点指向B点，光点与B点偏差越小好，跟踪反应时间越短越好。（20分）
>
> 5．其他（10分）
>
> 6．设计报告（20分）
>
> | **项目**        | **主要内容**                                         | **满分** |
> | -------------------- | ---------------------------------------------------- | -------- |
> | 方案论证             | 比较与选择，方案描述。                               | 3        |
> | 理论分析与计算       | 系统相关参数设计                                     | 5        |
> | 电路与程序设计       | 系统组成，原理框图与各部分电路图，系统软件与流程图。 | 5        |
> | 测试方案与测试结果   | 测试结果完整性，测试结果分析。                       | 5        |
> | 设计报告结构及规范性 | 摘要，正文结构规范，图表的完整与准确性。             | 2        |
> | **总分**             | 20                                                   |          |
>
>  **三、说明**
>
> 1．声源可以发出自定的规则声音，如滴、滴、滴的蜂鸣器声音等，音量以不造成严重噪声污染为宜，并注意避免周围噪声的影响。
>
> 2．声源可使用符合尺寸要求的微型有源音箱，播放事先录制的声音。
>
> 3．声源定位跟踪系统最大尺寸水平高度控制在离地面100cm以下，声源放置在D区不高于50cm。
>
> 4．测试要求：要求2、3测试时，先按启动按钮，再放置音源；或先放置音源，但声源和指示装置明显不在一个方向上，一键启动；一个点测完后，移动声源，测量下一个点，期间无人工干预装置；要求4测试时，一键启动后移动声源，期间亦无人工干预接触装置，直至该项测试结束。
>
> 5．距离和角度显示稳定，如出现不稳定，以跳动中出现的偏离最大的数字作为结果。



## 报告

*详细参见[报告.docx](./报告.docx)*

### 一、系统方案设计与论证

#### 1.1 总体方案设计框图


![image-20230804203939370](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804203939370.png)

总体方案框图如图 1.1 所示：我们从声源定位的目标出发，分别对声音产生装置和定位跟踪系统进行设计，声音产生装置需考虑频率选择、发声装置选择等方面，定位跟踪系统需要考虑麦克风阵列的摆放、定位算法的选择等方面，为了减小误差，需要在传感器后面设计信号调理级。



#### 1.2 发声器件的选择

**方案一：**蜂鸣器：蜂鸣器通过把压电蜂鸣片通过一定的固定方式装在一定体积的腔体中，产生共鸣，以得到放大的声音。蜂鸣器具有体积小、灵敏度高、耗电省、可靠性好,造价低廉的特点和良好的频率特性。

**方案二**：扬声器：扬声器是一种把电信号转变为声信号的换能器件，扬声器的性能优劣对音质的影响很大。与蜂鸣器不同的是，扬声器音色丰富；能够发出多种音调。

**方案三**：压电陶瓷片：压电陶瓷片是一种电子发音元件，在两片铜制圆形电极中间放入压电陶瓷介质材料，当在两片电极上面接通交流音频信号时，压电片会根据信号的大小频率发生震动而产生相应的声音来。其优点是造价低廉。

**方案选择**：由于压电陶瓷片只能发出微弱的声音，无法在题目要求的2.5米以上的距离传感器需要接收到信号，因此无法适用。从蜂鸣器和扬声器的结构和原理上比较与分析，扬声器输入的信号为音频信号，是不断变化的交流电，所以在结构上无振荡器；蜂鸣器输入的为直流电，为使其发出声音，在结构上必须配有振荡器（振荡器是一种产生交流信号的电路），将直流电转换为交流电，从而发出声音。因此扬声器可以具有更大的声音强度，同时可以通过自定义发出快速的短脉冲。选用扬声器不仅有利于提高接收信号的信噪比，同时短脉冲可以提升跟踪系统的计算速度，使其在更短的时间内产生响应。

#### 1.3 声源定位跟踪算法的选择



 **方案一**：TDOA算法：通过获得各个传感器接收信号的时延差，通过对已知位置的传感器与声源建立坐标系列写方程，求解声源的距离γ和方位角θ。其中各个传感器间的时延差可以直接获得，也可以利用信号与噪声间的不相关性，计算各路信号的互相关函数，其最大值对应的时延差Δt*=argmax(R(Δt))即为各传感器间信号的时延差，示意图如下。

![image-20230804203954747](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804203954747.png)

对于三个传感器的情况，可以将方程简化为如下形式：

![image-20230804204031303](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204031303.png)

该方程求解过程较为繁琐，但对于线性麦克风阵列的特殊情况来说，可以利用余弦定理列写方程，获得较为简单的求解过程。

该算法容易实现且运算量小，在求解的过程中无需远场近似，求解方程可以得到精确解。且对传感器数量要求低，最少只需三个传感器平行放置便可求解。但实际环境中很难直接测得传感器接收信号的时刻，而距离r对时延差较为敏感，尤其是对于声源位于较高方位角时，会带来不可忽视的误差，需要通过增加信号调理部分来减小误差。

**方案二**：波束成形法：在基于麦克风阵列的声源定位技术中是加权求和各个阵元输出，得到基阵的输出。确定出声源信号的方位信息则是利用对整个观测空间的波束进行扫描。波束成形法定位主要包括延迟求和波束算法和自适应滤波算法。延迟求和波束算法利用信号之间的相位延迟来改变波的方向，该计算方法计算量小、信号失真度小，但这种方法有个缺点就是对噪声的鲁棒性较差，需要更多的阵列来弥补。自适应滤波算法根据相应的决策依据调整权重，得到相应的最优波束形成器。常用的标准有最小均方误差等。自适应滤波算法计算量小、信号失真小，但对噪声鲁棒性强，所需阵列少。

![image-20230804204048568](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204048568.png)

波束成形所使用的麦克风阵列一般都为圆形。实际应用中，波束成形的麦克风阵列一般有三种，如下图所示。从左到右分别为环形、螺旋和随机分布。

波束成形对麦克风阵列以及数量都有较高的要求，本装置要求不超过10个麦克风，很难实现比较复杂的麦克风阵列，因此不适用于该方法。

**方案三**：空间谱定位算法：其基本原理是通过计算相关矩阵的空间之谱，利用每个元素信号的相关拟合矩阵得到声源的方向。该算法主要包括Characteristic Subspace算法和Subspace Fitting算法。Characteristic Subspace算法的基本原理是从阵列接收到的各种声音信号中提取自相关矩阵，然后将其分解为信号和噪声两个不同的特征值，通过比较两个子空间的特征值，对其进行空间谱搜索，完成位置估计。Subspace Fitting算法的基本原理是在阵列收到的信息数据的子空间与阵列形状之间建立拟合关系。

上述两种不同的定位算法主要针对的是区间较窄的信号。但由于声源信号大多使用的较宽的区间信号，因此定位算法的实用性方面不强。另外，该信号需要统计特别平稳的信号在时域中巧妙地估计相关矩阵。

本题需要对声音信号进行采样，声音信号为不平稳信号，空间内环境噪声丰富，在短时间内很难采集到足够且充分的数据来满足算法精确度的要求。空间谱方法对信号以及采样都有较高的要求，实际情况中难以实现。

**方案选择**：结合本题，由于麦克风数量和比赛时间的限制，我们无法完成复杂的麦克风阵列。实验室环境下声源能够发出较为稳定且清晰的波形，通过信号调理级可以获得精度可接受的时延差，使用TDOA算法可以在容易实现且运算量小的同时获得较高的精度。因此本装置选择方案一。

#### 1.4 麦克风阵列的摆放方式

**方案一：**线性麦克风阵列：为最简单的麦克风排列方式，即将麦克风水平排成一列。其可分为加性麦克风阵列和差分麦克风阵列。加性麦克风阵列的输出即为各个阵元的加权和，其具有最有波束方向可调，结构简单，布局方便的优点，但相较于其他方式的排列定位精确度较低，如图1.4.1所示。差分麦克风阵列输出则为两两麦克风之间的加权相减。

![image-20230804204113005](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204113005.png)

**方案二**：平面麦克风阵列：可以实现平面360°范围内等效拾音，定位精度更高。几种平面麦克风阵列如图1.4.2所示。

**方案三**：立体麦克风阵列：阵列较为复杂，真正实现了全空间360°无损拾音，且解决了平面阵高俯仰角信号响应差的问题。

**方案选择**：该问题仅要求在二维平面内进行测量，测量结果表示为极坐标形式，仅需输出距离γ和方位角θ两个参数，无需三维定位，因此没有必要选择立体麦克风阵列。同时，声源移动区间位于x∈[2.5m,3m],y∈[-1.5m,1.5m]的范围内，通过计算分析，方位角的最大值位于x=2.5m和y=±1.5m处，此时方位角约为31°，因此不会产生高方位角导致测量误差很大的问题，无需实现平面内360°拾音，因此针对该问题只需选择线性阵列作为麦克风的摆放方式。

#### 1.5 信号调理模块设计方案

信号调理级的框图如图1.5.1所示。主要包括滤波、放大、与基准源进行比对三个部分。

![image-20230804204159729](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204159729.png)

**滤波**：根据实验室环境下的噪声频谱分析，噪声主要集中在低频段，而我们选用的声源装置工作于高频段，首先需通过高通滤波器滤除低频噪声。高通滤波器的截止频率设置为 2KHz，即保证了对低频噪声的衰减效果，同时不对声源信号产生衰减，对信号的信噪比有较大的改善。

**放大**：放大级需要对信号的幅度进行放大，需要消除接收信号包络的缓慢上升沿所带来的测量误差，因此需设计增益可调的放大器。后续调试中可以选择合适的放大倍数尽可能减小各路信号上升沿带来的时延差测量误差。

**基准源**：基准源用于提供一个稳定、标准的电压源，三个通道与统一的基准源进行比对。该信号调理级中基准源也需要设置为可调。

### 二、理论分析与计算

#### 2.1 声源频率的选择

![图形用户界面  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image014.gif)
 声源频率需要满足噪声和干扰小、声源振幅足够大且不同周期内串扰较小的原则。通过对麦克风接收到的信号进行频谱分析，如图2.1.1所示。左图中低频有一明显尖峰，动态显示时可以发现一般人声都集中在300Hz~400Hz左右，一般的环境噪声也在2KHz以下。为了提高后续滤波效果和测量精度，声源信号应该在时域波形和频谱图上都与噪声信号有明显的区分，因此需要选择较高的声源频率。

同时为了提高接收信号的信噪比，声源信号的振幅应该尽量大。通过对声源在2KHz到6KHz之间不同频率激励下的响应测试，我们发现4KHz附近其可以产生较强的声音信号。4KHz时声源的波形如图2.1.2所示。此时该信号振幅较强、与噪声区分明显，且不同周期间信号串扰较小，有利于后续的信号处理，因此选择声源频率f = 4KHz。

![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image016.gif)  



#### 2.2 传感器距离γ与方位角θ公式推导

上图为声源与传感器的几何模型。上图S代表声源，A、A1、A2为传感器，其与声源的距离分别为![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image018.gif)、![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image020.gif)、![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image022.gif)，传感器之间间距为d，对三角形SA1A2和SAA2可以列写如下方程：

![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image024.gif)

由单片机采样可以测得三个传感器接受到信号的时刻，从而得到两传感器间的时延差，A与A2间的时延差记作![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image026.gif)，A1与A间的时延差记作![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image028.gif)，记声速为a，则有如下关系式：

![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image030.gif)

将该关系式代入上述方程组可以求解得：

 ![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image032.gif)

#### 2.3 传感器间距d的选取

本装置选择三个传感器组成线阵进行声源的定位。由于上述距离和方位角的表达式是与传感器间距d相关的函数，因此不同的间距d会给测量带来不一样的误差。根据实际测量经验，我们假定测量误差为50us进行不同间距d下的误差仿真，如图2.3所示。

![图表, 直方图  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image034.gif)
 对于方位角误差，在间距d稍大的时候表现的不明显；但距离误差随间距d的上升下降较为显著，因此传感器阵列应选择较大的间距。

![图2.3.2 距离和方位角在随机实验误差下的均值和方差与d的曲线图](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804200241422.png)
 考虑到该装置测量时会引入的最大误差是时延差，我们对时延差测量的误差进行仿真。假定误差满足正态分布，遍历d∈[0,0.5]范围内的间距，将时延差加上服从正态分布的随机数，通过对每个d进行2000次的随机误差生成，计算距离测量误差以及方位角测量误差的平均值和方差，如图2.3.2所示。

分析显示随着d的变化，时延差给方位角θ带来的误差一直在一个较小的范围内波动，可以忽略不计。而距离γ的测量误差随着d的增大而显著衰减。尤其是在测试区边界，即方位角θ较大的情况下，增加间距d对误差的改善是明显的。

但考虑到信号在空间内传播的衰减与距离的平方成反比，间距越大则信号衰减幅度越大，导致传感器接收到的信号幅度低，信噪比大，造成后续的测量误差。经过实际检验，我们设置传感器间距d=0.5m，可以保证在间距较大的情况下接收到较高强度的信号。

### 三、定位跟踪系统的设计

#### 3.1 系统整体设计




 ![图表, 折线图  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804200123787.png)




根据上文的方案论证与理论计算，我们搭建出一套声源定位跟踪系统。声源产生声音信号由三个水平排布的麦克风组成阵列进行接收，传感器接收到的信号经过调理输出数字波形。单片机对数字信号进行采样，获得时延差，用TDOA方法计算出声源的距离和方位角。随后控制激光笔转动，指向声源。搭建完成的装置如图3.1所示。



#### 3.2 信号调理级电路

 ![image-20230804204228254](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204228254.png)



信号调理级电路由滤波、放大、基准源与限幅电路构成，电路图以及仿真结果如图3.2.1所示。滤波器用于滤除环境噪声，保留高频声源信号，放大器用于调整模拟输出信号的幅值，基准源与限幅电路构成模拟信号向数字信号的转换，便于后续单片机采样。



信号调理电路首先是滤波器的设计。考虑到环境噪声一般在2KHz以下，我们选择高通滤波器的截止频率为2KHz。我们选用较为常用的sallen-key型二阶有源滤波器。


 滤波器的声源信号如图3.2.2所示。对其进行频谱分析，可以明显观察到其4KHz处的基波以及高次谐波，同时动态显示时低频噪声的幅度相较于滤波之前的信号有明显减小。   

![image-20230804204252245](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204252245.png) 

滤波器的增益设置为可调，两个通道放大后输出的模拟信号如图3.2.3所示。在示波器上可以明显观察到两路信号的时延差。由于两个传感器离声源的距离不同，两路信号的幅值也不同。通过调节滤波器增益可将信号调整至合适的幅值。

​     由于采样需获得信号到达传感器的时刻，因此需要模拟信号的第一个峰值的时刻与数字信号对齐，模拟信号转换为数字信号的时域波形如图3.2.4所示。

![电脑游戏的屏幕  描述已自动生成,图形用户界面  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image045.gif)

​     两个通道的数字信号波形如图3.2.5所示。该图显示转换后对数字信号采样仍可以精确得到两路信号的时延差。

![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image047.gif)





#### 3.3 单片机程序编写

![image-20230804204333551](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204333551.png)

单片机程序框架如图3.3.1所示。其中TIM14和PA7引脚用于PWM波的输出来控制激光笔旋转，P0~P5用作触发输入，PF8作为激光笔控制输出。



#### 3.4 误差校正

##### 3.4.1 系统时延误差的校正

​    由于每路信号在装置中传递时都会产生固定的时延差，且互不相等，会产生一个固有的系统误差。我们采用如下方法对该误差进行校正：

​    将声源分别置于移动区域的左侧和右侧，其与区域中心的距离相等，如图3.4.1所示。如果没有该系统误差，则其时延差应该满足t1 = t4，t2 = t3。而实际测试时发现测得的时延差并不满足如上关系。通过测量数据与上述等式可以求解并校正该系统误差。

![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image002.gif)

##### 3.4.2 数据拟合校正误差

![图形用户界面, 文本, 应用程序, 电子邮件  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image004.jpg)

为了校正装置带来的系统误差，我们使用最小二乘法将测量数据与真实数据进行线性拟合。我们通过所设计的装置对已知位置的声源进行定位，获得测量数据，将其与真实数据比对拟合缩小误差。最小二乘法的目的是使测量数据与真实数据的残差和最小，由此构建目标函数：

![图形用户界面, 文本  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image007.jpg)
 为了求解目标函数的最小值，需将目标函数对其两个参数求导：

 

![图形用户界面, 文本, 应用程序  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image009.jpg) 

求解得到： 

![图形用户界面, 文本, 应用程序  描述已自动生成](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image010.jpg) 

拟合得到线性函数![img](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/clip_image012.gif)。


拟合结果如下图所示：

![image-20230804204440966](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804204440966.png)

### 四、测量方案及测量结果

#### 测试指标及结果

**测试方案A**：将声源放置在要求区域内的任意一个位置，单片机输出声源的距离以及方位角，同时激光笔需要指在以声源为中心，直径5cm的圆内。

**测试结果：**如下表所示。

| 测量编号 | 距离γ/m | 方位角θ/deg | 距离误差 | 激光笔指向(是否在圆圈内) |
| -------- | ------- | ----------- | -------- | ------------------------ |
| 1        | 2.76    | -20.19      | 0.06m    | True                     |
| 2        | 2.82    | -19.39      | 0.04m    | True                     |
| 3        | 3.03    | 22.65       | 0.10m    | True                     |
| 4        | 3.17    | 27.35       | 0.08m    | True                     |
| 4        | 2.66    | 5.76        | 0.12m    | True                     |

**测试方案B**：将声源用细线牵引，以约0.2m/s的速度进行水平移动，观察装置是否能跟踪声源的移动。测试结果表明装只能较好地跟踪声源的移动。

测试结果部分图片如下所示：

![image-20230804200104422](http://framist-bucket-openread.oss-cn-shanghai.aliyuncs.com/img/image-20230804200104422.png)

### 五、参考文献

[1]孙肖子, 张企民.模拟电子技术基础[ M] .西安:电子科技大学出版社, 2008

[2]张揽月,杨德森.基于MUSIC算法的矢量水听器阵源方位估计[J].哈尔滨工程大学学报,2004(01):30-33.

[3]贺家琳. 声源定位算法研究及实现[D].天津大学,2009.

[4]谢庆. 基于空间谱估计的变压器局放超声阵列定位方法研究[D].华北电力大学（北京）,2010.

[5]韩霜,罗海勇,陈颖,丁玉珍.基于TDOA的超声波室内定位系统的设计与实现[J].传感技术学报,2010,23(03):347-353.

[6]史小红.基于TDOA的无线定位方法及其性能分析[J].东南大学学报(自然科学版),2013,43(02):252-257.

[7]居太亮. 基于麦克风阵列的声源定位算法研究[D].电子科技大学,2006.

[8]李文,夏秀渝,何培宇,李源.基于麦克风阵列的近场声源定位[J].四川大学学报(自然科学版),2008(02):307-310.

[NUEDC 2022 E | Framist's Little House](https://framist.github.io/2023/08/04/【电子设计与嵌入式】/NUEDC2022/)
