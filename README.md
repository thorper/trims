# trims ROS node

For a step-by-step process to bag a set of QREX data read the following document:
```
Taking_QREX_Data.docx
```
The "trims" ROS node is mainly a package that contains the custom message types to read streaming data from the Parallax Propeller board. Most of the time only a single launch file command is needed to begin collecting data, see below:
```
roslaunch trims MavProp.launch
```
Some calibration automation is available through the calStats node. This node monitors the Futek load cell and Arm Strain gauge analog counts. When these counts steady out a value is published that can be used for deadload calibration purposes. Run the node below using:
```
rosrun trims calStats
```

## Launch Files
Thare are several launch files in the launch folder that interface with nodes built to read the old arduino nanos and other launch files are for diagnostic purposes.

### MavProp.launch
This is the only launch file that is needed to gather flight data while flying the QREX. The data is saved as a ROS bag file and is stored locally on the Raspberry Pi.

## Message Types

### AAS.msg
ROS message type for reading streaming data from the Arduino Nano data acquisition shields. This message type was used to generate the data in the 2019 Conference Paper Titled: Test Vehicle for Determination of Multi-Rotor UAV Trim Conditions

- Header header *ROS time stamp generated from Raspberry Pi's clock*
- float32 voltage *Motor Voltage in analog counts*
- float32 current *Motor Current in analog counts*
- float32 thrust  *Motor Honeywell Load cell reading in analog counts*
- float32 ethrust *Arm strain gauge reading in analog counts*
- uint16 rpm *Motor RPM in rotations per minute*

### PROP.msg
This is the current ROS message type used as of 2/3/2020. It reads the messages published by the Parallax Propeller board. Dr. McCrink authored low-level codes to make the Parallax board appear to be a simple arduino board.

- uint16 V1 *Motor Voltage in analog counts*
- uint16 V2
- uint16 V3
- uint16 V4
- uint16 C1 *Motor Current in analog counts*
- uint16 C2
- uint16 C3
- uint16 C4
- uint16 THRUST1 *Motor Futek Load cell reading in analog counts*
- uint16 STRAIN1 *Arm strain gauge reading in analog counts*
- uint16 THRUST2
- uint16 STRAIN2
- uint16 THRUST3
- uint16 STRAIN3
- uint16 THRUST4
- uint16 STRAIN4
- uint16 RPM1 *Motor RPM in rotations per minute*
- uint16 RPM2
- uint16 RPM3
- uint16 RPM4

### PROP_check.msg
This message displays ERROR High, ERROR Low or OK depending on the analog reading. The analog reading thresholds are located in the source code file *msg_check.cpp*

### PROP_eng_units.msg
This message converts the analog readings in the PROP.msg to engineering units for purely diagnostic purposes. The gain values are nominal and should not be used in any data analysis. These values were meant to give field testers a general idea of the motor speeds, thrusts, voltages and currents to ensure all sensors are behaving properly before and after flights. The nominal analog to engineering unit gains are in *msg_units.cpp*

### PROP_stamped.msg
This is the same message as PROP.msg except that it has an ROS timestamp that is pulled from the Raspberry Pi system clock.

The Vertical Flight Society 2019 conference paper that this ROS-node was used in can be found [here]

[here]: https://vtol.org/store/product/flight-test-vehicle-for-determination-of-multirotor-uav-trim-conditions-14600.cfm
