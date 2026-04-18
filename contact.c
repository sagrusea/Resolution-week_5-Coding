#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 100

char *FILENAME = "contact_book_data.csv";
int contact_count;

typedef struct Contact {
    char name[INPUT_LENGTH];
    char adress[INPUT_LENGTH];
    char email[INPUT_LENGTH];
    char number[INPUT_LENGTH];
} Contact;

Contact contact_array[100];

int clear_screen();
int add_new_entry();
int list_contacts();
int read_from_csv(Contact *contact_array);
void print_contact(const Contact *contact);
void search(const char query[]);
void trim_quotes(char *str);

int main(int argc, char *argv[]) {
    clear_screen();
    
    char buff[2];
    char menu[] = "Contacts Menu:\n1.Add new contact\n2.List all contacts\n3.Search\n4.Quit\nTo bring this menu up type: \"h\"\n";
    if (argc > 1) {
        FILENAME = argv[1];
    }
    printf("%s", menu);


    while (1) {
        scanf("%c", &buff);
        getchar();
        switch (buff[0]) {
            case '1':
                clear_screen();
                add_new_entry();
                printf("%s", menu);
                break;
            case '2':
                clear_screen();
                list_contacts();
                printf("%s", menu);
                break;
            case '3':
                clear_screen();
                printf("Enter query:\n");
                char query[INPUT_LENGTH];
                fgets(query, INPUT_LENGTH, stdin);
                query[strcspn(query, "\n")] = 0;
                search(query);
                printf("%s", menu);
                break;
            case '4':
                return 0;
            case 'h':  
                clear_screen();
                printf("%s", menu);
                break;
            default:
                clear_screen();
                printf("%s is not a command\n", buff);
                printf("%s", menu);
        }
    }
    

    switch (buff[0]) {
        case 1:
            add_new_entry();
            break;
    }

    return 0;
}

int clear_screen() {
    #if defined(_WIN32)
        system("cls");
    #elif defined(__linux__)
        system("clear");
    #endif
}

int add_new_entry() {
    FILE *fptr;

    fptr = fopen(FILENAME, "a");

    char name[INPUT_LENGTH] = "name_here";
    char adress[INPUT_LENGTH] = "adress_here";
    char email[INPUT_LENGTH] = "email_here";
    char number[INPUT_LENGTH] = "number_here";

    char *data[] = {name, adress, email, number};
    char *labels[] = {"name", "adress", "email", "number"};
    // get data
    for (int i = 0; i < 4; i++) {
        printf("Enter %s:\n", labels[i]);

        fgets(data[i], INPUT_LENGTH, stdin);
        data[i][strcspn(data[i], "\n")] = 0;
        for (int j = 0; data[i][j]; j++) {
            if (data[i][j] == '"') data[i][j] = '\'';
        }
    }

    fprintf(fptr, "\n\"%s\",\"%s\",\"%s\",\"%s\"", name, adress, email, number);
    fclose(fptr);

    printf("\nSaved contact!\n\n");

    return 0;
}

int list_contacts() {
    contact_count = read_from_csv(contact_array);

    for (int i = 0; i < contact_count; i++) {
        print_contact(&contact_array[i]);
    }
    return 0;
}

void print_contact(const Contact *contact) {
    printf("Name: %s\n", contact->name);
    printf("Adress: %s\n", contact->adress);
    printf("Email: %s\n", contact->email);
    printf("Number: %s\n", contact->number);
    printf("\n");
}

int read_from_csv(Contact contact_array[]) {
    FILE *fptr;
    char buffer[(INPUT_LENGTH * 4 + 5)];

    int i = 0;

    fptr = fopen(FILENAME, "r");
    if (fptr == NULL) {
        printf("Could not open %s\n", FILENAME);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), fptr)) {
        char *p = buffer;
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        if (*p == '\0') continue;

        char *new_line_pointer = strchr(buffer, '\n');
        if (new_line_pointer) *new_line_pointer = '\0';

        char *field = strtok(buffer, "\"");
        int x = 0;

        while (field != NULL) {
            switch (x) {
            case 0:
                strncpy(contact_array[i].name, field, INPUT_LENGTH - 1);
                contact_array[i].name[INPUT_LENGTH - 1] = '\0';
                break;
            case 1:
                strncpy(contact_array[i].adress, field, INPUT_LENGTH - 1);
                contact_array[i].adress[INPUT_LENGTH - 1] = '\0';
                break;
            case 2:
                strncpy(contact_array[i].email, field, INPUT_LENGTH - 1);
                contact_array[i].email[INPUT_LENGTH - 1] = '\0';
                break;
            case 3:
                strncpy(contact_array[i].number, field, INPUT_LENGTH - 1);
                contact_array[i].number[INPUT_LENGTH - 1] = '\0';
                break;
            default:
                break;
            }
            x++;
            field = strtok(NULL, "\",\"");
        }

        i++;
        if (i >= 100) break;
    }
    

    fclose(fptr);

    return i;   

}

void search(const char query[]) {
    int found = 0;
    contact_count = read_from_csv(contact_array);
    for (int i = 0; i <= contact_count; i++) {
        //trim_quotes(contact_array[i].name);
        if (strstr(contact_array[i].name, query) != NULL) {
            found++;
            printf("Contact:%d:\n", found);
            /*trim_quotes(contact_array[i].adress);
            trim_quotes(contact_array[i].email);
            trim_quotes(contact_array[i].number);*/
            print_contact(&contact_array[i]);
        }
    }
    if (found == 0) {
        printf("contact not found");
        return;
    }
    return;
}

void trim_quotes(char *str) {
    int length = strlen(str);
    if (length >= 2) {
        memmove(str, str + 1, length - 1);
        str[length - 2] = '\0';
    }
}