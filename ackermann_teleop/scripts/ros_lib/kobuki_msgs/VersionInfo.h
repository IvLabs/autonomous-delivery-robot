#ifndef _ROS_kobuki_msgs_VersionInfo_h
#define _ROS_kobuki_msgs_VersionInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace kobuki_msgs
{

  class VersionInfo : public ros::Msg
  {
    public:
      typedef const char* _hardware_type;
      _hardware_type hardware;
      typedef const char* _firmware_type;
      _firmware_type firmware;
      typedef const char* _software_type;
      _software_type software;
      uint32_t udid_length;
      typedef uint32_t _udid_type;
      _udid_type st_udid;
      _udid_type * udid;
      typedef uint64_t _features_type;
      _features_type features;
      enum { SMOOTH_MOVE_START =  0000000000000001 };
      enum { GYROSCOPE_3D_DATA =  0000000000000002 };

    VersionInfo():
      hardware(""),
      firmware(""),
      software(""),
      udid_length(0), udid(NULL),
      features(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_hardware = strlen(this->hardware);
      varToArr(outbuffer + offset, length_hardware);
      offset += 4;
      memcpy(outbuffer + offset, this->hardware, length_hardware);
      offset += length_hardware;
      uint32_t length_firmware = strlen(this->firmware);
      varToArr(outbuffer + offset, length_firmware);
      offset += 4;
      memcpy(outbuffer + offset, this->firmware, length_firmware);
      offset += length_firmware;
      uint32_t length_software = strlen(this->software);
      varToArr(outbuffer + offset, length_software);
      offset += 4;
      memcpy(outbuffer + offset, this->software, length_software);
      offset += length_software;
      *(outbuffer + offset + 0) = (this->udid_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->udid_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->udid_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->udid_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->udid_length);
      for( uint32_t i = 0; i < udid_length; i++){
      *(outbuffer + offset + 0) = (this->udid[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->udid[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->udid[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->udid[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->udid[i]);
      }
      *(outbuffer + offset + 0) = (this->features >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->features >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->features >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->features >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->features >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->features >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->features >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->features >> (8 * 7)) & 0xFF;
      offset += sizeof(this->features);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_hardware;
      arrToVar(length_hardware, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_hardware; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_hardware-1]=0;
      this->hardware = (char *)(inbuffer + offset-1);
      offset += length_hardware;
      uint32_t length_firmware;
      arrToVar(length_firmware, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_firmware; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_firmware-1]=0;
      this->firmware = (char *)(inbuffer + offset-1);
      offset += length_firmware;
      uint32_t length_software;
      arrToVar(length_software, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_software; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_software-1]=0;
      this->software = (char *)(inbuffer + offset-1);
      offset += length_software;
      uint32_t udid_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      udid_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      udid_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      udid_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->udid_length);
      if(udid_lengthT > udid_length)
        this->udid = (uint32_t*)realloc(this->udid, udid_lengthT * sizeof(uint32_t));
      udid_length = udid_lengthT;
      for( uint32_t i = 0; i < udid_length; i++){
      this->st_udid =  ((uint32_t) (*(inbuffer + offset)));
      this->st_udid |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_udid |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_udid |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->st_udid);
        memcpy( &(this->udid[i]), &(this->st_udid), sizeof(uint32_t));
      }
      this->features =  ((uint64_t) (*(inbuffer + offset)));
      this->features |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->features |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->features |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->features |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->features |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->features |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->features |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->features);
     return offset;
    }

    const char * getType(){ return "kobuki_msgs/VersionInfo"; };
    const char * getMD5(){ return "3dc0ce6edd49be1fc86730f4a4a21861"; };

  };

}
#endif
