#ifndef __FSS_H__
#define __FSS_H__

#include <vector>
using namespace std;

class cFSS{

public:
    //! A constructor.
    /*!
     Constructs the FSS class, and assigns the values.
     */
	cFSS(char *file);
    
    //! A destructor.
    /*!
    Clears the memory.
     */
	~cFSS();
    
    //! A normal member taking in the schedule and returning the makespan.
    /*!
     \param the schedule
     \return The cost of the tours
     */
	float Makespan(vector<int> Schedule);
    
    //! Returns the number of jobs.
    /*!
     \param no parameters
     \return The number of jobs
     */
    int GetJobs();

    float** GetProcessTime();

    //! Returns the number of machines.
    /*!
     \param no parameters
     \return The number of machines
     */
    int GetMachines();
    float Max(float, float);
    void Initialize();
	
private:
    //! The processing time matrix.
	float** m_ProcessTime;
    //! The completion time matrix.
    float** m_CompletionTime;
    //! The number of jobs.
    int m_Jobs;
    //! The number of machines.
    int m_Machines;
};

#endif
