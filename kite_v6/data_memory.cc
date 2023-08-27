#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "defs.h"
//#include "data_cache.h"
#include "data_memory.h"

using namespace std;

data_memory_t::data_memory_t(uint64_t *m_ticks, uint64_t m_memory_size, uint64_t m_latency,uint32_t m_choice) :
    cache(0),
    ticks(m_ticks),
    memory(0),
    accessed(0),
    memory_size(m_memory_size),
    num_dwords(m_memory_size>>2),
    latency(m_latency),
    resp_ticks(0),
    req_block(0), 
    choice(m_choice)
{
    // Check if the memory size is a multiple of doubleword.
    if(memory_size & 0b111) {
        cerr << "Error: memory size must be a multiple of doubleword" << endl;
        exit(1);
    }
    if(memory_size < min_memory_size) {
        cerr << "Error: memory size has to be at least 2KB" << endl;
        exit(1);
    }

    // Allocate a memory space in unit of doublewords.
    memory = new int64_t[num_dwords];
    accessed = new bool[num_dwords];

    // Zero the memory space.
    memset(memory,   0, num_dwords * sizeof(int64_t));
    memset(accessed, 0, num_dwords * sizeof(bool));

    // Load initial memory state.
    load_memory_state(choice);
}

data_memory_t::~data_memory_t() {
    // Deallocate the memory space.
    delete [] memory;
    delete [] accessed;
}

// Connect to the upper-level cache.
void data_memory_t::connect(data_cache_t *m_cache) { cache = m_cache; }

// Run the data memory.
void data_memory_t::run() {
    if(req_block && (*ticks >= resp_ticks)) {
        // Invoke the upper-level cache to handle a returned response.
        //cache->handle_response(req_block);
        // Clear the requested block.
        req_block = 0;
    }
}

// Load a memory block.
void data_memory_t::load_block(uint64_t m_addr, uint64_t m_block_size) {
    // Check the doubleword alignment of memory address.
    if(m_addr & 0b111) {
        cerr << "Error4: invalid alignment of memory address " << m_addr << endl;
        exit(1);
    }
    // Check if the requested block size is within memory space.
    if((m_addr+m_block_size) > memory_size) {
        cerr << "Error: memory address " << m_addr << " is out of bounds" << endl;
        exit(1);
    }
    else if(m_addr < code_segment_size) {
        cerr << "Error: memory address " << m_addr << " is in the code segment" << endl;
        exit(1);
    }

    // Mark all doublewords in the requested block are accessed.
    for(uint64_t i = 0; i < m_block_size>>2; i++) { accessed[(m_addr>>2)+i] = true; }
    // Set pointer to a requested block.
    req_block = &memory[m_addr>>2];
    // Set time ticks to respond to the cache later.
    resp_ticks = *ticks + latency;
}
//read, write

