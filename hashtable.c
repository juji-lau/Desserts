#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hashtable.h"
#include "database_helper.h"


// Initialize a hash table, set the recipe count to zero, and set all bucket pointers to NULL.
// Return the pointer
// when mallocking, do you malloc the sizeof(variable),and it takes care of itself, or ...
struct Table *create_table()
{
  //struct Table *table = malloc(sizeof(size_t) + HASH_SIZE * sizeof(struct Bucket));
  struct Table *table = malloc(sizeof(struct Table));
  table->num_recipes = 0;
  for (int i = 0; i < HASH_SIZE; i++)
  {
    table->bucket_list[i].recipe_count = 0;
    table->bucket_list[i].next = NULL;
  }
  // malloc space for the recipe names
  table->first_recipe_name = malloc(sizeof(struct Name));
  table->first_recipe_name->next = NULL;
  // no need to free the table bc it's static after creation
  return table;
}
// Create a recipe, return a pointer
struct Recipe *create_recipe(char *rn, char *ing, char *insn, char *notes, char *rev, char *links)
{
  struct Recipe *this_recipe = malloc(sizeof(struct Recipe));
  memcpy(this_recipe->name, rn, MAX_NAME);
  memcpy(this_recipe->ingredients, ing, MAX_LINE_SIZE);
  memcpy(this_recipe->instructions, insn, (4*MAX_LINE_SIZE));
  memcpy(this_recipe->notes, notes, MAX_LINE_SIZE);
  memcpy(this_recipe->reviews, rev, MAX_LINE_SIZE);
  memcpy(this_recipe->link, links, MAX_LINK);
  this_recipe->next=NULL;
  return this_recipe;
}

int hash_function(const char *str)
{

  int str2int = string2summed_int(str);
  // print_ascii(str);
  // printf("HASHED: %s", str);
  // printf("to %d\n", str2int % HASH_SIZE);
  return str2int % HASH_SIZE;
}

int insert_recipe(struct Table *table, struct Recipe *recipe)
{

  char *name = recipe->name;
  // toLowerCase(name);
  int ind = hash_function(name);
  // We have recipes in this bucket
  if (table->bucket_list[ind].recipe_count > 0)
  {
    // Make this recipe point to the former head of recipes
    recipe->next = table->bucket_list[ind].next;
  }
  // Add it to the list of names
  if(table->num_recipes == 0){
    memcpy(table->first_recipe_name->recipe_name, name, MAX_NAME);
  } else {
    struct Name *new_recipe_name = malloc(sizeof(struct Name));
    new_recipe_name->next = table->first_recipe_name;
    memcpy(new_recipe_name->recipe_name, name, MAX_NAME);
    // set indices
    new_recipe_name-> index = 0;
    table->first_recipe_name = new_recipe_name;
  }
  table->bucket_list[ind].next = recipe;
  table->bucket_list[ind].recipe_count++;
  table->num_recipes++;
  return 0;
}

void populate_indices(struct Table *table){
  struct Name *temp;
  temp = table->first_recipe_name;
  int counter = 1;
  while(temp != NULL){
    temp->index = counter;
    temp = temp->next;
    counter++;
  }
}

int retrieve_recipe(struct Table *table, char *recipe_name, struct Recipe *returned_recipe)
{
  int recipe_ind = hash_function(recipe_name);
  // printf("LOOKINF FOR %s:", recipe_name);
  // If the recipe does not exist
  int recipe_count = table->bucket_list[recipe_ind].recipe_count;
  if (recipe_count == 0)
  {
    returned_recipe = NULL;
    fprintf(stderr, "Check your spelling! \n");
    // bucket has zero recipes; caused by incorrect string matching
    return -1;
  }
  // How to malloc for a recipe?
  // struct Recipe *curr_recipe = malloc(sizeof(struct Recipe));
  struct Recipe curr_recipe;
  curr_recipe = *(table->bucket_list[recipe_ind].next);
  // If you match, or if you don't have a next one, get out.
  while ((strcmp(curr_recipe.name, recipe_name) != 0) && (curr_recipe.next != NULL))
  {
    curr_recipe = *curr_recipe.next;
  }
  // if you got out and you match, return it.
  if (strcmp(curr_recipe.name, recipe_name) == 0)
  {
    memcpy(returned_recipe, &curr_recipe, sizeof(struct Recipe));
    return 0;
  }
  // Else return -1
  returned_recipe = NULL;
  fprintf(stderr, "The recipe is not in the database \n");
  return -1;
}

void free_table(struct Table *table)
{  
   // Free linkedlist in each bucket
   struct Recipe *this_recipe;
   struct Recipe *next;
   struct Bucket *this_bucket;
   for(int i = 0; i < HASH_SIZE; i++)
      {
	this_bucket = &(table->bucket_list[i]);
	// Free the linkedlist of recipe pointers that this bucket holds
	this_recipe = this_bucket->next;
	while(this_recipe != NULL)
	{
	  next = this_recipe->next;
	  free(this_recipe);
	  this_recipe = next;
	  
	}
	// Free the bucket even tho I didn't malloc?? (I'm guessing no need...)
	//free(this_bucket);
      }
   // Free linkedlist of recipe names
   struct Name *this_name;
   struct Name *next_name;
   this_name = table->first_recipe_name;
   while(this_name != NULL)
   {
     next_name = this_name->next;
     free(this_name);
     this_name = next_name;
   }
   free(table);
}
