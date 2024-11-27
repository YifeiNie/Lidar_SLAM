#pragma once

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#define TRIGGER_KEYS_NUM 8
#define W       0
#define A       1
#define S       2
#define D       3
#define UP      4
#define DOWN    5
#define LEFT    6
#define RIGHT   7

#define LATCH_KEYS_NUM 4
#define NUM1    0
#define NUM2    1
#define NUM3    2
#define NUM4    3
extern struct libevdev* dev;

class KeyboardCtr : public ros::NodeHandle{
public:
    KeyboardCtr(int queue_size, double publish_rate);
    void setTwist(double linear_x, double linear_y, double linear_z, double angular_x, double angular_y, double angular_z);
    bool* get_key_input();
    void update_latch_key_status(uint8_t key_index);
    void update_trigger_key_status(uint8_t key_index);
    void printKeyStatus();
    void get_namespace (uint8_t);
    void publish_vel_cmd();
private:
    ros::Timer timer;                // 定时器对象
    ros::Publisher publisher;        // 发布者
    struct input_event ev;
    bool latch_key_status[LATCH_KEYS_NUM];
    bool trigger_key_status[TRIGGER_KEYS_NUM];
    std::string ugv_name;
    geometry_msgs::Twist twist_msg;  // 要发布的 Twist 消息


    void timer_callback(const ros::TimerEvent& event);  // 定时器回调函数
};


