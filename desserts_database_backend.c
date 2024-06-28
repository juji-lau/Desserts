
// To run:
//gcc -o out desserts_database_backend.c database_helper.c hashtable.c
// ./out
// git clone
// access token: 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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "database_helper.h"
#include "hashtable.h"

const char *file_name = "test5.txt";
const char *page_break = "**********\n";
const char *ingredients_tag = "Ingredients:\n";
const char *instructions_tag = "Instructions:\n";
const char *tips_tag = "Notes/Tips:\n";
const char *reviews_tag = "Reviews:\n";
const char *links_tag = "Link(s):\n";

struct Table *global_table;
struct Recipe *global_recipe;

#define MAX_LINE_SIZE 1000

int print_recipe(struct Recipe *recipe);
int search_by_number(struct Table *this_table, int query, struct Recipe *this_recipe);

// read recipe into the database
void read_recipe(FILE *fp, struct Table *this_table)
{
  // malloc all the fields in a recipe
  char *tmp;
  char *out = malloc(MAX_LINE_SIZE * sizeof(char));
  char *name = malloc(MAX_NAME * sizeof(char));
  name[0] = '\0';
  char *ing = malloc(MAX_LINE_SIZE * sizeof(char));
  ing[0] = '\0';
  char *insn = malloc(4 * MAX_LINE_SIZE * sizeof(char));
  insn[0] = '\0';
  char *tips = malloc(MAX_LINE_SIZE * sizeof(char));
  tips[0] = '\0';
  char *revs = malloc(MAX_LINE_SIZE * sizeof(char));
  revs[0] = '\0';
  char *links = malloc(MAX_LINK * sizeof(char));
  links[0] = '\0';
  tmp = fgets(out, MAX_LINE_SIZE, fp);
  // Make the name lowercased
  toLowerCase(out);
  memcpy(name, out, strlen(out) + 1);
  while ((tmp != NULL) && (!strstr(out, page_break)))
  {
    if (strstr(out, ingredients_tag))
    {
      while (((tmp = fgets(out, MAX_LINE_SIZE, fp)) != NULL) && (!strstr(out, instructions_tag)))
      {
        // read in the ingredients
        strcat(ing, out);
        strcat(ing, "\n");
      }
    }
    else if (strstr(out, instructions_tag))
    {
      while (((tmp = fgets(out, MAX_LINE_SIZE, fp)) != NULL) && (!strstr(out, tips_tag)))
      {
        // read in the instructions
        strcat(insn, out);
        strcat(insn, "\n");
      }
    }
    else if (strstr(out, tips_tag))
    {
      while (((tmp = fgets(out, MAX_LINE_SIZE, fp)) != NULL) && (!strstr(out, reviews_tag)))
      {
        // read in notes/tips
        strcat(tips, out);
        strcat(tips, "\n");
      }
    }
    else if (strstr(out, reviews_tag))
    {
      while (((tmp = fgets(out, MAX_LINE_SIZE, fp)) != NULL) && (!strstr(out, links)))
      {
        // read in the reviews
        strcat(revs, out);
        strcat(revs, "\n");
      }
    }
    else if (strstr(out, links_tag))
    {
      while (((tmp = fgets(out, MAX_LINK, fp)) != NULL) && (!strstr(out, page_break)))
      {
        // read in the links
        strcat(links, out);
        strcat(links, "\n");
      }
    }
    else if (strstr(out, page_break))
    {
    }
    else
    {
      tmp = fgets(out, MAX_LINE_SIZE, fp);
    }
  }
  // Create the recipe object
  struct Recipe *this_recipe_ptr = create_recipe(name, ing, insn, tips, revs, links);
  // free all temporarily used variables
  free(name);
  free(ing);
  free(insn);
  free(tips);
  free(revs);
  free(links);
  free(out);
  // Insert the recipe into the table
  if (insert_recipe(this_table, this_recipe_ptr) != 0)
  {
    fprintf(stderr, "Failed to insert the recipe: %s into the table\n", name);
  }
}

// Reads the text file into the database
void read_to_database(const char *filename, struct Table *this_table)
{
  FILE *fp;
  fp = fopen(filename, "r");
  char *out = malloc(MAX_LINE_SIZE * sizeof(char));
  while (fgets(out, MAX_LINE_SIZE, fp) != NULL)
  {
    // New Recipe
    if (strstr(out, page_break))
    {
      read_recipe(fp, this_table);
    }
  }
  fclose(fp);
  free(out);
}

