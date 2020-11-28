#ifndef __FSSNW_H__
#define __FSSNW_H__

#include <vector>

using namespace std;

class cFSSNW
{
public:
  //! A constructor.
  /*!
     Constructs the FSSNW class, and assigns the values.
     */
  cFSSNW(char *file);

  //! A destructor.
  /*!
    Clears the memory.
     */
  ~cFSSNW();

  //! A normal member taking in the schedule and returning the cost.
  /*!
     \param A flowshop schedule
     \return The makespan value
     */
  float Makespan(vector<int> Schedule);

  //! Returns the number of jobs.
  /*!
     \param no parameters
     \return The number of jobs
     */
  int GetJobs();

  //! Returns the number of machines.
  /*!
     \param no parameters
     \return The number of machines
     */
  float **GetProcessTime();
  int GetMachines();
  void Initialize();

private:
  //! The processing time matrix.
  float **m_ProcessTime;
  //! The completion time matrix.
  float **m_CompletionTime;
  //! The number of jobs.
  int m_Jobs;
  //! The number of machines.
  int m_Machines;
};

#endif
