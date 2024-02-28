#include <stdio.h>
#include <stdlib.h>
int zero = 0; // gobal variable to make loop always be true

typedef struct proc // process structure to simulate processes for a CPU typedef to make it easier to call
{
    int id;
    int cpuTime;
    int period;
    int deadline;
    int timeLeft;
    int isPrem;
    int overallID;
} proccess;

int gcd(int a, int b) // greatest common denominator
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int lcm(proccess arr[], int n) // least common multiple but made for doing it on a process array
{
    int lcm = arr[0].period;
    for (int i = 1; i < n; i++)
    {
        lcm = (arr[i].period * lcm) / gcd(arr[i].period, lcm);
    }
    return lcm;
}
int compare(const void *a, const void *b) // Compare 2 deadlines if deadlines are equal choose the process that was created first
{
    if ((*(proccess *)a).deadline < (*(proccess *)b).deadline)
    {
        return -1;
    }
    else if ((*(proccess *)a).deadline > (*(proccess *)b).deadline)
    {
        return 1;
    }
    else
    {
        if ((*(proccess *)a).deadline == (*(proccess *)b).deadline)
        {
            if ((*(proccess *)a).overallID < (*(proccess *)b).overallID)
            {
                return -1;
            }
            else if ((*(proccess *)a).overallID > (*(proccess *)b).overallID)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
}
int compareID(const void *a, const void *b) // compare overall ID
{
    if ((*(proccess *)a).overallID < (*(proccess *)b).overallID)
    {
        return -1;
    }
    else if ((*(proccess *)a).overallID > (*(proccess *)b).overallID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int compareID2(const void *a, const void *b) // compare normal ID if the IDs are the same choose the one with less time left
{
    if ((*(proccess *)a).id < (*(proccess *)b).id)
    {
        return -1;
    }
    else if ((*(proccess *)a).id > (*(proccess *)b).id)
    {
        return 1;
    }
    else
    {
        if ((*(proccess *)a).timeLeft < (*(proccess *)b).timeLeft)
        {
            return -1;
        }
        else if ((*(proccess *)a).timeLeft > (*(proccess *)b).timeLeft)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int main()
{
    int num_of_Processes; // number of process we create
    while (zero == 0)     // while loop to use scanf to choose the number of processes to create must be a positive number
    {
        printf("Enter the number of processes to schedule: ");
        scanf("%d", &num_of_Processes);
        if (num_of_Processes > 0)
        {
            break;
        }
    }
    proccess *pros = (proccess *)malloc(num_of_Processes * sizeof(proccess)); // dynamically create first array of processes
    for (int i = 0; i < num_of_Processes; i++)//For loop to initalize the CPUTime and Period for each process. Both must be positive and CPU time must be less than period
    {
        while (zero == 0)
        {
            while (zero == 0)
            {
                printf("Enter the CPU time of process %d: ", i + 1);
                scanf("%d", &pros[i].cpuTime);
                if (pros[i].cpuTime > 0)
                {
                    break;
                }
            }
            while (zero == 0)
            {
                printf("Enter the period of process %d: ", i + 1);
                scanf("%d", &pros[i].period);
                if (pros[i].period > 0)
                {
                    break;
                }
            }
            if (pros[i].period >= pros[i].cpuTime)
            {
                break;
            }
        }
        pros[i].deadline = pros[i].period; //initalize the other fields of the structure
        pros[i].timeLeft = pros[i].cpuTime;
        pros[i].id = i + 1;
        pros[i].isPrem = 0;
        pros[i].overallID = i;
    }
    int lcmValue = lcm(pros, num_of_Processes);//get the lcm of all the periods
    int waitingTime = 0; //initalize the waiting time var
    int prosCreated = num_of_Processes; //initalize the number of processes created to current number of processes
    int queSize = num_of_Processes; //Variable to control the size of the queue
    int ids = num_of_Processes; //Variable to determine the overall ID
    int lastPrintID = -1; //Last print ID to prevent printouts twice
    proccess *queue = (proccess *)malloc(num_of_Processes * sizeof(proccess)); //create the queue currently to the size of the 
    proccess *missed = NULL; //get the missed array
    proccess temp; //temporary process
    for (int i = 0; i < num_of_Processes; i++) //copy the process array into the queue
    {
        queue[i] = pros[i];
    }
    printf("0: processes:");
    for (int i = 0; i < num_of_Processes; i++) //first printout of processes unsorted
    {
        printf(" %d (%d ms)", queue[i].id, queue[i].timeLeft);
    }
    printf("\n");
    qsort(queue, num_of_Processes, sizeof(proccess), compare); //sort the process by EDF
    int misses;//get number of misses
    for (int i = 0; i < lcmValue; i++) //For loop for the time
    {
        if (i != 0) //if the time isnt 0 run this
        {
            for (int j = 1; j < queSize; j++) //go through the queue and add to the waiting time except the current process
            {
                waitingTime++;
            }
            if (queue != NULL) //If the queue isnt empty 
            {
                queue[0].timeLeft--; //decrease time left of running process by 1 
                if (queue[0].timeLeft == 0) //If the current process is finished say it finished then realloc the size of the array to n-1 everything shifted left by 1
                {
                    printf("%d: process %d ends\n", i, queue[0].id);
                    for (int j = 0; j < queSize - 1; j++)
                    {
                        queue[j] = queue[j + 1];
                    }
                    queSize--;
                    queue = (proccess *)realloc(queue, queSize * sizeof(proccess));
                }
            }
            misses = 0; //reset misses to 0
            for (int j = queSize - 1; j >= 0; j--) //go through the queue if there is missed deadlines put them into a missed array increase deadline to next period
            {
                if (queue[j].deadline <= i)
                {
                    misses++;
                    missed = (proccess *)realloc(missed, misses * sizeof(proccess));
                    queue[j].deadline += queue[j].period;
                    missed[misses - 1] = queue[j];
                }
            }
            if (missed != NULL) //if the missed proccess array isnt empty print them out then reallocate back to a size of 0
            {
                qsort(missed, misses, sizeof(proccess), compareID2);
                for (int j = 0; j < misses; j++)
                {
                    printf("%d: process %d missed deadline (%d ms left)\n", i, missed[j].id, missed[j].timeLeft);
                }
                missed = (proccess *)realloc(missed, 0);
            }
            for (int j = 0; j < num_of_Processes; j++) //go through the original processes if we are a possible deadline (time mod period = 0) then add a new one to the queue
            {
                if ((i % pros[j].period) == 0)
                {
                    queSize++;
                    queue = (proccess *)realloc(queue, queSize * sizeof(proccess));
                    prosCreated++;
                    queue[queSize - 1] = pros[j];
                    queue[queSize - 1].deadline = i + pros[j].period;
                    queue[queSize - 1].overallID = ids;
                    ids++;
                }
            }
            for (int j = 0; j < num_of_Processes; j++) //Print out the current proccess in process whenever a new process is added put into tempArray for printing purposes
            {
                if ((i % pros[j].period) == 0)
                {
                    printf("%d: processes:", i);
                    proccess *tempArr = (proccess *)malloc(queSize * sizeof(proccess));
                    for (int k = 0; k < queSize; k++)
                    {
                        tempArr[k] = queue[k];
                    }
                    qsort(tempArr, queSize, sizeof(proccess), compareID);
                    for (int k = 0; k < queSize; k++)
                    {
                        printf(" %d (%d ms)", tempArr[k].id, tempArr[k].timeLeft);
                    }
                    printf("\n");
                    free(tempArr);
                    break;
                }
            }
            if (queue != NULL)//if the queue isnt null run this
            {
                if (queue[0].isPrem == 1 && queue[0].timeLeft != queue[0].cpuTime && queue[0].overallID != lastPrintID) //if the process has been preemted, wasnt the last print, or is at max time print the start
                {
                    queue[0].isPrem = 0;
                    printf("%d: process %d starts\n", i, queue[0].id);
                    lastPrintID = queue[0].overallID;
                }
                queue[0].isPrem = 1;
                temp = queue[0];
                qsort(queue, queSize, sizeof(proccess), compare); //we sort the queue 
                if (temp.overallID == queue[0].overallID) //if the running process hasnt changed we set the prem var back to 0
                {
                    queue[0].isPrem = 0;
                }
                else
                {
                    if (temp.timeLeft != temp.cpuTime) //if there was a prem and the current time isnt the max we say it was prem'd
                    {
                        printf("%d: process %d preempted!\n", i, temp.id);
                        if (queue[0].timeLeft != queue[0].cpuTime && queue[0].overallID != lastPrintID) //if the new process doesnt have the max time and wasnt the last print we say it starts
                        {
                            printf("%d: process %d starts\n", i, queue[0].id);
                            lastPrintID = queue[0].overallID;
                        }
                    }
                    else
                    {
                        if (queue[0].timeLeft != queue[0].cpuTime && queue[0].isPrem == 1 && queue[0].overallID != lastPrintID) //if there was a prem and the current time is the max we say it starts
                        {
                            printf("%d: process %d starts\n", i, queue[0].id);
                            lastPrintID = queue[0].overallID;
                            queue[0].isPrem = 0;
                        }
                    }
                }
            }
        }
        if (queue != NULL) //otherwise we check if the current process has the max time if so we print out that it starts
        {
            if (queue[0].overallID != lastPrintID)
            {
                if (queue[0].timeLeft == queue[0].cpuTime)
                {
                    printf("%d: process %d starts\n", i, queue[0].id);
                    lastPrintID = queue[0].overallID;
                }
            }
        }
    }
    if (queue != NULL) //we check if the queue is empty when the for loop finishes if it isnt empty we still make sure to remove one last time tick from the current process
    {
        queue[0].timeLeft--;
        if (queue[0].timeLeft == 0) //if it finished in the last time decrease we print out it ends
        {
            printf("%d: process %d ends\n", lcmValue, queue[0].id);
        }
    }
    for (int j = 1; j < queSize; j++)
    {
        waitingTime++;
    }
    printf("%d: Max Time reached\n", lcmValue); //print out all the stuff
    printf("Sum of all waiting times: %d\n", waitingTime);
    printf("Number of processes created: %d\n", prosCreated);
    printf("Average Waiting Time: %.2lf\n", (double)waitingTime / (double)prosCreated);
    free(pros); //FREE
    free(queue);
    exit(EXIT_SUCCESS);
}