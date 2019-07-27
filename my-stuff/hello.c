#include <stdio.h>
#include "libyang.h"

struct ly_ctx *ctx = NULL;
struct lyd_node *root_a = NULL;
struct lyd_node *root_b = NULL;
struct lyd_node *node = NULL;
struct lyd_difflist *diff;

int main()
{
   // printf() displays the string inside quotation
   printf("Hello, World!\n");
   char* yang_folder = NULL;
   yang_folder = "yang3";

   printf("Yang Location: %s", yang_folder);
   printf("\n");
   ctx = ly_ctx_new(yang_folder, 0);
   ly_ctx_load_module(ctx, "minimal-integrationtest", NULL);
   /* The python library does initialisation */


   root_a = lyd_new_path(NULL, ctx, "/minimal-integrationtest:types/str1", "A", 0, 1);
   lyd_new_path(root_a, NULL, "/minimal-integrationtest:types/str2", "B", 0, 1);
   lyd_new_path(root_a, NULL, "/minimal-integrationtest:types/int_8", "55", 0, 1);


   const char *str;
   struct ly_set *my_set = NULL;
   struct lyd_node *my_node = NULL;
   struct lyd_node_leaf_list my_leaf;

   my_set = lyd_find_path(root_a, "/minimal-integrationtest:types/*");
   printf("We hvae a set \n");
   if(!my_set){
     printf("-set is null\n");
   }else{
     if(my_set->number==0){
       printf("The set came back but has no results\n");
     }
     for (int i = my_set->number - 1; i > -1; --i) {
       my_node = my_set->set.d[i];
       if(!my_node){
         printf("my node is null %d\n", i);
       }else{
          printf("We have a node %d\n", i);
          str = my_set->set.d[i]->schema->name;
          printf("Node Name ... is pickable frmo schema: %s \n", str);
     //
     //     str = my_set->set.d[0]->attr->value_str;  <-- this Is a SEGAULT! becuase we need to cast
          if(my_set->set.d[i]->schema->nodetype & (LYS_LEAF | LYS_LEAFLIST)){
            printf("This is a leaf or a leaf list\n");
            my_node = my_set->set.d[i];
            //if(my_node){
              my_leaf = *(struct lyd_node_leaf_list *)my_node;
              str = my_leaf.value_str;
              printf("We have a value string..... %s\n", str);
            //}
          }else{
            printf(".... we are not a leaf or leaf list");
          }
      }
    }
   }
 //      // if (set== NULL){
 //      //   printf("lyd_find_path was null");
 //      // }
 //      // printf("found an attribute from lyd_find_path\n");
 //      // struct lys_type *type = NULL;
 //      // type = lyd_leaf_type((struct lyd_node_leaf_list *)set);
 //      //
 //      //


   /*  The python library does cleanup */


     FILE *f;
     f = fopen("/tmp/libyang.json", "w");
     lyd_print_file(f, root_a, LYD_JSON,LYP_WITHSIBLINGS);
     fclose(f);

    lyd_free_withsiblings(root_a);
   ly_ctx_destroy(ctx, NULL);

//
//

//    Documentation here:
//
//    https://netopeer.liberouter.org/doc/libyang/master/group__datatree.html#ga470c3225f3f10666971723f8f9977a1a
//
//    */
//    root_a = lyd_new_path(NULL, ctx, "/minimal-integrationtest:simpleleaf","a", 0, 1);
//    //root_a = lyd_new_path(NULL, ctx, "/integrationtest:simpleenum","A", 0, 0);
//    // lyd_new_path(root_a, NULL, "/integrationtest:simplelist[simplekey='A']/simplekey","A" , 0, 0);
//    // lyd_new_path(root_a, NULL, "/integrationtest:simplelist[simplekey='B']/simplekey","B" , 0, 0);
//    // lyd_new_path(root_a, NULL, "/integrationtest:simplelist[simplekey='B']/nonleafkey", "4444" , 0, 0);
//    //
//    // lyd_new_path(root_a, NULL, "/integrationtest:simplelist[simplekey='C']/simplekey","C" , 0, 0);
//    // lyd_new_path(root_a, NULL, "/integrationtest:simplelist[simplekey='D']/simplekey","D" , 0, 0);
//    //lyd_new_path(root_a, NULL, "/integrationtest:simpleenum","A", 0, 0);
//    root_b = lyd_new_path(NULL, ctx, "/minimal-integrationtest:simpleleaf","b", 0, 0);
//    // lyd_new_path(root_b, NULL, "/integrationtest:bronze/silver/gold/platinum/deep","in a cavern", 0, 0);
//    // lyd_new_path(root_b, NULL, "/integrationtest:container-and-lists/multi-key-list[A='a'][B='b']/inner/C", "c", 0, 0);
//    // lyd_new_path(root_b, NULL, "/integrationtest:container-and-lists/multi-key-list[A='aa'][B='bb']/inner/C", "cc", 0, 0);
//    /*
//    int 	lyd_change_leaf (struct lyd_node_leaf_list *leaf, const char *val_str)
//  	Change value of a leaf node. More...
//
//   struct lyd_node * 	lyd_new_anydata (struct lyd_node *parent, const struct lys_module *module, const char *name, void *value, LYD_ANYDATA_VALUETYPE value_type)
//  	Create a new anydata or anyxml node in a data tree. More...
//
//   */
//
//
//   /*
//
//   This will validate leaf-refs and everything.
//
//   Note: /integrationtest:simpleenum gets validated as we go.
//   int 	lyd_validate (struct lyd_node **node, int options, void *var_arg,...)
//  	Validate node data subtree. More...
//       libyang[0]: Invalid value "AZ" in "simpleenum" element. (path: /integrationtest:simpleenum)
//       libyang[0]: Failed to create node "simpleenum". (path: /integrationtest:simpleenum)
//
//   */
//
//    //
//    // diff = lyd_diff(root_a, root_b, 0);
//    // struct lyd_node *child_node = NULL;
//    // struct lyd_node *this_node = NULL;
//    // struct lyd_node *this_node2 = NULL;
//    // char* path = NULL;
//    // char* path2 = NULL;
//    //
//    //
//    // uint8_t i = 0;
//    //
//    // while(i<255){
//    //   // -> get memeber of a struct
//    //    printf("\n   Diff result %i\n", i);
//    //    // if (diff->type[i]){
//    //    //   break;
//    //    // }
//    //    LYD_DIFFTYPE diff_type = diff->type[i];
//    //
//    //    if(diff_type == LYD_DIFF_END){
//    //      printf("END OF DIFFSET\n");
//    //      break;
//    //    }
//    //
//    //    if(diff_type == LYD_DIFF_CHANGED){
//    //        printf("Diff changed\n");
//    //        path = lyd_path(diff->first[i]);
//    //        printf("Path: %s\n", path);
//    //        //printf("%s",diff->first[i]->attrvalue_str);
//    //    }
//    //    if(diff_type == LYD_DIFF_DELETED){
//    //        printf("Diff deleted\n");
//    //        path = lyd_path(diff->first[i]);
//    //        printf("Path: %s\n", path);
//    //    }
//    //
//    //    if(diff_type == LYD_DIFF_CREATED){
//    //        printf("Diff created\n");
//    //
//    //
//    //
//    //        this_node = diff->second[i];
//    //
//    //
//    //        while(this_node!= NULL){
//    //
//    //
//    //          this_node2 = this_node->next;
//    //          while(this_node2!=NULL){
//    //            printf(" ---- we have siblings\n");
//    //            path2= lyd_path(this_node2);
//    //            printf(" ---- sibling %s\n", path2);
//    //            this_node2 = this_node2->next;
//    //          }
//    //
//    //
//    //          if(this_node->schema->nodetype & (LYS_LEAF | LYS_LEAFLIST)){
//    //            this_node = NULL;
//    //          }else{
//    //            this_node=this_node->child;
//    //            path= lyd_path(this_node);
//    //          }
//    //
//    //          //child_node = child_node->child;
//    //          //printf("some more path stuff %s\n", path2);
//    //
//    //          printf("Path: %s\n", path);
//    //
//    //        }
//    //
//    //    }
//    //    if(diff_type == LYD_DIFF_MOVEDAFTER1 ){
//    //      printf("Diff moved after 1 \n");
//    //    }
//    //    if(diff_type == LYD_DIFF_MOVEDAFTER2 ){
//    //      printf("Diff moved after 2 \n");
//    //    }
//
//
//       //printf("Type %i: %s", i, diff->type[i]);
//       // switch(diff->type[i]){
//       //   case LYD_DIFF_END :
//       //     printf("END %i:\n", i);
//       //
//       //   case LYD_DIFF_CHANGED :
//       //     printf("CHANGED: %i\n", i);
//       //     if(diff->first[i]==NULL){
//       //       printf("   First object is null\n");
//       //     }
//       //     if(diff->first[i]==NULL){
//       //       printf("   Second object is null\n");
//       //     }
//       //     //printf((str = lyd_path(diff->second[i])));
//       //   case LYD_DIFF_DELETED :
//       //     printf("DELETED: %i\n", i);
//       //     if(diff->first[i]==NULL){
//       //       printf("   First object is null\n");
//       //     }
//       //     if(diff->first[i]==NULL){
//       //       printf("   Second object is null\n");
//       //     }
//       //     case LYD_DIFF_CREATED :
//       //       printf("CREATED: %i\n", i);
//       //       if(diff->first[i]==NULL){
//       //         printf("   First object is null\n");
//       //       }
//       //       if(diff->first[i]==NULL){
//       //         printf("   Second object is null\n");
//       //       }
//       //   default :
//       //     printf("UNKNOWN: %i\n", i);
//       //
//       //
//       // }
//     //see: test_diff2(void **state)
//
//      //
//      //printf("%s",diff[i]);
//    //   i =i +1;
//    // }
//
//    /*
//
//    We need to remove the diff result
//    void 	lyd_free_diff (struct lyd_difflist *diff)
//  	 Free the result of lyd_diff(). It frees the structure of the lyd_diff() result, not the referenced nodes. More...
//   */
//
//
// /*
// void lyd_free_withsiblings	(	struct lyd_node * 	node	)
//
//   - this will delete a node- but we need to get the node first.
//
//   struct ly_set* lyd_find_path	(	const struct lyd_node * 	ctx_node,
//   const char * 	path
//   )
//
// */
//     FILE *f;
// //    f = fopen("/tmp/libyang.xml", "w");
// //
// //      lyd_print_file(f, root_a, LYD_XML,LYP_WITHSIBLINGS);
// //
// //      fclose(f);
// //      f = fopen("/tmp/libyang.b.xml", "w");
// //
// //        lyd_print_file(f, root_b, LYD_XML,LYP_WITHSIBLINGS);
// //
// //        fclose(f);
// //
//       f = fopen("/tmp/libyang.json", "w");
//       lyd_print_file(f, root_a, LYD_JSON,LYP_WITHSIBLINGS);
//       fclose(f);
//      // struct ly_set *set = NULL;
//      // set = lyd_find_path(root_a, "/integrationtest:simpleleaf");
//      // if (set== NULL){
//      //   printf("lyd_find_path was null");
//      // }
//      // printf("found an attribute from lyd_find_path\n");
//      // struct lys_type *type = NULL;
//      // type = lyd_leaf_type((struct lyd_node_leaf_list *)set);
//      //
//      //
//      // printf("Segfaulty here");
// //     set = lyd_find_path(root_a, "/integrationtest:simplelist");
//      // lyd_free_withsiblings(set);
//
// // we can serialise into xml/json  - probably
// ///   https://netopeer.liberouter.org/doc/libyang/master/group__datatree.html#ga5dee9dd41c57edc1fc2185f6a2c233a3
//
// /*
//
// struct lys_type* lyd_leaf_type	(	const struct lyd_node_leaf_list * 	leaf	)
// Get the type structure of a leaf.
//
// In case of a union, the correct specific type is found. In case of a leafref, the final (if there is a chain of leafrefs) target's type is found.
//
// Parameters
// [in]	leaf	Leaf to examine.
// Returns
// Found type, NULL on error.
//
// */
//
//   struct lyd_node *my_node;
//   struct ly_set *my_set;
//   const char *str;
//
//   struct lyd_node_leaf_list my_leaf;
//
//
//   my_set = lyd_find_path(root_a, "/minimal-integrationtest:simpleleaf");
//   if(my_set == NULL){
//     printf("my_set is null");
//   }else{
//     printf("my_set is not null");
//     printf("my_set size: %d ", my_set->number);
//     my_node = my_set->set.d[0];
//     if(my_node == NULL){
//       printf("my node is null!!!");
//     }
//
//
//     /*
//
//     with a lot of work finally got to read the values.....
//
//     copied from....
//     sr_libyang_leaf_copy_value(const struct lyd_node_leaf_list *leaf, sr_val_t *value)
//
//     */
//     printf("\n");
//     str = my_set->set.d[0]->schema->name;
//     printf("sting : %s\n", str);
//     //str = my_set->set.d[0]->attr->value_str;
//     if(my_set->set.d[0]->schema->nodetype & (LYS_LEAF | LYS_LEAFLIST)){
//       printf("This is a leaf or a leaf list\n");
//       my_node = my_set->set.d[0];
//       my_leaf = *(struct lyd_node_leaf_list *)my_node;
//       str = my_leaf.value_str;
//       //.val_str;
//       // see
//
//     }
//     //str = my_set->set.d[0]->value_str;
//     printf("sting : %s\n", str);
//
//     //struct lyd_node *my_node;
//     //my_node = my_set->set.d[0];
//     //printf("%s",my_node->attr->value_str);
//   }

   return 0;
}
