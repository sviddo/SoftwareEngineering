#include <stdio.h>
#include <stdlib.h>



struct Sbook
{
    char name[20];
    char surname[20];
    char title[20];
    int year;
    int pages;
    int cost;
    struct Sbook * nextNode;
};



void PrintList(struct Sbook * tmp, char string[30], FILE * log);
int FileStringCount(FILE * file);
struct Sbook * SingleLinkedList(FILE * file, int notes);
double CalculateAverageCost(struct Sbook * tmp, int notes);
double CalculateAveragePages(struct Sbook * tmp, int notes);
void SortList(struct Sbook ** Head, int notes);
void BooksPiblishedAfter1980(struct Sbook * tmp, FILE * log);
void RemoveElements(struct Sbook * tmp, double AverageCost, double AveragePages);
FILE * OpenFile(const char FileName[30], const char Mode[5]);
int check_valid_num(int num, int val);
void AddElem(struct Sbook ** Head, int notes);
int check_valid_string(char str_to_check[20]);
void SortListYear(struct Sbook ** Head, int notes);



int main(void) {
    char answer_input, answer_output;
    printf("Do you want to read from file? y/n: ");
    scanf("%c", &answer_input);
    printf("Do you want to write all data in file? y/n: ");
    scanf(" %c", &answer_output);

    FILE * file = NULL;

    if(answer_input == 'y')
    {
        file = OpenFile("input.txt", "r");  // opening the file
    }

    FILE * log_file = NULL;

    if(answer_output == 'y')
    {
        log_file = OpenFile("log.txt", "w");
    }


    int notes = FileStringCount(file); // defining how many notes there are in file

    struct Sbook * Head = SingleLinkedList(file, notes);  // pointer on created single-linked list

    PrintList(Head, "Source list: ", log_file);

    char answer_add_input;
    printf("Do you want to add element? y/n: ");
    scanf(" %c", &answer_add_input);
    if(answer_add_input == 'y')
    {
        AddElem(&Head, notes);
        notes += 1;
    }

    double AverageCost = CalculateAverageCost(Head, notes);  // calculating average cost of all books
    double AveragePages = CalculateAveragePages(Head, notes);  // calculating average pages number of all books

    fclose(file);


    PrintList(Head, "Source list after element adding: ", log_file);

    printf("Average cost = %.2lf\n", AverageCost);
    printf("Average pages = %.2lf\n\n\n", AveragePages);


    SortList(&Head, notes + 1);

    PrintList(Head, "Sorted list by cost in descending order: ", log_file);

    char answer_year;
    printf("Do you want to additionally sort the list by year? y/n: ");
    scanf(" %c", &answer_year);
    if(answer_year == 'y')
    {
        struct Sbook * tmp = Head;
        SortListYear(&tmp, notes + 1);
        PrintList(tmp, "Sorted list by year in descending order: ", log_file);
    }


    BooksPiblishedAfter1980(Head, log_file);


    RemoveElements(Head, AverageCost, AveragePages);

    PrintList(Head, "List after elements removing: ", log_file);

    fclose(log_file);


    free(Head);
    Head = NULL;
    return 0;
}



int FileStringCount(FILE * file)
{
    // counting how many strings there are in opened file
    int count = 0;

    if(file == NULL)
    {
        printf("How many notes do you want to enter?: ");
        scanf("%d", &count);
        return count;
    }

    char c;

    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
        {
            count++;
        }
    }
    count++;

    freopen("input.txt", "r", file);
    return count;
}



struct Sbook * SingleLinkedList(FILE * file, int notes)
{
    // creating single-linked list

    struct Sbook * tmp = (struct Sbook *) malloc(sizeof(struct Sbook) * notes);

    if(file == NULL)
    {
        puts("Enter them separating by a gap in valid format: Name Surname \"Title\" Year Pages Cost:");

        for(int i = 0; i < notes; i++)
        {
            int check = scanf("%s %s %s %d %d %d", (tmp + i) -> name, (tmp + i) -> surname, (tmp + i) -> title, \
                &(tmp + i) -> year, &(tmp + i) -> pages, &(tmp + i) -> cost);

            check_valid_num(check, 6);

            (tmp + i) -> nextNode = (tmp + i + 1);
        }
        (tmp + notes - 1) -> nextNode = NULL;

        return tmp;
    }

    for(int i = 0; i < notes; i++)
    {
        int check = fscanf(file, "%s %s %s %d %d %d", (tmp + i) -> name, (tmp + i) -> surname, (tmp + i) -> title, \
            &((tmp + i) -> year), &((tmp + i) -> pages), &((tmp + i) -> cost));

            check_valid_num(check, 6);

        (tmp + i) -> nextNode = (tmp + i + 1);
    }

