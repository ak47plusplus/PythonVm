# PythonVm

### 编译项目
1.请确保先安装cmake,如果没有安装cmake,请执行一下命令安装 <br/>
$ sudo apt-get install build-essential  <br/>
$ sudo apt-get install cmake            <br/>
2.编译 <br/>
chmod 755 build.sh <br/>
./build.sh         <br/>

### 运行项目
官方的CPython包含编译器+虚拟机,本项目暂时不包含编译器，
因此需要利用官方的编译器执行下面命令，先将.py文件编译成.pyc文件,再由本虚拟机解释执行. <br/>
python -m compileall test.py <br/>
./mypython test.pyc

### 备注
1.python2和python3在切片的操作上发生了巨大变化    <br/>
Python2关于切片的字节码    <br/>
<pre>    
#define SLICE		30     <br/>
/* Also uses 31-33 */      <br/>
#define SLICE_1		31    <br/>
#define SLICE_2		32    <br/>
#define SLICE_3		33    <br/>
#define STORE_SLICE	40    <br/>
/* Also uses 41-43 */    <br/>
#define STORE_SLICE_1	41    <br/>
#define STORE_SLICE_2	42    <br/>
#define STORE_SLICE_3	43    <br/>
#define DELETE_SLICE	50    <br/>
/* Also uses 51-53 */    <br/>
#define DELETE_SLICE_1	51    <br/>
#define DELETE_SLICE_2	52    <br/>
#define DELETE_SLICE_3	53    <br/>
#define BUILD_SLICE 	133	/* Number of items */    <br/>
</pre>    <br/>
Python3中关于切片的字节码就缩减为一个:    <br/>
#define BUILD_SLICE 	133	/* Number of items */    <br/>