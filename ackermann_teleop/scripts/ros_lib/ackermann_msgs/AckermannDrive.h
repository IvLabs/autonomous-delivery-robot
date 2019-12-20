#ifndef _ROS_ackermann_msgs_AckermannDrive_h
#define _ROS_ackermann_msgs_AckermannDrive_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ackermann_msgs
{

  class AckermannDrive : public ros::Msg
  {
    public:
      typedef float _steering_angle_type;
      _steering_angle_type steering_angle;
      typedef float _steering_angle_velocity_type;
      _steering_angle_velocity_type steering_angle_velocity;
      typedef float _speed_type;
      _speed_type speed;
      typedef float _acceleration_type;
      _acceleration_type acceleration;
      typedef float _jerk_type;
      _jerk_type jerk;

    AckermannDrive():
      steering_angle(0),
      steering_angle_velocity(0),
      speed(0),
      acceleration(0),
      jerk(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_steering_angle;
      u_steering_angle.real = this->steering_angle;
      *(outbuffer + offset + 0) = (u_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_steering_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_steering_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_steering_angle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->steering_angle);
      union {
        float real;
        uint32_t base;
      } u_steering_angle_velocity;
      u_steering_angle_velocity.real = this->steering_angle_velocity;
      *(outbuffer + offset + 0) = (u_steering_angle_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_steering_angle_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_steering_angle_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_steering_angle_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->steering_angle_velocity);
      union {
        float real;
        uint32_t base;
      } u_speed;
      u_speed.real = this->speed;
      *(outbuffer + offset + 0) = (u_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->speed);
      union {
        float real;
        uint32_t base;
      } u_acceleration;
      u_acceleration.real = this->acceleration;
      *(outbuffer + offset + 0) = (u_acceleration.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_acceleration.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_acceleration.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_acceleration.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->acceleration);
      union {
        float real;
        uint32_t base;
      } u_jerk;
      u_jerk.real = this->jerk;
      *(outbuffer + offset + 0) = (u_jerk.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_jerk.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_jerk.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_jerk.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->jerk);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_steering_angle;
      u_steering_angle.base = 0;
      u_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->steering_angle = u_steering_angle.real;
      offset += sizeof(this->steering_angle);
      union {
        float real;
        uint32_t base;
      } u_steering_angle_velocity;
      u_steering_angle_velocity.base = 0;
      u_steering_angle_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_steering_angle_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_steering_angle_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_steering_angle_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->steering_angle_velocity = u_steering_angle_velocity.real;
      offset += sizeof(this->steering_angle_velocity);
      union {
        float real;
        uint32_t base;
      } u_speed;
      u_speed.base = 0;
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->speed = u_speed.real;
      offset += sizeof(this->speed);
      union {
        float real;
        uint32_t base;
      } u_acceleration;
      u_acceleration.base = 0;
      u_acceleration.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_acceleration.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_acceleration.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_acceleration.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->acceleration = u_acceleration.real;
      offset += sizeof(this->acceleration);
      union {
        float real;
        uint32_t base;
      } u_jerk;
      u_jerk.base = 0;
      u_jerk.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_jerk.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_jerk.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_jerk.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->jerk = u_jerk.real;
      offset += sizeof(this->jerk);
     return offset;
    }

    const char * getType(){ return "ackermann_msgs/AckermannDrive"; };
    const char * getMD5(){ return "3512e91b48d69674a0e86fadf1ea8231"; };

  };

}
#endif
