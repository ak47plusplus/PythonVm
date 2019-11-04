# PythonVm

### 编译项目
1.请确保先安装cmake,如果没有安装cmake,请执行一下命令安装 <br/>
$ sudo apt-get install build-essential  <br/>
$ sudo apt-get install cmake            <br/>
2.编译
chmod 755 build.sh <br/>
./build.sh         <br/>

### 运行项目
官方的CPython包含编译器+虚拟机,本项目暂时不包含编译器，
因此需要利用官方的编译器执行下面命令，先将.py文件编译成.pyc文件,再由本虚拟机解释执行. <br/>
python -m compileall test.py <br/>
./mypython test.pyc
