#ifndef _ROS_dvs_msgs_EventArray_h
#define _ROS_dvs_msgs_EventArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "dvs_msgs/Event.h"

namespace dvs_msgs
{

  class EventArray : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint32_t _height_type;
      _height_type height;
      typedef uint32_t _width_type;
      _width_type width;
      uint32_t events_length;
      typedef dvs_msgs::Event _events_type;
      _events_type st_events;
      _events_type * events;

    EventArray():
      header(),
      height(0),
      width(0),
      events_length(0), events(NULL)
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
      *(outbuffer + offset + 0) = (this->events_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->events_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->events_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->events_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->events_length);
      for( uint32_t i = 0; i < events_length; i++){
      offset += this->events[i].serialize(outbuffer + offset);
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
      uint32_t events_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      events_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      events_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      events_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->events_length);
      if(events_lengthT > events_length)
        this->events = (dvs_msgs::Event*)realloc(this->events, events_lengthT * sizeof(dvs_msgs::Event));
      events_length = events_lengthT;
      for( uint32_t i = 0; i < events_length; i++){
      offset += this->st_events.deserialize(inbuffer + offset);
        memcpy( &(this->events[i]), &(this->st_events), sizeof(dvs_msgs::Event));
      }
     return offset;
    }

    const char * getType(){ return "dvs_msgs/EventArray"; };
    const char * getMD5(){ return "5e8beee5a6c107e504c2e78903c224b8"; };

  };

}
#endif
