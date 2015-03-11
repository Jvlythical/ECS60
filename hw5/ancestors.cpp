#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef struct
{
  int length;
  int numAncestors;
  int ancestors[1000];
  bool shown;

} Job;


void readFile(const char* filename, Job *jobs,  int numJobs)
{
  int ID = 0, i, children, child;
  char s[1024];

  for(int i = 0; i < numJobs; i++)
    jobs[i].numAncestors = 0;

  ifstream inf(filename);
  while(inf.getline(s, 1024))
  {
    strtok(s, ",");
    jobs[ID].length = atoi(strtok(NULL, ","));
    children  = atoi(strtok(NULL,","));
    for(i = 0; i < children; i++)
    {
      child = atoi(strtok(NULL, ","));
      jobs[child].ancestors[jobs[child].numAncestors++] = ID;
    }
    ID++;
  } // while more jobs to read

} // readFile()

void showAncestors(int ID, Job *jobs)
{
  if(jobs[ID].shown)
    return;

  jobs[ID].shown = true;
  cout << "Job #" << setw(5) << ID << " Len: " << setw(3) << jobs[ID].length;


  for(int i = 0; i < jobs[ID].numAncestors; i++)
    cout << setw(5) << jobs[ID].ancestors[i];

  cout << endl;

  for(int i = 0; i < jobs[ID].numAncestors; i++)
    showAncestors(jobs[ID].ancestors[i], jobs);

}  // showAncestors()

int main(int argc, char* argv[])  // argv[1] = filename,
{
  int numJobs, numChildren, jobNum;

  if(argc == 1)
  {
    cerr << "You must pass a file name as a command line argument.\n";
    return 1;
  }  // if no file name

  sscanf(argv[1], "Jobs-%d-%d", &numJobs, &numChildren);
  Job *jobs = new Job[numJobs];
  readFile(argv[1], jobs, numJobs);
  cout << "Job # (-1 = done): ";
  cin >> jobNum;

  while (jobNum >= 0)
  {
    for(int i = 0; i < numJobs; i++)
      jobs[i].shown = false;
    showAncestors(jobNum, jobs);
    cout << "\n\nJob # (-1 = done): ";
    cin >> jobNum;
  }

  return 0;
}
//---------------------------------------------------------------------------
