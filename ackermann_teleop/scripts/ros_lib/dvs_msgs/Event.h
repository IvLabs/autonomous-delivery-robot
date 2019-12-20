#ifndef _ROS_dvs_msgs_Event_h
#define _ROS_dvs_msgs_Event_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace dvs_msgs
{

  class Event : public ros::Msg
  {
    public:
      typedef uint16_t _x_type;
      _x_type x;
      typedef uint16_t _y_type;
      _y_type y;
      typedef ros::Time _ts_type;
      _ts_type ts;
      typedef bool _polarity_type;
      _polarity_type polarity;

    Event():
      x(0),
      y(0),
      ts(),
      polarity(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->x >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->x >> (8 * 1)) & 0xFF;
      offset += sizeof(this->x);
      *(outbuffer + offset + 0) = (this->y >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->y >> (8 * 1)) & 0xFF;
      offset += sizeof(this->y);
      *(outbuffer + offset + 0) = (this->ts.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ts.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ts.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ts.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ts.sec);
      *(outbuffer + offset + 0) = (this->ts.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ts.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ts.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ts.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ts.nsec);
      union {
        bool real;
        uint8_t base;
      } u_polarity;
      u_polarity.real = this->polarity;
      *(outbuffer + offset + 0) = (u_polarity.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->polarity);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->x =  ((uint16_t) (*(inbuffer + offset)));
      this->x |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->x);
      this->y =  ((uint16_t) (*(inbuffer + offset)));
      this->y |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->y);
      this->ts.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->ts.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ts.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ts.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ts.sec);
      this->ts.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->ts.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ts.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ts.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ts.nsec);
      union {
        bool real;
        uint8_t base;
      } u_polarity;
      u_polarity.base = 0;
      u_polarity.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->polarity = u_polarity.real;
      offset += sizeof(this->polarity);
     return offset;
    }

    const char * getType(){ return "dvs_msgs/Event"; };
    const char * getMD5(){ return "40d339b261445b8ce9a6fef01267cd5d"; };

  };

}
#endif
