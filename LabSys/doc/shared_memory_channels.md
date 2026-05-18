## Goal

Designing shared memory channels correctly is one of the most critical parts 
of a high-performance automation system like those behind NI LabVIEW or 
robotics frameworks like Robot Operating System.

Goal is:
	zero-copy data transfer
	very low latency
	safe concurrency between C++ and Rust
	support for streaming signals
The correct structure is typically a lock-free ring buffer in shared memory.

1. Conceptual Channel Model
Each connection between nodes becomes a channel.



2. Channel Architecture
A channel has three layers:
Channel
 ├── metadata
 ├── synchronization primitives
 └── ring buffer
 
Shared Memory Region

+---------------------------------------------------+
| Channel Header                                    |
|---------------------------------------------------|
| write_index (atomic)                              |
| read_index (atomic)                               |
| capacity                                          |
| element_size                                      |
| flags                                             |
+---------------------------------------------------+
|                                                   |
| Ring Buffer                                       |
| sample[0]                                         |
| sample[1]                                         |
| sample[2]                                         |
| ...                                               |
| sample[N-1]                                       |
|                                                   |
+---------------------------------------------------+

3. Channel Header Structure
struct ChannelHeader
{
    std::atomic<uint64_t> write_index;
    std::atomic<uint64_t> read_index;

    uint64_t capacity;
    uint64_t element_size;

    uint32_t flags;
};

Rules:
write_index → producer
read_index → consumer

4. Ring Buffer Logic
Buffer index calculation:
index = counter % capacity
capacity = 8

write_index = 10
10 % 8 = 2

buffer[write_index % capacity]

5. Single Producer Single Consumer Channel
Write logic:
if(write_index - read_index < capacity)
   write element
   write_index++
else
   buffer full
   
Read Logic:
if(write_index > read_index)
   read element
   read_index++
else
   buffer empty
   
6. Memory Layout in Shared Memory
Example of allocation:
+-----------------------------------+
| ChannelHeader                     |
+-----------------------------------+
| sample0                           |
| sample1                           |
| sample2                           |
| sample3                           |
| ...                               |
+-----------------------------------+
Allocation size:
sizeof(ChannelHeader)
+
capacity * element_size

7. Channel Allocation Service
System should have a Channel Registry.
Responsibilities:
	allocate shared memory
	assign channel IDs
	map memory to processes
	track channel metadata
Example API:
	create_channel(name, type, capacity)
	get_channel(name)
	destroy_channel(name)
	
8. Shared Memory Backends
Linux options:
POSIX shared memory
	shm_open
	mmap
Very common in industrial systems.

Faster modern option
	memfd_create
	mmap

Rust libraries:
	memmap2
	shared_memory

C++ libraries:
	Boost.Interprocess
	
9. Example Device Loop
Pseudo code:

while(running)
{
   sample = input_channel.read()

   result = process(sample)

   output_channel.write(result)
}

Latency is extremely small because:
	no serialization
	no network
	no kernel context switch
