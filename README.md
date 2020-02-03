# trims ROS node

The "trims" ROS node is mainly a package that contains the custom message types to read streaming data from the Parallax Propeller board. 

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
