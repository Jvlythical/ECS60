#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <set>

using namespace std;
typedef struct
{
  short length;
  int *dependencies;
  short numDependencies;
} Job;



void createJobs(int numJobs, int breadth, Job *jobs)
{
  int i, j, k, temp, numWorkers, count = 1, range, workerID, length, realID;
  int *firstPossibleDependency = new int[numJobs], *IDs = new int[numJobs];

  firstPossibleDependency[0] = 1;
  set <int> dependencies;
  set<int>::iterator itr;

  for(i = 0; i < numJobs; i++)
    IDs[i] = i;

  for(i = 0 ; i < numJobs * 2; i++)
  {
    j = rand() % numJobs;
    k = rand() % numJobs;
    temp = IDs[j];
    IDs[j] = IDs[k];
    IDs[k] = temp;
  }  // randomize the IDs.

  for(int ID = 0; ID < numJobs; ID++)
  {
    dependencies.clear();
    numWorkers = rand() % breadth + 1;
    length = rand() % 50 + 1;
    if(numWorkers > numJobs - count)
      numWorkers = numJobs - count;

    range = count + numWorkers - firstPossibleDependency[ID];

    for(int worker = 0; worker < numWorkers;)
    {
      workerID = (rand() % range) + firstPossibleDependency[ID];
      if(workerID >= count)
        workerID = count++;
      if(dependencies.find(workerID) == dependencies.end())
      // if duplicate add new worker
      {
        dependencies.insert(workerID);
        worker++;
      }
    } // for each worker

    realID = IDs[ID];
    jobs[realID].length = length;
    jobs[realID].numDependencies = numWorkers;
    jobs[realID].dependencies = new int[dependencies.size()];
    i = 0;
    for(itr = dependencies.begin(); itr != dependencies.end();
      itr++)
    {
      firstPossibleDependency[*itr] = count;
      jobs[realID].dependencies[i++] = IDs[*itr];
    }  // for each worker
  }  // for each job
}  // createJobs()

void writeFile(char *filename, Job *jobs, int numJobs)
{
  ofstream outf(filename);
  for(int i = 0; i < numJobs; i++)
  {
    outf << i << "," << jobs[i].length << "," << jobs[i].numDependencies;

    for(int j = 0; j < jobs[i].numDependencies; j++)
      outf << "," << jobs[i].dependencies[j];
    outf << endl;
  } // for each job

  outf.close();
} // writeFile()

int main(int argc, char* argv[])
{
  int numJobs, seed, breadth;
  char filename[80];
  cout << "Number of Jobs: ";
  cin >> numJobs;
  cout << "Maximum number of workers per job: ";
  cin >> breadth;
  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "Jobs-%d-%d-%d.csv",numJobs, breadth, seed);
  Job *jobs = new Job[numJobs];
  createJobs(numJobs, breadth, jobs);
  writeFile(filename, jobs, numJobs);
  return 0;
} // main()

