//###########################################################################################################
//
//        Disclaimer!!        Compiling should be done with a windows terminal!
//
//###########################################################################################################

/*
    Tema 10
    Echipa: Neagu Marian - 244
            Soare Cristian - 244
            Ignat Eduardo - 244

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int nr_of_jobs, sim_speed, rr_quantum, global_time = 0, idle_time = 0, alg_option = -1;
// rr_quantum: round-robin algorithm generally employs time-sharing, giving each job a time slot or "quantum"

int *ready_queue;

struct job 
{
    int arrive, start, finish, priority; //priority is only used in Priority algorithm
    int wait, turnaround;
    int burst, remain;
};

struct job* jobs;

void menu();
void exit_print();
void _print(int i);
int avg_turnaround();
double avg_wait();
float cpu_utilization();
void RoundRobin();
void Priority(); //Preemptive priority
void FCFS();
void PriorityN();
void SJF();
void SJFN();
void push_back(int new_job);
void remove_first();
void update_priority_queue();
void idle_print();
void update_job_wait();
void update_wait_ready_queue();
void update_priorityN_queue();
void update_SJF_queue();
void update_SJFN_queue();

int main()
{
    menu();
    ready_queue = (int*)malloc(nr_of_jobs * sizeof(int));
    // initialize the ready queue 

    for(int i = 0; i < nr_of_jobs; i++){
        ready_queue[i] = -1;
    }
    jobs = (struct job*)malloc(nr_of_jobs* sizeof(struct job));
    

    // generate random values for each process
    srand(time(0));
    int lower = 2, upper1 = 30, upper2 = 100, upper3 = 10;

    for(int i = 0; i < nr_of_jobs; i++){ // initialization loop
        jobs[i].arrive = (rand() % (upper1 - lower + 1)) + lower;
        jobs[i].burst = (rand() % (upper3 - lower + 1)) + lower;
        jobs[i].priority = (rand() % (upper2 - lower + 1)) + lower;
        
        jobs[i].start = 0;
        jobs[i].finish = 0;
        jobs[i].wait = 0;
        jobs[i].turnaround = 0;
        jobs[i].remain = jobs[i].burst;
    }
    jobs[0].arrive = 1;
    // first porcess will always start at time 1

    printf("These are your jobs:\n");
    printf("Process:\t Arrive\t Burst\t Priority\n\n");
    for(int i = 0; i < nr_of_jobs; i++){
        printf("%d:\t\t %d\t %d\t %d\t\t\n\n", i+1, jobs[i].arrive, jobs[i].burst, jobs[i].priority);
    }

    printf("Time:\t Job\t Remain Burst\t CPU\t Idle\t Avg Wait\t Avg Turnaround\n");
    switch (alg_option)
    {
    case 1:
        FCFS();
        break;
    case 2:
        RoundRobin();
        break;
    case 3:
        Priority();
        break;
    case 4:
        PriorityN();
        break;
    case 5:
        SJF();
        break;
    case 6:
        SJFN();
        break;
    default:
        break;
    }
    
    Sleep(sim_speed * 500);
    printf("\n\n");
    exit_print(jobs);

    printf("\n\nAvg wait: %f \n\n", avg_wait());

    free(jobs);
    return 0;
}

void menu(){
    
    printf("Disclaimer! Introduceti numai cifre de la tastatura\n\n");
    while (nr_of_jobs < 1 || nr_of_jobs > 8){
        printf("Desired number of jobs (max 8, min 1): ");
        scanf("%d", &nr_of_jobs);
        printf("\n");
    }
    while (alg_option < 1 || alg_option > 7){
        printf("Choose your algorithm option (1-3):\n1.First come first served\n2.Round-robin\n3.Priority preemtive\n4.Priority non-preemptive\n5.Shortest Job First preemtive\n6.Shortest Job First non-preemtive\nMy option: ");
        scanf("%d", &alg_option);
        printf("\n");
    }
    while(alg_option == 2 && (rr_quantum < 1 || rr_quantum > 6)){
        printf("Quantum (max 6, min 1): ");
        scanf("%d", &rr_quantum);
        printf("\n");
    }
    while(sim_speed < 1 || sim_speed > 3){
        printf("Choose your simulation speed (max 3, min 1): ");
        scanf("%d", &sim_speed);
        if(sim_speed == 3)
            sim_speed = 1;
        else if (sim_speed == 1)
            sim_speed = 3;
    }
    printf("\n");
}

void exit_print(){
    
    printf("Job:\t Arrive\t Burst\t Priority\t Start\t Finish\t Wait\t Turnaround\n\n");
    for (int i = 0; i < nr_of_jobs; i++){
        printf("%d:\t %d\t %d\t %d\t\t %d\t %d\t %d\t %d\n\n", i+1, jobs[i].arrive, jobs[i].burst, jobs[i].priority, jobs[i].start, jobs[i].finish, jobs[i].wait, jobs[i].turnaround);
    }

}

void _print(int i){
    printf("%d:\t %d\t %d\t\t %d%%\t %d\t %d\t\t %d\n", global_time, i+1, jobs[i].remain, (int)cpu_utilization(), idle_time, avg_wait(), avg_turnaround());
}

int avg_turnaround(){
    int s = 0;
    for (int i = 0; i < nr_of_jobs; i++){
        s = s + jobs[i].turnaround;
    }
    return (int)(s / nr_of_jobs);
}

double avg_wait(){
    double s = 0;
    for (int i = 0; i < nr_of_jobs; i++)
        s += jobs[i].wait;
    s = s / nr_of_jobs;
    return s;
}

float cpu_utilization()
{
    float aux = (float)(global_time - idle_time) / (float)global_time;
    return aux * 100;
}

void push_back(int new_job)
{
    int i = 0;
    while(ready_queue[i] != -1)
        i++;
    ready_queue[i] = new_job;
}

void remove_first()
{
    for(int i = 0; i < nr_of_jobs-1; i++)
        ready_queue[i] = ready_queue[i+1];
    ready_queue[nr_of_jobs-1] = -1;
}

void update_job_wait(){ 
    for (int i = 0; i < nr_of_jobs; i++){
        if (ready_queue[i] != -1){
            jobs[ready_queue[i]].wait++;
        }
    }
}

void update_priority_queue(){ //Sorting the ready_queue by priority
    for (int i = 0; i < nr_of_jobs; i++)
        if (jobs[i].arrive == global_time && jobs[i].start == 0)
            push_back(i);
            
    int aux, n = 0;
    for (int i = 0; i < nr_of_jobs; i++){
        if (ready_queue[i] != -1)
            n = i + 1;
    }

    for (int i = 0; i < n - 1; i++)
        for (int j = i+1; j < n; j++){
            if (jobs[ready_queue[j]].priority > jobs[ready_queue[i]].priority){
                aux = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = aux;
            }
            if(jobs[ready_queue[j]].priority == jobs[ready_queue[i]].priority)
                if (jobs[ready_queue[j]].arrive < jobs[ready_queue[i]].arrive){
                    aux = ready_queue[i];
                    ready_queue[i] = ready_queue[j];
                    ready_queue[j] = aux;
                }
        }
}

void update_priorityN_queue(){
    for (int i = 0; i < nr_of_jobs; i++)
        if (jobs[i].arrive == global_time && jobs[i].start == 0)
            push_back(i);
}

void update_SJF_queue(){
    for (int i = 0; i < nr_of_jobs; i++)
        if (jobs[i].arrive == global_time && jobs[i].start == 0)
            push_back(i);
            
    int aux, n = 0;
    for (int i = 0; i < nr_of_jobs; i++){
        if (ready_queue[i] != -1)
            n = i + 1;
    }

    for (int i = 0; i < n - 1; i++)
        for (int j = i+1; j < n; j++){
            if (jobs[ready_queue[j]].remain < jobs[ready_queue[i]].remain){
                aux = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = aux;
            }
            if(jobs[ready_queue[j]].remain == jobs[ready_queue[i]].remain)
                if (jobs[ready_queue[j]].arrive < jobs[ready_queue[i]].arrive){
                    aux = ready_queue[i];
                    ready_queue[i] = ready_queue[j];
                    ready_queue[j] = aux;
                }
        }
}

void update_SJFN_queue(){
    for (int i = 0; i < nr_of_jobs; i++)
        if (jobs[i].arrive == global_time && jobs[i].start == 0)
            push_back(i);
}


void RoundRobin(){
    //to do update job wait
    int current_job = 0;
    int process_time = -1; // the time passed while the current process was running
    int all_processes_done = 0; // a boolean that turns 1 if all processes has no remain burst time
    int in_idle = 0; 
    
    jobs[0].start = 1;

    while(!all_processes_done)
    {
        global_time++;   
        
        // check if there are any processes that has the arrival time equal
        // with the global time. if there is one => it goes in the queue (push_back)
        for(int i = 0; i < nr_of_jobs; i++)
            if(jobs[i].arrive == global_time)
                push_back(i);
        
       

        // when the ready queue is empty but there are still jobs with remaining burst
        // than the cpu is in idle
        if(ready_queue[0] == -1 && !all_processes_done)
        {
            printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());
            in_idle = 1;
            process_time = -1;
            idle_time++;
        }

        // when the ready queue is not empty
        while(ready_queue[0] != -1)
        {
            if (in_idle)
            {
                current_job = ready_queue[0];
                in_idle = 0;
                if(jobs[current_job].start == 0)
                    jobs[current_job].start = global_time;
            }
            process_time++;
           
            // check if we need to go the next process, by checking the current quantum value
            if(process_time == rr_quantum)
            {
                // reset the process time and remove the last process from the queue
                process_time = 0;
                remove_first();

                // if the last job has remaining burst time, than push back in the ready queue
                if(jobs[current_job].remain > 0)
                    push_back(current_job);
                else jobs[current_job].finish = global_time;

                

                // update the current job (only if there are any jobs in the ready queue)
                if(ready_queue[0] != -1)
                {
                    current_job = ready_queue[0];
                    if(jobs[current_job].start == 0)
                        jobs[current_job].start = global_time;
                }
                
            }

            // check if the current job has no remaining burst time 
            if(jobs[current_job].remain == 0)
            {
                // set the finish and turnaround values
                jobs[current_job].finish = global_time;
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].arrive;

                // remove the job from the queue
                remove_first();

                // check if this was the last job
                // if so, then we change the all_processes_done to "true", and break from the current loop
                all_processes_done = 1;
                for(int i = 0; i < nr_of_jobs; i++)
                    if(jobs[i].remain > 0)
                        all_processes_done = 0;           
                if(all_processes_done)
                    break;

                                        

                // if that was not the last job, than reset the process time and update the 
                // current job if there are any jobs in the ready queue
                process_time = 0;
                if(ready_queue[0] != -1)
                {
                    current_job = ready_queue[0];
                    if(jobs[current_job].start == 0)
                        jobs[current_job].start = global_time;
                }
                // if there are no any jobs in the ready queue, break from the current loop
                // to let it enter in the idle condition 
                else
                {
                    global_time--;
                    break;
                }
            }

            // update the remaining burst time
            jobs[current_job].remain--;

            // print the current job status
            _print(current_job);

            // delay according to the chosen simulation speed
            Sleep(sim_speed * 500);

            for(int i = 1; i < nr_of_jobs; i++)
                if(ready_queue[i] != -1)
                    jobs[ready_queue[i]].wait++;

            global_time++;

            // check if there are any processes that has the arrival time equal
            // with the global time. if there is one => it goes in the queue (push_back)
            for(int i = 0; i < nr_of_jobs; i++)
                if(jobs[i].arrive == global_time)
                    push_back(i);

            // check if this was the last job
            // if so, then we change the all_processes_done to "true", and break from the current loop
            all_processes_done = 1;
            for(int i = 0; i < nr_of_jobs; i++)
                if(jobs[i].remain > 0)
                    all_processes_done = 0;
            if(all_processes_done)
            {
                // set the finish value for the last job, then break
                jobs[current_job].finish = global_time;
                break;
            }
                
        }
        // delay according to the chosen simulation speed
        Sleep(sim_speed * 500);

        for(int i = 1; i < nr_of_jobs; i++)
                if(ready_queue[i] != -1)
                    jobs[ready_queue[i]].wait++;
    }
}

void Priority(){ //preemtive
    int ct = nr_of_jobs; //number of unfinished processes
    global_time++; 
    update_priority_queue();
    int current_job = ready_queue[0];
    
    while (ct != 0){//loop that stops when there are no more unfinished processes
        while (current_job != -1){//loop that keeps going while there is an active process in the ready_queue
            if (jobs[current_job].start == 0)
                jobs[current_job].start = global_time;
                //setting the start time for each process

            jobs[current_job].remain--;
            //decreasing the remain burst every step
            if (jobs[current_job].remain == 0){//if the current job is finished
                remove_first(current_job);
                //removing the current finished process from the ready_queue
                ct --;
                //Each time a process finishes its remain the number of unfinished processes decreases(ct)
                jobs[current_job].finish = global_time + 1;
                //setting finish time
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].arrive;
            }
            _print(current_job);
            //printing every step
            Sleep(sim_speed * 500);
            update_job_wait();
            //updating the wait time for each process from the ready queue
            if (jobs[current_job].finish == 0)
                jobs[current_job].wait--;
                //excluding the current_job
            global_time ++;
            update_priority_queue();
            //sorting the ready_queue by priority
            current_job = ready_queue[0];
            //updating the current job every step to ensure the preemptive aspect of the algorithm
        }
        
        while(current_job == -1 && ct > 0){
            //idle loop for when there is no process in the ready_queue
            printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());
            global_time ++;
            idle_time ++;
            Sleep(sim_speed * 500);
            update_priority_queue();
            current_job = ready_queue[0];
            //updating the current job every step to exit the idle loop
        }
    }
}

void FCFS()
{
    int current_job = -1,nr_of_jobs_done = 0; // first process will always be first -- line 64
    global_time = 0;
    while(nr_of_jobs_done<nr_of_jobs){ 

        global_time++;//current time

        update_job_wait();

        for(int i = 0; i < nr_of_jobs; i++){ // look for a job

            if(jobs[i].arrive == global_time){

                if(current_job == -1){
                    current_job = i;//found a job
                }
                else{
                    push_back(i);//job 'i' arrived in queue
                }
            }

        }

        if(current_job != -1){ // found a job or had a job 

            if(jobs[current_job].remain != 0){ // if the current job is not done yet
                if(jobs[current_job].remain == jobs[current_job].burst){ // mark start if it's the first step
                    jobs[current_job].start = global_time;
                    
                }
            }
            else{ // if current job is done
                nr_of_jobs_done++;
                jobs[current_job].finish = global_time;
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].start;
                if(ready_queue[0] == -1){//if ready queue is empty then there is no job to do
                    current_job = -1;
                }
                else{ // if ready queue is not empty
                    current_job = ready_queue[0];
                    remove_first();
                    if(jobs[current_job].remain == jobs[current_job].burst){ // mark start if it's the first step
                        jobs[current_job].start = global_time;
                    }
                }
            }
        }
        if(current_job == -1){// don't have a job at current time
            idle_time++;  
        }

        if(nr_of_jobs_done<nr_of_jobs){ // so that the last one doesn't show
            
            
            jobs[current_job].remain --;
            if(current_job != -1)
                _print(current_job);
            else
                printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());

            Sleep(sim_speed*500);
        }
    }
    idle_time--; //last iteration
    global_time--;
}

void PriorityN(){ //non-preemtive
    int ct = nr_of_jobs; //number of unfinished processes
    global_time++; 
    update_priorityN_queue();
    int current_job = ready_queue[0];
    
    while (ct != 0){//loop that stops when there are no more unfinished processes
        while (current_job != -1){//loop that keeps going while there is an active process in the ready_queue
            if (jobs[current_job].start == 0)
                jobs[current_job].start = global_time;
                //setting the start time for each process

            jobs[current_job].remain--;
            //decreasing the remain burst every step
            if (jobs[current_job].remain == 0){//if the current job is finished
                remove_first(current_job);
                //removing the current finished process from the ready_queue
                ct --;
                //Each time a process finishes its remain the number of unfinished processes decreases(ct)
                jobs[current_job].finish = global_time + 1;
                //setting finish time
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].arrive;
                update_priority_queue();
            }
            _print(current_job);
            //printing every step
            Sleep(sim_speed * 500);
            update_job_wait();
            //updating the wait time for each process from the ready queue
            if (jobs[current_job].finish == 0)
                jobs[current_job].wait--;
                //excluding the current_job
            global_time ++;
            update_priorityN_queue();
            //sorting the ready_queue by priority
            current_job = ready_queue[0];
            //updating the current job every step to ensure the preemptive aspect of the algorithm
        }
        
        while(current_job == -1 && ct > 0){
            //idle loop for when there is no process in the ready_queue
            printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());
            global_time ++;
            idle_time ++;
            Sleep(sim_speed * 500);
            update_priorityN_queue();
            current_job = ready_queue[0];
            //updating the current job every step to exit the idle loop
        }
    }
}

void SJF(){
    int ct = nr_of_jobs; //number of unfinished processes
    global_time++; 
    update_SJF_queue();
    int current_job = ready_queue[0];
    
    while (ct != 0){//loop that stops when there are no more unfinished processes
        while (current_job != -1){//loop that keeps going while there is an active process in the ready_queue
            if (jobs[current_job].start == 0)
                jobs[current_job].start = global_time;
                //setting the start time for each process

            jobs[current_job].remain--;
            //decreasing the remain burst every step
            if (jobs[current_job].remain == 0){//if the current job is finished
                remove_first(current_job);
                //removing the current finished process from the ready_queue
                ct --;
                //Each time a process finishes its remain the number of unfinished processes decreases(ct)
                jobs[current_job].finish = global_time + 1;
                //setting finish time
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].arrive;
            }
            _print(current_job);
            //printing every step
            Sleep(sim_speed * 500);
            update_job_wait();
            //updating the wait time for each process from the ready queue
            if (jobs[current_job].finish == 0)
                jobs[current_job].wait--;
                //excluding the current_job
            global_time ++;
            update_SJF_queue();
            //sorting the ready_queue by priority
            current_job = ready_queue[0];
            //updating the current job every step to ensure the preemptive aspect of the algorithm
        }
        
        while(current_job == -1 && ct > 0){
            //idle loop for when there is no process in the ready_queue
            printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());
            global_time ++;
            idle_time ++;
            Sleep(sim_speed * 500);
            update_SJF_queue();
            current_job = ready_queue[0];
            //updating the current job every step to exit the idle loop
        }
    }
}

void SJFN(){ //non-preemtive
    int ct = nr_of_jobs; //number of unfinished processes
    global_time++; 
    update_SJFN_queue();
    int current_job = ready_queue[0];
    
    while (ct != 0){//loop that stops when there are no more unfinished processes
        while (current_job != -1){//loop that keeps going while there is an active process in the ready_queue
            if (jobs[current_job].start == 0)
                jobs[current_job].start = global_time;
                //setting the start time for each process

            jobs[current_job].remain--;
            //decreasing the remain burst every step
            if (jobs[current_job].remain == 0){//if the current job is finished
                remove_first(current_job);
                //removing the current finished process from the ready_queue
                ct --;
                //Each time a process finishes its remain the number of unfinished processes decreases(ct)
                jobs[current_job].finish = global_time + 1;
                //setting finish time
                jobs[current_job].turnaround = jobs[current_job].finish - jobs[current_job].arrive;
                update_SJF_queue();
            }
            _print(current_job);
            //printing every step
            Sleep(sim_speed * 500);
            update_job_wait();
            //updating the wait time for each process from the ready queue
            if (jobs[current_job].finish == 0)
                jobs[current_job].wait--;
                //excluding the current_job
            global_time ++;
            update_SJFN_queue();
            //sorting the ready_queue by priority
            current_job = ready_queue[0];
            //updating the current job every step to ensure the preemptive aspect of the algorithm
        }
        
        while(current_job == -1 && ct > 0){
            //idle loop for when there is no process in the ready_queue
            printf("%d: idle..\t\t\t %d%%\n", global_time,(int)cpu_utilization());
            global_time ++;
            idle_time ++;
            Sleep(sim_speed * 500);
            update_SJFN_queue();
            current_job = ready_queue[0];
            //updating the current job every step to exit the idle loop
        }
    }
}