// Prompt user input and return request, in lowercase.
int return_request(int max_input, char *user_input)
{
  printf("To quit, enter <Q>.\n");
  printf("For searching by number, enter <A>.\n");
  printf("For searching by name, enter <B>\n");
  fgets(user_input, max_input, stdin);
  toLowerCase(user_input);
  if (strcmp(user_input, "a\n") == 0)
  {
    printf("Enter a number please\n");
    fgets(user_input, max_input, stdin);
    printf("You've entered: %s\n", user_input);
    return 1;
  }
  else if (strcmp(user_input, "b\n") == 0)
  {
    printf("Enter a recipe name (< %d characters please!):  ", max_input);
    fgets(user_input, max_input, stdin);
    printf("You've entered: %s\n", user_input);
    toLowerCase(user_input);
    return 2;
  }
  else if (strcmp(user_input, "q\n") == 0)
  {
    return -1;
  }
  else
  {
    printf("Bro, at least enter something valid :/\n");
    return 0;
  }
  return 0;
}

// print all the recipe names
int print_all_recipe_names(struct Table *this_table)
{
  struct Name *temp;
  temp = this_table->first_recipe_name;
  while (temp != NULL)
  {
    printf("%d.  %s\n", temp->index, temp->recipe_name);
    temp = temp->next;
  }
  return 0;
}

// Allows searching by number
int search_by_number(struct Table *this_table, int query, struct Recipe *this_recipe)
{
  if (query < 1 || query > this_table->num_recipes)
  {
    fprintf(stderr, "Invalid number entered, try again \n");
    return 0;
  }
  struct Name *temp;
  temp = this_table->first_recipe_name;
  while (temp != NULL)
  {
    if (temp->index == query)
    {
      // printf("Found query!!\n");
      retrieve_recipe(this_table, temp->recipe_name, this_recipe);
      return 0;
    }
    temp = temp->next;
  }
  fprintf(stderr, "Went thru the while, but try again. \n");
  return -1;
}

// print the recipe
int print_recipe(struct Recipe *recipe)
{
  printf("Recipe Name: %s \n", recipe->name);
  printf("Ingredients: %s \n", recipe->ingredients);
  printf("Instructions: %s \n", recipe->instructions);
  printf("Notes: %s \n", recipe->notes);
  printf("Reviews: %s \n", recipe->reviews);
  printf("Links: %s \n", recipe->link);
  return 0;
}

int main()
{
  // Read the recipe into the database and create the table
  global_table = create_table();
  read_to_database(file_name, global_table);
  populate_indices(global_table);
  printf("You have %lu recipes!\n", global_table->num_recipes);
  // Read in user's requests unless the user wants to quit
  int max_input = 200;
  int user_quit = 0;
  while (!user_quit)
  {
    char *user_input = malloc(max_input * sizeof(char));
    global_recipe = malloc(sizeof(struct Recipe));
    printf("******************MENU******************\n\n");
    if (print_all_recipe_names(global_table) != 0)
    {
      fprintf(stderr, "Error: Could not print all recipe names\n");
    }
    // allow users to search by number or name
    int choice = return_request(max_input, user_input);
    if (choice == -1)
    {
      // User quits
      free(user_input);
      free(global_recipe);
      fprintf(stderr, "Quitting... \n");
      break;
    }
    else if (choice == 1)
    {
      // Search by number
      if (search_by_number(global_table, atoi(user_input), global_recipe) != 0)
      {
        fprintf(stderr, "Searching by number failed \n");
      }
      else
      {
        print_recipe(global_recipe);
      }
    }
    else if (choice == 2)
    {
      // Search by name
      if (retrieve_recipe(global_table, user_input, global_recipe) != 0)
      {
        fprintf(stderr, "Failed to return the correct recipe \n");
      }
      else
      {
        print_recipe(global_recipe);
      }
    }
    else
    {
      // invalid entry.
    }
    // Free everything
    free(user_input);
    free(global_recipe);
  }
  free_table(global_table);
  return 0;
}

// int *a;
// *a = 5;
// printf("%d", a); // value of a, the int ptr
// printf("\n");
// printf("%d", &a); // address of the int ptr
// printf("\n");
// printf("%d", *a); // content that a points to should be 5
// printf("\n HELLO \n");
// int *b;
// printf("%d", b);  // value of b, the int ptr (empty space in memory)
// printf("\n");
// printf("%d", &b);  // address of the int ptr
// printf("\n");
// printf("%d", *b);   // content that b points to (garbage)
// b=a;
// printf("\n");
// printf("\n");
// printf("%d", b);  // value of b, int ptr, should be a
// printf("\n");
// printf("%d", &b); // address of b, int ptr, should NOT be equal to a
// printf("\n");
// printf("%d", *b);   // what b points to, should be 5
