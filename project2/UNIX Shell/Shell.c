#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024
int in,in_store,out,out_store;
int i=0,p_wait_flag=0;
void ParseCommand(char * command, char ** args);
int main(void)
{
	int should_run=1,pipe_store[2],his_count=0; 
	int state=0,state1=0;
	char *args[MAX_LINE/2+1],*arg_L[MAX_LINE/2+1],*arg_R[MAX_LINE/2+1];
	char command[MAX_LINE],com_parse[MAX_LINE];
	char his_com[MAX_LINE];
	
	while(should_run)
	{
		printf("osh>");
		memset(command,0,MAX_LINE); //初始化command
		fgets(command,MAX_LINE,stdin);
		fflush(stdout);
		
		if(strcmp(command,"\n")==0) 	
			{continue;}
				
		strcpy(com_parse,command);
		
		p_wait_flag=0;
		ParseCommand(com_parse, args); //解析命令
		//printf("wait_flag and %d \n",p_wait_flag);

		if(strcmp(args[0],"!!")==0)  //将最近的命令放入args
		{
			if(his_count<=0){printf("No commands in history5\n");continue;}
			else
			{
				printf("%s\n",his_com);
				
				p_wait_flag=0;
				strcpy(com_parse,his_com);
				strcpy(command,his_com);
				ParseCommand(com_parse, args);
			}
		}
		strcpy(his_com,command); 
		his_count++;
		
		//printf("args: %s %s %s\n",args[0],args[1],args[2]);

		int in_flag=0,out_flag=0;
		for(i=0;args[i]!=NULL;i++) //重定向
		{
			if(strcmp(args[i],"<")==0)
			{
				
					in = open(args[i+1],O_RDONLY);
					if(in>=0){in_store=dup(0);dup2(in,0);close(in);}
					else {perror("open");}
					in_flag=1;
					break;
			}

			if(strcmp(args[i],">")==0)
			{
				out=open(args[i+1],O_WRONLY | O_TRUNC | O_CREAT , S_IRUSR | S_IWUSR);
				if(out>=0){out_store=dup(1);dup2(out,1);close(out);}
				else{perror("open");}
				out_flag=1;
				break;
			}			
		}
		//printf("inflag %d and outflag %d \n",in_flag,out_flag);
		int pipe_flag=0;
		for(i=0;args[i]!=NULL;i++)//将pipe的两个命令分别保存
		{
			if(strcmp(args[i],"|")==0 && args[i+1]!=NULL)
			{
				int j;
				for(j=0;j<i;j++)
					arg_L[j]=args[j];
				arg_L[j]=NULL;
				
				for(j=i+1;args[j]!=NULL;j++)
					arg_R[j-i-1]=args[j];
				arg_R[j-i-1]=NULL;
				pipe_flag=1;
				break;
			}
		}
		
		//printf("pipeflag %d and command: %s and %s \n",pipe_flag,arg_L[0],arg_R[0]);
		
		if(strcmp(args[0],"exit")==0)
			{should_run=0;continue;}

		pid_t pid = fork();  //执行命令
		if(pid<0) {perror("fork");return 0;}
		else if(pid==0)
		{
			if(in_flag==1||out_flag==1)
			{
				char *tmp[MAX_LINE/2+1];
				for(i=0;strcmp(args[i],"<")!=0&&strcmp(args[i],">")!=0;i++)
					tmp[i]=args[i];
				if(execvp(tmp[0],tmp)<0){printf("Valid Command!4\n");exit(0);}	
			}
			else if(pipe_flag==1)
			{
				pid_t pid1;
				pipe(pipe_store);
				pid1=fork();
				if(pid1<0){perror("fork");}
				if(pid1==0)
				{
					out_store=dup(1);
					dup2(pipe_store[1],1);
					close(pipe_store[0]);
					if(execvp(arg_L[0],arg_L)<0){printf("Valid Command!3\n");exit(0);}
				}
				else if(pid1>0)
				{
					wait(NULL);
					in_store=dup(0);
					dup2(pipe_store[0],0);
					close(pipe_store[1]);
					if(execvp(arg_R[0],arg_R)<0){printf("Valid Command!2\n");exit(0);}
				}
				
			}
			else
				{if(execvp(args[0],args)<0){printf("Valid Command1!\n");exit(0);}}
			exit(0);
		}
		else
		{	if(p_wait_flag==1)wait(NULL);	}

		dup2(out_store,1);
		dup2(in_store,0);
		
	}
	return 0;
}

void ParseCommand(char * command, char ** args)
{
    int args_count =0;                  
    int command_len = strlen(command);  
    int arg_i = -1;                     
    int prev_command = 0;
    int history = 0;
    

    for(int i =0; i < command_len; i++)
    {                     

        switch(command[i])
        {
            case ' ':
            case '\t': 
            if(arg_i != -1)           
            {
                args[args_count] = &command[arg_i];
                args_count++;
            }
            command[i] = '\0';         
            arg_i = -1;   
            break;

            case '\n':
            if(arg_i != -1)           
            {
                args[args_count] = &command[arg_i];
                args_count++;
            }
            command[i] = '\0';   
            break;

            default:                     

            if(arg_i == -1)
              arg_i = i;
            if(command[i] == '&')
            {
                p_wait_flag = 0;    
                command[i] = '\0'; 
                i++;
            }
            break;
                           

        }    
    }  
    
args[args_count] = NULL;

}


