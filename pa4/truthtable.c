/* CS 211 PA4
 * Created for: nsn29
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct circuitOp {
    char gateType[17];
    int nAm;
    int sAm;
    int *inputs;
    int *outputs;
    int *selectors;
};

int increaseInput(int *arr, int incrAm) {
    int k;
    for (k=incrAm+1; k>1; k--) {
        arr[k] = !(arr[k]);
        if (arr[k] != 0) {
            return 1;
        }
    }
    return 0;
}

void gateNOT(int inpInd, int *vals, int outInd) {
    vals[outInd] = !(vals[inpInd]);
    return;
}
void gateAND(int inpInd1, int inpInd2, int *vals, int outInd) {
    vals[outInd] = vals[inpInd1] && vals[inpInd2];
    return;
}
void gateOR(int inpInd1, int inpInd2, int *vals, int outInd) {
    vals[outInd] = vals[inpInd1] || vals[inpInd2];
    return;
}
void gateNAND(int inpInd1, int inpInd2, int *vals, int outInd) {
    vals[outInd] = !(vals[inpInd1] && vals[inpInd2]);
    return;
}
void gateNOR(int inpInd1, int inpInd2, int *vals, int outInd) {
    vals[outInd] = !(vals[inpInd1] || vals[inpInd2]);
    return;
}
void gateXOR(int inpInd1, int inpInd2, int *vals, int outInd) {
    vals[outInd] = vals[inpInd1] ^ vals[inpInd2];
    return;
}
void gateDECODER(int n, int *inpInd, int *vals, int *outInd) {
    int s=0;
    int i;
    for (i = 0; i < pow(2,n); i++) {
        vals[outInd[i]] = 0;
    }
    for (i = 0; i < n; i++) {
        s += vals[inpInd[i]] * pow(2, n-i-1);
    }
    vals[outInd[s]] = 1;
    return;
}
void gateMULTIPLEXER(int n, int *inpInd, int *sindex, int *vals, int outInd) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s += vals[sindex[i]] * pow(2, n-i-1);
    }
    vals[outInd] = vals[inpInd[s]];
    return;
}
void gatePASS(int inpInd, int *vals, int outInd) {
    vals[outInd] = vals[inpInd];
    return;
}

int indexOf(int size, char **arr, char *var) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(arr[i], var)==0) {
            return i;
        }
    }
    return -1;
}

void clearVals(int size, int *arr) {
    int i;
    for (i = 0; i<size; i++) {
        arr[i] = 0;
    }
    arr[1] = 1;
    return;
}

int main(int argc, char** argv) {


    FILE *file = fopen(argv[1], "r");

    struct circuitOp* ops = NULL;
    int size = 2, scount=0, incrAm=0, outAm=0;
    char circOp[17];
    char **names;
    int *vals = malloc(sizeof(int)*size);

    fscanf(file, " %s", circOp);
    fscanf(file, "%d", &incrAm);

    size += incrAm;

    names = malloc(size * sizeof(char *));
    names[0] = malloc(2 * sizeof(char));
    names[0] = "0\0";
    names[1] = malloc(2 * sizeof(char));
    names[1] = "1\0";

    int i;
    for (i = 0; i < incrAm; i++) {
        names[i + 2] = malloc(17 * sizeof(char));
        fscanf(file, "%*[: ]%16s", names[i + 2]);
    }

    fscanf(file, " %s", circOp);
    fscanf(file, "%d", &outAm);

    size += outAm;

    names = realloc(names,sizeof(char *)*size);
    for (i = 0; i < outAm; i++) {
        names[i+incrAm+2] = malloc(sizeof(char)*17);
        fscanf(file, "%*[: ]%16s", names[i + incrAm + 2]);
    }

    while (!feof(file)) {
        int numInputs = 2, numOutputs = 1;
        struct circuitOp op;
        int sc = fscanf(file, " %s", circOp); 
        if (sc != 1) {
            break;
        }
        scount++;
        op.nAm = 0;
        op.sAm = 0;
        char vars[17];
        strcpy(op.gateType, circOp);

        if (strcmp(circOp, "NOT")==0) {
            numInputs = 1;
        }
        if (strcmp(circOp, "PASS")==0) {
            numInputs = 1;
        }
        if (strcmp(circOp, "DECODER")==0) {
            fscanf(file, "%d", &numInputs);
            op.nAm = numInputs;
            numOutputs = pow(2, numInputs);
        }
        if (strcmp(circOp, "MULTIPLEXER")==0) {
            fscanf(file, "%d", &numInputs);
            op.sAm = numInputs;
            numInputs = pow(2, numInputs);
        }

        op.inputs = malloc(sizeof(int)*numInputs);
        op.outputs = malloc(sizeof(int)*numOutputs);
        op.selectors = malloc(sizeof(int)*op.sAm);

        
        for (i = 0; i < numInputs; i++) {
            fscanf(file, "%*[: ]%16s", vars);
            
            op.inputs[i] = indexOf(size, names, vars);
        }

        for (i = 0; i < op.sAm; i++) {
            fscanf(file, "%*[: ]%16s", vars);

            op.selectors[i] = indexOf(size, names, vars);
        }
 
        for (i = 0; i < numOutputs; i++) {
            fscanf(file, "%*[: ]%16s", vars);

            int index = indexOf(size, names, vars);
            if (index>=0){
                op.outputs[i] = index;
            }
            else {
                size++;
                names = realloc(names,sizeof(char *)*size);
                names[size - 1] = malloc(sizeof(char)*17);
                strcpy(names[size - 1], vars);
                op.outputs[i] = size - 1;
            }
        }
        
        if (ops==0) {
            ops = malloc(sizeof(struct circuitOp));
        } else {
            ops = realloc(ops, scount * sizeof(struct circuitOp));
        }
        ops[scount-1] = op;
    }

    vals = malloc(sizeof(int)*size);
    clearVals(size, vals);

    while(1) {
  
        for (i=0; i<incrAm; i++) {
            printf("%d ", vals[i + 2]);
        }
        printf("|");

        for (i=0; i<scount; i++) {
            struct circuitOp op = ops[i];
            if (strcmp(op.gateType, "NOT")==0) {
                gateNOT(op.inputs[0], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "AND")==0) {
                gateAND(op.inputs[0], op.inputs[1], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "OR")==0) {
                gateOR(op.inputs[0], op.inputs[1], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "NAND")==0) {
                gateNAND(op.inputs[0], op.inputs[1], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "NOR")==0) {
                gateNOR(op.inputs[0], op.inputs[1], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "XOR")==0) {
                gateXOR(op.inputs[0], op.inputs[1], vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "DECODER")==0) {
                gateDECODER(op.nAm, op.inputs, vals, op.outputs);
            }
            else if (strcmp(op.gateType, "MULTIPLEXER")==0) {
                gateMULTIPLEXER(op.sAm, op.inputs, op.selectors, vals, op.outputs[0]);
            }
            else if (strcmp(op.gateType, "PASS")==0) {
                gatePASS(op.inputs[0], vals, op.outputs[0]);
            }
        }

        for (i=0; i<outAm; i++) {
            printf(" %d", vals[incrAm + i + 2]);
        }
        printf("\n");

        if (increaseInput(vals, incrAm)==0) {
            break;
        }
    }

    return EXIT_SUCCESS;
}