/**
 * @file
 *
 * This file contains the code for the RenameIdentifiers traversal.
 * The traversal has the uid: CI
 *
 * @brief This module implements a demo traversal of the abstract syntax tree
 * that prefixes any variable found by two underscores.
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav_data.h"
#include "palm/hash_table.h"
#include "palm/memory.h"
#include "palm/str.h"
#include "user_types.h"
#include <stdio.h>

void CIinit() {
  struct data_ci *data = DATA_CI_GET();
  data->table = HTnew_String(100);
  return;
}

node_st *CIprogram(node_st *node) {
  struct data_ci *data = DATA_CI_GET();

  TRAVchildren(node);

  printf("Identifier counts:\n");
  for (htable_iter_st *iter = HTiterate(data->table); iter;
       iter = HTiterateNext(iter)) {
    char *key = (char *)HTiterKey(iter);
    int *value = (int *)HTiterValue(iter);
    printf("  %s: %d\n", key, *value);
  }

  return node;
}

void CIfini() {
  struct data_ci *data = DATA_CI_GET();

  for (htable_iter_st *iter = HTiterate(data->table); iter;
       iter = HTiterateNext(iter)) {
    char *key = (char *)HTiterKey(iter);
    int *value = (int *)HTiterValue(iter);
    MEMfree(key);
    MEMfree(value);
  }

  HTdelete(data->table);
  return;
}

static void increment_name_counter(node_st *node) {
  struct data_ci *data = DATA_CI_GET();
  char *lookup_name = VARS_NAME(node);

  htable_stptr table = data->table;
  void *result = HTlookup(table, lookup_name);

  if (result == NULL) { // First time seeing this name, create a counter
    char *owned_name = STRcpy(lookup_name);
    int *count = MEMmalloc(sizeof(int));
    *count = 1;
    HTinsert(table, owned_name, count);
  } else { // Just incrmemnt;
    int *count = (int *)result;
    (*count)++;
  }
}

/**
 * @fn CIvarlet
 */
node_st *CIvarlet(node_st *node) {
  increment_name_counter(node);
  return node;
}

/**
 * @fn CIvar
 */
node_st *CIvar(node_st *node) {
  increment_name_counter(node);
  return node;
}
