// envi - env interpreter
//
// alternative to env
//
// use multiple bash scripts
// to prepare the environment
// to execute a script
//
// this allows nested wrapping
// by adding more .env*.sh files
//
// when the user calls
// /usr/bin/test.py
// then the first env file is
// /usr/bin/.test.py.env0.sh
//
// a list of *all* remaining env files is passed to .env0.sh in $@
// if there is *no* next env file, then $1 is --
// the next env file after .env0.sh is .env1.sh
//
// it is the job of each env file (.env0.sh .env1.sh .env2.sh .env3.sh ...)
// to either:
// * load the next env file
// or:
// * exec the script with the provided arguments
//
// (c) 2022 Milan Hauth -> MIT license



#include <libgen.h> // dirname basename
#include <stdio.h> // printf
#include <string.h> // strdup strlen
#include <assert.h>
#include <stdlib.h> // malloc
#include <unistd.h> // execv access



// dynamic array of strings
// https://stackoverflow.com/a/3536261/10440128

typedef struct {
  char **array;
  size_t used;
  size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(char *));
  a->array[0] = (char *) NULL;
  a->used = 0;
  assert(initialSize > 0);
  a->size = initialSize;
}

void pushArray(Array *a, char *value) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size
  if (a->used == a->size - 1) {
    // grow the array
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(char *));
  }
  a->array[a->used++] = value;
  a->array[a->used + 1] = (char *) NULL;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}



int main(int argc, char **argv) {

char *script;
char *dir;
char *base;
char *env;
int envSize;
int i;
int envId;

char *bash = "/nix/store/d60gkg5dkw4y5kc055n4m0xyvcjz65im-bash-interactive-5.1-p16/bin/bash";

printf("envi: argv:\n");
for (i = 0; i < argc; i++) {
  printf("envi: $%i = %s\n", i, argv[i]);
}

//assert(argc == 2); // no. script args land in argv

script = argv[1];
// TODO realpath
// use absolute paths for script and env's so we can chdir

base = strdup(script);
base = basename(base);

dir = strdup(script);
dir = dirname(dir);

printf("envi: script = %s\n", script);
printf("envi: dir = %s\n", dir);
printf("envi: base = %s\n", base);

// +2 from ./ (to make sure)
// +1 from .
// +10 from .env1234.sh
// +1 for \0
envSize = strlen(script) +2 +1 +10 +1;
printf("envi: envSize = %i\n", envSize);


//char **args;
Array *args;
args = malloc(sizeof(Array));

initArray(args, 10);

pushArray(args, bash);
pushArray(args, "-e"); // bash should throw on error

// limitation: maximum 10K env files
for (envId = 0; envId < 9999; envId++) {
  env = malloc(envSize);
  assert(envSize > snprintf(env, envSize, "%s/.%s.env%i.sh", dir, base, envId)); // throws if env is too small
  if(access(env, F_OK) == 0) {
    // file exists
    //printf("env[%i] = %s\n", envId, env);
    pushArray(args, env);
  } else {
    break;
  }
}

pushArray(args, "--"); // end of env list

// copy argv
// note: argv[0] is envi -> start at argv[1] == script
// argv[2] is the first argument for script
for (i = 1; i < argc; i++) {
  pushArray(args, argv[i]);
}

printf("envi: args:\n");
for (i = 0; i < args->used; i++) {
  printf("envi: $%i = %s\n", i, args->array[i]);
}

printf("envi: exec bash\n");

execv(args->array[0], args->array);

// no need to free at end of program?
/*
// free strings?
for (i = 0; i < argc; i++) {
  printf("argv[%i] = %s\n", i, argv[i]);
  pushArray(args, argv[i]);
}

freeArray(args);
*/

printf("envi: exec bash done\n"); // never reached?

}
