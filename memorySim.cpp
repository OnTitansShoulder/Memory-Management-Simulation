#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;

const int BEST_FIT = 1;
const int WORST_FIT = 2;

struct Page{
  string name;
  int size;
  Page* next;
};

void initialize(Page* freeHead, Page* usedHead);
void printMenu();
void addPage(Page* freeHead, Page* usedHead, int memoryManagementMethod);
void addBest(Page* freeHead, Page* usedHead, string programName, int size);
void addWorst(Page* freeHead, Page* usedHead, string programName, int size);
void killPage(Page* freeHead, Page* usedHead);
void fragmentation(Page* freeHead);
void printMemory(Page* freeHead, Page* usedHead);

int main(int argc, char* argv[]){
  int memoryManagementMethod = 0;

  if(argc == 2 && argv[1] != NULL){
    if(strcmp(argv[1], "best") == 0){
      memoryManagementMethod = BEST_FIT; //best-fit
      cout << "Using best fit algorithm" << endl;
    } else if(strcmp(argv[1], "worst") == 0){
      memoryManagementMethod = WORST_FIT; //worst-fit
      cout << "Using worst fit algorithm" << endl;
    }
  } else {
    cout << "Plesase run the program with correct number of arguments." << endl;
    return 0;
  }

  Page* freeHead = new Page;
  Page* usedHead = new Page;
  initialize(freeHead, usedHead);
  printMenu();

  int choice = 0;
  while(choice != 5){
    cout << "choice - ";
    cin >> choice;
    switch (choice) {
      case 1:
      addPage(freeHead, usedHead, memoryManagementMethod);
      break;
      case 2:
      killPage(freeHead, usedHead);
      break;
      case 3:
      fragmentation(freeHead);
      break;
      case 4:
      printMemory(freeHead, usedHead);
      break;
      case 5:
      break;
      default:
      cout << "\nInput is wrong.\n" << endl;
    }
  }

  return 0;
}

void initialize(Page* freeHead, Page* usedHead){
  Page* freeNode = new Page;
  freeNode->name = "Free";
  freeNode->size = 32;
  freeNode->next = NULL;

  freeHead->name = "Head";
  freeHead->next = freeNode;
  usedHead->name = "Head";
  usedHead->next = NULL;
}

void printMenu(){
  cout << "\n1. Add program" << endl;
  cout << "2. Kill program" << endl;
  cout << "3. Fragmentation" << endl;
  cout << "4. Print memory" << endl;
  cout << "5. Exit\n" << endl;
}

void addPage(Page* freeHead, Page* usedHead, int memoryManagementMethod){
    string programName;
    cout << "Program name - ";
    cin >> programName;
    int size;
    cout << "Program size (KB) - ";
    cin >> size;
    size = (int)((size + 3) / 4);
    if(memoryManagementMethod == BEST_FIT){
      addBest(freeHead, usedHead, programName, size);
    } else {
      addWorst(freeHead, usedHead, programName, size);
    }
    cout << "Success! " << size << " page(s) are used." << endl;
}

void addBest(Page* freeHead, Page* usedHead, string programName, int size){
  Page* freePrevious = freeHead;
  Page* freeCurrent = freeHead->next;
  int min = 33, used = 0, usedSkipped = 0;
  Page* bestPrevious = NULL;
  Page* bestCurrent = NULL;
  while(freeCurrent != NULL){
    if(freeCurrent->name.compare("Free") == 0){
      if(freeCurrent->size < min && freeCurrent->size >= size){
        min = freeCurrent->size;
        bestCurrent = freeCurrent;
        bestPrevious = freePrevious;
        usedSkipped = used;
      }
    } else{
      used++;
    }
    freePrevious = freeCurrent;
    freeCurrent = freeCurrent->next;
  }
  if(min == 33){
    cout << "Program does not fit." <<endl;
    return;
  } else if(min == size){
    bestCurrent->name = "Used";
  } else{
    Page* newPage = new Page;
    newPage->name = "Used";
    newPage->size = size;
    newPage->next = bestCurrent;
    bestPrevious->next = newPage;
    bestCurrent->size = bestCurrent->size - size;
  }

  Page* previous = usedHead;
  Page* current = usedHead->next;
  while(usedSkipped > 0){
    previous = current;
    current = current->next;
    usedSkipped--;
  }
  Page* newUsedPage = new Page;
  newUsedPage->name = programName;
  newUsedPage->size = size;
  previous->next = newUsedPage;
  newUsedPage->next = current;
}

