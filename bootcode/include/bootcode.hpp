#ifndef BOOTCODE_HPP
#define BOOTCODE_HPP

#include<vector>
#include<memory>
#include<string>
#include<map>
#include<functional>
namespace bootcode {
    typedef unsigned long long int address;
    typedef long long int value;
    typedef std::string opcode;

    template<typename args>
    class instruction {
    public:
        opcode opc;
        args arg;
    };

    template<typename args>
    class processor {
    public:
        enum class status{
            STOPPED,
            RUNNING,
            FINISH,
            ERROR,
        };
    private: 
        std::map<opcode, std::function<void (processor&, args arg)>> arch;
        address pc;
        value acc;
        status state;
        std::vector<instruction<args>> instr_mem;
        std::map<address, std::function< bool(processor<args>& )>> breakpoints;
    public:    
        processor(const std::map<opcode, std::function<void (processor&, args arg)>>& arch,
                const std::vector<instruction<args>>& instructions) :
            arch(arch), 
            pc(0), 
            acc(0), 
            state(status::STOPPED), 
            instr_mem(instructions) {};

    public:
        //RUN
        bool step();
        bool run(bool=false);
        void reset();


        //REGISTER MANIPULATION
        inline address get_pc() const {return pc;};
        inline value get_acc() const {return acc;};
        inline status get_state() const {return state;};
        inline const instruction<args>& get_instr(const address addr) const {
            if(0 <= addr && addr < instr_mem.size()) {
                return instr_mem[addr];
            } else throw -1;
        };
        inline const instruction<args>& get_curr_inst(const address addr) const {
            if(0 <= addr && addr < instr_mem.size()) {
                return instr_mem[pc];
            } else throw -1;
        }
        inline void set_pc(address new_pc) {pc = new_pc;};
        inline void set_acc(value new_acc) {acc = new_acc;};
        inline void inc_pc(address offset=1) {pc += offset;};
        inline void inc_acc(value increment=1) {acc += increment;};
        //PROGRAM MEMORY OPERATIONS
        inline void flash(const std::vector<instruction<args>>& instructions) {instr_mem = instructions;};
        inline void write_instr_mem(address addr, const instruction<args>& instr) { instr_mem[addr] = instr;}; 
        inline instruction<args> read_flash_mem(address addr) const { return instr_mem[addr];};
        //DEBUGGING
        inline void set_breakpoint(
            address addr, 
            const std::function<bool(const processor<args>&)> cond = [](const processor<args>&)->bool{return true;}) 
        {
            breakpoints.emplace(addr, cond);
        }
        void remove_breakpoint(address addr) {
            breakpoints.erase(addr);
        }


    private:
        inline bool stop_condition() {
            return pc >= instr_mem.size(); 
        }
    };


    int plus1(int);
}


#endif