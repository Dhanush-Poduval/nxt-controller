from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():

    robot_desc = Command([
        "xacro ",
        os.path.join(
            get_package_share_directory("nxt_hardware"),
            "models/robot.urdf.xacro"
        )
    ])

    rsp = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_desc}]
    )

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            {"robot_description": robot_desc},
            os.path.join(
                get_package_share_directory("nxt_hardware"),
                "config/diff_drive_controller.yaml"
            )
        ],
        output="screen"
    )

    return LaunchDescription([
        rsp,
        control_node
    ])
