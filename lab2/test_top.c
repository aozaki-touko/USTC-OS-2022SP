#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sched.h>
void sort(int *pid,int count ,long long *time);
int find(int pid,int count,int*pid_old);
void cp_comm(int *pid,char*comm,int count,char*pro_name);
int main(){
    int count=0;
    int first=1;
    int i=0;
    int pid[400]={};
    char name[400*16]={};
    long state[10000];
    long long runtime[400]={};
    long long old_runtime[10000]={0};
    long long one_sec_runtime[10000]={0};
    char pro_name[10000*16];
    while(1){
        if(first){   
            first=0;
            syscall(332,name,pid,&count,state,runtime);
            cp_comm(pid,name,count,pro_name);
            for(i=0;i<count;i++){
                old_runtime[pid[i]]=runtime[i];
                one_sec_runtime[pid[i]]=runtime[i];
            }
            printf("NAME                PID   RUNNING  CPU\n");
            sort(pid,count,one_sec_runtime);
            for(i=0;i<20;i++){
                printf("%s    %d    %ld        %f\n",pro_name+16*pid[i],pid[i],state[pid[i]],((float)(one_sec_runtime[pid[i]]))/1000000);
            }
        }else{
            system("clear");
            syscall(332,name,pid,&count,state,runtime);
            cp_comm(pid,name,count,pro_name);
            printf("NAME                PID   RUNNING  CPU\n");
            for(i=0;i<count;i++){
                one_sec_runtime[pid[i]]=runtime[i]-old_runtime[pid[i]];
            }
            for(i=0;i<count;i++){
                old_runtime[pid[i]]=runtime[i];
            }
            sort(pid,count,one_sec_runtime);
            for(i=0;i<20;i++){
                printf("%s    %d    %ld        %f\n",pro_name+16*pid[i],pid[i],state[pid[i]],((float)(one_sec_runtime[pid[i]]))/1000000);
            }
            sleep(1);
        }


    }
        return 0;
}
void sort(int *pid,int count ,long long  *time){
    int i=0,j=0;
    int temp;
    for(i=0;i<count;i++){
        for(j=0;j<count-i-1;j++){
            if(time[pid[j]]<time[pid[j+1]]){
                temp=pid[j];
                pid[j]=pid[j+1];
                pid[j+1]=temp;
            }
        }
    }
}
int find(int pid,int count,int*pid_old){
    int i=0;
    for(i=0;i<count;i++){
        if(pid_old[i]==pid){
            return i;
        }
    }
    return -1;
}
void cp_comm(int *pid,char*comm,int count,char *pro_name){
    int i=0,j=0;
    for(i=0;i<count;i++){
        for(j=0;j<16;j++){
            pro_name[16*(pid[i])+j]=comm[16*i+j];
        }
    }
}