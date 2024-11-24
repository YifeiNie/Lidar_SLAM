# Lidar_SLAM

### 2024.11.22 -- by nyf 安装pytorch
- 首先使用nvidia-smi查看显卡是否在工作，如果发现显卡占用一直为0，说明显卡没在工作，对此安装驱动如下步骤
    - 查询电脑最适合的显卡驱动版本`ubuntu-drivers devices`，找到后面跟着`recommended`的对应驱动
    - `sudo add-apt-repository ppa:graphics-drivers/ppa`
    - `sudo apt-get update`
    - `sudo apt-get install nvidia-driver-525`，此处数字要对应上面查询到的recommended版本号
    - 重启
- 使用`nvidia-smi`检查cuda版本，根据版本进入[pytorch官网](https://pytorch.org/)选择对应版本下载，如果出现问题，参考下面执行操作
- ROS会创建一个系统级别的python环境，不要更改，直接下载(miniconda3[https://repo.anaconda.com/miniconda/])进行虚拟环境管理，防止同名不同版本的包互相干扰导致各种报错
    - 更新py:如果报错`ERROR: Package 'networkx' requires a different Python: 3.8.10 not in '>=3.9'`，说明python版本过低，需要更新python
        - 注意！！！从这里开始，务必保持一个空的终端在后台开启，因为终端是基于python的，更新版本后终端将打不开，而处理这种情况又需要打开终端，所以建议保留，如果实在忘记，也可以通过vscode的终端操作，前提是vscode已经解锁了sudo命令
        - 使用如下命令更新：
            - `sudo add-apt-repository ppa:deadsnakes/ppa`
            - `sudo apt update`
            - `sudo apt install python3.11`这里是你需要的版本
        - 此时系统里有多个版本的py，使用`sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 2`最后一位数字表示优先级，数字越大最高，将最新版本的py设置为默认，同时**一定要把老版本也按顺序排列第二第三，否则会导致冲突，所有与系统环境下python相关的应用全部失效报错！！！**
        - 使用`sudo update-alternatives --config python3`命令查看并切换当前默认的python版本，然后使用`python3 --version`·检查是否设置成功
    - 安装这个包`sudo apt-get install python3.11-distutils`
    - 此时你会发现终端以及terminator都打不开了，此时需要分别在`/usr/bin/gnome-terminal`和`/usr/bin/terminator`里把第一个语句`#!/usr/bin/python3`改为`#!/usr/bin/python3.8`，然后就可以了，不过这种更改需要sudo，如果之前没有保留一个空白终端，vscode的sudo命令也不能使用，则可以进入tty terminal修改
    - 同时与python相关的比如apt，cmake等使用时也报错`ModuleNotFoundError: No module named 'apt_pkg'`,解决方法如下，参考[这里](https://blog.csdn.net/a18838956649/article/details/117612374)
    - 重新执行pytorch的安装，然后使用`pip cache purge`删除缓存
    - 以上参考了[这里](https://blog.csdn.net/DEVELOPERAA/article/details/133743389)和[这里](https://blog.csdn.net/chdlr/article/details/136989643)
- 运行python程序时报错`/usr/bin/env: ‘python’: No such file or directory报错`，使用`sudo ln -s /usr/bin/python3 /usr/bin/python`创建符号连接就能解决

### 2024.11.24 -- by nyf 
- 部署[RING/RING++算法](https://github.com/lus6-Jenny/RING)
    - 创建conda环境，使用python3.10版本，因为有一个依赖(PyFrameObject)10以后就不支持了，然后按照教程执行
    - 依照教程进行两个test.py之前，首先安装[KNN_cuda](https://github.com/unlimblue/KNN_CUDA)，如果报错`"ERROR: Could not install packages due to an OSError: HTTPSConnectionPool"`，参考[这里](https://gist.github.com/Kalffman/d873d84099784db808dce6c1bea65799)在`--upgrade`前加上`--trusted-host=pypi.python.org --trusted-host=pypi.org --trusted-host=files.pythonhosted.org `
    - 安装ninja: `pip install ninja`
    - 在文件夹`generate_bev_pointfeat_cython`下的test.py文件前添加`import voxelocc`
    - 运行两个test，如果运行第二个test报错segmentation相关错误并显示`core dumped`，并且按照教材修改了栈大小还是不行后，说明numpy的版本太新了，退回为1.26.4，问题解决，此时两个test应该都能正常运行
- 部署[MR_SLAM](https://github.com/MaverickPeter/MR_SLAM)
    - 按照教程安装依赖，注意两个标记为optional的依赖也要安装，注意安装ros-noetic-octomap*后面的星号！！！！
    - 全局搜索文件`kindrConfig.cmake`，添加上这一句：`set(kindr_FOUND true)`
    - 安装grid-map包：`sudo apt-get install ros-noetic-grid-map`
    - 在教程的Installation部分，
        - **Make Mapping**: 先安装`sudo apt-get install libceres-dev`，然后如果catkin_make的时候报错缺少FFTW3这个包，使用apt安装是不行的，因为apt安装这个包没有包含comfig.cmake文件，所以即使用apt安装了，catkin_make还是找不到，对此应该使用[源码](https://www.fftw.org/download.html)安装，参考[这里](https://blog.csdn.net/weixin_39258979/article/details/109941424)，注意安装fftw-3.3.10.tar.gz这个版本，其他版本没有cmake文件无法通过经典的`cmake .`和`sudo make install`来安装FFTW3
        - **Make Localization**：安装[livox_sdk](https://github.com/Livox-SDK/Livox-SDK)和[livox_ros_driver](https://github.com/Livox-SDK/livox_ros_driver)，注意不要安装driver2和sdk2，否则catkin_make的时候会报找不到livox_def.h和livox_sdk.h，然后记得安装后source一下才catkin_make
        - **Make Costmap**：将/Costmap/src/costmap路径下的CMakeLists.txt中的全部`-std=c++11`改为`-std=c++14`再进行编译即可
        - **Make LoopDetection**：直接按照教程安装即可



