#include "workflow-engine-state.h"
#include "workflow-engine-runs.h"

#include <cstring>

void workflow_engine_state_reset(workflow_engine_state_t *state)
{
    if (!state)
        return;
    bool running = state->running;
    unsigned long generation = state->generation;
    workflow_engine_run_t *owner_run = state->owner_run;
    memset(state, 0, sizeof(*state));
    state->running = running;
    state->generation = generation;
    state->owner_run = owner_run;
    for (size_t i = 0; i < WORKFLOW_MAX_NODES; ++i)
        workflow_engine_node_runtime_reset(&state->node_runtime[i]);
}

void workflow_engine_state_begin(workflow_engine_state_t *state, workflow_t *workflow)
{
    if (!state)
        return;
    unsigned long generation = state->generation + 1;
    workflow_engine_state_reset(state);
    state->workflow = workflow;
    state->running = workflow != nullptr;
    state->generation = generation;
}

void workflow_engine_state_stop(workflow_engine_state_t *state)
{
    if (!state)
        return;
    if (!state->stopping)
        workflow_engine_run_filter_instances_cleanup(state->owner_run);
    state->stopping = true;
    state->running = false;
    state->waiting_for_shortcut = false;
    state->shortcut_source_id[0] = '\0';
    state->generation++;
}

bool workflow_engine_state_is_active(const workflow_engine_state_t *state)
{
    return state && state->running && !state->stopping;
}

void workflow_engine_state_delay_begin(workflow_engine_state_t *state)
{
    if (!state)
        return;
    if (state->owner_run)
        workflow_engine_run_retain(state->owner_run);
    ++state->pending_branches;
}

void workflow_engine_state_delay_end(workflow_engine_state_t *state)
{
    if (!state || !state->pending_branches)
        return;
    --state->pending_branches;
    workflow_engine_run_t *owner_run = state->owner_run;
    if (!state->pending_branches)
        workflow_engine_state_stop(state);
    if (owner_run)
        workflow_engine_run_release(owner_run);
}

workflow_engine_node_runtime_t *workflow_engine_state_node_runtime(
    workflow_engine_state_t *state, const char *node_id)
{
    if (!state || !node_id || !*node_id)
        return nullptr;
    for (size_t i = 0; i < WORKFLOW_MAX_NODES; ++i)
        if (!strcmp(state->node_runtime[i].node_id, node_id))
            return &state->node_runtime[i];
    for (size_t i = 0; i < WORKFLOW_MAX_NODES; ++i)
        if (!state->node_runtime[i].node_id[0]) {
            strncpy(state->node_runtime[i].node_id, node_id,
                    sizeof(state->node_runtime[i].node_id) - 1);
            return &state->node_runtime[i];
        }
    return nullptr;
}

const workflow_engine_node_runtime_t *workflow_engine_state_node_runtime_const(
    const workflow_engine_state_t *state, const char *node_id)
{
    if (!state || !node_id || !*node_id)
        return nullptr;
    for (size_t i = 0; i < WORKFLOW_MAX_NODES; ++i)
        if (!strcmp(state->node_runtime[i].node_id, node_id))
            return &state->node_runtime[i];
    return nullptr;
}
