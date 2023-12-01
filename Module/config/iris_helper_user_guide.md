#Iris Helper by Aestas_Britannia
#The module is not allowed for any commercial use

Configuration format:
app: "package_name"
params_a: Type-ID params_amount param0 param1 param2 etc.
params_b: Type-ID params_amount param0 param1 param2 etc.
......
params_d: Type-ID params_amount param0 param1 param2 etc.

The file should end with:
END

For example:
app: "com.tencent.tmgp.sgame"
params_a: 258 4 40 -1 34 -1 3 60
(-1 is ignored so it doesn't increase params_amount)
Enable dual MEMC, low latency, normal game, use YuanShen preset, original FPS is 60.

For Param 2 in Type 258, bits 0-3 represent a binary number.
For example, number 4 corresponds to eMV game mode 2.
Convert this into binary, and add a zero at the beginning:
4 → 0100
Now, repeat this process for the next four bits.
For instance, if you prefer ultra-low latency (symbolized by number 3), convert it to binary:
3 → 0011
At this point, you have completed eight bits.
If desired, continue this process until you achieve your desired setting.
If not, stop and fill in any remaining spaces with zeros.
Arrange these from highest to lowest order:
00110100
Convert this to decimal form, resulting in the number 52.
Therefore, the Param 2 you want is 52.

--------

For Yuni Kernel, the module supports modifying FEAS parameters.
Enable FEAS while pfmgr_enable: 1
Enable powersave mode for FEAS while pfmgr_pwsave: 1
Set target FPS for FEAS while f_t_fps exists
Lock max FPS at df while it exists
If there's only a package name without any params for the app,
the module will apply presets in joyose-9200.sql instead resetting params to default.

--------

Here's the types of Iris_x7 parameters:

--------

MEMC - HDR Formal Solution:
Type:258 	Feature: Enable/Disable MEMC/HDR
Param 0: mode setting
0	 Disable HDR
2	 Enable HDR
10	 Enable single MEMC
14	 Enable TNR
40	 Enable dual MEMC

Param 1: Ignored
-1	 Ignored

Param 2: scene setting
- MEMC scene	 Bit0~3
0	 normal video
1	 MMI test
2	 normal game
3	 eMV game mode 1
4	 eMV game mode 2
5	 short video
6	 eMV API allowed
- low latency mode	 Bit4~7
0	 disable low latency
1	 normal latency
2	 low latency
3	 ultra-low latency
- MEMC level	 Bit8~11
0	 level0
- N2M mode	 Bit12~15
0	 disable N2M
1	 enable N2M
- reserved	 Bit16~31

Param 3: Ignored
-1	 Ignored

Param 4: Game app list
0	 HePingJingYing
1	 WanMeiShiJie
2	 TianDao
3	 YuanShen
4	 BengHuai3
5	 GuiQi
6	 TianYu
7	 ZhanShenYiJi
8	 DiWuRenGe
9	 TBD
100	 BuLuoChongTu(WaiXiaoJiXing)
101	 ShiMingZhaoHuan(WaiXiaoJiXing)
102	 YuanShen(WaiXiaoJiXing)

Param 5: Game fps
0	 Auto
30	 30fps
45	 45fps
60	 60fps

Param 6: HePingJingYingSighting
0	 Off(default)
1	 HePingJingYingZhunXing
2	 HePingJingYingQuanXi

--------

SDR2HDR:
Type: 267	Feature: Set SDR2HDR Setting

Param 0: Setting Type
3	 Pre-defined SDR2HDR setting(Recommended) 

Param 1: Pre-defined Setting
0	 All off 
1	 Video (Recommended)
2	 Enhanced video 
3	 Game 
4	 Low power video 
10	 Dark area brightening filter 
11	 Anti-snow blindness filter 
12	 movie mode
13	 vivid mode
14	 bright mode 
15	 gentle mode 
16	 LOL 
17	 YuanShen 
18	 HePingJingYing 
19	 WangZheRongYao 
20	 Revelation style 
21	 GunsGirl style 
22	 Identity-V style 
23	 Onmyoji style 

--------

SR: The SR feature works in PT mode.
Type: 273	Feature: Set SR Setting

Param 0: Setting Type
0	 Disable SR(Recommended)
1	 Enable  SR 

Param 1: Game App list
0	 HePingJingYing 
1	 WanMeiShiJie 
2	 TianDao 
3	 YuanShen 
4	 BengHuai3 
5	 GuiQi 
6	 TianYu 
7	 ZhanShenYiJi 
8	 DiWuRenGe 
9	 TBD 
100	 BuLuoChongTu(WaiXiaoJiXing)
101	 ShiMingZhaoHuan(WaiXiaoJiXing)
102	 YuanShen(WaiXiaoJiXing)

--------

HDR:
Type: 47 	Feature: HDR mode
Param 0: HDR mode
0	 Disable (Recommended)
61	 HLG HDR
62	 HDR10
3	 Online video
4	 Short video
5	 Local video
6	 Gallery photo
7	 Game
8	 Reading
9	 AI Online video
10	 AI Short video
11	 AI Local video
12	 AI Gallery photo
13	 AI Game
14	 AI Reading

--------