// Read data from cache.
void data_memory_t::read(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    uint64_t data_size = 0, data_mask = -1, addr_mask = 0;
    bool sign_ext = 1;
    switch(m_inst->op) {
        case op_fld: { sign_ext = 0; }
        case op_ld:  { data_size = 8; data_mask = -1;         addr_mask = 0b111; break; }
        case op_lwu: { sign_ext = 0; }
        case op_lw:  { data_size = 4; data_mask = 0xffffffff; addr_mask = 0b11;  break; }
        case op_lhu: { sign_ext = 0; }
        case op_lh:  { data_size = 2; data_mask = 0xffff;     addr_mask = 0b1;   break; } 
        case op_lbu: { sign_ext = 0; }
        case op_lb:  { data_size = 1; data_mask = 0xff;       addr_mask = 0;     break; }
        default:     { break; }
    }
    if(addr & addr_mask) {
        cerr << "Error1: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

        //memory->load_block(addr & ~block_mask, block_size);
        int32_t* data = &m_inst->rd_val;
        *data=0;
        //cout<<"addr"<<addr<<" mask: "<<addr_mask<<endl;
    //cout<<memory[addr>>3]<<"addr"<<addr<<"data:"<<*data<<endl;
        memcpy(data,&memory[addr>>2],data_size);
        accessed[addr>>2]=true;
        *data |= (sign_ext && (*data >> ((data_size << 3) -1))) ? ~data_mask : 0;
    //cout<<"read:"<<memory[addr>>3]<<"addr"<<addr<<"data:"<<*data<<endl;
    //cout<<"read2:"<<memory[612]<<"and "<<memory[8532]<<endl;

}

// Write data in memory.
void data_memory_t::write(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    uint64_t data_size = 0, addr_mask = 0;
    switch(m_inst->op) {
        case op_fsd:
        case op_sd:  { data_size = 8; addr_mask = 0b111; break; }
        case op_sw:  { data_size = 4; addr_mask = 0b11;  break; }
        case op_sh:  { data_size = 2; addr_mask = 0b1;   break; } 
        case op_sb:  { data_size = 1; addr_mask = 0;     break; }
        default:     { break; }
    }
    // Check if the address is aligned with the data size.
    // Misaligned accesses are prohibited.
    if(addr & addr_mask) {
        cerr << "Error2: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

        //memory->load_block(addr & ~block_mask, block_size);
        int32_t* data = &m_inst->rd_val;
        *data=0;
        //memcpy(data,&memory[addr],data_size);
        memcpy(&memory[addr>>2],&m_inst->rs2_val,data_size);
        //cout<<"write:"<<memory[addr]<<addr<<endl;
        accessed[addr>>2]=true;
}

// Load initial memory state.
void data_memory_t::load_memory_state(uint32_t choice) {
    // Open a memory state file.
    fstream file_stream;
    if(choice==0)
    {
    file_stream.open("memory_state", fstream::in);
    }
    else if(choice==1)
    {
    file_stream.open("flash_state", fstream::in);
    }
    if(!file_stream.is_open()) {
        cerr << "Error: failed to open memory_state" << endl;
        exit(1);
    }

    string line;
    size_t line_num = 0;
    while(getline(file_stream, line)) {
        line_num++;
        // Crop everything after a comment symbol.
        if(line.find_first_of("#") != string::npos) { line.erase(line.find_first_of("#")); }
        // Erase all spaces.
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        // Skip blank lines.
        if(!line.size()) { continue; }
        // Store memory state.
        size_t l = line.find_first_of("=");
        string addr_str = line.substr(0, l);
        // Trim the line.
        line.erase(0, l+1);
        string data_str = line;

        // Check if the memory address and data are valid.
        if(!is_num_str(addr_str) || (!is_num_str(data_str) && !is_fp_str(data_str)) ||
           !addr_str.length()    || !data_str.length()) {
            cerr << "Error: invalid memory address and/or data " << addr_str
                 << " = " << data_str << " at line #" << line_num
                 << " of memory_state" << endl;
            exit(1);
        }

        // Convert the memory address and data string to numbers.
        uint64_t memory_addr = get_imm(addr_str); 
        int64_t memory_data = is_fp_str(data_str) ?
                              read_int(get_fp(data_str)) : get_imm(data_str);
        // Check the alignment of memory address.
        if(memory_addr & 0b11) {
            cerr << "Error3: invalid alignment of memory address " << memory_addr
                 << " at line #" << line_num << " of memory_state" << endl;
            exit(1);
        }
        // The memory address goes out of bounds.

        //temp_mem
            uint32_t temp_mem = memory_addr;
            temp_mem = (temp_mem>>28)&0xf;
            
                //cout<<"addr:"<<temp_mem<<endl;
                //temp_mem==0 -> real value(x) -> delete
                if(temp_mem==8) { 
                        memory_addr-=(unsigned)(8<<28);
                }
                else if(temp_mem==2) {
                        memory_addr-=(unsigned)(2<<28);
                }
                else
                {
                    cout<<"exception222: "<<std::hex<<memory_addr<<endl;
                }
        
                if((memory_addr+8) > memory_size) {
            cerr << "Error: memory address " << memory_addr << " is out of bounds"
                 << " at line #" << line_num << " of memory_state" << endl;
            exit(1);
        }
            
        //cout << "load_memory_state:"<<(memory_addr>>2)<<endl;
        int64_t &dword = memory[memory_addr>>2];
        if(dword && (dword != memory_data)) {
            cerr << "Error: memory address " << memory_addr
                 << " has multiple values defined at line # " << line_num
                 << " of memory_state" << endl;
            exit(1);
        }
        // Store the memory data.
        dword = memory_data;
        //memory state read -> done
    //cout<<memory[memory_addr>>3]<<"addr"<<memory_addr<<"data:"<<memory_data<<endl;
    }

    // Close the memory state file.
    file_stream.close();
}

// Print memory state.
// data: 0, flash: 1
void data_memory_t::print_state(uint32_t choice) const {
    if(choice==0) cout << "Data ";
    if(choice==1) cout << "Flash ";
    cout << "Memory state (only accessed addresses):" << endl;
    for(uint64_t i = 0; i < num_dwords; i++) {
        if(accessed[i]) {
            if(choice==0)
            {
            cout << "(" <<std::hex<< (i<<2)+(unsigned)(8<<28) << ") = ";
            }
            else if(choice==1)
            {
            cout << "(" <<std::hex<< (i<<2)+(unsigned)(2<<28) << ") = ";
            }
            cout <<std::dec<< memory[i] << endl; 
            //cout << memory[4112] << endl; 
            //if(i==145) cout <<"wow"<<read_fp(memory[i]) << endl; 
            // If exponent field bits are non-zero, it is assumed to be fp.
            //if(memory[i] & (uint64_t(0x7ff) << 52)) { cout << read_fp(memory[i]) << endl; }
            //else { cout << memory[i] << endl; }
        }
    }
}

