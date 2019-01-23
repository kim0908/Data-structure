#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//read
#include <ev.h>


//linklist//
struct timelist{
	int hour;
	int min;
	int repeat;
	
	struct timelist *next;
};
typedef struct timelist list; //struct timelist=list
list *head = NULL;
list *last = NULL; //ckeck if it is ok to use


void insert (list *pre,list *current){
	list *new;
	new=(list*)malloc(sizeof(list));
	if(new==NULL){
		printf("no memory");
		return ;
	}

	new->hour=current->hour;
	new->min=current->min;
	new->repeat=current->repeat;
	new->next=pre->next;
	pre->next=new;
}


//sort
//push
list* push(list* train,list* head){
    list* current;
    current=(list*)malloc(sizeof(list));
    if(current==NULL)
        return NULL;
    current->hour = head->hour;
    current->min = head->min;
    current->repeat = head->repeat;
    current->next=train;
    return current;
}
//find where to insert new list
list* sortlist(list* train,list* head){
    list* current, *pre=NULL;
    current=train;
    for(;(current!=NULL)&&(current->hour < head->hour);){    
        pre=current;
        current=current->next;}
    for(;(current!=NULL)&&(current->hour < head->hour)&&(current->min <= head->min);){ 
        pre=current;
        current=current->next;}
    for(;(current!=NULL)&&(current->hour == head->hour)&&(current->min <= head->min);){ 
        pre=current;
        current=current->next;}
    if(pre==NULL){
        train=push(train,head);}
    else
        insert(pre,head);
    return train;
}




//call back
time_t nowtime;
struct tm *tm_info;
void watch(int,int);//watch time


void control_cb(struct ev_loop *loop, struct ev_io *w, int revents);
void nowtime_cb(struct ev_loop *loop, struct ev_io *w, int revents){
    // get time from 1970/01/01
    time(&nowtime);
    // convert time_t to struct time
    tm_info = localtime(&nowtime);//turn the time to local time
}    



void watch(int hour,int min){
    
     if(hour==tm_info->tm_hour&&min==tm_info->tm_min)
        printf("Alarm ringing, time is %d:%d\n",tm_info->tm_hour,tm_info->tm_min); 
       
    return;
}



void checktime_cb(struct ev_loop *loop, struct ev_timer *w, int revents){
	list *current=NULL;
	for(current=head;current!=NULL;current=current->next){
        
                 watch(current->hour,current->min);
	}
    
}