    (tmp + notes - 1) -> nextNode = NULL;

    return tmp;
}



double CalculateAverageCost(struct Sbook * tmp, int notes)
{
    // calculating all books average cost

    int temp_value = 0;

    while(tmp != NULL)
    {
        temp_value += tmp -> cost;
        tmp = tmp -> nextNode;
    }

    return (double)(temp_value) / notes;
}



double CalculateAveragePages(struct Sbook * tmp, int notes)
{
    // calculating average pages numbers of all books

    int temp_value = 0;

    while(tmp != NULL)
    {
        temp_value += tmp -> pages;
        tmp = tmp -> nextNode;
    }

    return (double)(temp_value) / notes;
}



void SortList(struct Sbook ** Head, int notes)
{
    // sorting the list by cost field of each element

    for(int i = notes - 1; i--;)
    {
        if(((*Head) -> cost) < (((*Head) -> nextNode) -> cost))
        {
            struct Sbook * temp = (*Head) -> nextNode;
            (*Head) -> nextNode = (*Head) -> nextNode -> nextNode;
            temp -> nextNode = *Head;
            *Head = temp;
        }

        struct Sbook * ptr = *Head;

        while((ptr -> nextNode -> nextNode) != NULL)
        {
            if(((ptr -> nextNode) -> cost) < ((ptr -> nextNode -> nextNode) -> cost))
            {
                struct Sbook * temp = ptr -> nextNode -> nextNode;
                ptr -> nextNode -> nextNode = ptr -> nextNode -> nextNode -> nextNode;
                temp -> nextNode = ptr -> nextNode;
                ptr -> nextNode = temp;
            }
            ptr = ptr -> nextNode;
        }
    }
}



FILE * OpenFile(const char FileName[30], const char Mode[5])
{
    // opening file for reading

    FILE * file = fopen(FileName, Mode);
    if(file == NULL) {
        fclose(file);
        printf("FATAL ERROR. Failed to open the file.");
        exit(EXIT_FAILURE);
    }
    return file;
}



void BooksPiblishedAfter1980(struct Sbook * tmp, FILE * log)
{
    // printing books published after 1980
    struct Sbook * tmp2 = tmp;

    puts("Books published after 1980: ");
    printf("---------------------------------------------------------------\n");
    printf("| Name      | Surname   | Title         | Year | Pages | Cost |\n");
    printf("---------------------------------------------------------------\n");

    while(tmp != NULL)
    {
        if((tmp -> year) > 1980)
        {
            printf("| %-9s | %-9s | %-13s | %-4d |  %-3d  | %-4d |\n", tmp -> name, tmp -> surname, tmp -> title, \
            tmp -> year, tmp -> pages, tmp -> cost);
        }
        tmp = tmp -> nextNode;
    }

    printf("---------------------------------------------------------------\n\n\n");

    if(log)
    {
        fprintf(log, "Books published after 1980: \n");
        fprintf(log, "---------------------------------------------------------------\n");
        fprintf(log, "| Name      | Surname   | Title         | Year | Pages | Cost |\n");
        fprintf(log, "---------------------------------------------------------------\n");

        while(tmp2 != NULL)
        {
            if((tmp2 -> year) > 1980)
            {
                fprintf(log, "| %-9s | %-9s | %-13s | %-4d |  %-3d  | %-4d |\n", tmp2 -> name, tmp2 -> surname, tmp2 -> title, \
                tmp2 -> year, tmp2 -> pages, tmp2 -> cost);
            }
            tmp2 = tmp2 -> nextNode;
        }

        fprintf(log, "---------------------------------------------------------------\n\n\n");
    }
}



void PrintList(struct Sbook * tmp, char string[30], FILE * log)
{
    // printing the list
    struct Sbook * tmp2 = tmp;

    printf("%s\n", string);
    printf("---------------------------------------------------------------\n");
    printf("| Name      | Surname   | Title         | Year | Pages | Cost |\n");
    printf("---------------------------------------------------------------\n");

    while(tmp != NULL)
    {
        printf("| %-9s | %-9s | %-13s | %-4d |  %-3d  | %-4d |\n", tmp -> name, tmp -> surname, tmp -> title, \
            tmp -> year, tmp -> pages, tmp -> cost);
        tmp = tmp -> nextNode;
    }

    printf("---------------------------------------------------------------\n\n\n");

    if(log)
    {
        fprintf(log, "%s\n", string);
        fprintf(log, "---------------------------------------------------------------\n");
        fprintf(log, "| Name      | Surname   | Title         | Year | Pages | Cost |\n");
        fprintf(log, "---------------------------------------------------------------\n");

        while(tmp2 != NULL)
        {
            fprintf(log, "| %-9s | %-9s | %-13s | %-4d |  %-3d  | %-4d |\n", tmp2 -> name, tmp2 -> surname, tmp2 -> title, \
                tmp2 -> year, tmp2 -> pages, tmp2 -> cost);
            tmp2 = tmp2 -> nextNode;
        }

        fprintf(log, "---------------------------------------------------------------\n\n\n");
    }
}



