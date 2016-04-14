#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "summary.c"

/*
 * MC AIXI - C port of PyAIXI w/ MPI
 * This port contains an additional implementation of MC AIXI on top of the PyAIXI platform, which was only
 * single-threaded and makes it multi-threaded. Additionally, it is now written to be faster in C.
 */

typedef struct app_options {
    int agent;
    int agent_horizon;
    int ct_depth;
    int environment;
    float exploration;
    float explore_decay;
    int learning_period;
    int mc_simulations;
    bool profile;
    int terminate_age;
    bool verbose;
} app_options;

app_options* _make_default_options() {
    app_options* options = malloc(sizeof(app_options));
    options->agent = 0;
    options->agent_horizon = 5;
    options->ct_depth = 30;
    options->environment = 0;
    options->exploration = 0.0f;
    options->explore_decay = 1.0f;
    options->learning_period = 0;
    options->mc_simulations = 300;
    options->profile = false;
    options->terminate_age = 0;
    options->verbose = false;

    return options;
}

Environment* _get_environment() {
    // TODO: Do we need to set some options here for the environment?
    return (Environment*) malloc(sizeof(Environment));
}

float _random_0_1() {
    return (float ) rand() / (float) RAND_MAX;
}
void _interaction_loop(Agent* agent, Environment* environment, app_options* options) {
    printf("MC AIXI training warming up...");
    srand(1337);

    bool explore =  options->exploration > 0;
    if(0f > options->exploration || 0f > options->explore_decay || options->explore_decay > 1f) {
        printf("Some exploration parameter is invalid. Application force quitting.\n");
        exit(1);
    }

    bool terminate_check = options->terminate_age > 0;
    bool isEnvironmentFinished = false;
    int cycle = 1;

    printf("%-20s%-20s%-20s%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n", "Cycle", "Observation", "Reward", "Action", "Explored", "Explore Rate", "Total Reward", "Average Reward", "Time", "Model Size");

    while(!isEnvironmentFinished) {
        // TODO: Implement an API to get the agent age (Line #112)
        int agent_age = 0;

        if(terminate_check && agent_age > options->terminate_age) {
            break;
        }

        long cycle_start = time(NULL);

        // TODO: Implement lines 120 and 121
        int observation = 0;
        int reward = 0;

        if(options->learning_period > 0 && cycle > options->learning_period) {
            explore = false;
        }

        // TODO: Implement the agent model perceptron update
        // Implement line #129


        bool explored =  false;
        int action = -1;

        if (explore && _random_0_1() < options->exploration) {
            explored =  true;
            printf("Agent is trying action at random...\n");
            //  TODO: Implement agent random action (line 141)
            // note: in both of these TODOs, we should update the action ID
        }
        else {
            printf("Agent is trying to choose best action...\n");
            // TODO: IMplement agent search (line 149)
        }

        // TODO: Line 153 to 156 - Perform agent action and update w/ action
        // environment.perform_action(action)
        // agent.model_update_action(action)

        long ticks_taken = time(NULL) - cycle_start;

        // Just a large padded statement about what is going on in the world as we step through
        printf("%-20d%-20d%-20d%-20d%-20d%-20f%-20s%-20s%-20lu%-20s\n", cycle, observation, reward, action, explored, options->exploration, "N/A", "N/A", ticks_taken, "N/A");

        if(explore) {
            options->exploration *= options->explore_decay;
        }

        cycle++;

        // TODO: Update the environment finish state
        // See beginning of this loop in PyAIXI.. need to call some kind of environment_is_finished(environment)
        // or something like that
    }

}

int main() {
    // TODO: Later, maybe suppor command line arguments...
    app_options* appOptions = _make_default_options();

    printf("Booting MC AIXI kernel...\n");

    // Load the environment here that we want to get
    Environment* environment = _get_environment();
    Agent* agent = create_agent(environment, appOptions ->ct_depth, appOptions ->mc_simulations);

    // TODO: Line #443: Do we need to copy some of the options into here?
    // options->actions_bits = environment->action_bits()... for example

    if(appOptions->profile) {
        printf("Profiling is not currently supported. Ignoring.\n");
    }

    _interaction_loop(agent, environment, appOptions);
}