#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "cities.h"
#include <string>
#include <Queue>
#include <chrono>
#include <thread>
#include <stack>
#include "Tour.h"
#include <condition_variable>
#include <atomic>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/stack.hpp>

using namespace std;

//#define Iterative_BFS
#define Iterative_DFS
#define Recursive_DFS
#define P_Recursive_DFS
#define P_Iterative_DFS
//#define CQ_Recursive_DFS
//#define CS_Iterative_DFS


int NUM_THREADS = 10;
atomic<int> active_threads (0); // 0 to n-1 for array Indexing
//atomic<Tour> csDFSbest(UINT_MAX);
Tour BFSbest(UINT_MAX), DFSbest(UINT_MAX), PSbest(UINT_MAX), iDFSbest(UINT_MAX), prDFSbest(UINT_MAX), cqDFSbest(UINT_MAX), csDFSbest(UINT_MAX); // globals for best tour so far - initialized high
mutex g_lock;
condition_variable cv;

void DFS(Tour& t);	// Recursive DFS given by Rivoire
void iDFS(Tour t);	// Iterative DFS from Utah lecture notes
void siDFS(Tour t); // Helper function for Parallel DFS. Implements iterative DFS.
void BFS(Tour t);	// Iterative BFS. Very slow.
void PS(Tour t);	// First try at parallelization. Workload managed explicitly by main thread.
void prDFS(Tour& t);	// Improves upon PS by using recursive instead of iterative DFS. 
void prDFSwrapper(Tour& t);	// Helper function for prDFS
void prDFShelper(Tour& t); // Helper function for prDFS
void cqDFS(Tour& t);	// Improves upon prDFS by using a concurrent queue to manage workload.
void cqDFShelper(Tour& t); // Helper function for cqDFS
void cqDFSwrapper(boost::lockfree::queue<Tour*>* BFSfrontier, unsigned int tid);
void csDFS(Tour& t);
void csDFShelper(boost::lockfree::stack<Tour*>* frontier);




int main(int argc, char* argv[]) {
	Tour t;

	if (argc < 2) {
		cout << "Number of threads unspecified. Using " << NUM_THREADS << '.' << endl;
	}
	else {
		NUM_THREADS = strtol(argv[1], NULL, 10);
	}
	
#ifdef Iterative_BFS
	chrono::high_resolution_clock::time_point BFSStart = std::chrono::high_resolution_clock::now();
	BFS(t);
	chrono::high_resolution_clock::time_point BFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> BFSTime = chrono::duration_cast<std::chrono::duration<double>>(BFSEnd - BFSStart);
	
	cout << BFSbest;
	t.clear();
#endif
	
#ifdef Recursive_DFS
	chrono::high_resolution_clock::time_point DFSStart = std::chrono::high_resolution_clock::now();
	DFS(t);
	chrono::high_resolution_clock::time_point DFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> DFSTime = chrono::duration_cast<std::chrono::duration<double>>(DFSEnd - DFSStart);
	
	cout << DFSbest;
	t.clear();
#endif
	
#ifdef Iterative_DFS
	chrono::high_resolution_clock::time_point siDFSStart = std::chrono::high_resolution_clock::now();
	siDFS(t);
	chrono::high_resolution_clock::time_point siDFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> siDFSTime = chrono::duration_cast<std::chrono::duration<double>>(siDFSEnd - siDFSStart);

	cout << iDFSbest;
	t.clear();
#endif

#ifdef P_Iterative_DFS
	chrono::high_resolution_clock::time_point PSStart = std::chrono::high_resolution_clock::now();
	PS(t);
	chrono::high_resolution_clock::time_point PSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> PSTime = chrono::duration_cast<std::chrono::duration<double>>(PSEnd - PSStart);

	cout << PSbest;
	t.clear();
#endif

#ifdef P_Recursive_DFS
	chrono::high_resolution_clock::time_point prDFSStart = std::chrono::high_resolution_clock::now();
	prDFS(t);
	chrono::high_resolution_clock::time_point prDFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> prDFSTime = chrono::duration_cast<std::chrono::duration<double>>(prDFSEnd - prDFSStart);

	cout << prDFSbest;
	t.clear();
#endif

#ifdef CQ_Recursive_DFS
	chrono::high_resolution_clock::time_point cqDFSStart = std::chrono::high_resolution_clock::now();
	cqDFS(t);
	chrono::high_resolution_clock::time_point cqDFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> cqDFSTime = chrono::duration_cast<std::chrono::duration<double>>(cqDFSEnd - cqDFSStart);

	cout << cqDFSbest;
	t.clear();
#endif

#ifdef CS_Iterative_DFS
	chrono::high_resolution_clock::time_point csDFSStart = std::chrono::high_resolution_clock::now();
	csDFS(t);
	chrono::high_resolution_clock::time_point csDFSEnd = std::chrono::high_resolution_clock::now();
	chrono::duration<double> csDFSTime = chrono::duration_cast<std::chrono::duration<double>>(csDFSEnd - csDFSStart);

	cout << csDFSbest;
	t.clear();
#endif

#ifdef Iterative_BFS
	cout << "Iterative BFS time: " << BFSTime.count() << " seconds." << endl;
#endif
#ifdef Recursive_DFS
	cout << "Recursive DFS time: " << DFSTime.count() << " seconds." << endl;
#endif
#ifdef Iterative_DFS
	cout << "Iterative DFS time: " << siDFSTime.count() << " seconds." << endl;
#endif
#ifdef P_Iterative_DFS
	cout << "Parallel Iterative DFS time: " << PSTime.count() << " seconds." << endl;
#endif
#ifdef P_Recursive_DFS
	cout << "Parallel Recursive DFS time: " << prDFSTime.count() << " seconds." << endl;
#endif
#ifdef CQ_Recursive_DFS
	cout << "Concurrent Queue Recursive DFS time: " << cqDFSTime.count() << " seconds." << endl;
#endif
#ifdef CS_Iterative_DFS
	cout << "Concurrent Stack Iterative DFS time: " << csDFSTime.count() << " seconds." << endl;
#endif
	
	getchar();
	return 0;
}


