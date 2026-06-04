from launch import LaunchDescription
from launch.descriptions import executable
from launch_ros.actions import Node
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterFile
from launch_ros.substitutions import FindPackageShare
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():

    xacro_file = os.path.join(
        get_package_share_directory("nxt_hardware"),
        "models",
        "robot.urdf.xacro"
    )

    robot_desc = Command(["xacro ", xacro_file])

    controller_yaml = os.path.join(
        get_package_share_directory("nxt_hardware"),
        "config",
        "diff_drive_controller.yaml"
    )
    # teleop = Node(
    #     package="teleop_twist_keyboard",
    #     executable="teleop_twist_keyboard",
    #     prefix="xterm -e",
    #     remappings=[("cmd_vel","/diff_drive_controller/cmd_vel")]
    # )
    rsp = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_desc}],
        output="screen"
    )

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            {"robot_description": robot_desc},
            ParameterFile(controller_yaml, allow_substs=True)
        ],
        output="screen"
    )

    return LaunchDescription([
        rsp,
        control_node,
        #teleop
    ])