void addWorst(Page* freeHead, Page* usedHead, string programName, int size){
  Page* freePrevious = freeHead;
  Page* freeCurrent = freeHead->next;
  int max = -1, used = 0, usedSkipped = 0;
  Page* maxPrevious = NULL;
  Page* maxCurrent = NULL;
  while(freeCurrent != NULL){
    if(freeCurrent->name.compare("Free") == 0){
      if(freeCurrent->size > max){
        max = freeCurrent->size;
        maxCurrent = freeCurrent;
        maxPrevious = freePrevious;
        usedSkipped = used;
      }
    } else{
      used++;
    }
    freePrevious = freeCurrent;
    freeCurrent = freeCurrent->next;
  }
  if(max < size){
    cout << "Program does not fit." <<endl;
    return;
  } else if(max == size){
    maxCurrent->name = "Used";
  } else{
    Page* newPage = new Page;
    newPage->name = "Used";
    newPage->size = size;
    newPage->next = maxCurrent;
    maxPrevious->next = newPage;
    maxCurrent->size = maxCurrent->size - size;
  }

  Page* previous = usedHead;
  Page* current = usedHead->next;
  while(usedSkipped > 0){
    previous = current;
    current = current->next;
    usedSkipped--;
  }
  Page* newUsedPage = new Page;
  newUsedPage->name = programName;
  newUsedPage->size = size;
  previous->next = newUsedPage;
  newUsedPage->next = current;
}

void killPage(Page* freeHead, Page* usedHead){
  string programName;
  cout << "Program name - ";
  cin >> programName;
  bool found = false;
  int counter = 0;
  Page* previous = usedHead;
  Page* current = usedHead->next;
  while(current != NULL){
    if(current->name.compare(programName) == 0){
      found = true;
      break;
    }
    counter++;
    previous = current;
    current = current->next;
  }
  if(!found){
    cout << "Program not found." << endl;
    return;
  }

  previous->next = current->next;
  delete current;

  Page* freePrevious = freeHead;
  Page* freeCurrent = freeHead->next;
  while(counter > 0){
    if(freeCurrent->name.compare("Used") == 0){
      counter--;
    }
    freePrevious = freeCurrent;
    freeCurrent = freeCurrent->next;
  }
  while(freeCurrent->name.compare("Used") != 0){
    freePrevious = freeCurrent;
    freeCurrent = freeCurrent->next;
  }
  freeCurrent->name = "Free";
  if(freePrevious->name.compare("Free") == 0){
    freePrevious->size = freePrevious->size + freeCurrent->size;
    freePrevious->next = freeCurrent->next;
    delete freeCurrent;
    freeCurrent = freePrevious;
  }
  if(freeCurrent->next->name.compare("Free") == 0){
    Page* temp = freeCurrent->next;
    freeCurrent->size = freeCurrent->size + temp->size;
    freeCurrent->next = temp->next;
    delete temp;
  }
}

void fragmentation(Page* freeHead){
  int count = 0;
  Page* current = freeHead->next;
  while(current!=NULL){
    if(current->name.compare("Free") == 0)
      count++;
    current = current->next;
  }
  cout << "There are " << count << " fragment(s)." << endl;
}

void printMemory(Page* freeHead, Page* usedHead){
  int counter = 0;
  int pageSize = 0;
  string currentName;
  Page *freeCurrent = freeHead->next, *usedCurrent = usedHead->next;
  while(counter < 32){
    if(freeCurrent->name.compare("Free") == 0){
      currentName = "Free";
      pageSize = freeCurrent->size;
    } else{
      currentName = usedCurrent->name;
      pageSize = usedCurrent->size;
      usedCurrent = usedCurrent->next;
    }
    while(pageSize>0){
      printf("%-6s ", currentName.c_str());
      counter++;
      if(counter % 8 == 0)
        cout << endl;
      pageSize--;
    }
    freeCurrent = freeCurrent->next;
  }
}
