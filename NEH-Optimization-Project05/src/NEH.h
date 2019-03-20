// Shane Vance
// 5/29/2018

#include <iostream>
#include <vector>
#include "FSS.h"
#include "FSSB.h"
#include "FSSNW.h"

using namespace std;

/**
 *
 * This is a structure for the Jobs used for the NEH heuristic class.
 * 
 * @typedef Job the contruction for each of the jobs
 */
typedef struct Job {
	Job(int num, int time) : number(num), tTime(time) {}

	int number;
	int tTime;

	bool operator>(const Job &j) const {
		return tTime > j.tTime;
	}
} Job;

class NEH {

public:

	NEH();
	~NEH();
	void FSSNEH(cFSS *FSS);
	void FSSBNEH(cFSSB *FSSB);
	void FSSNWNEH(cFSSNW *FSSNW);
	float GetBestMakespan();
	void PrintBestSchedule(bool flag);

private:

	float best;
	bool flag;

	void swap(int &a, int &b) {
		int temp = a;
		a = b;
		b = temp;
	}

	void outputResults(float best, double tTaken, int fnCalls, int jCount, vector<int> Schedule, bool pSchedule) {
		if (pSchedule) {
			// output the best job sequence
			for (int i = 0; i < jCount; i++) {
				cout << "J" << Schedule[i];
				if (i < jCount - 1) {
					cout << " - ";
				}
			}
			cout << endl;
		}

		cout << best << "," << tTaken << "," << fnCalls;
	}

	vector<Job> calculateFSSJobs(cFSS *FSS) {
		int totalTime = 0;
		vector<Job> jobs;

		// calculate all our total times for each job for each machine
		for (int i = 0; i < FSS->GetJobs(); i++) {
			totalTime = 0;
			for (int j = 0; j < FSS->GetMachines(); j++) {
				totalTime += FSS->GetProcessTime()[j][i];
			}
			jobs.push_back(Job(i + 1, totalTime));
		}

		return jobs;
	}

	vector<Job> calculateFSSBJobs(cFSSB *FSSB) {
		int totalTime = 0;
		vector<Job> jobs;

		// calculate all our total times for each job for each machine
		for (int i = 0; i < FSSB->GetJobs(); i++) {
			totalTime = 0;
			for (int j = 0; j < FSSB->GetMachines(); j++) {
				totalTime += FSSB->GetProcessTime()[j][i];
			}
			jobs.push_back(Job(i + 1, totalTime));
		}

		return jobs;
	}

	vector<Job> calculateFSSNWJobs(cFSSNW *FSSNW) {
		int totalTime = 0;
		vector<Job> jobs;

		// calculate all our total times for each job for each machine
		for (int i = 0; i < FSSNW->GetJobs(); i++) {
			totalTime = 0;
			for (int j = 0; j < FSSNW->GetMachines(); j++) {
				totalTime += FSSNW->GetProcessTime()[j][i];
			}
			jobs.push_back(Job(i + 1, totalTime));
		}

		return jobs;
	}

};

