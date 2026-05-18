Device Plugin Interface

## Goal

The goal is:
	load devices dynamically
	allow C++ and Rust plugins
	keep ABI stable
	allow runtime discovery of device capability
	
1. Core Idea
A device plugin is a dynamic library:
Linux:
	libdevice_filter.so
	libdevice_camera.so
	libdevice_fft.so

Loaded by the runtime using:
	dlopen()

Each plugin exposes a standard C interface.
Important rule:
	Always use C ABI for plugin boundary

This allows Rust, C++, or even Python plugins.

2. Device Plugin Architecture
	Runtime Engine
		  │
		  ▼
	Plugin Loader
		  │
		  ▼
	Device Plugin (.so / .dll)
		  │
		  ▼
	Device Instance
	
Flow:
	load plugin
	discover device type
	instantiate device
	connect channels
	execute
	
3. Device Lifecycle
Every device follows this lifecycle:
	create
	configure
	connect_channels
	start
	process
	stop
	destroy
	
Execution loop:
process() called repeatedly
or
device thread runs process loop



4. Core Device Interface (C ABI)
Header file shared by runtime and plugins.
typedef struct DeviceAPI DeviceAPI;


typedef struct DeviceContext
{
    void* runtime;
    void* channels;
} DeviceContext;


struct DeviceAPI
{
    void (*configure)(void* instance, const char* json_config);

    void (*connect_input)(void* instance, const char* name, void* channel);
    void (*connect_output)(void* instance, const char* name, void* channel);

    void (*start)(void* instance);
    void (*process)(void* instance);
    void (*stop)(void* instance);

    void (*destroy)(void* instance);
};


5. Plugin Entry Points
Every plugin exports two functions:

extern "C"
{
	const char* device_name();
	void* create_device();
	DeviceAPI* get_device_api();
}


Meaning:
	device_name()  → metadata
	create_device() → new instance
	get_device_api() → function table

This structure describes all device operations.


6. Runtime Plugin Loader
Example loader:
void load_plugin(const char* path)
{
    void* lib = dlopen(path, RTLD_NOW);
    auto create =
        (void*(*)())dlsym(lib,"create_device");
    auto get_api =
        (DeviceAPI*(*)())dlsym(lib,"get_device_api");
    void* device = create();
    DeviceAPI* api = get_api();
}

Now the runtime can call:
	api->start(device);
	api->process(device);
	
	

7. Device Class (Internal C++ Implementation)
Inside plugin:
class FilterDevice
{
public:

   Channel* input;
   Channel* output;

   float alpha;

   void process()
   {
       float sample = input->read();

       float result = alpha * sample;

       output->write(result);
   }
};

Wrapper functions expose this class through C ABI.



8. Channel Interface
	Devices should not know shared memory details.
	They receive abstract channels.

Example:

struct Channel
{
   void* memory;

   void (*write)(Channel*, void*);
   void (*read)(Channel*, void*);
};

Runtime attaches actual implementation.



9. Example Plugin: Low Pass Filter
Simplified example.

class LowPass
{
public:

   Channel* in;
   Channel* out;

   float alpha;

   void process()
   {
      float x;
      in->read(in,&x);

      float y = alpha * x;

      out->write(out,&y);
   }
};

Plugin API wrappers:
void process(void* instance)
{
   ((LowPass*)instance)->process();
}

10. Rust Device Plugin
Because we use C ABI, Rust plugins are easy.

Example:

#[repr(C)]
pub struct DeviceAPI {
    process: extern "C" fn(*mut c_void)
}

Device implementation:

pub struct FFTDevice {
   input: *mut Channel,
   output: *mut Channel
}

Export entrypoints:

#[no_mangle]
pub extern "C" fn create_device() -> *mut c_void {
    Box::into_raw(Box::new(FFTDevice::new()))
}
11. Device Metadata System

Each plugin should also describe:

inputs
outputs
parameters
UI metadata

Example JSON metadata:

{
 "device":"LowPassFilter",
 "inputs":["signal"],
 "outputs":["filtered"],
 "parameters":[
   {"name":"alpha","type":"float"}
 ]
}

The frontend uses this to build nodes automatically.

12. Plugin Directory Layout

Example:
plugins
   filters
       liblowpass.so
       libhighpass.so

   instruments
       liboscilloscope.so
       libsignalgen.so

   analysis
       libfft.so
       libai_detector.so
       
13. Device Isolation Strategy
Large automation systems isolate devices.

Two models:
In-process
plugin inside runtime

Fastest.

Out-of-process
plugin runs as separate process

Safer.

Used for:

hardware drivers
unstable code

14. Industrial Feature: Capability Discovery
Runtime should query device:

supported sample types
channel formats
execution model

Example:
	float32 stream
	int16 waveform
	image frame
	
15. Threading Model
Device types:

passive device
process() called by scheduler
active device
device runs own thread

Examples:
	camera
	DAQ hardware
	network receiver
	
16. Example Execution Graph with Plugins
SignalGeneratorPlugin
        │
        ▼
LowPassPlugin
        │
        ▼
FFTPlugin
        │
        ▼
PlotPlugin

Runtime builds this graph dynamically from plugins.

17. Recommended Plugin ABI Design
Industrial systems usually define:
	core ABI
	channel ABI
	device ABI
	metadata ABI

Versioning example:
device_api_version = 1
This prevents compatibility issues.

18. Critical Rule
	Never expose C++ classes across plugin boundary.
Always expose:
	opaque pointer
	C functions

Otherwise ABI breaks between compilers.








