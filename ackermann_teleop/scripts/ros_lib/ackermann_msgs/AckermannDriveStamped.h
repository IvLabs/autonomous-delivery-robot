#ifndef _ROS_ackermann_msgs_AckermannDriveStamped_h
#define _ROS_ackermann_msgs_AckermannDriveStamped_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ackermann_msgs/AckermannDrive.h"

namespace ackermann_msgs
{

  class AckermannDriveStamped : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef ackermann_msgs::AckermannDrive _drive_type;
      _drive_type drive;

    AckermannDriveStamped():
      header(),
      drive()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->drive.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->drive.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "ackermann_msgs/AckermannDriveStamped"; };
    const char * getMD5(){ return "1fd5d7f58889cefd44d29f6653240d0c"; };

  };

}
#endif
