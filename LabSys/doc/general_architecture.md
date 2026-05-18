## GENERAL ARCHITECTURE

The Architecture used in modern industrial automation systems (e.g., systems inspired by tools like NI LabVIEW, MATLAB Simulink, 
and even robotics frameworks like Robot Operating System). 

A web-based front end with a high-performance backend in C++/Rust is a strong combination.


>>>> 1] HIGH LEVEL SYSTEM ARCHITECTURE
Web UI --> Control Server --> Graph Compiler -->
 ┌─────────────────────────────┐
 │        Web Frontend         │
 │ React + ChartFlow Editor    │
 └──────────────┬──────────────┘
                │ WebSocket / REST
 ┌──────────────▼──────────────┐
 │       Control API Layer     │
 │ Node / Rust service         │
 │ Graph execution manager     │
 └──────────────┬──────────────┘
                │
 ┌──────────────▼──────────────┐
 │     Execution Runtime       │
 │ C++ / Rust device runtime   │
 │ Actor system / message bus  │
 └──────────────┬──────────────┘
                │
 ┌──────────────▼──────────────┐
 │   IPC + Shared Memory Bus   │
 │ Channels / streams          │
 └──────────────┬──────────────┘
                │
 ┌──────────────▼──────────────┐
 │ Devices / Drivers / IO      │
 │ instruments, sensors etc    │
 └─────────────────────────────┘
 

>>>> 2] FRONTEND 
 Libs:
 React Flow
 
 frontend
 ├─ graph_editor
 ├─ device_library
 ├─ run_controller
 ├─ realtime_monitor
 └─ project_manager
 
 DeviceNode
    inputs[]
    outputs[]
    config
    
    
Edges represent channels.
When user presses Run, the graph is compiled to JSON:
Example 1:
{
 "devices":[
   { "id":"osc1", "type":"Oscilloscope", "config":{...} },
   { "id":"filter1", "type":"LowPass" }
 ],
 "connections":[
   { "from":"osc1.ch1", "to":"filter1.input" }
 ]
}


Example 2:
{
 "nodes":[
  {"id":"gen1","type":"SignalGenerator"},
  {"id":"filter1","type":"LowPass"},
  {"id":"fft1","type":"FFT"}
 ],
 "edges":[
  {"from":"gen1.out","to":"filter1.in"},
  {"from":"filter1.out","to":"fft1.in"}
 ]
}


>>>> 3] CONTROL LAYER

This layer manages:
projects, graphs, execution sessions, monitoring, scheduling
Recommended implementation: Rust service Axum (safe concurrency, good networking)

Responsibilities: 
	API server
	Graph validator
	Execution compiler
	Runtime supervisor

Communication:
	WebSocket → realtime updates
	REST → configuration 
	
Additionally:
UI graph must be compile to an execution plam:
UI graph
   ↓
validated graph
   ↓
topological sort
   ↓
runtime nodes
   ↓
execution plan

>>>> 4] EXECUTION RUNTIME
RUNTIME MANAGER

Actor model (Each device = actor)
Libraries:
	C++: CAF
	Rust: Actix/Tokio
	
Example of Device Actor:
DeviceActor
   init()
   configure()
   start()
   process()
   stop()
   
Example of messages:
DeviceActor
   init()
   configure()
   start()
   process()
   stop()
   
Additionally:
Responsibilities (Mini orchestration system):
	spawn device actors
	load plugins
	allocate channels
	monitor execution
	restart crashed devices
	
>>>> 5] INTERPROCASS COMMUNICATION
Recommended design (Using Ringbuffers)
Libraries: 
	C++: Boost.Interprocess (modern: Apache Arrow)
	Rust: memmap2, shared_memory
Example:
ChannelRegistry
   allocate channel
   return shared memory pointer
   
struct Channel
{
   atomic<uint64_t> write_index;
   atomic<uint64_t> read_index;

   Sample buffer[N];
};


>>>> 6] MESSAGE BROKER (Control Messages)
Control messages should NOT go through shared memory. Use a message broker.
Lightweight (Recomended):
	NATS
Heavy:
	Apache Kafka

Best in this case (Simple and very fast):
	NATS
	
Additionally (Responsibilities for Message Broker):
	start
	stop
	pause
	configuration
	errors
	monitoring
	
Example messages:
	DeviceStart
	DeviceStop
	DeviceConfig
	DeviceStatus
	
	
>>>> 7] DEVICE PLUGIN SYSTEM
In order not to rebuild system all the time (plugin architecture)
	C++: dlopen()
	Rust: libloading
	
Device Interface example:
class Device
{
public:
   virtual void configure(Config c)=0;
   virtual void start()=0;
   virtual void process()=0;
};


Each plugin exports:
create_device()

Additionally:
Example plugins:
	oscilloscope
	signal generator
	filter
	FFT
	AI analyzer

Interface example:
DeviceAPI
  configure()
  start()
  process()
  stop()
  
  
>>>> 8] BUILD SYSTEM
For mixed C++ + Rust projects the best setup is:
	C++: CMake
	Rust: Cargo
