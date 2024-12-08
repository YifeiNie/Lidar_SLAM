# 启动MR-SLAM三机器狗仿真
cd ~/Code/MR-SLAM/
rosbag play 3_dog.bag --clock --pause

conda activate RING-env
cd ~/Code/MR-SLAM/MR_SLAM 
cd LoopDetection && source devel/setup.bash
cd src/RING_ros
python main_RINGplusplus.py

conda activate RING-env
cd ~/Code/MR-SLAM/MR_SLAM
cd Mapping && source devel/setup.bash
roslaunch global_manager global_manager.launch

rviz -d Visualization/vis.rviz

cd ~/Code/MR-SLAM/MR_SLAM
cd Localization && source devel/setup.bash
roslaunch fast_lio robot_1_sim.launch

# 启动gazebo单个小车仿真
cd ~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo && source devel/setup.bash
roslaunch wheeltec_gazebo_control my_single_wheeltec_gazebo.launch

cd ~/Code/MR-SLAM/FAST-LIO-sim && source devel/setup.bash
roslaunch fast_lio single_mapping_mid360_simu.launch

# 启动MR-SLAM四小车仿真
cd ~/Code/MR-SLAM/MR_SLAM/Localization && source devel/setup.bash
roslaunch fast_lio 4_robots.launch
# roslaunch fast_lio robot_1_sim.launch
# roslaunch fast_lio robot_2_sim.launch
# roslaunch fast_lio robot_3_sim.launch

conda activate RING-env
cd ~/Code/MR-SLAM/MR_SLAM 
cd LoopDetection && source devel/setup.bash
cd src/RING_ros
python main_RINGplusplus.py

conda activate RING-env
cd ~/Code/MR-SLAM/MR_SLAM
cd Mapping && source devel/setup.bash
ulimit -s 81920
roslaunch global_manager global_manager.launch

cd Code/MR-SLAM/MR_SLAM/Costmap/ && source devel/setup.bash
roslaunch costmap_converter create_costmap.launch

rviz -d Visualization/vis.rviz

# 启动gazebo四小车仿真(卡)
cd ~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo && source devel/setup.bash
roslaunch wheeltec_gazebo_control multi_wheeltec_gazebo_control.launch

cd ~/Code/MR-SLAM/FAST-LIO-sim && source devel/setup.bash
roslaunch fast_lio mapping_mid360_simu.launch

# 启动gazebo四小车仿真(流畅)
cd ~/Code/MR-SLAM/gazebo_sim/nexus_4wd_mecanum_simulator && source devel/setup.bash
roslaunch nexus_4wd_mecanum_gazebo nexus_4wd_mecanum_multi.launch

cd ~/Code/MR-SLAM/FAST-LIO-sim && source devel/setup.bash
roslaunch fast_lio mapping_mid360_simu.launch

# 键盘控制小车
sudo chmod 666 /dev/input/event*
cd ~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo/ && source devel/setup.bash
rosrun keyboard_ctr keyboard_ctr
