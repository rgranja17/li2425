
#include <stdio.h>
#include <stdlib.h>
#include "data_manager/database_manager.h"
#include "batchMode.h"
#define OUTPUT_DIR "resultados/"

int main(int argc, char *argv[]) {

   if(argc > 3) {
      printf("./programa-principal <path dataset> <input>");
      return 1;
   }

   Database_manager* db = newDatabase();
   char* datasetDir = argv[1];
   char* inputFile = argv[2]; 
   char* outputDir = OUTPUT_DIR;

   if(argc == 3) batchMode(inputFile,outputDir,db,datasetDir);


   free_database(db);
   return 0;
}