void inserttime_cb(struct ev_loop *loop, struct ev_io *w, int revents){
    char enter[30];
    read(w->fd, enter, sizeof(enter));
    
	if((strcmp(enter,"exit")-10)==0){
	printf("Enter what do you want to do?\n");
	printf("Enter 'settime' to set the alarm\n");
	printf("Enter 'events' to do another things\n");
	 ev_io_stop(loop, w);
     ev_io_init(w, control_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
 }else{
 	list *current=(list*)malloc(sizeof(list));
 	if(current==NULL){
 		printf("no memory\n");
 		return;
 	}

        current->next = NULL;
        char* hour_tok;
        char* min_tok;
        hour_tok = strtok(enter,":");
        current->hour = atoi(hour_tok);
        hour_tok = strtok(NULL,":");
        min_tok = strtok(hour_tok,"->");
        current->min = atoi(min_tok);
        min_tok = strtok(NULL,"->");
        current->repeat = atoi(min_tok); //read need touse it
 if(head!=NULL){
 	insert(last,current);
 	last=last->next;
 	free(current);
 }else{
 	head=current;
 	last=head;
 }
}
}


void updatetime_cb(struct ev_loop *loop, struct ev_io *w, int revents){

  char enter[30];
    read(w->fd, enter, sizeof(enter));
    
    if((strcmp(enter,"exit")-10)==0){
    printf("Enter what do you want to do?\n");
    printf("Enter 'settime' to set the alarm\n");
    printf("Enter 'events' to do another things\n");
     ev_io_stop(loop, w);
     ev_io_init(w, control_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
 }else{
    list *current=(list*)malloc(sizeof(list));
    list*find=(list*)malloc(sizeof(list));
    if(current==NULL&&find==NULL){
        printf("no memory\n");
        return;
    }
  
     
        char* hour_tok;
        char* min_tok;
        hour_tok = strtok(enter,":");
        current->hour = atoi(hour_tok);
        hour_tok = strtok(NULL,":");
        min_tok = strtok(hour_tok,"->");
        current->min = atoi(min_tok);
        min_tok = strtok(NULL,"->");
        current->repeat = atoi(min_tok); //read need to use it


     find=head;
 if(head!=NULL&&head->next!=NULL){
  
    head=head->next;
    if(head->hour==current->hour&&head->min==current->min&&head->repeat==current->repeat){
    
    find->next=head->next;
    
    free(current);
    free(head);
    }   
}
find=head;
}//delete
printf("Enter hour and minute and repeat(1) or not repeat(2)\n" );
printf("For exmple , you want insert 10:05 and repeat enter 10:05->1 \n");
printf("Enter 'exit' will go back\n");
     ev_io_stop(loop, w);
     ev_io_init(w, inserttime_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 

}




void deletetime_cb(struct ev_loop *loop, struct ev_io *w, int revents){
   char enter[30];
    read(w->fd, enter, sizeof(enter));
    
    if((strcmp(enter,"exit")-10)==0){
    printf("Enter what do you want to do?\n");
    printf("Enter 'settime' to set the alarm\n");
    printf("Enter 'events' to do another things\n");
     ev_io_stop(loop, w);
     ev_io_init(w, control_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
 }else{
    list *current=(list*)malloc(sizeof(list));
    list*find=(list*)malloc(sizeof(list));
    if(current==NULL&&find==NULL){
        printf("no memory\n");
        return;
    }
        char* hour_tok;
        char* min_tok;
        hour_tok = strtok(enter,":");
        current->hour = atoi(hour_tok);
        hour_tok = strtok(NULL,":");
        min_tok = strtok(hour_tok,"->");
        current->min = atoi(min_tok);
        min_tok = strtok(NULL,"->");
        current->repeat = atoi(min_tok); //read need touse it


     find=head;
  
if(find->hour==current->hour&&find->min==current->min&&find->repeat==current->repeat){
    head=head->next;
    free(current);
    free(find);
}
  

   if(head!=NULL&&head->next!=NULL){
    head=head->next;
    if(head->hour==current->hour&&head->min==current->min&&head->repeat==current->repeat){
    
    find->next=head->next;
    
    free(current);
    free(head);
    } 
head=find; 
}

}
}


void events_cb(struct ev_loop *loop, struct ev_io *w, int revents){
    char enter[20];   
    read(w->fd, enter, sizeof(enter));

	if((strcmp(enter,"insert")-10)==0){
    printf("Enter hour and minute and repeat(1) or not repeat(2)\n" );
    printf("For exmple , you want insert 10:05 and repeat enter 10:05->1 \n");
    printf("Enter 'exit' will go back\n");
	 ev_io_stop(loop, w);
     ev_io_init(w, inserttime_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
	}

	else if((strcmp(enter,"delete")-10)==0){
    printf("which alarm you want to delete it?(enter the alarm ex:10;05->1)(if you want exit ,enter exit) \n");
    printf("if you want exit ,enter exit \n");
     ev_io_stop(loop, w);
     ev_io_init(w, deletetime_cb, w->fd, EV_READ);
     ev_io_start(loop, w);
    }

	else if((strcmp(enter,"dump")-10)==0){
	 list *train=NULL, *current=NULL;
	 current=head;
	 int count=1;
	 while(current != NULL){
            train = sortlist(train,current);
            current = current->next;
        }
        head = train;
        for(current=head;current!=NULL;current=current->next){
            switch(current->repeat){
                case 1:
                    printf("alarm %d :time is %d:%d,it need repeat\n", count, current->hour, current->min);
                    break;
                case 2:
                    printf("alarm %d :time is %d:%d, it doesn't need to repeat\n", count, current->hour, current->min);
                    break;
            }
            count += 1;
            if(current != NULL){
                last = current;    // insert new list became the last pointer
            }
        }

}

else if((strcmp(enter,"update")-10)==0){
	printf("which alarm you want to update it?(enter the alarm ex:10;05->1) \n");
    
     ev_io_stop(loop, w);
     ev_io_init(w, updatetime_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
}

else if((strcmp(enter,"exit")-10)==0){
	printf("Enter what do you want to do?\n");
	printf("Enter 'settime' to set the alarm\n");
	printf("Enter 'events' to do another things\n");
	 ev_io_stop(loop, w);
     ev_io_init(w, control_cb, w->fd, EV_READ);
     ev_io_start(loop, w); 
 }else {
 	printf("Error\n");
 	return;
 }
}

void control_cb(struct ev_loop *loop, struct ev_io *w, int revents){
    
	char enter[30];   
    read(w->fd, enter, sizeof(enter));
	if((strcmp(enter,"settime")-10) == 0){  
	printf("Enter hour and minute and repeat(1) or not repeat(2)\n" );
    printf("For exmple , you want insert 10:05 and repeat enter 10:05->1 \n");
    printf("Enter 'exit' will go back\n" ); 
        ev_io_stop(loop, w);
        ev_io_init(w, inserttime_cb, w->fd, EV_READ);
        ev_io_start(loop, w); 
    }else if((strcmp(enter,"events")-10) == 0){
    	printf("Enter 'insert' set new alarm\n");
        printf("Enter 'delete' delete  alarm\n");
        printf("Enter 'dump'   display alarm\n");
        printf("Enter 'update' update alarm\n");
        printf("Enter 'exit'   go back\n");
    	ev_io_stop(loop, w);
        ev_io_init(w, events_cb, w->fd, EV_READ);
        ev_io_start(loop, w);
    }else{
    printf("Error\n");
 	return;
    }
}

int main(){
    struct ev_loop *loop = ev_default_loop(0);//initial
    ev_timer checktime_watcher;
    ev_timer nowtime_watcher;
    ev_io io_watcher;
    printf("Enter what do you want to do?\n");
	printf("Enter 'settime' to set the alarm\n");
	printf("Enter 'events' to do another things\n");
    
    ev_io_init(&io_watcher, control_cb, 0, EV_READ); //initia io watcher + event will be control
    ev_io_start(loop, &io_watcher);// insert watcher to event loop
    
    
    ev_timer_init(&nowtime_watcher, nowtime_cb, 0, 2);//  event occur immediately + get now time every 2 sec
    ev_timer_start(loop, &nowtime_watcher);
   

    ev_timer_init(&checktime_watcher, checktime_cb, 2, 1);// event occur after 2 sec, and check alarm every 1 sec.
    ev_timer_start(loop, &checktime_watcher);
   
    ev_run(loop, 0); //start event loop
    return 0;
}
