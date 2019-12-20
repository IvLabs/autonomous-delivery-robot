#ifndef _ROS_esim_msgs_OpticFlow_h
#define _ROS_esim_msgs_OpticFlow_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace esim_msgs
{

  class OpticFlow : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint32_t _height_type;
      _height_type height;
      typedef uint32_t _width_type;
      _width_type width;
      uint32_t flow_x_length;
      typedef float _flow_x_type;
      _flow_x_type st_flow_x;
      _flow_x_type * flow_x;
      uint32_t flow_y_length;
      typedef float _flow_y_type;
      _flow_y_type st_flow_y;
      _flow_y_type * flow_y;

    OpticFlow():
      header(),
      height(0),
      width(0),
      flow_x_length(0), flow_x(NULL),
      flow_y_length(0), flow_y(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->height >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->height >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->height >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->height >> (8 * 3)) & 0xFF;
      offset += sizeof(this->height);
      *(outbuffer + offset + 0) = (this->width >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->width >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->width >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->width >> (8 * 3)) & 0xFF;
      offset += sizeof(this->width);
      *(outbuffer + offset + 0) = (this->flow_x_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->flow_x_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->flow_x_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->flow_x_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->flow_x_length);
      for( uint32_t i = 0; i < flow_x_length; i++){
      union {
        float real;
        uint32_t base;
      } u_flow_xi;
      u_flow_xi.real = this->flow_x[i];
      *(outbuffer + offset + 0) = (u_flow_xi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_flow_xi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_flow_xi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_flow_xi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->flow_x[i]);
      }
      *(outbuffer + offset + 0) = (this->flow_y_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->flow_y_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->flow_y_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->flow_y_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->flow_y_length);
      for( uint32_t i = 0; i < flow_y_length; i++){
      union {
        float real;
        uint32_t base;
      } u_flow_yi;
      u_flow_yi.real = this->flow_y[i];
      *(outbuffer + offset + 0) = (u_flow_yi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_flow_yi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_flow_yi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_flow_yi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->flow_y[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->height =  ((uint32_t) (*(inbuffer + offset)));
      this->height |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->height |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->height |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->height);
      this->width =  ((uint32_t) (*(inbuffer + offset)));
      this->width |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->width |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->width |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->width);
      uint32_t flow_x_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      flow_x_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      flow_x_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      flow_x_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->flow_x_length);
      if(flow_x_lengthT > flow_x_length)
        this->flow_x = (float*)realloc(this->flow_x, flow_x_lengthT * sizeof(float));
      flow_x_length = flow_x_lengthT;
      for( uint32_t i = 0; i < flow_x_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_flow_x;
      u_st_flow_x.base = 0;
      u_st_flow_x.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_flow_x.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_flow_x.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_flow_x.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_flow_x = u_st_flow_x.real;
      offset += sizeof(this->st_flow_x);
        memcpy( &(this->flow_x[i]), &(this->st_flow_x), sizeof(float));
      }
      uint32_t flow_y_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      flow_y_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      flow_y_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      flow_y_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->flow_y_length);
      if(flow_y_lengthT > flow_y_length)
        this->flow_y = (float*)realloc(this->flow_y, flow_y_lengthT * sizeof(float));
      flow_y_length = flow_y_lengthT;
      for( uint32_t i = 0; i < flow_y_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_flow_y;
      u_st_flow_y.base = 0;
      u_st_flow_y.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_flow_y.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_flow_y.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_flow_y.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_flow_y = u_st_flow_y.real;
      offset += sizeof(this->st_flow_y);
        memcpy( &(this->flow_y[i]), &(this->st_flow_y), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "esim_msgs/OpticFlow"; };
    const char * getMD5(){ return "1d447e443e01be233472e6f68c671fd7"; };

  };

}
#endif
