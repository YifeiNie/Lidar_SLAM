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