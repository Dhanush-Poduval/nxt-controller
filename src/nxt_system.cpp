#include "nxt_hardware/nxt_system.hpp"
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include <vector.h>
#include "hardware_interface/types/hardware_interface_type_values.hpp"
using namespace std;
namespace nxt_hardware {
  hardware_interface::CallbackReturn NXT_SYSTEM::on_init(const hardware_interface::HardwareInf & info){
    if(hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS){
      return hardware_interface::CallbackReturn::ERROR;
    };
    return hardware_interface::CallbackReturn::SUCCESS;
  };
  vector<hardware_interface::StateInterface>NXTSystem::export_state_interface(){
    vector<hardware_interface::StateInterface> state_interfaces;
    state_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_POSITION,&left_pos);
    state_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_VELOCITY,&left_vel);
    state_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_POSITION,&right_pos);
    state_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_VELOCITY,&right_vel);
    return state_interfaces;
  };
  vector<hardware_interface::CommandInterface>NXTSystem::export_command_interface(){
    vector<hardware_interface::CommandInterface> command_interfaces;
    command_interfaces.emplace_back("left_wheel_joint",hardware_interface::HW_IF_VELOCITY,&left_cmd);
    command_interfaces.emplace_back("right_wheel_joint",hardware_interface::HW_IF_VELOCITY,&right_cmd);
    return command_interfaces;
  };
  hardware_interface::return_type NXTSystem::read(const rclcpp::Time & /*time*/,const rclcpp::Duration & period){
    left_vel=left_cmd;
    right_vel=right_cmd;
    left_pos=left_vel * period.seconds();
    right_pos=right_vel * period.seconds();
    return hardware_interface::return_type::OK;
  };
  hardware_interface::return_type NXTSystem::write(const rclcpp::Time & /*time */, const rclcpp::Duration & /*period */){
    return hardware_interface::return_type::OK;
  };

};

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
    nxt_hardware::NXTSystem,
    hardware_interface::SystemInterface
)
