#ifndef _ROS_ackermann_teleop_cmd_h
#define _ROS_ackermann_teleop_cmd_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ackermann_teleop
{

  class cmd : public ros::Msg
  {
    public:
      typedef int16_t _speed_type;
      _speed_type speed;
      typedef int16_t _steering_angle_type;
      _steering_angle_type steering_angle;

    cmd():
      speed(0),
      steering_angle(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_speed;
      u_speed.real = this->speed;
      *(outbuffer + offset + 0) = (u_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->speed);
      union {
        int16_t real;
        uint16_t base;
      } u_steering_angle;
      u_steering_angle.real = this->steering_angle;
      *(outbuffer + offset + 0) = (u_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_steering_angle.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->steering_angle);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_speed;
      u_speed.base = 0;
      u_speed.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->speed = u_speed.real;
      offset += sizeof(this->speed);
      union {
        int16_t real;
        uint16_t base;
      } u_steering_angle;
      u_steering_angle.base = 0;
      u_steering_angle.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_steering_angle.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->steering_angle = u_steering_angle.real;
      offset += sizeof(this->steering_angle);
     return offset;
    }

    const char * getType(){ return "ackermann_teleop/cmd"; };
    const char * getMD5(){ return "ad20cf577c2ac8ebb986234ee4183d91"; };

  };

}
#endif
