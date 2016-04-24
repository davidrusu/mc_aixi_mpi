#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "environment/environment.r"
#include "environment/environment.h"
#include "environment/class.r"
#include "environment/class.h"
#include "environment/coin_flip.r"
#include "environment/coin_flip.h"
#include "agent/agent.h"
#include "_utils/macros.h"

#ifdef USE_MPI
    #include "mpi.h"
#endif

#define CTW_DATA_FILE "/scratch/drusu/ctw.dat"
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
    options->exploration = 0.001f;
    options->explore_decay = 1.0f;
    options->learning_period = 0;
    options->mc_simulations = 300;
    options->profile = false;
    options->terminate_age = 0;
    options->verbose = false;

    return options;
}

float _random_0_1() {
    return (float)rand()/(float)(RAND_MAX/1);
}

#ifdef USE_MPI
void mpi_main(Agent* agent, struct Environment* environment, app_options* options, int argc, const char* argv[]) {
  int rank, P;

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    printf("Error\n");
    return 1;
  }


  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
     printf("%d\n", agent->context_tree);
    printf("desu\n");
      agent_proc(P, agent, environment, options);
  } else {
    search_proc(rank, P, agent, environment, options);
  }

  MPI_Finalize();
}
#endif
void agent_proc(int P, Agent* agent, struct Environment* environment, app_options* options) {
  printf("MC AIXI training warming up...\n");
  srand(1337);

  bool explore =  options->exploration > 0;
  if(0.0f > options->exploration || 0.0f > options->explore_decay || options->explore_decay > 1.0f) {
    printf("Some exploration parameter is invalid. Application force quitting.\n");
    exit(1);
  }

  bool terminate_check = options->terminate_age > 0;
  bool isEnvironmentFinished = false;
  int cycle = 1;

  while(!isEnvironmentFinished) {
    int agent_age = agent->age;

    if(terminate_check && agent_age > options->terminate_age) {
      TRACE("Interaction looked broken; terminate age exceeded.\n", "main");
      break;
    }

    long cycle_start = time(NULL);

    // TODO: Implement lines 120 and 121 (properly, compile?
    u32 observation = environment->_observation;
    u32 reward = environment->_reward;

    if(options->learning_period > 0 && cycle > options->learning_period) {
      explore = false;
    }

    Agent_model_update_percept(agent, observation, reward);

    bool explored =  false;

    u32 best_action = 0;
    double best_mean = -1;
    u32 action;
    if (explore && _random_0_1() < options->exploration) {
      explored =  true;
      printf("Agent is trying action at random...\n");
      action = Agent_generate_random_action(agent);
    } else {
      ctw_save(agent->context_tree, CTW_DATA_FILE);
      int i;
      for (i = 1; i < P; i++) {
	int data = 1;
#ifdef USE_MPI
	MPI_Send(&data, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
#endif
	double mean;
#ifdef USE_MPI
	MPI_Recv(&action, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(&mean, 1, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
	if (mean > best_mean) {
	  best_action = action;
	  best_mean = mean;
	}
      }
    }

    // TODO: Line 153 to 156 - Perform agent action and update w/ action
    perform_action(environment, action);

    Agent_model_update_action(agent, action);
	
    long ticks_taken = time(NULL) - cycle_start;

    if (cycle % 1 == 0) {
      printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Cycle", "Observe.", "Reward", "Action", "Explored", "Exp. Rate", "Tot. Reward", "Avg Reward", "Time", "Model Size");
    }   // Just a large padded statement about what is going on in the world as we step through
        //printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "Cycle", "Observation", "Reward", "Action", "Explored", "Explore Rate", "Total Reward", "Average Reward", "Time", "Model Size");
    printf("%-12d%-12u%-12u%-12u%-12d%-12f%-12f%-12f%-12lu%-12u\n", cycle, observation, reward, action, explored, options->exploration, agent->total_reward, Agent_average_reward(agent), ticks_taken, ctw_size(agent->context_tree));

    if(explore) {
      options->exploration *= options->explore_decay;
    }

    cycle++;

    // TODO: Update the environment finish state
    // See beginning of this loop in PyAIXI.. need to call some kind of environment_is_finished(environment)
    // or something like that
    isEnvironmentFinished = environment->_is_finished;
  }
}

void search_proc(int rank, int P, Agent* agent, struct Environment* environment, app_options* options) {
  for (;;) {
    int data;
#ifdef USE_MPI
    MPI_Recv(&data, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
    ctw_load(agent->context_tree, CTW_DATA_FILE);
    double mean;
    int action = Agent_search_mean(agent, &mean);
#ifdef USE_MPI
    MPI_Send(&action, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&mean, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
#endif
  }
}

void _interaction_loop(Agent* agent, struct Environment* environment, app_options* options) {
    printf("MC AIXI training warming up...\n");
    srand(1337);

    bool explore =  options->exploration > 0;
    if(0.0f > options->exploration || 0.0f > options->explore_decay || options->explore_decay > 1.0f) {
        printf("Some exploration parameter is invalid. Application force quitting.\n");
        exit(1);
    }

    bool terminate_check = options->terminate_age > 0;
    bool isEnvironmentFinished = false;
    int cycle = 1;


    while(!isEnvironmentFinished) {
        int agent_age = agent->age;

        if(terminate_check && agent_age > options->terminate_age) {
            TRACE("Interaction looked broken; terminate age exceeded.\n", "main");
            break;
        }

        long cycle_start = time(NULL);

        // TODO: Implement lines 120 and 121 (properly, compile?
        u32 observation = environment->_observation;
        u32 reward = environment->_reward;

        if(options->learning_period > 0 && cycle > options->learning_period) {
            explore = false;
        }

        Agent_model_update_percept(agent, observation, reward);

        bool explored =  false;

    	u32 action = 0;

        if (explore && _random_0_1() < options->exploration) {
            explored =  true;
            printf("Agent is trying action at random...\n");
            action = Agent_generate_random_action(agent);
        } else {
	    action = Agent_search(agent);
    	}

        // TODO: Line 153 to 156 - Perform agent action and update w/ action
        perform_action(environment, action);

        Agent_model_update_action(agent, action);
	
        long ticks_taken = time(NULL) - cycle_start;

	if (cycle % 1 == 0) {
	  printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Cycle", "Observe.", "Reward", "Action", "Explored", "Exp. Rate", "Tot. Reward", "Avg Reward", "Time", "Model Size");
    }   // Just a large padded statement about what is going on in the world as we step through
        //printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "Cycle", "Observation", "Reward", "Action", "Explored", "Explore Rate", "Total Reward", "Average Reward", "Time", "Model Size");
        printf("%-12d%-12u%-12u%-12u%-12d%-12f%-12f%-12f%-12lu%-12u\n", cycle, observation, reward, action, explored, options->exploration, agent->total_reward, Agent_average_reward(agent), ticks_taken, ctw_size(agent->context_tree));

        if(explore) {
            options->exploration *= options->explore_decay;
        }

        cycle++;

        // TODO: Update the environment finish state
        // See beginning of this loop in PyAIXI.. need to call some kind of environment_is_finished(environment)
        // or something like that
        isEnvironmentFinished = environment->_is_finished;
    }

}

int main(int argc, const char* argv[]) {
    // TODO: Later, maybe support command line arguments...
    app_options* appOptions = _make_default_options();

    printf("Booting MC AIXI kernel...\n");

    TRACE("Creating environment...\n", "desu");

    // TODO: Create an environment...
    struct Coin_Flip* environment = new (Coin_Flip, 0.6f);

    TRACE("Creating agent... please be patient\n", "desu");

    // TODO: Create the agent
    TRACE("allocate agent\n", "main");
    Agent* agent = malloc(sizeof(Agent));
    TRACE("init agent\n", "main");
    agent = Agent_init(agent, environment, appOptions->learning_period);

    // TODO: Line #443: Do we need to copy some of the options into here?
    // options->actions_bits = environment->action_bits()... for example

    if(appOptions->profile) {
        printf("Profiling is not currently supported. Ignoring.\n");
    }

    //    _interaction_loop(agent, environment, appOptions);
    mpi_main(agent, environment, appOptions, argc, argv);
}
