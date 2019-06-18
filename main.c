#include <pthread.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

#define MAX_THREADS 10000

void *doesNothing(void *ignored) {
  sleep(1);
  return NULL;
}

size_t getCurrentRSS() {
  long rss = 0L;
  FILE *fp = NULL;
  if ((fp = fopen("/proc/self/statm", "r")) == NULL) {
    return (size_t)0L;
  }
  if (fscanf(fp, "%*s%ld", &rss) != 1) {
    fclose(fp);
    return (size_t)0L;
  }
  fclose(fp);
  return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
}

void report(int threads) {
  const size_t rss = getCurrentRSS();
  printf("%d threads. Using %.lu B (%.1f MiB).\n", threads, rss,
         rss / 1024.0 / 1024.0);
  printf("%.1f KiB per thread.\n", rss / 1024.0 / threads);
}

int main() {
  int threads = 1;
  pthread_t threadIds[MAX_THREADS] = {};
  while (threads < MAX_THREADS) {
    if (pthread_create(threadIds + threads, NULL, doesNothing, NULL) != 0) {
      printf("Failed to create thread.\n");
      return 1;
    }
    threads++;
  }
  report(threads);
  for (int i = 1; i < MAX_THREADS; i++) {
    pthread_join(threadIds[i], NULL);
  }
  return 0;
}
