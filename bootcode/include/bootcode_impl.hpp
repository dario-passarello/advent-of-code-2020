#include "bootcode.hpp"
namespace bootcode {
    template <typename args>
    bool processor<args>::run(bool debug) {
        reset();
        state = status::RUNNING;
        if(debug) {
            while (!stop_condition()) {
                if(breakpoints.count(pc)== 1 && breakpoints.at(pc)(*this)) {
                    return false;
                }
                arch[instr_mem[pc].opc](*this, instr_mem[pc].args);
                pc += 1;
            }

        } else {
            while (!stop_condition()) {
                arch[instr_mem[pc].opc](*this, instr_mem[pc].args);
                pc += 1;
            }
        }
        state = status::FINISH;
        return true;
    }

    template <typename args>
    bool processor<args>::step() {
        if(state == status::STOPPED) {
            reset();
            state = status::RUNNING;
        }
        if(state != status::FINISH) {
            arch[instr_mem[pc].opc](*this, instr_mem[pc].arg);
            pc += 1;
        }
        if(stop_condition()) {
            state = status::FINISH;
            return true;
        }
        return false;
    }

    template <typename args>
    void processor<args>::reset() {
        pc = 0;
        acc = 0;
        state = status::STOPPED;
    }

    int plus1(int a){
        return a+1;
    }
}