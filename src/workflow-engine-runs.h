#pragma once

#include "workflow-engine-state.h"
#include "workflow-model.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct workflow_engine_runs workflow_engine_runs_t;
typedef struct workflow_filter_instance_set workflow_filter_instance_set;

workflow_engine_runs_t *workflow_engine_runs_create(void);
void workflow_engine_runs_destroy(workflow_engine_runs_t *runs);
workflow_engine_run_t *workflow_engine_runs_start(workflow_engine_runs_t *runs, workflow_t *workflow);
workflow_engine_state_t *workflow_engine_run_state(workflow_engine_run_t *run);
const workflow_engine_state_t *workflow_engine_run_state_const(const workflow_engine_run_t *run);
workflow_engine_run_t *workflow_engine_run_next(const workflow_engine_run_t *run);
workflow_engine_run_t *workflow_engine_runs_head(workflow_engine_runs_t *runs);
workflow_engine_run_t *workflow_engine_runs_current(workflow_engine_runs_t *runs);
workflow_engine_run_t *workflow_engine_runs_find_shortcut(workflow_engine_runs_t *runs,
                                                           const char *workflow_id,
                                                           const char *source_id);
workflow_filter_instance_set *workflow_engine_run_filter_instances(workflow_engine_run_t *run);
void workflow_engine_run_filter_instances_cleanup(workflow_engine_run_t *run);
void workflow_engine_runs_stop_all(workflow_engine_runs_t *runs);
bool workflow_engine_runs_any_active(const workflow_engine_runs_t *runs);
void workflow_engine_run_retain(workflow_engine_run_t *run);
void workflow_engine_run_release(workflow_engine_run_t *run);

#ifdef __cplusplus
}
#endif
