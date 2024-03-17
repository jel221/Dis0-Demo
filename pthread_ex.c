#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t main_tid;
pthread_t child_tid;

void* helper2(void* a) {
  pthread_join(child_tid, NULL);
  sleep(1);
  printf("This one will not get to run in Project 2.\n");
  return NULL;
}

void* helper(void* a) {
  child_tid = pthread_self();
  pthread_join(main_tid, NULL);
  pthread_t t;
  pthread_create(&t, NULL, &helper2, NULL);
  printf("This one should always run.\n");
  pthread_exit(NULL);
}

int main() {
  main_tid = pthread_self();
  pthread_t t;
  pthread_create(&t, NULL, &helper, NULL);
  printf("Main thread exiting.\n");
  pthread_exit(NULL);
}