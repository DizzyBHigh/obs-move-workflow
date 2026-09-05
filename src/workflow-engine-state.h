#pragma once

#include "workflow-engine-node-runtime.h"
#include "workflow-model.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct workflow_engine_run workflow_engine_run_t;

typedef struct workflow_engine_state {
    workflow_t *workflow;
    workflow_engine_run_t *owner_run;
    char current_node_id[WORKFLOW_MAX_NAME];
    char shortcut_source_id[WORKFLOW_MAX_NAME];
    workflow_engine_node_runtime_t node_runtime[WORKFLOW_MAX_NODES];
    bool running;
    bool stopping;
    bool waiting_for_shortcut;
    unsigned long generation;
    size_t pending_branches;
} workflow_engine_state_t;

void workflow_engine_state_reset(workflow_engine_state_t *state);
void workflow_engine_state_begin(workflow_engine_state_t *state, workflow_t *workflow);
void workflow_engine_state_stop(workflow_engine_state_t *state);
bool workflow_engine_state_is_active(const workflow_engine_state_t *state);
void workflow_engine_state_delay_begin(workflow_engine_state_t *state);
void workflow_engine_state_delay_end(workflow_engine_state_t *state);

workflow_engine_node_runtime_t *workflow_engine_state_node_runtime(
    workflow_engine_state_t *state, const char *node_id);
const workflow_engine_node_runtime_t *workflow_engine_state_node_runtime_const(
    const workflow_engine_state_t *state, const char *node_id);

#ifdef __cplusplus
}
#endif
