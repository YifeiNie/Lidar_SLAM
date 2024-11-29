# Lidar_SLAM

### 2024.11.22 -- by nyf 安装pytorch
- 首先使用nvidia-smi查看显卡是否在工作，如果发现显卡占用一直为0，说明显卡没在工作，对此安装驱动如下步骤
    - 查询电脑最适合的显卡驱动版本`ubuntu-drivers devices`，找到后面跟着`recommended`的对应驱动
    - `sudo add-apt-repository ppa:graphics-drivers/ppa`
    - `sudo apt-get update`
    - `sudo apt-get install nvidia-driver-525`，此处数字要对应上面查询到的recommended版本号
    - 重启
- 使用`nvidia-smi`检查cuda版本，根据版本进入[pytorch官网](https://pytorch.org/)选择对应版本下载，如果出现问题，参考下面执行操作
- ROS会创建一个系统级别的python环境，不要更改，直接下载[miniconda3](https://repo.anaconda.com/miniconda/)进行虚拟环境管理，防止同名不同版本的包互相干扰导致各种报错
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
    - 运行两个test，如果运行第二个test报错segmentation相关错误并显示`core dumped`，并且按照教材修改了栈大小还是不行后，说明numpy的版本太新了，退回为1.26.4，问题解决，此时两个test应该都能正常运行，参考[这里](https://stackoverflow.com/questions/78778444/segmentation-fault-when-pass-a-numpy-array-to-open3d-utility-vector3dvector)
- 部署[MR_SLAM](https://github.com/MaverickPeter/MR_SLAM)
    - 按照教程安装依赖，注意两个标记为optional的依赖也要安装，注意安装ros-noetic-octomap*后面的星号！！！！
    - 全局搜索文件`kindrConfig.cmake`，添加上这一句：`set(kindr_FOUND true)`
    - 安装grid-map包：`sudo apt-get install ros-noetic-grid-map`
    - 在教程的Installation部分，
        - **Make Mapping**: 先安装`sudo apt-get install libceres-dev`，然后如果catkin_make的时候报错缺少FFTW3这个包，使用apt安装是不行的，因为apt安装这个包没有包含comfig.cmake文件，所以即使用apt安装了，catkin_make还是找不到，对此应该使用[源码](https://www.fftw.org/download.html)安装，参考[这里](https://blog.csdn.net/weixin_39258979/article/details/109941424)，注意安装fftw-3.3.10.tar.gz这个版本，其他版本没有cmake文件无法通过经典的`cmake .`和`sudo make install`来安装FFTW3
        - **Make Localization**：安装[livox_sdk](https://github.com/Livox-SDK/Livox-SDK)和[livox_ros_driver](https://github.com/Livox-SDK/livox_ros_driver)，注意不要安装driver2和sdk2，否则catkin_make的时候会报找不到livox_def.h和livox_sdk.h，然后记得安装后source一下才catkin_make
        - **Make Costmap**：将/Costmap/src/costmap路径下的CMakeLists.txt中的全部`-std=c++11`改为`-std=c++14`再进行编译即可
        - **Make LoopDetection**：直接按照教程安装即可

### 2024.11.25 -- by nyf 正式运行
- 测试DISCO算法时，需要安装tensorboard和tensorboardX，进入虚拟环境后使用pip安装即可
- 运行DISCO时，进入disco_ros文件夹，使用python main.py运行，而不要按照教程中的使用rosrun来运行，除非你执行了教程中说的"add #!/xxx/python3 in the first line of RING_ros/main.py"
- 进入rviz可视化时，点击左下角add，添加pointcloud2，注意要选择话题"/global_manager/merged_cloud"，并调整点的大小形状即可
### 2024.11.28 -- by nyf
- gazebo时间计算根运算负载有关，证据是单车仿真，默认情况下仿真时间/真实时间的值要大于多车方针，也即真实时间过去一秒，仿真环境过去的时间
- fast-lio接受的livox雷达的点云消息不是传统的PointcCloud2类型，而是livox自定义的CustomMsg类型，因此需要在`~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo/src/wheeltec_robot_gazebo/wheeltec_description/urdf`路径下的livox_mid360.xacro文件中修改`<publish_pointcloud_type>3</publish_pointcloud_type>`的值为3，参考[这里](https://github.com/ChEnYuAnYiKe/wheeltec_ugv_gazebo/tree/master/src/wheeltec_robot_gazebo/Mid360_simulation_plugin)
- rostopic中的/robot_1/submap话题中，submap数据是rosbag发出来的，如果实机运行，则直接输出的是keyframePC
- 使用RING++直接运行发现最终合并的地图出现漂移，其中一台机器人建的图和其他两个建的图没有重合，反而倾斜向上，对此修改global_manager.launch中的`icp_fitness_score`参数为0.15，增强回环检测，然后运行发现问题解决
### 2024.11.30 -- by nyf
- 进行四小车仿真的时候，注意要在rviz选择delay time，然后选择cloud_registered即可看到fastlio建的点云地图
- 代码制定了好几个进程，进程函数都命名为`xxxxThread`，进程循环执行指导程序终止
- 进程discoveryThread中的`void GlobalManager::discovery()`用于捕获当时所有的ROS话题，并进行处理
- 进程MapComposingThread中的`void GlobalManager::discoveryThread()`函数`PointCloud GlobalManager::composeGlobalMap()`

- 需要修改
    - void GlobalManager::publishPoseGraph()