void RemoveElements(struct Sbook * tmp, double AverageCost, double AveragePages)
{
    // removing the list elements which cost is lower than average and pages number is higher than average

    while((tmp -> nextNode -> nextNode) != NULL)
    {
        if(((double)((tmp -> nextNode) -> cost) > AverageCost) && ((double)((tmp -> nextNode) -> pages) < AveragePages))
        {
            tmp -> nextNode = tmp -> nextNode -> nextNode;
        }
        tmp = tmp -> nextNode;
    }
}



void AddElem(struct Sbook ** Head, int notes)
{
    void * ptr2 = realloc(*Head, ((sizeof(struct Sbook) * notes) + (sizeof(struct Sbook))));

    if(ptr2)
    {
        *Head = (struct Sbook *) ptr2;
        ptr2 = NULL;
    }
    else
    {
        puts("FATAL ERROR. Failed to reallocate the memory.");
        free(*Head);
        exit(EXIT_FAILURE);
    }

    puts("Enter values for new element: ");
    printf("Enter the name: ");
    scanf("%s", (*Head + notes) -> name);
    check_valid_string((*Head + notes) -> name);

    printf("Enter the surnname: ");
    scanf("%s", (*Head + notes) -> surname);
    check_valid_string((*Head + notes) -> surname);

    printf("Enter the title: ");
    scanf("%s", (*Head + notes) -> title);
    check_valid_string((*Head + notes) -> title);

    printf("Enter the year: ");
    check_valid_num(scanf("%d", &((*Head + notes) -> year)), 1);

    printf("Enter the pages: ");
    check_valid_num(scanf("%d", &((*Head + notes) -> pages)), 1);

    printf("Enter the cost: ");
    check_valid_num(scanf("%d", &((*Head + notes) -> cost)), 1);

    (*Head + notes) -> nextNode = NULL;

    int position;
    printf("Enter the position where you want to insert your element: ");
    scanf("%d", &position);
    printf("\n\n");

    if(position == 1)
    {
        (*Head + notes) -> nextNode = *Head;
        *Head = (*Head + notes);
    }
    else if(position == notes + 1)
    {
        struct Sbook * tmp = *Head;
        while((tmp -> nextNode) != NULL)
        {
            tmp = tmp -> nextNode;
        }
        tmp -> nextNode = (*Head + notes);
    }
    else
    {
        struct Sbook * tmp = *Head;
        for(int i = 0; i < position - 2; i++)
        {
            tmp = tmp -> nextNode;
        }
        (*Head + notes) -> nextNode = tmp -> nextNode;
        tmp -> nextNode = (*Head + notes);
    }
}



int check_valid_string(char str_to_check[20])
{
    for(int i = 0; str_to_check[i] != '\0'; i++)
    {
        if(str_to_check[i] >= 'a' && str_to_check[i] <= 'z' || str_to_check[i] >= 'A' && str_to_check[i] <= 'Z')
        {
            continue;
        }
        else if(str_to_check[i] == '"')
        {
            continue;
        }
        
        puts("String contains invalid value.");
        exit(EXIT_FAILURE);
    }

    return 0;
}



int check_valid_num(int num, int val)
{
    if(num == val)
    {
        return 0;
    }
    else
    {
        puts("String contains invalid value.");
        exit(EXIT_FAILURE);
    }
}



void SortListYear(struct Sbook ** Head, int notes)
{
    // sorting the list by cost field of each element

    for(int i = notes - 1; i--;)
    {
        if(((*Head) -> year) < (((*Head) -> nextNode) -> year))
        {
            struct Sbook * temp = (*Head) -> nextNode;
            (*Head) -> nextNode = (*Head) -> nextNode -> nextNode;
            temp -> nextNode = *Head;
            *Head = temp;
        }

        struct Sbook * ptr = *Head;

        while((ptr -> nextNode -> nextNode) != NULL)
        {
            if(((ptr -> nextNode) -> year) < ((ptr -> nextNode -> nextNode) -> year))
            {
                struct Sbook * temp = ptr -> nextNode -> nextNode;
                ptr -> nextNode -> nextNode = ptr -> nextNode -> nextNode -> nextNode;
                temp -> nextNode = ptr -> nextNode;
                ptr -> nextNode = temp;
            }
            ptr = ptr -> nextNode;
        }
    }
}