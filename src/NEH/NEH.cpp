// Shane Vance
// 5/29/2018

#include <ctime>
#include <functional>
#include <algorithm>
#include "NEH.h"

/**
 * This is the default constructor the for the NEH heuristic class. NEH is regarded as the best constructive method for
 * solving Flow-shop Scheduling. It is a permutation based heuristic relying on partial sequences for 2 or more jobs. It has
 * very fast execution time.
 * 
 * @param no parameters
 */
NEH::NEH() : best(0.0f), flag(false) {}

/**
 * This is the default constructor for the NEH heuristic class.
 * 
 * @param no parameters
 */
NEH::~NEH() = default;

/**
 *
 * This is a NEH heuristic algorithm that takes into consideration tie-breaking for the Flow-shop scheduling (FSS) sequence. 
 * It runs very fast. This will output to the the console the result of the best found makespan for the job sequence.
 * 
 * @param FSS this is the flow-shop scheduling class
 * @return no return
 */
void NEH::FSSNEH(cFSS* FSS) {
	
	//! Initilaize our variables
    vector<int> Schedule, currBest;
    vector<vector<int>> mBest;
    float cBest = 0.0f;
    int totalTime = 0, fnCalls = 0;
    double tTaken = 0.0;
    bool tieBreak = true;

	best = 0.0f;

	double start = clock();

	vector<Job> job = calculateFSSJobs(FSS);

	// rank each job by total time
    sort(job.begin(), job.end(), greater<Job>());

    // take the first two jobs initially, with the worst times
    for (int i = 0; i < 2; i++) {
        Schedule.push_back(job[i].number);
    }

    // start with L=3 (aka 2) given the NEH algorithm via Johnson
    for (int i = 2; i <= FSS->GetJobs(); i += 2) {
        // set up our best solution sequence of jobs
        currBest = Schedule;
        best = FSS->Makespan(currBest);
        fnCalls++;

        // clear our solutions list and set tie break to true
        mBest.clear();
        tieBreak = true;

        // check for the current best of the iteration
        for (int j = 0; j < Schedule.size() - 1; j++) {
            // do some swapping
            swap(Schedule[j], Schedule[j + 1]);
            cBest = FSS->Makespan(Schedule);
            fnCalls++;

            // check if for every iteration there is the same makespan
            // push_back() onto the list
            if (cBest == best && tieBreak) {
                mBest.push_back(Schedule);
            }

            // check current best to overall best
            if (cBest < best) {
                // set our best solutions
                best = cBest;
                currBest = Schedule;

                // set the tie breaker to false
                tieBreak = false;
            }

        }

        // push the overall best onto the list for the next job sequence to be checked
        mBest.push_back(currBest);

        if (i < FSS->GetJobs()) {
            float tBest = 0;
            vector<int> rBest;

            for (int k = mBest.size() - 1; k >= 0; k--) {
                // insert the next job to be evaluated
                mBest[k].insert(mBest[k].begin(), job[i].number);

                // evaluate the current best solutions
                rBest = mBest[k];
                tBest = FSS->Makespan(mBest[k]);

                // initialize the overall best for the new next job
                if (k == mBest.size() - 1) {
                    best = tBest;
                    currBest = rBest;
					fnCalls++;
                }

                // iterate through all possible solutions of the job sequence
                for (int g = 0; g < mBest[k].size() - 1; g++) {
                    // do some swapping
                    swap(mBest[k][g], mBest[k][g + 1]);
                    cBest = FSS->Makespan(mBest[k]);
										
					if (k == mBest.size() - 1) {
						fnCalls++;
					}

                    // check current best to overall best
                    if (cBest < tBest) {
                        tBest = cBest;
                        rBest = mBest[k];
                    }
                }

                // check and set our best solutions if better
                if (tBest < best) {
                    best = tBest;
                    currBest = rBest;
                }
            }
			
            // check whether the total amount of jobs is odd
            if (!((i + 1) == FSS->GetJobs() && FSS->GetJobs() % 2 == 1)) {
                currBest.insert(currBest.begin(), job[i + 1].number);
            }
        }

        Schedule = currBest;
    }

    double stop = clock();

    tTaken = (stop - start) / double(CLOCKS_PER_SEC);

    outputResults(best, tTaken, fnCalls, FSS->GetJobs(), Schedule, flag);
	cout << endl;

}

