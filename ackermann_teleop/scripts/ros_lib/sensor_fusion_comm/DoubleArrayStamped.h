#ifndef _ROS_sensor_fusion_comm_DoubleArrayStamped_h
#define _ROS_sensor_fusion_comm_DoubleArrayStamped_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensor_fusion_comm
{

  class DoubleArrayStamped : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t data_length;
      typedef double _data_type;
      _data_type st_data;
      _data_type * data;

    DoubleArrayStamped():
      header(),
      data_length(0), data(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->data_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->data_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->data_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->data_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data_length);
      for( uint32_t i = 0; i < data_length; i++){
      union {
        double real;
        uint64_t base;
      } u_datai;
      u_datai.real = this->data[i];
      *(outbuffer + offset + 0) = (u_datai.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_datai.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_datai.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_datai.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_datai.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_datai.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_datai.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_datai.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->data[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t data_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->data_length);
      if(data_lengthT > data_length)
        this->data = (double*)realloc(this->data, data_lengthT * sizeof(double));
      data_length = data_lengthT;
      for( uint32_t i = 0; i < data_length; i++){
      union {
        double real;
        uint64_t base;
      } u_st_data;
      u_st_data.base = 0;
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_st_data.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->st_data = u_st_data.real;
      offset += sizeof(this->st_data);
        memcpy( &(this->data[i]), &(this->st_data), sizeof(double));
      }
     return offset;
    }

    const char * getType(){ return "sensor_fusion_comm/DoubleArrayStamped"; };
    const char * getMD5(){ return "fb60495edd59d3fcf90e173153ae8a9a"; };

  };

}
#endif
