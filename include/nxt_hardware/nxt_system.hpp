#pragma once 
#include <vector>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "nxt_hardware/nxt.h"
namespace nxt_hardware {
  class NXTSystem: public hardware_interface::SystemInterface{
    public:
      hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
      std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
      std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
      hardware_interface::return_type read(
        const rclcpp::Time & time,
        const rclcpp::Duration & period) override;

      hardware_interface::return_type write(
        const rclcpp::Time & time,
        const rclcpp::Duration & period) override;
    private:
      nxt_device_usb_t nxt;
      double left_pos=0.0;
      double right_pos=0.0;
      double left_vel=0.0;
      double right_vel=0.0;
      double left_cmd=0.0;
      double right_cmd=0.0;
  };
}
