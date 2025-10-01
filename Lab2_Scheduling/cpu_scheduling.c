#include <stdio.h>
#include <string.h>

#define MAXN 100
#define MAXSEG 2000

typedef struct {
    int pid;
    int at;
    int bt;
} P;

int seg_label[MAXSEG];     /* -1 = IDLE, else pid (1..n) */
int seg_start[MAXSEG];
int seg_end[MAXSEG];
int seg_cnt = 0;

void push_seg(int label, int start, int end){
    if(start==end) return;
    seg_label[seg_cnt] = label;
    seg_start[seg_cnt] = start;
    seg_end[seg_cnt]   = end;
    seg_cnt++;}

void reset_segs(){ seg_cnt = 0; }

void print_gantt(){
    for(int i=0;i<seg_cnt;i++){
        if(seg_label[i]==-1) printf("| IDLE ");
        else printf("| P%d ", seg_label[i]);
    }
    printf("|\n");
    printf("%-3d", seg_start[0]);
    for(int i=0;i<seg_cnt;i++) printf("%-4d", seg_end[i]);
    printf("\n");}

void print_table(int n, int at[], int bt[], int ct[]){
    int tat[MAXN], wt[MAXN];
    double sum_tat=0, sum_wt=0;
    for(int i=0;i<n;i++){
        tat[i] = ct[i] - at[i];
        wt[i]  = tat[i] - bt[i];
    }
    printf("\n%-8s%6s%6s%6s%6s%6s\n","Process","AT","BT","CT","TAT","WT");
    for(int i=0;i<n;i++){
        printf("P%-7d%6d%6d%6d%6d%6d\n", i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
        sum_tat += tat[i];
        sum_wt  += wt[i];
    }
    printf("\nAverage TAT = %.2f\n", sum_tat/n);
    printf("Average WT  = %.2f\n", sum_wt/n);}

void fcfs(int n, P a[], int out_ct[]){
    P p[MAXN];
    for(int i=0;i<n;i++) p[i]=a[i];
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(p[j].at < p[i].at || (p[j].at==p[i].at && p[j].pid < p[i].pid)){
                P t=p[i]; p[i]=p[j]; p[j]=t;
            }
    reset_segs();
    int time=0;
    for(int i=0;i<n;i++){
        if(time < p[i].at){ push_seg(-1, time, p[i].at); time=p[i].at; }
        push_seg(p[i].pid, time, time+p[i].bt);
        time += p[i].bt;
        out_ct[p[i].pid-1] = time;
    }
    printf("\n--- FCFS Result ---\n");}

void sjf_np(int n, P a[], int out_ct[]){
    int done[MAXN]={0};
    int completed=0, time=0;
    reset_segs();
    while(completed<n){
        int best=-1;
        for(int i=0;i<n;i++)
            if(!done[i] && a[i].at<=time)
                if(best==-1 || a[i].bt<a[best].bt ||
                   (a[i].bt==a[best].bt && a[i].at<a[best].at) ||
                   (a[i].bt==a[best].bt && a[i].at==a[best].at && a[i].pid<a[best].pid))
                    best=i;
        if(best==-1){
            int next=1e9;
            for(int i=0;i<n;i++) if(!done[i] && a[i].at<next) next=a[i].at;
            push_seg(-1, time, next);
            time=next;
            continue;
        }
        push_seg(a[best].pid, time, time+a[best].bt);
        time += a[best].bt;
        out_ct[a[best].pid-1]=time;
        done[best]=1;
        completed++;
    }
    printf("\n--- SJF (non-preemptive) Result ---\n");}

void rr(int n, P a[], int q, int out_ct[]){
    int rem[MAXN], order[MAXN];
    for(int i=0;i<n;i++){ rem[i]=a[i].bt; order[i]=i; }
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(a[order[j]].at < a[order[i]].at ||
               (a[order[j]].at==a[order[i]].at && a[order[j]].pid < a[order[i]].pid)){
                int t=order[i]; order[i]=order[j]; order[j]=t;
            }
    int qbuf[MAXN*4], head=0, tail=0;
    int time=0, ip=0, finished=0;
    reset_segs();
    while(ip<n && a[order[ip]].at<=time) qbuf[tail++]=order[ip++];
    while(finished<n){
        if(head==tail){
            int next=a[order[ip]].at;
            if(time<next){ push_seg(-1, time, next); time=next; }
            while(ip<n && a[order[ip]].at<=time) qbuf[tail++]=order[ip++];
            continue;
        }
        int i=qbuf[head++], run = rem[i]<q?rem[i]:q;
        push_seg(a[i].pid, time, time+run);
        time += run;
        rem[i]-=run;
        while(ip<n && a[order[ip]].at<=time) qbuf[tail++]=order[ip++];
        if(rem[i]==0){ out_ct[a[i].pid-1]=time; finished++; }
        else qbuf[tail++]=i;
    }
    printf("\n--- Round Robin (q=%d) Result ---\n", q);}

int main(){
    int n, choice, q;
    int at[MAXN], bt[MAXN], ct[MAXN];
    P a[MAXN];

    printf("=== CPU Scheduling (C) ===\n");
    printf("How many processes? ");
    if(scanf("%d",&n)!=1 || n<=0 || n>MAXN){ printf("Bad input\n"); return 0; }

    printf("Enter %d Arrival Times (space-separated): ", n);
    for(int i=0;i<n;i++) scanf("%d",&at[i]);
    printf("Enter %d Burst Times   (space-separated): ", n);
    for(int i=0;i<n;i++) scanf("%d",&bt[i]);

    for(int i=0;i<n;i++){ a[i].pid=i+1; a[i].at=at[i]; a[i].bt=bt[i]; }

    printf("\nChoose algorithm:\n 1) FCFS\n 2) SJF (non-preemptive)\n 3) Round Robin\nYour choice (1/2/3): ");
    if(scanf("%d",&choice)!=1){ printf("Bad input\n"); return 0; }

    if(choice==1){
        fcfs(n,a,ct);
        print_table(n,at,bt,ct);
        printf("\nGantt Chart:\n");
        print_gantt();
    }else if(choice==2){
        sjf_np(n,a,ct);
        print_table(n,at,bt,ct);
        printf("\nGantt Chart:\n");
        print_gantt();
    }else if(choice==3){
        printf("Time quantum: ");
        if(scanf("%d",&q)!=1 || q<=0){ printf("Bad quantum\n"); return 0; }
        rr(n,a,q,ct);
        print_table(n,at,bt,ct);
        printf("\nGantt Chart:\n");
        print_gantt();
    }else{
        printf("Invalid choice\n");
    }
    return 0;}