/**
 *
 * This is a NEH heuristic algorithm that does not take into consideration tie-breaking for Flow-shop scheduling with blocking (FSSB). 
 * It runs very fast. This will output to the the console the result of the best found makespan for the job sequence.
 * 
 * @param FSSB this is the flow-shop with blocking scheduling class
 * @return no return
 */
void NEH::FSSBNEH(cFSSB* FSSB) {

    //! Initilaize our variables
    vector<int> Schedule, currBest;
    vector<vector<int>> mBest;
    float cBest = 0.0f;
    int totalTime = 0, fnCalls = 0;
    double tTaken = 0.0;
    bool tieBreak = true;

    best = 0.0f;

    double start = clock();

    // calculate all our total times for each job for each machine
    vector<Job> job = calculateFSSBJobs(FSSB);

    // rank each job by total time
    sort(job.begin(), job.end(), greater<Job>());

    // take the first two jobs initially, with the worst times
    for (int i = 0; i < 2; i++) {
        Schedule.push_back(job[i].number);
    }

    // start with L=3 (aka 2) given the NEH algorithm via Johnson
    for (int i = 2; i <= FSSB->GetJobs(); i += 2) {
        // set up our best solution sequence of jobs
        currBest = Schedule;
        best = FSSB->Makespan(currBest);
        fnCalls++;

        // clear our solutions list and set tie break to true
        mBest.clear();
        tieBreak = true;

        // check for the current best of the iteration
        for (int j = 0; j < Schedule.size() - 1; j++) {
            // do some swapping
            swap(Schedule[j], Schedule[j + 1]);
            cBest = FSSB->Makespan(Schedule);
            fnCalls++;

            // check if for every iteration there is the same makespan
            // push_back() onto the list
            if (cBest == best && tieBreak) {
                mBest.push_back(Schedule);
            }

            // check current best to overall best
            if (cBest < best) {
                // set our best solutions
                best = cBest;
                currBest = Schedule;

                // set the tie breaker to false
                tieBreak = false;
            }

        }

        // push the overall best onto the list for the next job sequence to be checked
        mBest.push_back(currBest);

        if (i < FSSB->GetJobs()) {
            float tBest = 0;
            vector<int> rBest;

            for (int k = mBest.size() - 1; k >= 0; k--) {
                // insert the next job to be evaluated
                mBest[k].insert(mBest[k].begin(), job[i].number);

                // evaluate the current best solutions
                rBest = mBest[k];
                tBest = FSSB->Makespan(mBest[k]);

                // initialize the overall best for the new next job
                if (k == mBest.size() - 1) {
                    best = tBest;
                    currBest = rBest;
                    fnCalls++;
                }

                // iterate through all possible solutions of the job sequence
                for (int g = 0; g < mBest[k].size() - 1; g++) {
                    // do some swapping
                    swap(mBest[k][g], mBest[k][g + 1]);
                    cBest = FSSB->Makespan(mBest[k]);

                    if (k == mBest.size() - 1) {
                        fnCalls++;
                    }

                    // check current best to overall best
                    if (cBest < tBest) {
                        tBest = cBest;
                        rBest = mBest[k];
                    }
                }

                // check and set our best solutions if better
                if (tBest < best) {
                    best = tBest;
                    currBest = rBest;
                }
            }

            // check whether the total amount of jobs is odd
            if (!((i + 1) == FSSB->GetJobs() && FSSB->GetJobs() % 2 == 1)) {
                currBest.insert(currBest.begin(), job[i + 1].number);
            }
        }

        Schedule = currBest;
    }

    double stop = clock();

    tTaken = (stop - start) / double(CLOCKS_PER_SEC);

    outputResults(best, tTaken, fnCalls, FSSB->GetJobs(), Schedule, flag);
	cout << endl;

}

