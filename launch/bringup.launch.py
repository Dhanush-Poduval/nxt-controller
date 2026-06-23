from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterFile
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
            ParameterFile(controller_yaml, allow_substs=True),
            {"diff_drive_controller.enable_stamped_cmd_vel":False}
        ],
        remappings=[
            ("/diff_drive_controller/cmd_vel", "/cmd_vel")
        ],
        output="screen"
    )

    # 1. Spawn the Joint State Broadcaster
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"],
        output="screen"
    )

    # 2. Spawn the Diff Drive Controller and pass the param file explicitly to the spawner
    diff_drive_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "diff_drive_controller", 
            "--controller-manager", "/controller_manager"
        ],
        output="screen"
    )

    return LaunchDescription([
        rsp,
        control_node,
        joint_state_broadcaster_spawner,
        diff_drive_controller_spawner
    ])
