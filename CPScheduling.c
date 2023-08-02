#include <stdio.h>
#include <stdlib.h>
struct process
{
    char name[10];
    int burst_time;
    int priority;
};
int i, j;
void fifo(struct process *p, int num)
{
    int total_wait = 0,
        current_time = 0,
        wait_time = 0,
        turn_around = 0,
        total_turn_around = 0;
    printf("process\tBurst time\tWait Time\tTurn Around Time\n");
    for (i = 0; i < num; i++)
    {
        wait_time = current_time;
        turn_around = p[i].burst_time + wait_time;

        printf("%s\t", p[i].name);
        printf("%d\t\t", p[i].burst_time);
        printf("%d\t\t\t", wait_time);
        printf("%d", turn_around);
        printf("\n");

        total_wait += wait_time;
        total_turn_around += turn_around;
        current_time += p[i].burst_time;
    }
    float avg_wait_time = (float)total_wait / num;
    float avg_turn_around_time = (float)turn_around / num;
    printf("\nAverage Wait Time:%0.3f", avg_wait_time);
    printf("\nAverage Turn Around Time:%0.3f", avg_turn_around_time);
}
void sjf(struct process *p, int num)
{
    for (i = 0; i < num; i++)
    {
        for (j = 0; j < num - i - 1; j++)
        {
            if (p[j].burst_time > p[j + 1].burst_time)
            {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    fifo(p, num);
}
void priority(struct process *p, int num)
{
    for (i = 0; i < num; i++)
    {
        for (j = 0; j < num - i - 1; j++)
        {
            if (p[j].priority > p[j + 1].priority)
            {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    fifo(p, num);
}
void roundrobin(struct process *p, int num, int time_quantum)
{
    int total_waiting_time = 0,
        total_turnaround_time = 0,
        current_time = 0,
        waiting_time = 0,
        turnaround_time = 0;
    int remtime[num];
    for (i = 0; i < num; i++)
        remtime[i] = -1;

    int wait[num];
    int turn[num];
    printf("name\tburst time\twait time\tturnaround time\n");
    for (i = 0; i < num; i++)
        remtime[i] = p[i].burst_time;

    int complete;
    while (1)
    {
        complete = 1;
        for (i = 0; i < num; i++)
        {
            if (remtime[i] > 0)
            {
                complete = 0;
                if (remtime[i] > time_quantum)
                {
                    current_time += time_quantum;
                    remtime[i] -= time_quantum;
                }
                else
                {
                    current_time += remtime[i];
                    remtime[i] = 0;
                    waiting_time = current_time - p[i].burst_time;
                    turnaround_time = waiting_time + p[i].burst_time;

                    total_waiting_time += waiting_time;
                    total_turnaround_time += turnaround_time;

                    wait[i] = waiting_time;
                    turn[i] = turnaround_time;
                }
            }
        }
        if (complete)
            break;
    }
    for (i = 0; i < num; i++)
    {
        printf("%s\t", p[i].name);
        printf("%d\t\t\t", p[i].burst_time);
        printf("%d\t\t", wait[i]);
        printf("%d\n", turn[i]);
    }
    float avg_waiting_time = (float)total_waiting_time / num;
    float avg_turnaround_time = (float)total_turnaround_time / num;
    printf("\nAverage Waiting Time: %f", avg_waiting_time);
    printf("\nAverage Turnaround Time: %f", avg_turnaround_time);
}
void restore(struct process *p, struct process *original, int num)
{
    for (i = 0; i < num; i++)
        p[i] = original[i];
}
int main()
{
    int num = 0, choice = 0;
    printf("\nEnter no of process:");
    scanf("%d", &num);

    struct process *p = (struct process *)malloc(num * sizeof(struct process));
    struct process *original = (struct process *)malloc(num * sizeof(struct process));
    printf("\nEnter process name & bursttime:\n");
    for (i = 0; i < num; i++)
    {
        scanf(" %s", &p[i].name);
        scanf("%d", &p[i].burst_time);
        p[i].priority = 0;
    }

    for (i = 0; i < num; i++)
        original[i] = p[i];

    printf("|\tProcess\t|\tBurst\t|\n");
    for (i = 0; i < num; i++)
    {
        printf("|\t%s\t|\t%d\t|\n", p[i].name, p[i].burst_time);
    }
    while (choice != 5)
    {
        printf("\n1.FIFO\n2.SJF\n3.Priority\n4.RoundRobin\n5.Exit\nEnter Choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
        {
            fifo(p, num);
            restore(p, original, num);
            break;
        }
        case 2:
        {
            sjf(p, num);
            restore(p, original, num);
            break;
        }
        case 3:
        {
            printf("\nEnter priority\n");
            for (i = 0; i < num; i++)
            {
                printf("\nEnter Priority of Process %d:", i + 1);
                scanf("%d", &p[i].priority);
            }
            priority(p, num);
            restore(p, original, num);
            break;
        }
        case 4:
        {
            printf("\nEnter Time Quantum:");
            int time_quantum;
            scanf("%d", &time_quantum);
            roundrobin(p, num, time_quantum);
            restore(p, original, num);
            break;
        }
        case 5:
        {
            printf("\nEXIT\n");
            free(p);
            free(original);
            break;
        }
        default:
            printf("\nENTER VALID CHOICE!!!\n");
        }
    }
    return 0;
}