#include <stdio.h>

int at(int a);
void vstop(int *cpu_state, int code);
void run_cpu(int *memory, int *cpu_state);

int main(void)
{
    int memory[256];
    int cpu_state[8];
    int i;
    int v;
    for (i = 0; i < 256; i++) {
        memory[i] = 0;
    }

    i = 0;
    while (i < 256 && scanf("%d", &v) == 1) {
        memory[i] = v;
        i++;
    }
    

    cpu_state[0] = 3;
    cpu_state[1] = 0; // acc
    cpu_state[2] = 0; // ir
    cpu_state[3] = memory[0];// вектор прерываний
    cpu_state[4] = 0; // флаг остановки
    cpu_state[5] = 0;// код ошибки
    cpu_state[6] = 0;
    cpu_state[7] = 0;

    run_cpu(memory, cpu_state);

    if (cpu_state[5] != 0) {
        printf("Программа завершена с ошибкой\n");
    } else {
        printf("Программа завершена\n");
    }
    return 0;
}

int at(int a)
{
    return a >= 0 && a <= 255;
}

void vstop(int *cpu_state, int code)
{
    if (cpu_state[5] != 0){
        cpu_state[4] = 1;
        return;}
    cpu_state[5] = code;
    cpu_state[0] = cpu_state[3];
    
}

void run_cpu(int *memory, int *cpu_state)
{
    int pc;
    int op;

    while (cpu_state[4] == 0) {
        pc = cpu_state[0];

        if (!at(pc)) {
            cpu_state[2] = -1;
            vstop(cpu_state, 3);
            continue;
        }
        cpu_state[2] = memory[pc];

        if (cpu_state[2] == 0) {
            printf("pc=%d: команда 0 (стоп), acc=%d\n", pc, cpu_state[1]);
            cpu_state[4] = 1;
            break;
        }

        if (cpu_state[2] < 0 || cpu_state[2] > 5) {
            vstop(cpu_state, 2);
            continue;
        }

        if (!at(pc + 1)) {
            vstop(cpu_state, 3);
            continue;
        }
        op = memory[pc + 1];
        if (!at(op)) {
            vstop(cpu_state, 3);
            continue;
        }
        if (cpu_state[2] == 1) {// LOAD
            cpu_state[1] = memory[op];
        } else if (cpu_state[2] == 2) {// MUL
            cpu_state[1] = cpu_state[1] * memory[op];
        } else if(cpu_state[2] == 3) {// DIV
            if (memory[op] == 0) {
                vstop(cpu_state, 1);
                continue;
            }
            cpu_state[1] = cpu_state[1] / memory[op];
        } else if (cpu_state[2] == 4){ // jump
            int aim = memory[pc + 1];
            if (!at(aim)){
                vstop(cpu_state,3);
                continue;
            }
            cpu_state[0] = aim;
            printf("pc = %d: JUMP, aim = %d\n", pc, aim);
            continue;
        }

        cpu_state[0] = pc + 2;
        printf("pc=%d: команда %d, op=%d, acc=%d\n", pc, cpu_state[2], op, cpu_state[1]);
    }
}