Bridge: cbindgen, bindgen


>>>> 9] DATABASE
For Configuration
	devices
	projects
	graphs
Use:
PostgreSQL

For Time series and Measurements
Use:
TimescaleDB


>>>> 10] EXECUTION MODEL
Graph compiled into runtime graph
Example:
	Oscilloscope -> Filter -> FFT -> Plot
	
Runtime scheduling:
	device threads
	shared memory channels
	event triggers
	
Possible scheduling modes:
	continuous
	triggered
	periodic
	
	
SHARED MEMORY DATA PLANE
Real Signal Data Flow:
Do NOT use message brokers for signal data. Use shared memory channels:
- Zero-copy, very low latency, high throughput
Lib: Books.Interprocess 
Rust: memmap2, shared_memory

Channel Structure:
Channel
 ├── metadata
 ├── ring buffer
 └── synchronization primitives
 
 
Memory Layout:
+------------------+
| write_index      |
| read_index       |
| sample_buffer[]  |
+------------------+
 
 
	
>>>> 11] MONITORING
Critical for automation.
	device status
	channel throughput
	latency
	errors
Use metrics system:
	Prometheus
Visualization:
	Grafana
	
Metrics collected:
	CPU usage
	device latency
	channel throughput
	dropped samples
	errors
	
	
>>>> 12] POSSIBLE PROJECT LAYOUT
automation-platform
│
├─ frontend (React)
│
├─ control-server (Rust)
│
├─ runtime
│   ├─ core (C++)
│   ├─ channels (shared memory)
│   ├─ scheduler
│   └─ device_api
│
├─ devices
│   ├─ oscilloscope
│   ├─ signal_generator
│   └─ filters
│
├─ ipc
│   ├─ message_bus
│   └─ channel_registry
│
├─ storage
│   ├─ postgres
│   └─ timeseries
│
└─ deployment
    ├─ docker
    └─ configs
    
>>>> 13] CRITICAL DESIGN IDEA
Control Plane:
	Commands
	Configuration 
	Monitoring
Data plane:
	Signal Streams 
	Device IO 
	Shared Memory

Execution Scheduling:
Mossible modes:
	Push model 
	Pull model
	Clock driven
	
	
Design Summary:
Frontend:
React + ReactFlow

Control:
Rust (tokio + axum)

Runtime:
C++ core

IPC:
shared memory ring buffers

Control messages:
NATS

Database:
PostgreSQL + Timescale

Plugins:
C++ dynamic libraries

Additionally:
MORE DETAILED INDUSTRIAL ARCHITECTURE

I’ll show the architecture in four layers:
	UI & Modeling
	Control Plane
	Execution Engine
	Data Plane / Device IO

                     ┌──────────────────────────────┐
                     │          Web UI              │
                     │ React + Graph Editor         │
                     │ (ReactFlow / ChartFlow)      │
                     └──────────────┬───────────────┘
                                    │
                             WebSocket / REST
                                    │
                                    ▼
                     ┌──────────────────────────────┐
                     │        Control Server        │
                     │  Project + Graph Manager     │
                     │  Rust (Tokio / Axum)         │
                     └──────────────┬───────────────┘
                                    │
                                    │ Graph JSON
                                    ▼
                     ┌──────────────────────────────┐
                     │        Graph Compiler        │
                     │ converts UI graph → runtime  │
                     │ execution plan               │
                     └──────────────┬───────────────┘
                                    │
                                    ▼
                     ┌──────────────────────────────┐
                     │        Runtime Manager       │
                     │  process supervision         │
                     │  device lifecycle            │
                     └──────────────┬───────────────┘
                                    │
                ┌───────────────────┴───────────────────┐
                │                                       │
                ▼                                       ▼
      ┌──────────────────────┐               ┌──────────────────────┐
      │   Message Broker     │               │    Channel Registry  │
      │  control messages    │               │ shared memory pools  │
      │ (NATS / ZeroMQ)      │               │ channel allocation   │
      └───────────┬──────────┘               └───────────┬──────────┘
                  │                                      │
                  ▼                                      ▼
       ┌──────────────────────┐              ┌──────────────────────┐
       │     Device Actors    │              │ Shared Memory Bus    │
       │  Rust/C++ processes  │◄────────────►│ ring buffer channels │
       │  plugin architecture │              │ zero-copy streams    │
       └───────────┬──────────┘              └───────────┬──────────┘
                   │                                     │
                   ▼                                     ▼
        ┌──────────────────────┐             ┌──────────────────────┐
        │     Device Drivers   │             │      Data Logger     │
        │ instruments / DAQ    │             │ time-series storage  │
        └───────────┬──────────┘             └───────────┬──────────┘
                    │                                    │
                    ▼                                    ▼
           ┌──────────────────┐              ┌──────────────────┐
           │ Hardware Devices │              │ Databases        │
           │ sensors/actuator │              │ Postgres / TSDB  │
           └──────────────────┘              └──────────────────┘
