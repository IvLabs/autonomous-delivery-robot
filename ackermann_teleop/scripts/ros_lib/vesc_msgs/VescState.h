#ifndef _ROS_vesc_msgs_VescState_h
#define _ROS_vesc_msgs_VescState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace vesc_msgs
{

  class VescState : public ros::Msg
  {
    public:
      typedef double _voltage_input_type;
      _voltage_input_type voltage_input;
      typedef double _temperature_pcb_type;
      _temperature_pcb_type temperature_pcb;
      typedef double _current_motor_type;
      _current_motor_type current_motor;
      typedef double _current_input_type;
      _current_input_type current_input;
      typedef double _speed_type;
      _speed_type speed;
      typedef double _duty_cycle_type;
      _duty_cycle_type duty_cycle;
      typedef double _charge_drawn_type;
      _charge_drawn_type charge_drawn;
      typedef double _charge_regen_type;
      _charge_regen_type charge_regen;
      typedef double _energy_drawn_type;
      _energy_drawn_type energy_drawn;
      typedef double _energy_regen_type;
      _energy_regen_type energy_regen;
      typedef double _displacement_type;
      _displacement_type displacement;
      typedef double _distance_traveled_type;
      _distance_traveled_type distance_traveled;
      typedef int32_t _fault_code_type;
      _fault_code_type fault_code;
      enum { FAULT_CODE_NONE = 0 };
      enum { FAULT_CODE_OVER_VOLTAGE = 1 };
      enum { FAULT_CODE_UNDER_VOLTAGE = 2 };
      enum { FAULT_CODE_DRV8302 = 3 };
      enum { FAULT_CODE_ABS_OVER_CURRENT = 4 };
      enum { FAULT_CODE_OVER_TEMP_FET = 5 };
      enum { FAULT_CODE_OVER_TEMP_MOTOR = 6 };

    VescState():
      voltage_input(0),
      temperature_pcb(0),
      current_motor(0),
      current_input(0),
      speed(0),
      duty_cycle(0),
      charge_drawn(0),
      charge_regen(0),
      energy_drawn(0),
      energy_regen(0),
      displacement(0),
      distance_traveled(0),
      fault_code(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_voltage_input;
      u_voltage_input.real = this->voltage_input;
      *(outbuffer + offset + 0) = (u_voltage_input.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_voltage_input.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_voltage_input.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_voltage_input.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_voltage_input.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_voltage_input.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_voltage_input.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_voltage_input.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->voltage_input);
      union {
        double real;
        uint64_t base;
      } u_temperature_pcb;
      u_temperature_pcb.real = this->temperature_pcb;
      *(outbuffer + offset + 0) = (u_temperature_pcb.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_temperature_pcb.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_temperature_pcb.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_temperature_pcb.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_temperature_pcb.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_temperature_pcb.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_temperature_pcb.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_temperature_pcb.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->temperature_pcb);
      union {
        double real;
        uint64_t base;
      } u_current_motor;
      u_current_motor.real = this->current_motor;
      *(outbuffer + offset + 0) = (u_current_motor.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current_motor.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_current_motor.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_current_motor.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_current_motor.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_current_motor.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_current_motor.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_current_motor.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->current_motor);
      union {
        double real;
        uint64_t base;
      } u_current_input;
      u_current_input.real = this->current_input;
      *(outbuffer + offset + 0) = (u_current_input.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current_input.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_current_input.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_current_input.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_current_input.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_current_input.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_current_input.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_current_input.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->current_input);
      union {
        double real;
        uint64_t base;
      } u_speed;
      u_speed.real = this->speed;
      *(outbuffer + offset + 0) = (u_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_speed.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_speed.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_speed.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_speed.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_speed.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->speed);
      union {
        double real;
        uint64_t base;
      } u_duty_cycle;
      u_duty_cycle.real = this->duty_cycle;
      *(outbuffer + offset + 0) = (u_duty_cycle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_duty_cycle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_duty_cycle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_duty_cycle.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_duty_cycle.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_duty_cycle.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_duty_cycle.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_duty_cycle.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->duty_cycle);
      union {
        double real;
        uint64_t base;
      } u_charge_drawn;
      u_charge_drawn.real = this->charge_drawn;
      *(outbuffer + offset + 0) = (u_charge_drawn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_charge_drawn.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_charge_drawn.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_charge_drawn.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_charge_drawn.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_charge_drawn.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_charge_drawn.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_charge_drawn.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->charge_drawn);
      union {
        double real;
        uint64_t base;
      } u_charge_regen;
      u_charge_regen.real = this->charge_regen;
      *(outbuffer + offset + 0) = (u_charge_regen.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_charge_regen.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_charge_regen.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_charge_regen.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_charge_regen.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_charge_regen.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_charge_regen.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_charge_regen.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->charge_regen);
      union {
        double real;
        uint64_t base;
      } u_energy_drawn;
      u_energy_drawn.real = this->energy_drawn;
      *(outbuffer + offset + 0) = (u_energy_drawn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_energy_drawn.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_energy_drawn.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_energy_drawn.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_energy_drawn.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_energy_drawn.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_energy_drawn.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_energy_drawn.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->energy_drawn);
      union {
        double real;
        uint64_t base;
      } u_energy_regen;
      u_energy_regen.real = this->energy_regen;
      *(outbuffer + offset + 0) = (u_energy_regen.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_energy_regen.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_energy_regen.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_energy_regen.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_energy_regen.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_energy_regen.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_energy_regen.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_energy_regen.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->energy_regen);
      union {
        double real;
        uint64_t base;
      } u_displacement;
      u_displacement.real = this->displacement;
      *(outbuffer + offset + 0) = (u_displacement.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_displacement.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_displacement.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_displacement.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_displacement.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_displacement.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_displacement.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_displacement.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->displacement);
      union {
        double real;
        uint64_t base;
      } u_distance_traveled;
      u_distance_traveled.real = this->distance_traveled;
      *(outbuffer + offset + 0) = (u_distance_traveled.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_distance_traveled.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_distance_traveled.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_distance_traveled.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_distance_traveled.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_distance_traveled.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_distance_traveled.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_distance_traveled.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->distance_traveled);
      union {
        int32_t real;
        uint32_t base;
      } u_fault_code;
      u_fault_code.real = this->fault_code;
      *(outbuffer + offset + 0) = (u_fault_code.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_fault_code.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_fault_code.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_fault_code.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->fault_code);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_voltage_input;
      u_voltage_input.base = 0;
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_voltage_input.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->voltage_input = u_voltage_input.real;
      offset += sizeof(this->voltage_input);
      union {
        double real;
        uint64_t base;
      } u_temperature_pcb;
      u_temperature_pcb.base = 0;
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_temperature_pcb.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->temperature_pcb = u_temperature_pcb.real;
      offset += sizeof(this->temperature_pcb);
      union {
        double real;
        uint64_t base;
      } u_current_motor;
      u_current_motor.base = 0;
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_current_motor.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->current_motor = u_current_motor.real;
      offset += sizeof(this->current_motor);
      union {
        double real;
        uint64_t base;
      } u_current_input;
      u_current_input.base = 0;
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_current_input.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->current_input = u_current_input.real;
      offset += sizeof(this->current_input);
      union {
        double real;
        uint64_t base;
      } u_speed;
      u_speed.base = 0;
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_speed.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->speed = u_speed.real;
      offset += sizeof(this->speed);
      union {
        double real;
        uint64_t base;
      } u_duty_cycle;
      u_duty_cycle.base = 0;
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_duty_cycle.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->duty_cycle = u_duty_cycle.real;
      offset += sizeof(this->duty_cycle);
      union {
        double real;
        uint64_t base;
      } u_charge_drawn;
      u_charge_drawn.base = 0;
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_charge_drawn.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->charge_drawn = u_charge_drawn.real;
      offset += sizeof(this->charge_drawn);
      union {
        double real;
        uint64_t base;
      } u_charge_regen;
      u_charge_regen.base = 0;
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_charge_regen.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->charge_regen = u_charge_regen.real;
      offset += sizeof(this->charge_regen);
      union {
        double real;
        uint64_t base;
      } u_energy_drawn;
      u_energy_drawn.base = 0;
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_energy_drawn.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->energy_drawn = u_energy_drawn.real;
      offset += sizeof(this->energy_drawn);
      union {
        double real;
        uint64_t base;
      } u_energy_regen;
      u_energy_regen.base = 0;
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_energy_regen.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->energy_regen = u_energy_regen.real;
      offset += sizeof(this->energy_regen);
      union {
        double real;
        uint64_t base;
      } u_displacement;
      u_displacement.base = 0;
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_displacement.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->displacement = u_displacement.real;
      offset += sizeof(this->displacement);
      union {
        double real;
        uint64_t base;
      } u_distance_traveled;
      u_distance_traveled.base = 0;
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_distance_traveled.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->distance_traveled = u_distance_traveled.real;
      offset += sizeof(this->distance_traveled);
      union {
        int32_t real;
        uint32_t base;
      } u_fault_code;
      u_fault_code.base = 0;
      u_fault_code.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_fault_code.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_fault_code.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_fault_code.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->fault_code = u_fault_code.real;
      offset += sizeof(this->fault_code);
     return offset;
    }

    const char * getType(){ return "vesc_msgs/VescState"; };
    const char * getMD5(){ return "81214bb4c1945e7c159bd76ec397ac04"; };

  };

}
#endif
