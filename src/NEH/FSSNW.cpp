#include "FSSNW.h"
#include <iostream>
#include <fstream>

using namespace std;

cFSSNW::cFSSNW(char *file)
{
  ifstream infile;
  char *fname = file;
  infile.open(fname);

  if (!infile.is_open())
  {
    cout << "Error Opening File.\n";
    exit(1);
  }
  else
  {
    infile >> m_Machines;
    infile >> m_Jobs;

    m_ProcessTime = new float *[m_Machines];
    for (int i = 0; i < m_Machines; i++)
    {
      m_ProcessTime[i] = new float[m_Jobs];
    }

    for (int i = 0; i < (m_Machines); i++)
    {
      for (int j = 0; j < m_Jobs; j++)
      {
        infile >> m_ProcessTime[i][j];
      }
    }

    m_CompletionTime = new float *[m_Machines];
    for (int i = 0; i < m_Machines; i++)
    {
      m_CompletionTime[i] = new float[m_Jobs];
    }
  }

  infile.close();
}

cFSSNW::~cFSSNW()
{
  for (int i = 0; i < m_Machines; i++)
  {
    delete[] m_ProcessTime[i];
  }
  delete[] m_ProcessTime;

  for (int i = 0; i < m_Machines; i++)
  {
    delete[] m_CompletionTime[i];
  }
  delete[] m_CompletionTime;
}

int cFSSNW::GetMachines()
{

  return m_Machines;
}

int cFSSNW::GetJobs()
{

  return m_Jobs;
}

float cFSSNW::Makespan(vector<int> Schedule)
{
  int offset;
  double sum = 0.0;
  Initialize();

  // Calculate processing time for all jobs on first machine
  m_CompletionTime[0][0] = m_ProcessTime[0][Schedule[0] - 1];

  for (int i = 1; i < m_Machines; i++)
  {
    m_CompletionTime[i][0] = (m_CompletionTime[i - 1][0] + m_ProcessTime[i][Schedule[0] - 1]);
  }

  // Calculate for each subsequent job
  for (int i = 1; i < Schedule.size(); i++)
  {
    for (int j = 0; j < m_Machines; j++)
    {
      if (j == 0)
      {
        m_CompletionTime[j][i] = m_CompletionTime[j][i - 1] + m_ProcessTime[j][Schedule[i] - 1];
      }
      else
      {
        if (m_CompletionTime[j - 1][i] < m_CompletionTime[j][i - 1])
        {
          offset = m_CompletionTime[j][i - 1] - m_CompletionTime[j - 1][i];
          for (int k = j - 1; k >= 0; k--)
          {
            m_CompletionTime[k][i] = m_CompletionTime[k][i] + offset;
          }
        }
        m_CompletionTime[j][i] = m_CompletionTime[j - 1][i] + m_ProcessTime[j][Schedule[i] - 1];
      }
    }
  }

  // Calculate the flowtime.
  for (int i = 0; i < Schedule.size(); i++)
  {
    sum += m_CompletionTime[m_Machines - 1][i];
  }

  return sum;
}

float **cFSSNW::GetProcessTime()
{
  return m_ProcessTime;
}

void cFSSNW::Initialize()
{
  for (int i = 0; i < m_Machines; i++)
  {
    for (int j = 0; j < m_Jobs; j++)
    {
      m_CompletionTime[i][j] = 0;
    }
  }
}
