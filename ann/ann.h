#ifndef __ANN_H__
#define __ANN_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Neuron
 */
typedef struct {

    double *weights;

    uint32_t ninputs;

} ann_neuron_t;

/**
 * Neuron Group
 */
typedef struct {

    ann_neuron_t *neurons;

    uint32_t size;

} ann_neuron_group_t;

/**
 * Neural Network
 */
typedef struct {

    uint32_t noutputs;

    uint32_t ngroups;

    ann_neuron_group_t *groups;

} ann_neural_network_t;

/**
 * Chromosome
 */
typedef struct {

    double *weights;

    uint32_t nweights;

    double fitness;

} ann_chromosome_t;

/**
 *
 */
extern ann_neuron_t* ann_neuron_new(uint32_t ninputs);

/**
 *
 */
extern ann_neural_network_t* ann_neural_network_new();

#ifdef __cplusplus
}
#endif

#endif /* __ANN_H__ */
