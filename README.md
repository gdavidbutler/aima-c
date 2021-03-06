## aima-c

[C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) implementation of [aima-pseudocode](https://github.com/aimacode/aima-pseudocode) from [Russell And Norvig's "Artificial Intelligence - A Modern Approach, Fourth Edition"](http://aima.cs.berkeley.edu).

[Why C](https://sqlite.org/whyc.html)?
It is the best "get it done" computer programming language.

C is a *small* programming language.
The [first edition book](https://archive.org/details/TheCProgrammingLanguageFirstEdition) is 236 pages.
The [second edition book](https://archive.org/details/the_c_programming_language_2_20181213) is 288 pages.
The "reference manual" section is only 35 pages and 49 pages respectively.

Then why is C considered *hard*?
The usual answer is *pointers*.
The real answer is **choice**.
C only provides portable access to general processor operations on memory.
The language has no support for dynamic memory or I/O (input / output).
C callable routines (e.g. the [C standard library](https://en.wikipedia.org/wiki/C_standard_library)) must be provided to enable C programs to interface with an environment.
Everything else is programmer choice.

For example, there are a few conventions used in the book that neither C nor the standard library address:

* From the [preface](http://aima.cs.berkeley.edu/newchap00.pdf), "The main unifying theme is the idea of an [intelligent agent](https://en.wikipedia.org/wiki/Intelligent_agent)."
* From Appendix B "notes on languages and algorithms", the concepts "persistent variables" and "yield".

These concurrent styles can be implemented using various [coroutine](https://en.wikipedia.org/wiki/Coroutine#Implementations_for_C) techniques but don't enable parallelism.
For both concurrency and parallelism [POSIX Threads](https://en.wikipedia.org/wiki/POSIX_Threads) is used.
To reduce the complexity of using threads a [channel](https://en.wikipedia.org/wiki/Channel_(programming)) style is used using [pthreadChannel](https://github.com/gdavidbutler/pthreadChannel).
These choices provide high level clarity and low level performance.

If there is nothing in the "C implementation" column, I haven't gotten that far.

## Index of implemented [algorithms](http://aima.cs.berkeley.edu/algorithms.pdf)

Pseudo-code Algorithm | Prototype | Implementation
----------------------|-----------|----------------
TABLE-DRIVEN-AGENT | [tableDrivenAgent.h](tableDrivenAgent.h) | [tableDrivenAgent.c](tableDrivenAgent.c)
REFLEX-VACUUM-AGENT | [reflexVacuumAgent.h](reflexVacuumAgent.h) | [reflexVacuumAgent.c](reflexVacuumAgent.c)
SIMPLE-REFLEX-AGENT | [simpleReflexAgent.h](simpleReflexAgent.h) | [simpleReflexAgent.c](simpleReflexAgent.c)
MODEL-BASED-REFLEX-AGENT | [modelBasedReflexAgent.h](modelBasedReflexAgent.h) | [modelBasedReflexAgent.c](modelBasedReflexAgent.c)
BEST-FIRST-SEARCH |
BREADTH-FIRST-SEARCH |
UNIFORM-COST-SEARCH |
ITERATIVE-DEEPENING-SEARCH |
DEPTH-LIMITED-SEARCH |
BIBF-SEARCH |
RECURSIVE-BEST-FIRST-SEARCH |
HILL-CLIMBING |
SIMULATED-ANNEALING |
GENETIC-ALGORITHM |
AND-OR-SEARCH |
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
