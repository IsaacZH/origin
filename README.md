# MotorTest

## 功能
- 测试大疆GM6020,RM3508,RM2006电机
- 理论上只需要留意<font color=Darkorange>control.c</font>这个文件中<font color=DarkTurquoise>control</font>结构体成员的值
## 使用方法
- 一次只能测一个电机，无需配置CAN和电机ID，插上就可以收到信息，电机接受信息和电机状态会同步到<font color=DarkTurquoise>control</font>结构体中
- 所有的参数在<font color=Darkorange>control.c</font>中配置，可配置的参数有：
  - <font color=DarkTurquoise>**pid_type**</font>：预设了6020，3508，2006速度环和位置环的PID，更改<font color=DarkTurquoise>pid_type</font>的值就可以选用对应的PID
  - <font color=DarkTurquoise>**control_mode**</font>：分为速度环和位置环，更改<font color=DarkTurquoise>control_mode</font>的值就可以切换到对应的控制模式
  - <font color=DarkTurquoise>**target_speed**</font>：电机目标速度
  - <font color=DarkTurquoise>**target_position**</font>：电机目标位置  
- 如果你想更改PID参数只能更改<font color=Darkorange>control.c</font>中对应的数组，debug时也是
- 默认关闭检测是否开控，如果你想开控后才有输出，可以更改<font color=Darkorange>remote.h</font>中<font color=DarkTurquoise>RC_ONLINE</font>的宏定义
