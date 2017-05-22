# TSP_benchmarks
Experiments with C++ parallel programming
## Algorithms
* BFS - Simple single-threaded BFS. It's super slow because, unlike with DFS, we must expand the entire graph before getting a result, which requires a ton of memory and makes pruning much less effective.
* Iterative DFS - Uses much less memory than BFS and is able to stop exploring branch if it's going to be longer than a previous path.
* Recursive DFS - At least with current implementation, the recursive version performs better - it must be faster to keep data in stack frames than passing-by-value to an STL stack.
* Parallel Iterative DFS - First expands the graph with BFS, then passes those sub-graphs off worker threads to do DFS. Thread count is monitored with an atomic counter. If a thread finishes DFSing, more work can be given out by the main thread. However, it may (likely) be duplicate work done by another thread.
* Parallel Recursive DFS - Same as above, but with each worker thread using the (currently) faster recursive DFS.
* Concurrent Queue Recursive DFS - Instead of manually tracking active threads and giving out (duplicate) work, this implementation allows all threads to share a boost::lockfree:queue containing the sub-graphs to explore. If a thread finishes on a subgraph, it can just grab another (if any remain).
* Concurrent Stack Iterative DFS - The idea is to allow all threads to pull off the same boost:lockfree:stack for their DFS. Of course, the idea is bad because they end up spending most of their time fighting over the stack. 
### Four threads, 15 Cities, old code (December Commit)
* BFS: Broken at first, and then out-of-memory once fixed
* Recursive DFS: 2.46331s
* Iterative DFS: 6.3623s
* Parallel Iterative DFS: 11.953s
* Parallel Recursive DFS: 0.722591s
* Concurrent Queue Recursive DFS: 0.662083s
* Concurrent Stack Iterative DFS: 33.7057s

Lots of pass-by-value and raw pointers going on. Will update and see how performance is affected. 


