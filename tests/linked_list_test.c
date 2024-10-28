#include<stdbool.h>
#include<stdio.h>
#include "../allocator/allocator.h"

typedef struct Node{
int val;
struct Node * next;
}Node_t;

Node_t* gen_list(size_t n){

Node_t * head=(Node_t *)mid_alloc(sizeof(Node_t));
head->val=1;
Node_t*cur=head;
for(size_t i=2;i<=n;i++)
{
    cur->next=(Node_t*)mid_alloc(sizeof(Node_t));
    cur=cur->next;
    cur->val=i;
}
cur->next=NULL;
return head;
}

bool linked_list_test(){
Node_t * head=gen_list(10);
Node_t * cur=head;
int i=1;
while(cur)
{
if(cur->val!=i)
return false;
i++;
cur=cur->next;
}
if(i!=11)
return false;
return true;
}