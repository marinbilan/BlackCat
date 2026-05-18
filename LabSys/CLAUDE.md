# LabSys
[ MODERN AUTOMATION SYSTEM ]

Progress:
[3.17.2026]
Control Plane (NATS) ----> Data plane (shmem, ring buffers)

Channel Registry
Device Registry

Use JSON instead XML -> Graph Compiler
For now lets implement SPSC model of channel
I like your minimal path for now 

Let's focus on Runtime Manager as main orchestrator with states (Create -> Init -> Create Channels -> Attach)
But here we need to distinct some things:
We need Channel Registry as singleton (or some design similar), created before all devices
Also, we need Factory
Create FIRST output channels:
We need each device communicate with Channel Registry which receive channels name and create output channels 
AFTER output channel - attach input channels to already created output channels (for each device)


## Overview and Goals

This project is inspired by tools like NI LabVIEW, MATLAB Simulink. Idea is to build maximal flexible system which will provide remote control (via web browser). It will be possible to operate with physical instances (in real lab environment):
PID regulators
Measurement devices 
Spectrum 
Analyzers
Signal Generators
...
and simulated devices on PC (for example using Z-transform):
FIR filters,
Discrete Systems (PID -> discrete PID)
Different digital filters 
...


## Architecture
For detailed general architecture, check:
./general_architecture.md

For now I would like to separate project in two 'modules':
Frontend 
Backend

[Frontend devices creation] <-- JSON Configuration --> [Backend devices creation]
Both sides should be aware of devices related to configuration (there will be many configurations - flexible design).

JSON Configuration example:
{
  "devices": [
    {
      "type": "DeviceType_0",
      "role": "Master",
      "instances": ["DeviceType_0_Dev_0", "DeviceType_0_Dev_1"]
    },
    {
      "type": "DeviceType_1",
      "role": "Master",
      "instances": ["DeviceType_1_Dev_0", "DeviceType_1_Dev_1"]
    }
  ]
}


>>>> [Frontend]
./frontend_architecture.md
Frontend should use Chart Flow as starting point where devices will be able to be drag'n'droped and control (as nodes).
React (Grafana) for visualization (this is open, but in general widgets - voltage, current ... should be able to
receive data (JSONs) from backend and visualize some state from particular device (ex: voltage on channel_N on device_N).
Also this signal should be able to receive dynamically and update widget display periodically.


>>>> [Backend]
./device_plugin_interface.md
./shared_memory_channels.md


Rust/C++
For C++ build and package management we will use:
cMake
Conan 2.0


Software Structure:

Each device should have 'Master class' device, which is always created and it behaves as manager.
In general 'Master' device receives signals (from frontend) related to creation of all devices from configuration.

Signal -- JSON configuration --> Master extract data about devices and create devices.
Example:
Let's suppose lab has 3 different Signal Generators (from differen vendors).
Signal [JSON] -- SG_0, SG_1, SG_2 --> Signal_Generator_Master creates and manages 3 devices.

Signal_Generator_Master
	|
	+- SG_0
	|
	+- SG_1
	|
	+- SG_2

Each 'set of devices' [MasterDevice and Device0, Device1, Device2 ...] 'live' in it's own process 
and structure of each process should be something like:

/common
/libs
...
/device
	/if  - virtual interface(s)
	/inc - all .h files
	/src - all .cpp files 

In General
Devices can be written in Rust and/or C++, so some kind of (interprocess) communication should be established.
For example gRPC (but this is open).

+----------------------+
| receiver (from front)| -- Reference to --> [MasterDevice0 (Dev0, Dev1, Dev2 ...) - get from JSON config]
| Runtime Manager      | -- Reference to --> [MasterDevice1 (Dev0, Dev1, Dev2 ...) - get from JSON config]
|                      | -- Reference to --> [MasterDevice2 (Dev0, Dev1, Dev2 ...) - get from JSON config]
+----------------------+

Runtime Manager receives messages from frontend in JSON form (but this is also open).

State Machine - Runtime Manager should be able to change state.
	Create          - Send message to Master's (to each process) to create dynamically Devices (Objects)
	Init            - Send message to Master's to init devices (get parameters from config JSON)
	Create Channels - This is open (should each device create channels or channels should be created from 
			  configuration files (predefined). Each device create unique output channels.

		For example:
		+---------+
		|         | -- OutputChannel.Device0.Ch0
		| Device0 | -- OutputChannel.Device0.Ch1
		|	  | -- OutputChannel.Device0.Ch2
		+---------+			
		All channels should be registered in Channel Registry (this is also device) with Master-Devices structure.
	Attach to output channel(s) - Each device should attach (input) on output channel. Becasuse output channels
	                              are already created.

		Example:
		+---------+
		|         | -- OutputChannel.Device0.Ch0
		| Device0 | -- OutputChannel.Device0.Ch1                               +---------+
		|	  | -- OutputChannel.Device0.Ch2 -- InputChannel.Device3.Ch0 --|         |
		+---------+	                                                       | Device3 | -- OutputChannel.Device3.Ch0
		+---------+                                                            |         |
		|         | -- OutputChannel.Device1.Ch0 -- InputChannel.Device3.Ch1 --|         |
		| Device1 | -- OutputChannel.Device1.Ch1                               +---------+
		|	  | -- OutputChannel.Device1.Ch2
		+---------+	

Other possible devices will be mix between 'real physical devices' and simulated devices like:
- PID regulators
- FIR filters
- Different analog/digital filters
- Scripting devices (possibility to change channel's value in runtime) (python or some other scripting language)
- Recorder devices 
- Measurement devices 
- Storage devices - Communication with DB 
- High frequency logger (possible to log in high speed from different concurrent processes)

Also we will not differentiate for now real time executors and other devices.
Real Time devices will be (for now) implemented as simple 'while loop' Task(s).

We will investigate in details real time systems when architecture will be established.
Special devices will be implemented to handle different types of Real-Time systems from 
simple (while loops) to more sophisticated.

Topics:
1. Hard Real-Time
2. Firm Real-Time
3. Soft Real-Time

Real-Time Scheduling
	Static Scheduling
	Dynamic Scheduling
	Earliest Deadline First (EDF) systems
	Real-Time Operating Systems (RTOS)
	...


## Progress / Notes

<!-- Running notes, decisions, ideas -->

## TODO

- [ ]





























