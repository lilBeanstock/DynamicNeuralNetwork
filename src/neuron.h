#include <stdlib.h>
#include <ansiColours.h>
#define true (unsigned char)1
#define false (unsigned char)0

char colourRanges[][10] = {
    HRED,HYEL,GRN,CYN,HBLU
};
int colours = sizeof(colourRanges)/sizeof(colourRanges[0]);

// gets the absolute value of a double
double dabs(double x) { // just for less bloat
    return (x < 0) ? -x : x;
}

enum Type {
    Input=0, Middle, Output
};

typedef struct Neuron {
    enum Type type;
    unsigned char evaluated;
    double bias;
    double totalsum;
    double value;
} Neuron;

typedef struct Connection {
    Neuron* fromNeu;
    Neuron* toNeu;
    double weight;
} Connection;

typedef struct Bundle {
    Connection** c;
    size_t cCou;
} Bundle;

typedef struct Network {
    Neuron* n;
    Connection* c;
    size_t n_size;
    size_t c_size;
} Network;

// apply sigmoid function to A.totalsum into A.value and update A.evaluated to 1 (true);
void evaluateNeuron(Neuron* A) {
    // faster sigmoid function (without e^-x).
    // at |v| > 4, it starts to approach higher than 1 and lower than -1. if v is higher than 4 or lower than -4, 
    // use other equation for more accuracy.
    double v = A->totalsum + A->bias;
    A->value = (v > -4 || v < 4) ?
        (3 * v) / (5 + 2 * dabs(v)) : // x = 8 -> y = 1.1429 too high
        (3 * v) / (1 + 3 * dabs(v)); // x = 8 -> y = 0.96 correct
    A->evaluated = true;
}

void doConnection(Connection* con) {
    Neuron* A = con->fromNeu;
    Neuron* B = con->toNeu;

    if (!A || !B) {
        printf("ERR: neurons for connection do not exist!\n");
        exit(255);
    }

    if (!A->evaluated && A->type != Input) {
        evaluateNeuron(A); // do sigmoid
    }

    B->totalsum += con->weight * A->value; // add to sum
    B->evaluated = false; // make sure it is not considered evaluated with updated total, enables cyclic connections.
} 

void doAll(Network* net) {
    for (size_t i = 0; i<net->c_size; i++) {
        doConnection(&net->c[i]);
    }
    for (size_t i = 0; i<net->n_size; i++) {
        if (net->n[i].type == Output) evaluateNeuron(&net->n[i]);
    }
}

// ==== MISC ====

void printNeuron(Neuron* A) {
    // get appropriate index, dynamic
    int i = (A->value+1)/2.0 * (colours-1);
    if (i < 0) i = 0;
    if (i > colours-1) i = colours-1;

    // You cannot switch on a float/double; use if-else or remove this block.
    // Example: print the value directly.
    printf("%s %.3f (%.3f) %s", colourRanges[i],A->value,A->bias,CRESET);
}

void printlnNeuron(Neuron* A) {
    // get appropriate index, dynamic
    int i = (A->value+1)/2.0 * (colours-1);
    if (i < 0) i = 0;
    if (i > colours-1) i = colours-1;

    // You cannot switch on a float/double; use if-else or remove this block.
    // Example: print the value directly.
    printf("%s %.3f (%.3f) %s\n", colourRanges[i],A->value,A->bias,CRESET);
}

void printConnection(Connection* con) {
    printf("- %.3f -",con->weight);
}

void printManyCon(Bundle v) {
    printf("- (");
    for (size_t i = 0; i<v.cCou; i++) {
        printf("%c%.3f",(i!=0) ? ',' : ' ',v.c[i]->weight);
    }
    printf(" ) -");
}