/**
 *
 * This is a NEH heuristic algorithm that takes into consideration tie-breaking for Flow-shop scheduling with no-wait (FSSNW). 
 * It runs very fast. This will output to the the console the result of the best found makespan for the job sequence.
 * 
 * @param FSSNW this is the flow-shop with no-wait scheduling class
 * @return no return
 */
void NEH::FSSNWNEH(cFSSNW* FSSNW) {
	
	//! Initilaize our variables
    vector<int> Schedule, currBest;
    vector<vector<int>> mBest;
    float cBest = 0.0f;
    int totalTime = 0, fnCalls = 0;
    double tTaken = 0.0;
    bool tieBreak = true;

	best = 0.0f;

    double start = clock();

    // calculate all our total times for each job for each machine
    vector<Job> job = calculateFSSNWJobs(FSSNW);

    // rank each job by total time
    sort(job.begin(), job.end(), greater<Job>());

    // take the first two jobs initially, with the worst times
    for (int i = 0; i < 2; i++) {
        Schedule.push_back(job[i].number);
    }

    // start with L=3 (aka 2) given the NEH algorithm via Johnson
    for (int i = 2; i <= FSSNW->GetJobs(); i += 2) {
        // set up our best solution sequence of jobs
        currBest = Schedule;
        best = FSSNW->Makespan(currBest);
        fnCalls++;

        // clear our solutions list and set tie break to true
        mBest.clear();
        tieBreak = true;

        // check for the current best of the iteration
        for (int j = 0; j < Schedule.size() - 1; j++) {
            // do some swapping
            swap(Schedule[j], Schedule[j + 1]);
            cBest = FSSNW->Makespan(Schedule);
            fnCalls++;

            // check if for every iteration there is the same makespan
            // push_back() onto the list
            if (cBest == best && tieBreak) {
                mBest.push_back(Schedule);
            }

            // check current best to overall best
            if (cBest < best) {
                // set our best solutions
                best = cBest;
                currBest = Schedule;

                // set the tie breaker to false
                tieBreak = false;
            }

        }

        // push the overall best onto the list for the next job sequence to be checked
        mBest.push_back(currBest);

        if (i < FSSNW->GetJobs()) {
            float tBest = 0;            
            vector<int> rBest;

            for (int k = mBest.size() - 1; k >= 0; k--) {
            	// insert the next job to be evaluated
                mBest[k].insert(mBest[k].begin(), job[i].number);

                // evaluate the current best solutions
                rBest = mBest[k];
                tBest = FSSNW->Makespan(mBest[k]);

                // initialize the overall best for the new next job
                if (k == mBest.size() - 1) {
                    best = tBest;
                    currBest = rBest;
					fnCalls++;
                }

                // iterate through all possible solutions of the job sequence
                for (int g = 0; g < mBest[k].size() - 1; g++) {
                    // do some swapping
                    swap(mBest[k][g], mBest[k][g + 1]);
                    cBest = FSSNW->Makespan(mBest[k]);

					if (k == mBest.size() - 1) {
						fnCalls++;
					}

                    // check current best to overall best
                    if (cBest < tBest) {
                        tBest = cBest;
                        rBest = mBest[k];
                    }
                }

                // check and set our best solutions if better
                if (tBest < best) {
                    best = tBest;
                    currBest = rBest;
                }
            }
						
            // check whether the total amount of jobs is odd
            if (!((i + 1) == FSSNW->GetJobs() && FSSNW->GetJobs() % 2 == 1)) {
                currBest.insert(currBest.begin(), job[i + 1].number);
            }
        }

        Schedule = currBest;
    }

    double stop = clock();

    tTaken = (stop - start) / double(CLOCKS_PER_SEC);

    outputResults(best, tTaken, fnCalls, FSSNW->GetJobs(), Schedule, flag);

}

/**
 *
 * This will tell the NEH heuristic class whether to print the resulting best schedule or not.
 *
 * @param flag whether to print the solution schedule or not
 * @return no return
 */
void NEH::PrintBestSchedule(bool flag) {

	this->flag = flag;

}

/**
 *
 * This will get the best makespan.
 * 
 * @param no parameter
 * @return the best makespan
 */
float NEH::GetBestMakespan() {
	
	return best;

}