#include "nxt_hardware/nxt_system.hpp"
#include <cmath>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <vector>
#include "hardware_interface/types/hardware_interface_type_values.hpp"
namespace nxt_hardware {
  hardware_interface::CallbackReturn NXTSystem::on_init(const hardware_interface::HardwareInfo & info){
    if(hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS){
      return hardware_interface::CallbackReturn::ERROR;
    };
    return hardware_interface::CallbackReturn::SUCCESS;
  };
  std::vector<hardware_interface::StateInterface>NXTSystem::export_state_interfaces(){
    std::vector<hardware_interface::StateInterface> state_interfaces;
    state_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_POSITION,&left_pos);
    state_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_VELOCITY,&left_vel);
    state_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_POSITION,&right_pos);
    state_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_VELOCITY,&right_vel);
    return state_interfaces;
  };
  std::vector<hardware_interface::CommandInterface>NXTSystem::export_command_interfaces(){
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    command_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_VELOCITY,&left_cmd);
    command_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_VELOCITY,&right_cmd);
    return command_interfaces;
  };
  hardware_interface::return_type NXTSystem::read(const rclcpp::Time & /*time*/,const rclcpp::Duration & period){
    left_vel=left_cmd;
    right_vel=right_cmd;
    left_pos+=left_vel * period.seconds();
    right_pos+=right_vel * period.seconds();
    return hardware_interface::return_type::OK;
  };
  hardware_interface::return_type NXTSystem::write(const rclcpp::Time & /*time */, const rclcpp::Duration & /*period */){
    if(std::abs(left_cmd)>0.001 || std::abs(right_cmd)>0.001){
      std::cout<<"left_cmd : "<<left_cmd<<" right_cmd : "<<right_cmd<<std::endl;
    }
    return hardware_interface::return_type::OK;
  };

};

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
    nxt_hardware::NXTSystem,
    hardware_interface::SystemInterface
)
