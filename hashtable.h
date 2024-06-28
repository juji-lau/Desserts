#define HASH_SIZE 1000
#define MAX_NAME 300
#define MAX_LINK 400
#define MAX_LINE_SIZE 1000
#define MAX_RECIPES 500

#ifndef RECIPE_H
#define RECIPE_H

// Recipe name : [ingredients:"xxx", instructions:"xxx", notes : "...", reviews: "...", link: "..."]
struct Recipe
{
  char name[MAX_NAME];
  char ingredients[MAX_LINE_SIZE];
  char instructions[4 * MAX_LINE_SIZE];
  char notes[MAX_LINE_SIZE];
  char reviews[MAX_LINE_SIZE];
  char link[MAX_LINK];
  struct Recipe *next;
};

struct Recipe *create_recipe(char *rn, char *ing, char *insn, char *notes, char *rev, char *links);

#endif // RECIPE_H

#ifndef BUCKET_H
#define BUCKET_H
struct Bucket
{
  int recipe_count;
  struct Recipe *next;
};
#endif // BUCKET_H

struct Name
{
  char recipe_name[MAX_NAME];
  struct Name *next;
  int index;                   // associated index
  // struct Recipe *recipe;    // associated recipe
};

#ifndef TABLE_H
#define TABLE_H
struct Table
{
  size_t num_recipes;
  struct Bucket bucket_list[HASH_SIZE];
  // Array of char pointers
  struct Name* first_recipe_name;
};

struct Table *create_table();

// Hash function
int hash_function(const char *str);

#endif // TABLE_H

// Insert a recipe into the table.
int insert_recipe(struct Table *table, struct Recipe *recipe);

// Retrieve a recipe, and return 0 if successful, else retun -1
int retrieve_recipe(struct Table *table, char *recipe_name, struct Recipe *returned_recipe);

// Populate the indices of each name;
void populate_indices(struct Table *table);

// Free the table
void free_table(struct Table *table);