/*Parallel search using combination of BFS and iterative DFS
*/
void PS(Tour t) {
	queue<Tour> frontier;
	Tour curTour;
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();
		
		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == num_cities) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, PSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
		//if we have available worker threads, make them DFS
		else if (active_threads < NUM_THREADS) {
			++active_threads;
			threadPool.push_back(thread(iDFS, curTour));
		}
		//if not, wait until we do
		else {
			unique_lock<mutex> lock(g_lock);
			cv.wait(lock);
			++active_threads;
			threadPool.push_back(thread(iDFS, curTour));
		}
	}
	for (thread& t : threadPool) {
		t.join();
	}
}







/* Do an iterative breadth-first search to find the best tour
* starting with the incomplete tour t
*/
void BFS(Tour t) {
	queue<unique_ptr<Tour>> frontier; //Queue for BFS
	Tour curTour;
	frontier.push(make_unique<Tour>(t)); //start with just the hometown

	while (!frontier.empty()) {
		curTour = *frontier.front();
		frontier.pop();
		
		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == num_cities) {
			if (curTour.better_than(BFSbest)) {
				BFSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, BFSbest)) {
					Tour copyForQueue(curTour);
					frontier.push(make_unique<Tour>(copyForQueue));
					curTour.remove_last();
				}
			}
		}
	}

}





/* Do a depth-first search to find the best tour
* starting with the partially complete tour t
*/
void DFS(Tour& t) {
	if (!t.better_than(DFSbest)) return; // if we've already seen something better

	// If we've added all the cities and we're still here, this must be
	// the best tour
	if (t.get_num_cities() == num_cities) {
		DFSbest = t;
	}
	// Otherwise, try to add the next city and recursively call DFS
	// if successful
	else {
		for (unsigned int i = 0; i < num_cities; i++) {
			if (t.add_city(i, DFSbest)) {
				DFS(t);
				t.remove_last();
			}
		}
	}
}






/* Do an interative depth-first search to find the best tour
* starting with the partially complete tour t
Only use with PS. Use siDFS for serial
*/
void iDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour;
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == num_cities) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, PSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
	}
	--active_threads;
	cv.notify_all();
}




/* Do an interative depth-first search to find the best tour
* starting with the partially complete tour t
*/
void siDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour;
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == num_cities) {
			if (curTour.better_than(iDFSbest)) {
				iDFSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, iDFSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
	}

}



/*Parallel implementation using recursive DFS
*/
void prDFS(Tour& t) {
	queue<Tour> frontier;
	Tour curTour;
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == num_cities) {
			if (curTour.better_than(prDFSbest)) {
				prDFSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, prDFSbest)) {
					Tour copyForQueue(curTour);
					frontier.push(copyForQueue);
					curTour.remove_last();
				}
			}
		}
		//if we have available worker threads, make them DFS
		else if (active_threads < NUM_THREADS) {
			++active_threads;
			threadPool.push_back(thread(prDFSwrapper, curTour));
		}
		//if not, wait until we do
		else {
			unique_lock<mutex> lock(g_lock);
			cv.wait(lock);
			++active_threads;
			threadPool.push_back(thread(prDFSwrapper, curTour));
		}
	}
	for (thread& t : threadPool) {
		t.join();
	}
}

