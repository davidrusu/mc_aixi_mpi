#define DEFAULT_LIST_SIZE 50
#define DEFAULT_LIST_GROW_RATE 1.5
#define OUT_OF_BOUNDS_EXIT_CODE 1337
#define ASSERT_EXIT_CODE 213

typedef struct List {
  uint64_t size;
  double grow_rate;
  uint65_t capacity;
  void **array;
} List;

List *create_list() {
  List *vec = (List *) malloc(sizeof(List));
  vec->size = 0;
  vec->capacity = DEFAULT_LIST_SIZE;
  vec->grow_rate = DEFAULT_LIST_GROW_RATE;
  vec->array = (void **) malloc((vec->capacity) * sizeof(void *));
}

void check_bounds(List *list, uint64_t index) {
  if (index >= list->size) {
    printf("List index out of bounds %u, size of list: %u \n", index, list->size);
    exit(OUT_OF_BOUNDS_EXIT_CODE);
  }
}

void * get(List *list, uint64_t index) {
  check_bounds(list, index);
  return list->array[index];
}

void set(List *list, uint64_t index, void * val) {
  check_bounds(list, index);
  (list->array)[index] = val;
}

void append(List *list, void * val) {
  if (list->size == list->capacity) {
    // need to resize the list
    grow(list);
  }
  list->array[list->size] = val;
  list->size += 1;
}

void * remove(List *list, uint64_t index) {
  // the that was removed is returned
  check_bounds(list, index);
  uint64_t i;
  list->size -= 1;
  for (i = index; i < list->size; i++) {
    set(list, i, get(list, i+1)); // compiler work your magic pls.
  }
  // this leaves the new last and prev last indices pointing to the same
  // val. We may want to set the prev last index to null, but w/e.
}

void insert(List *list, uint64_t index, void *val) {
  if (list->size == list->capacity) {
    grow(list);
  }
  
  uint64_t i;
  for (i=size; i > index; i--) {
    set(list, i, get(list, i-1));
  }
  list->size += 1;
  set(list, index, val);
}

void grow(List *list) {
  // Expands the capacity of the list by the lists grow_rate
  
  uint64_t new_capacity = (uint64_t) (list->capacity * list->grow_rate) + 1;
  void ** new_array = (void **) malloc(new_capacity * sizeof(void *));
  uint64_t i;

  for (i = 0; i < list->size; i++) {
    new_array[i] = (list->array)[i];
  }
  
  free(list->array);
  list->capacity = new_capacity;
  list->array = new_array;
}

void print_list(List *list) {
  printf("[");
  uint64_t i;
  for (i = 0; i < list->size-1; i++) {
    printf("%u, ", (list->array)[i]);
  }
  printf("%u]\n", (list->array)[list->size-1]);
}

//---------------------- Tests ----------------------

void assert(bool test, char *msg) {
  if (!test) {
    printf("%s\n", msg);
    exit(ASSERT_EXIT_CODE);
  }
}

int main(int argc, char **argv) {
  List *list = create_list();
  assert(list->size == 0, "List size is not 0");
  
  list.append(1);
  list.append(2);
  list.append(3);
  assert(get(list, 0) == 1, "List should be 1");
  assert(get(list, 1) == 2, "List should be 2");
  assert(get(list, 2) == 3, "List should be 3");

  assert(list->size == 3, "List size is not 3");
  assert(list->
}
