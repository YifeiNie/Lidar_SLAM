
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


cd ~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo && source devel/setup.bash
roslaunch wheeltec_gazebo_control multi_wheeltec_gazebo_control.launch

cd ~/Code/MR-SLAM/FAST-LIO-sim && source devel/setup.bash
roslaunch fast_lio mapping_mid360_simu.launch

sudo chmod 666 /dev/input/event10
cd ~/Code/MR-SLAM/gazebo_sim/wheeltec_ugv_gazebo/ && source devel/setup.bash
rosrun keyboard_ctr keyboard_ctr