/* Do a depth-first search to find the best tour
* starting with the partially complete tour t
*/
void prDFShelper(Tour& t) {
	if (!t.better_than(prDFSbest)) return; // if we've already seen something better

	// If we've added all the cities and we're still here, this must be
	// the best tour
	if (t.get_num_cities() == num_cities) {
		std::lock_guard<std::mutex> lock(g_lock);
		prDFSbest = t;
	}
	// Otherwise, try to add the next city and recursively call DFS
	// if successful
	else {
		for (unsigned int i = 0; i < num_cities; i++) {
			if (t.add_city(i, prDFSbest)) {
				prDFShelper(t);
				t.remove_last();
			}
		}
	}
}


/*Calls prDFShelper and notifies main thread
* on completion
*/
void prDFSwrapper(Tour& t) {
	prDFShelper(t);
	--active_threads;
	cv.notify_all();
}


/*Parallel implementation using concurrent queue
* instead of making the main thread manage work.
*/
void cqDFS(Tour& t) {
	boost::lockfree::queue<Tour*> frontier(num_cities*100);
	Tour *curTour;
	vector<thread> threadPool(NUM_THREADS);
	frontier.push(&t);
	size_t frontierSize = 1;

	//Expand tree to create enough work for threads
	while (!frontier.empty() && frontierSize < NUM_THREADS * 2) {
		frontier.pop(curTour);
		--frontierSize;
		for (unsigned int i = num_cities - 1; i > 0; --i) {
			//If city isn't already on tour and doesn't exceed best mileage,
			//add it to queue for exploration later.
			if (curTour->add_city(i, cqDFSbest)) {
				Tour *copyForQueue = new Tour(*curTour);
				frontier.push(copyForQueue);
				++frontierSize;
				curTour->remove_last();
			}
		}
	}

	for (unsigned int tid = 0; tid < NUM_THREADS; ++tid){
		threadPool[tid] = thread(cqDFSwrapper, &frontier, tid);
	}
	for (unsigned int tid = 0; tid < NUM_THREADS; ++tid){
		threadPool[tid].join();
	}

}


/*Thread-safe Recursive DFS for cqDFS
*/
void cqDFShelper(Tour& t) {
	if (!t.better_than(cqDFSbest)) return; // if we've already seen something better

	// If we've added all the cities and we're still here, this must be
	// the best tour
	if (t.get_num_cities() == num_cities) {
		std::lock_guard<std::mutex> lock(g_lock);
		cqDFSbest = t;
	}
	// Otherwise, try to add the next city and recursively call DFS
	// if successful
	else {
		for (unsigned int i = 0; i < num_cities; i++) {
			if (t.add_city(i, cqDFSbest)) {
				cqDFShelper(t);
				t.remove_last();
			}
		}
	}
}

/*Conducts DFS on tours in the BFSfrontier
*tid would be used for workload stealing
*/
void cqDFSwrapper(boost::lockfree::queue<Tour*>* BFSfrontier, unsigned int tid) {
	Tour* curTour;
	while (BFSfrontier->pop(curTour)) {
		cqDFShelper(*curTour);
	}
}

void csDFS(Tour& t) {
	boost::lockfree::stack<Tour*>* frontier = new boost::lockfree::stack<Tour*>(0);		//Stack for DFS
	frontier->unsynchronized_push(&t);			//start with just the hometown
	vector<thread> threadPool(NUM_THREADS);

	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threadPool[i] = thread(csDFShelper, frontier);
	}
	for (unsigned int i = 0; i < NUM_THREADS; ++i) {
		threadPool[i].join();
	}

}

void csDFShelper(boost::lockfree::stack<Tour*>* frontier) {
	Tour *curTour;

	while (frontier->pop(curTour)) {
		//If tour is complete, check if it's the best
		if (curTour->get_num_cities() == num_cities) {
			if (curTour->better_than(csDFSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				csDFSbest = *curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = num_cities - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to stack for exploration later.
				if (curTour->add_city(i, csDFSbest)) {
					Tour *copyForStack = new Tour(*curTour);
					frontier->push(copyForStack);
					curTour->remove_last();
				}
			}
		}

	}
}