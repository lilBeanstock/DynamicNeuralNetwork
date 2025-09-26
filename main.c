#include <stdio.h>
#include <neuron.h>

/*
THEORY:
    each neuron is independent of other neurons, but connected using a dynamically
    structured topological network. There exists no layers, but they may be
    allocated as inputs or outputs. Each neuron is stored in a list of neurons. 
    Each connection is stored in a list of connections. A neuron is generally only 
    a value with checks, considering that it does nothing but store a value for 
    later use -- while connections are used as "tasks" which are done in order of hierarchy.

    The first problem that occurs is that having to find all the appropriate
    connections for every single neuron is tedious. Instead, neurons are fitted with
    a "total sum" variable which is the sum of all products of values and weights
    before the sigmoid function is used on them. This will not be reset until the end
    of the neural network. This makes sure that even cyclical connections will work,
    considering that the sigmoid function can be run again to re-evaluate the neuron.
*/

int main() {
    static Neuron n[] = {
        { Input, false, 0.0, 0.0, 0.0 },
        { Input, false, 0.0, 0.0, 0.0 },
        { Middle, false, 0.0, 0.0, 0.0 },
        { Output, false, 0.0, 0.0, 0.0 },
    };
    static Connection c[] = {
        {&n[0],&n[2],1.2},
        {&n[1],&n[2],-3},
        {&n[2],&n[3],0.8}
    };
    static Network net = (Network){
        n,c,
        sizeof(n)/sizeof(n[0]),
        sizeof(c)/sizeof(c[0])
    };

    net.n[0].value = 0.4;
    net.n[1].value = 1.3;
    Bundle cbun = {
        (Connection*[]){ &net.c[0], &net.c[1] }, 2
    };
    
    printNeuron(&net.n[0]);
    printManyCon(
        cbun
    );
    printNeuron(&net.n[2]);
    printConnection(&net.c[2]);
    printlnNeuron(&net.n[3]);
    printlnNeuron(&net.n[1]);

    doAll(&net);

    printNeuron(&net.n[0]);
    printManyCon(
        cbun
    );
    printNeuron(&net.n[2]);
    printConnection(&net.c[2]);
    printlnNeuron(&net.n[3]);
    printlnNeuron(&net.n[1]);

    return 0;
}