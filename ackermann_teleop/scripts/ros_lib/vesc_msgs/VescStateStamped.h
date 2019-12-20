#ifndef _ROS_vesc_msgs_VescStateStamped_h
#define _ROS_vesc_msgs_VescStateStamped_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "vesc_msgs/VescState.h"

namespace vesc_msgs
{

  class VescStateStamped : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef vesc_msgs::VescState _state_type;
      _state_type state;

    VescStateStamped():
      header(),
      state()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->state.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->state.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "vesc_msgs/VescStateStamped"; };
    const char * getMD5(){ return "3a2b3a0e5b5f10ce6bbf973d767cdc4d"; };

  };

}
#endif
