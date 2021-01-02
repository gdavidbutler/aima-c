## aima-c

[C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) implementation of [aima-pseudocode](https://github.com/aimacode/aima-pseudocode) from [Russell And Norvig's "Artificial Intelligence - A Modern Approach, Fourth Edition"](http://aima.cs.berkeley.edu).

[Why C](https://sqlite.org/whyc.html)?
It is the best "get it done" computer programming language.

From the [book's preface](http://aima.cs.berkeley.edu/newchap00.pdf), "The main unifying theme is the idea of an [intelligent agent](https://en.wikipedia.org/wiki/Intelligent_agent)."
C is an imperative procedural language with no direct support for a [software agent](https://en.wikipedia.org/wiki/Software_agent) computing model.
C is a structured programming language with no direct support for data access methods.
And there is no direct support for dynamic memory.
Or I/O.
But in a proper environemnt, with C callable routines, these are not limitations.

Dependencies:
* [libc](https://en.wikipedia.org/wiki/C_standard_library) - of course, restricted to C99 features
* [pthreads](https://en.wikipedia.org/wiki/POSIX_Threads) - for concurrency and parallelism
* [pthreadChannel](https://github.com/gdavidbutler/pthreadChannel) - for an asynchronous [CSP](https://en.wikipedia.org/wiki/Communicating_sequential_processes) model
* [SQLite](https://sqlite.org) - for a declarative data language and engine

If there is nothing in the "C implementation" column, I haven't gotten that far yet.

## Index of Implemented Algorithms

Pseudo-code Algorithm | C implementation
----------------------|-----------------
TABLE-DRIVEN-AGENT | algorithm template
REFLEX-VACUUM-AGENT | [reflexVacuumAgent](reflexVacuumAgent.c)
SIMPLE-REFLEX-AGENT | algorithm template
MODEL-BASED-REFLEX-AGENT | algorithm template
SIMPLE-PROBLEM-SOLVING-AGENT | algorithm template
BEST-FIRST-SEARCH |
BREADTH-FIRST-SEARCH |
ITERATIVE-DEEPENING-SEARCH |
BIBF-SEARCH |
UNIFORM-COST-SEARCH |
DEPTH-LIMITED-SEARCH |
RECURSIVE-BEST-FIRST-SEARCH |
HILL-CLIMBING |
SIMULATED-ANNEALING |
GENETIC-ALGORITHM |
AND-OR-GRAPH-SEARCH |
ONLINE-DFS-AGENT |
LRTA*-AGENT |
MINIMAX-SEARCH |
ALPHA-BETA-SEARCH |
MONTE-CARLO-TREE-SEARCH |
AC-3 |
BACKTRACKING-SEARCH |
MIN-CONFLICTS |
TREE-CSP-SOLVER |
KB-AGENT |
TT-ENTAILS |
PL-RESOLUTION |
PL-FC-ENTAILS? |
DPLL-SATISFIABLE? |
WALKSAT |
HYBRID-WUMPUS-AGENT |
SATPLAN |
UNIFY |
FOL-FC-ASK |
FOL-BC-ASK |
APPEND |
AIR-CARGO-TRANSPORT-PROBLEM |
SPARE-TIRE-PROBLEM |
BLOCKS-WORLD |
HAVE-CAKE-AND-EAT-CAKE-TOO-PROBLEM |
GRAPHPLAN |
REFINEMENT-HIGH-LEVEL-ACTIONS |
HIERARCHICAL-SEARCH |
ANGELIC-SEARCH |
JOB-SHOP-SCHEDULING-PROBLEM |
DT-AGENT |
ENUMERATION-ASK |
ELIMINATION-ASK |
PRIOR-SAMPLE |
REJECTION-SAMPLING |
LIKELIHOOD-WEIGHTING |
GIBBS-ASK |
FORWARD-BACKWARD |
FIXED-LAG-SMOOTHING |
PARTICLE-FILTERING |
OUPM |
NET-VISA |
RADAR |
GENERATE-IMAGE |
GENERATE-MARKOV-LETTERS |
INFORMATION-GATHERING-AGENT |
VALUE-ITERATION |
POLICY-ITERATION |
POMDP-VALUE-ITERATION |
DOUBLES-TENNIS-PROBLEM |
LEARN-DECISION-TREE |
CROSS-VALIDATION-WRAPPER |
DECISION-LIST-LEARNING |
ADABOOST |
CURRENT-BEST-LEARNING |
VERSION-SPACE-LEARNING |
MINIMAL-CONSISTENT-DET |
FOIL |
PASSIVE-ADP-AGENT |
PASSIVE-TD-AGENT |
Q-LEARNING-AGENT |
HITS |
CYK-PARSE |
SENTENCE-TREE |
MONTE-CARLO-LOCALIZATION |
POWERS-OF-2 |
