#ifndef _ROS_kobuki_msgs_ControllerInfo_h
#define _ROS_kobuki_msgs_ControllerInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace kobuki_msgs
{

  class ControllerInfo : public ros::Msg
  {
    public:
      typedef uint8_t _type_type;
      _type_type type;
      typedef double _p_gain_type;
      _p_gain_type p_gain;
      typedef double _i_gain_type;
      _i_gain_type i_gain;
      typedef double _d_gain_type;
      _d_gain_type d_gain;
      enum { DEFAULT =   0 };
      enum { USER_CONFIGURED =   1 };

    ControllerInfo():
      type(0),
      p_gain(0),
      i_gain(0),
      d_gain(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      union {
        double real;
        uint64_t base;
      } u_p_gain;
      u_p_gain.real = this->p_gain;
      *(outbuffer + offset + 0) = (u_p_gain.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_p_gain.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_p_gain.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_p_gain.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_p_gain.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_p_gain.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_p_gain.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_p_gain.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->p_gain);
      union {
        double real;
        uint64_t base;
      } u_i_gain;
      u_i_gain.real = this->i_gain;
      *(outbuffer + offset + 0) = (u_i_gain.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_i_gain.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_i_gain.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_i_gain.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_i_gain.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_i_gain.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_i_gain.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_i_gain.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->i_gain);
      union {
        double real;
        uint64_t base;
      } u_d_gain;
      u_d_gain.real = this->d_gain;
      *(outbuffer + offset + 0) = (u_d_gain.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_d_gain.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_d_gain.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_d_gain.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_d_gain.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_d_gain.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_d_gain.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_d_gain.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->d_gain);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      union {
        double real;
        uint64_t base;
      } u_p_gain;
      u_p_gain.base = 0;
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_p_gain.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->p_gain = u_p_gain.real;
      offset += sizeof(this->p_gain);
      union {
        double real;
        uint64_t base;
      } u_i_gain;
      u_i_gain.base = 0;
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_i_gain.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->i_gain = u_i_gain.real;
      offset += sizeof(this->i_gain);
      union {
        double real;
        uint64_t base;
      } u_d_gain;
      u_d_gain.base = 0;
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_d_gain.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->d_gain = u_d_gain.real;
      offset += sizeof(this->d_gain);
     return offset;
    }

    const char * getType(){ return "kobuki_msgs/ControllerInfo"; };
    const char * getMD5(){ return "46af38e72f0640f41ed22174d43a2550"; };

  };

}
#endif
