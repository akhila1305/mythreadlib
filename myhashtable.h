//hash table routines for tasklets

#ifndef _MHASH_H
#define _MHASH_H


#define MHASH_TABLE_SIZE  ( 1 << 12) 

#define GET_MHASH_INDEX(pid)   ( ( (pid) ^ MAX_PID) & (MHASH_TABLE_SIZE - 1) )


extern struct task_struct *task_hash_table[MHASH_TABLE_SIZE];

/*Link the task struct into the hash table */

static __inline__ void task_hash_add(struct task_struct *task) {
  struct task_struct **hash_table = &task_hash_table[GET_MHASH_INDEX(task->pid)]; //get the index into the table
  
  if( (task->h_next = *hash_table) ) {
    (*hash_table)->h_prev = &task->h_next; 
  }
  *hash_table = task; //write the task into the hash index
  task->h_prev = hash_table;
  return ;
}

/*Unhash an entry */

static __inline__ void task_hash_del(struct task_struct *task) {

  if(task->h_next) {
    task->h_next->h_prev = task->h_prev; //reset the next guys prev. entry
  }
  *task->h_prev  = task->h_next ; //write out the next entry in the previous guys address
  return ;
}

/*Find an entry by pid */

static __inline__ struct task_struct *task_hash_find(int pid) {
  struct task_struct *head = task_hash_table[GET_MHASH_INDEX(pid)];
  struct task_struct *ptr;
  for(ptr = head; ptr && ptr->pid != pid; ptr = ptr->h_next) ;
  return ptr;  
}

#endif
