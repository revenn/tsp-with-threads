#pragma once
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "Cities.h"
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

int NUM_THREADS = 10;
atomic<int> active_threads(0); // 0 to n-1 for array Indexing
//atomic<Tour> csDFSbest(UINT_MAX);
Tour BFSbest(UINT_MAX), DFSbest(UINT_MAX), PSbest(UINT_MAX), iDFSbest(UINT_MAX), prDFSbest(UINT_MAX), cqDFSbest(UINT_MAX), csDFSbest(UINT_MAX); // globals for best tour so far - initialized high
mutex g_lock;
condition_variable cv;


void iDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour(t.miasta);
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
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

void PS(Tour t) {
	queue<Tour> frontier;
	Tour curTour(t.miasta);
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
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

/* Do a depth-first search to find the best tour
* starting with the partially complete tour t
*/
void prDFShelper(Tour& t) {
	if (!t.better_than(prDFSbest)) return; // if we've already seen something better

	// If we've added all the cities and we're still here, this must be
	// the best tour
	if (t.get_num_cities() == t.miasta.rozmiar) {
		std::lock_guard<std::mutex> lock(g_lock);
		prDFSbest = t;
	}
	// Otherwise, try to add the next city and recursively call DFS
	// if successful
	else {
		for (unsigned int i = 0; i < t.miasta.rozmiar; i++) {
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


void prDFS(Tour& t) {
	queue<Tour> frontier;
	Tour curTour(t.miasta);
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(prDFSbest)) {
				prDFSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
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

void siDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour(t.miasta);
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(iDFSbest)) {
				iDFSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
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
