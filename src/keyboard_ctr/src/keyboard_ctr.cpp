#include <ros/ros.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <libevdev/libevdev.h>

#include "keyboard_ctr.hpp"

struct libevdev* dev = nullptr;

// 禁用终端回显
void disableEcho() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;  // 关闭回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// 恢复回显
void enableEcho() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= ECHO;  // 恢复回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// 简单的打印
void KeyboardCtr::printKeyStatus() {
    std::cout << "NUM_1: " << (latch_key_status[NUM1] ? "1" : "0") << ", ";
    std::cout << "NUM_2: " << (latch_key_status[NUM2] ? "1" : "0") << ", ";
    std::cout << "NUM_3: " << (latch_key_status[NUM3] ? "1" : "0") << ", ";
    std::cout << "NUM_4: " << (latch_key_status[NUM4] ? "1" : "0") << ", ";
    std::cout << "W: " << (trigger_key_status[W] ? "1" : "0") << ", ";
    std::cout << "A: " << (trigger_key_status[A] ? "1" : "0") << ", ";
    std::cout << "S: " << (trigger_key_status[S] ? "1" : "0") << ", ";
    std::cout << "D: " << (trigger_key_status[D] ? "1" : "0") << ", ";
    std::cout << "Up: " << (trigger_key_status[UP] ? "1" : "0") << ", ";
    std::cout << "Down: " << (trigger_key_status[DOWN] ? "1" : "0") << ", ";
    std::cout << "Left: " << (trigger_key_status[LEFT] ? "1" : "0") << ", ";
    std::cout << "Right: " << (trigger_key_status[RIGHT] ? "1" : "0") << std::endl;
    usleep(1000);
}

// 节点类的构造函数
KeyboardCtr::KeyboardCtr(int queue_size, double publish_rate){

    ugv_name = {};
    publisher = advertise<geometry_msgs::Twist>("/cmd_vel", queue_size);
    setTwist(0.0, 0.0, 0.0, 0.0, 0.0, 0.0); 
    ROS_INFO("CmdVelPublisher initialized for topic: cmd_vel");

    timer = createTimer(ros::Duration(1.0 / publish_rate), &KeyboardCtr::timer_callback, this);
    std::fill(latch_key_status, latch_key_status + LATCH_KEYS_NUM, false);
    std::fill(trigger_key_status, trigger_key_status + TRIGGER_KEYS_NUM, false);
}

// 更新键盘输入状态
void KeyboardCtr::update_trigger_key_status(uint8_t key_index){
    if (ev.value == 1) {         // 按下
        trigger_key_status[key_index] = true;
    } else if (ev.value == 0) {  // 松开
        trigger_key_status[key_index]= false;
    } else if (ev.value == 2) {  // 按住
        // 持续按住不改变状态
    }
    // printKeyStatus();
}
void KeyboardCtr::update_latch_key_status(uint8_t key_index){
    if (ev.value == 1){
        for (uint8_t n = 0; n < LATCH_KEYS_NUM ; n++){
            if (n == key_index){
                latch_key_status[n] = !latch_key_status[n];
            }
            else{
                latch_key_status[n] = false;
            }
        }
        get_namespace(key_index);
    }
    printKeyStatus();
    
}

// get函数
bool* KeyboardCtr::get_key_input(){
    return trigger_key_status;
}

void KeyboardCtr::get_namespace (uint8_t num){
    std::string num_str = std::to_string(num + 1);
    ugv_name = "/ugv_" + num_str + "/cmd_vel";
}

void KeyboardCtr::publish_vel_cmd(){
    // std::cout << ugv_name <<std::endl;
    publisher = advertise<geometry_msgs::Twist>(ugv_name, 10);
    setTwist(5,5,5,5,5,5);
    publisher.publish(twist_msg);
}

// 检查按键事件，并更新相应的标志位
void KeyboardCtr::timer_callback(const ros::TimerEvent& event){
    if (libevdev_next_event(dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev) == 0) {
        if (ev.type == EV_KEY) {
            switch (ev.code) {
                case KEY_W:
                    update_trigger_key_status(W);
                    break;
                case KEY_A:
                    update_trigger_key_status(A);
                    break;
                case KEY_S:
                    update_trigger_key_status(S);
                    break;
                case KEY_D:
                    update_trigger_key_status(D);
                    break;
                case KEY_UP:
                    update_trigger_key_status(UP);
                    break;
                case KEY_DOWN:
                    update_trigger_key_status(DOWN);
                    break;
                case KEY_LEFT:
                    update_trigger_key_status(LEFT);
                    break;
                case KEY_RIGHT:
                    update_trigger_key_status(RIGHT);
                    break;
                case KEY_1:
                    update_latch_key_status(NUM1);
                    break;
                case KEY_2:
                    update_latch_key_status(NUM2);
                    break;
                case KEY_3:
                    update_latch_key_status(NUM3);
                    break;
                case KEY_4:
                    update_latch_key_status(NUM4);
                    break;
                default:
                    break;
            }
        }      
    }
    publish_vel_cmd();
    // printKeyStatus();
}

int KeyboardCtr::dir_judge(uint8_t dir1, uint8_t dir2){
    if (trigger_key_status[dir1] == true && trigger_key_status[dir2] == false){
        return 1;
    }
    else if (trigger_key_status[dir1] == false && trigger_key_status[dir2] == true){
        return -1;
    }
    return 0;
}

void KeyboardCtr::setTwist(double linear_x, double linear_y, double linear_z, double angular_x, double angular_y, double angular_z) {
    twist_msg.linear.x = linear_x*dir_judge(UP, DOWN);
    twist_msg.linear.y = linear_y*dir_judge(A, D);
    twist_msg.linear.z = linear_z;

    twist_msg.angular.x = angular_x;
    twist_msg.angular.y = angular_y;
    twist_msg.angular.z = angular_z*dir_judge(LEFT, RIGHT);
}

int main(int argc, char** argv) {
    disableEcho();
    const char* dev_path = "/dev/input/event10";  // 输入设备路径，根据实际情况修改
    int fd = open(dev_path, O_RDONLY);
    if (fd == -1) {
        perror("Open device error");
        return 0;
    }

    if (libevdev_new_from_fd(fd, &dev) != 0) {
        perror("Libevdev init error");
        close(fd);
        return 0;
    }
    // 初始化 ROS 节点
    ros::init(argc, argv, "keyboard_cmd_vel_publisher");

    // 创建 CmdVelPublisher 类的实例
    KeyboardCtr cmd_vel_pub = KeyboardCtr(10, 100.0); // 发布频率为 10 Hz
    // ROS 主循环
    ros::spin();
    enableEcho();
    return 0;
}