#include <iostream>
#include <math.h> //for mathematical calculation
#include <iomanip> //header file used of setw() function 
using namespace std;
int main()
{
  int n;
  cout << "\nEnter the number of processes: "; 
  cin >> n; //processes size
  int bt[n], p[n], s[n], wt[n], tat[n], ts, its[n], tq[n][n], rbt[n], ord[n]; //bt=burst time, p=priority, s=shortness component, tat=turn around time, its=intelligent time slice, tq=time quantum
  //rbt=remaining burst time, ord=order

//initialization of waiting time, turn around time, shortness component and time quantum
  for (int i = 0; i < n; i++)
  {
    wt[i] = tat[i] = 0;
    s[i] = 1;
    for (int j = 0; j < n; j++)
      tq[i][j] = 0; 
  }


  cout << "\nEnter the initial time slice: "; 
  cin >> ts; //input time first time slice
  
  for (int i = 0; i < n; i++)
  {
    ord[i] = i + 1; //increment the order by 1 for each iteration
    cout << "\nEnter burst time for process " << i + 1 << ": ";
    cin >> bt[i]; //take burst time for each process
    cout << " \nEnter the priority of the process " << i + 1 << ": ";
    cin >> p[i]; //enter the priority of each process
  }

  int flag = 0, j = 0;
  //Sorting the process according to their burst times

  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - 1; j++)
      if (bt[j] > bt[j - 1])
      {
        ///swapping burst time based on burst time
        int t = bt[j];
        bt[j] = bt[j + 1];
        bt[j + 1] = t;
        //swapping prioirity based on burst time
        t = p[j];
        p[j] = p[j + 1];
        p[j + 1] = t;
        //swapping order based on burst time 
        t = ord[j];
        ord[j] = ord[j + 1];
        ord[j + 1] = t;
      }

  for (int i = 0; i < n; i++)
    p[i] += i; //priority incrementing

  //Sorting processes according to priorities
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - 1; j++)
      if (p[j] > p[j + 1]) //if the priority of current process is more than next process's priority
      {
        //swapping burst times 
        int t = bt[j]; 
        bt[j] = bt[j + 1];
        bt[j + 1] = t;
        //swapping priorities
        t = p[j];
        p[j] = p[j + 1];
        p[j + 1] = t;
        //swapping order
        t = ord[j];
        ord[j] = ord[j + 1];
        ord[j + 1] = t;
      }

  // at the initial stage as the processesing wasn't started the burst time
  // be the same as remaining burst times
  for (int i = 0; i < n; i++)
    rbt[i] = bt[i]; //rbt=remaining burst time

  while (!flag) //flag is working as an indicator
  {
    for (int i = 0; i < n; i++)
    {
        //prioritizing each component
      if (p[i] > 0.67 * n) //(2n/3)
        p[i] = 0;
      else if (p[i] > 0.33 * n) //(n/3)
        p[i] = 1;
      else
        p[i] = 2;

      if (i != 0)
        if ((bt[i] - bt[i - 1]) > 0) //defining shortness component based on burst time of current and previous process
          s[i] = 0;
    //calculating intelligent time slice of each process
      its[i] = ts + bt[i] + s[i] + p[i];

//************************************************************************//
      if (j == 0) //setting time quantum and remaining burst time of round (1)
      {
        if (s[i] == 1)
          tq[j][i] = its[i];
        else
          tq[j][i] = ceil(0.5 * (float)its[i]); //ceil(x) : Returns the smallest integer that is greater than or equal to x (i.e : rounds up the nearest integer).

        if (rbt[i] < tq[j][i])
          tq[j][i] = rbt[i];


        rbt[i] = rbt[i] - tq[j][i];

      }
      // rounds are represented along x-axis
      // time quantums for each process is represented along y-axis

      else //setting time quantum and remaining burst time for remaining rounds
      {
        if (rbt[i] <= 0)
          tq[j][i] = 0;
        else if (s[i] == 1)
          tq[j][i] = 2 * tq[j - 1][i];
        else
          tq[j][i] = 1.5 * tq[j - 1][i];

        if (rbt[i] < tq[j][i])
          tq[j][i] = rbt[i];

        rbt[i] = rbt[i] - tq[j][i];
      }
    }


    j++; //j is for going to next value in a round(y-axis), i is for rounds iteration(x-axis) 

    flag = -1;
    for (int i = 0; i < n; i++)
      if (rbt[i] > 0) //breaking while loop
        flag = 0; 
  }
//***************************************************************//

  //print outputs
  cout << "\n\nProcess no.:\n";
  for (int i = 0; i < n; i++)
    cout << setw(5) << ord[i]; //setw(5) is a function which gives space of 5 in output

  cout << "\n\nBurst Times for the processes:\n";
  for (int i = 0; i < n; i++)
    cout << setw(5) << bt[i];

  cout << "\n\nIntelligent Time Slices for the processes:\n";
  for (int i = 0; i < n; i++)
    cout << setw(5) << its[i];

  cout << "\n\nDynamic Time Quantums for the processes:\n";
  for (int x = 0; x < j; x++)
  {
    cout << "Round " << x + 1 << ":" << endl;
    for (int y = 0; y < n; y++)
      cout << setw(5) << tq[x][y];
    cout << endl;
  }

  //calculation of wait time and turn around time 
  for (int x = 0; x < n; x++)
  {
    flag = -1;
    for (int y = 0; y < j; y++)
    {
      for (int z = 0; z < n; z++)
      {
        if (z != x)
          wt[x] += tq[y][z];
        else if (z == x && tq[y + 1][z] == 0)
        {
          flag = 0;
          break;
        }
      }
      tat[x] += tq[y][x];
      if (flag == 0)
        break;
    }
    tat[x] += wt[x];
  }
  //printing WT and TAT
  cout << "\nWaiting time for the processes:\n";
  for (int i = 0; i < n; i++)
    cout << setw(5) << wt[i];
  cout << "\n\nTurnaround time for the processes:\n";
  for (int i = 0; i < n; i++)
    cout << setw(5) << tat[i];
//calculate avg wait time and turn around time
  float avwt = 0, avtat = 0; 
  for (int i = 0; i < n; i++)
  {
    avwt += wt[i];
    avtat += tat[i];
  }
  avwt /= n;
  avtat /= n;
  cout << "\n\nAverage waiting time: " << avwt << endl;
  cout << "\nAverage turnaround time: " << avtat << endl;
}
