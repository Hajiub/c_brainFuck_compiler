#define ALIB_IMPLEMENTATION
#include "alib.h" 

typedef enum {
    OP_INC             = '+',
    OP_DEC             = '-',
    OP_LEFT            = '<',
    OP_RIGHT           = '>',
    OP_OUTPUT          = '.',
    OP_INPUT           = ',',
    OP_JUMP_IF_ZERO    = '[',
    OP_JUMP_IF_NONZERO = ']',
} Op_Type;


typedef struct {
    Op_Type type;
    size_t count;
} Op;

typedef struct {
    Op *data;
    size_t currentSize;
    size_t capacity;    
} Ops;

void append(Ops *ops, Op op);
void freeOps(Ops *ops);

int main(int ac, char **av)
{
    char *prog = *av;
    if (ac <= 0) {
        logMessage(ERROR,"Usage %s <file_path>", prog);
        return -1;
    }

    StringBuffer sb = {0};
    if(!readEntireFile(av[1], &sb))
        return 1;
    Ops ops = {0};

    for (size_t i = 0; i < sb.count; i++) {
        switch(sb.data[i]) {
            case OP_INC: {
                Op op = {
                    .type = OP_INC;
                    .operand = 1;
                }
                append(&ops, op);
            } break;

            case OP_DEC: {
                Op op = {
                    .type =  OP_DEC;
                    .operand = 1;
                }
                append(&ops, op);
            } break;

            case OP_LEFT: {
                Op op = {
                    .type = OP_LEFT;
                    .operand = 1;
                }
                append(&ops, op);
            } break;

            case OP_RIGHT: {
                Op op = {
                    .type = OP_RIGHT;
                    .operand = 1;
                }
                append(&ops, op);
 
            } break;
            default:
                break;
        }
    }
    free(sb.data);
    return 0;
}

void append(Ops *ops, Op op)
{
   if (ops->currentSize >= ops->capacity) {
        // I only have 8 GB of RAM
        ops->capacity += 8;
        // Rich ppl ops->capacity = (ops->capacity == 0) ? 1 : ops->capacity * 2;
        ops->data = (Op *)realloc(ops->data, ops->capacity * sizeof(Op));
        if (ops->data == NULL) {
            logMessage(ERROR,"Your computer is too old for this!");
            logMessage(INFO,"change ops->capacity += 8 to ops->capacity += 1 && good luck!");
            exit(1);
        }
   }
   ops->data[ops->currentSize++] = op; 
}

void freeOps(Ops *ops)
{
    ops->currentSize = ops->capacity = 0;
    free(ops->data);
}


