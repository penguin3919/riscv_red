#ifndef __KITE_DATA_MEMORY_H__
#define __KITE_DATA_MEMORY_H__

#include <stdint.h>
#include "inst.h"

class data_cache_t;

// Memory
class data_memory_t {
public:
    data_memory_t(uint64_t *m_ticks, uint64_t m_memory_size, uint64_t m_latency = 0,uint32_t memory_choice=0);
    ~data_memory_t();

    void connect(data_cache_t *m_cache);                    // Connect to the upper-level cache.
    void load_block(uint64_t m_addr, uint64_t m_block_size);// Load a memory block.
    void run();                                             // Run the data memory.
    void print_state(uint32_t choice) const;                               // Print memory state.
    void read(inst_t *m_inst);                  // Read data from cache.
    void write(inst_t *m_inst);                 // Write data in cache.

private:
    void load_memory_state(uint32_t choice);                               // Load initial memory state.

    data_cache_t *cache;                                    // Pointer to the upper-level cache
    uint64_t *ticks;                                        // Pointer to processor ticks

    int64_t *memory;                                        // Memory space
    bool *accessed;                                         // Access history map
    uint64_t memory_size;                                   // Memory size
    uint64_t num_dwords;                                    // Number of doublewords

    uint64_t latency;                                       // Memory latency
    uint64_t resp_ticks;                                    // Response ticks
    int64_t *req_block;                                     // Requested block data
    uint32_t choice;                                        // Memory choice(0: mem, 1: flash)
};

#endif

