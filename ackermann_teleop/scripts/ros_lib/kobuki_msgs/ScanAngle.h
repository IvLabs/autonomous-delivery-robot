#ifndef _ROS_kobuki_msgs_ScanAngle_h
#define _ROS_kobuki_msgs_ScanAngle_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace kobuki_msgs
{

  class ScanAngle : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef double _scan_angle_type;
      _scan_angle_type scan_angle;

    ScanAngle():
      header(),
      scan_angle(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        double real;
        uint64_t base;
      } u_scan_angle;
      u_scan_angle.real = this->scan_angle;
      *(outbuffer + offset + 0) = (u_scan_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_scan_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_scan_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_scan_angle.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_scan_angle.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_scan_angle.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_scan_angle.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_scan_angle.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->scan_angle);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        double real;
        uint64_t base;
      } u_scan_angle;
      u_scan_angle.base = 0;
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_scan_angle.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->scan_angle = u_scan_angle.real;
      offset += sizeof(this->scan_angle);
     return offset;
    }

    const char * getType(){ return "kobuki_msgs/ScanAngle"; };
    const char * getMD5(){ return "06e2c839dc5c7f5c13ac09a8b4ff0a6a"; };

  };

}
#endif
