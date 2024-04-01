// 220101102 SUDE FİLİKCİ && 230101015  AYSEGUL ALTINTAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 30

// Dosyayı açar veya oluşturur.
FILE* openFileForUpdate();
// Kişi ekleme işlemi.
FILE* addContact(FILE *, int, char**);
// Kişileri listeler.
FILE* listContacts(FILE *);
// Kişileri metin dosyasına dışa aktarır.
FILE* exportContacts(FILE*, const char*);
// Kişi arama işlemi.
FILE* searchContacts(FILE*, char, const char*);
// Kişi güncelleme işlemi.
FILE* updateContact(FILE*, int, const char*, const char*, const char*);
// Kişi silme işlemi.
FILE* deleteContact(FILE*, int);

typedef struct {
    int id;
    int inlinePhone;
    char name[STR_SIZE];
    char lName[STR_SIZE];
    char department[STR_SIZE];
} Contact;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Komut satiri.\n");
        return 1;
    }

    FILE *fPtr = openFileForUpdate();

    switch(argv[1][0]) {
        case 'a':
            if(argc != 7) {
                printf("Kisi eklemek icin dogru arguman girin.\n");
                return 1;
            }
            addContact(fPtr, argc, argv);
            break;
        case 'l':
            listContacts(fPtr);
            break;
        case 'e':
            if(argc != 3) {
                printf("Export işlemi için dosya adı gerekli.\n");
                return 1;
            }
            exportContacts(fPtr, argv[2]);
            break;
        case 'u':
            if(argc != 6) {
                printf("Guncelleme islemi icin dogru argumanlari saglayin.\n");
                return 1;
            }
            updateContact(fPtr, atoi(argv[2]), argv[3], argv[4], argv[5]);
            break;
        case 'd':
            if(argc != 3) {
                printf("Silmek icin  phone number gerekli.\n");
                return 1;
            }
            deleteContact(fPtr, atoi(argv[2]));
            break;
        case 's':
            if(argc != 4) {
                printf("Arama islemi icin p-n-l-d secin.\n");
                return 1;
            }
            searchContacts(fPtr, argv[2][0], argv[3]);

            break;
        default:
            printf("Gecersiz.\n");
            return 1;
    }

    fclose(fPtr);
    return 0;
}

FILE* openFileForUpdate() {
    FILE * fPtr=fopen("phoneBook.dat","rb+");
    if(fPtr==NULL) {
        printf("Dosya acilmadi.\n");
        fPtr=fopen("phonebook.dat","wb+");
        if(fPtr==NULL) {
            printf("Dosya olusturulamadi.\n");
            exit(1);
        }
    }
    return fPtr;
}

FILE* addContact(FILE *fPtr, int argc, char** argv) {
    Contact contact;
    contact.id = atoi(argv[2]);
    contact.inlinePhone = atoi(argv[3]);
    strcpy(contact.name, argv[4]);
    strcpy(contact.lName, argv[5]);
    strcpy(contact.department, argv[6]);

    if(contact.id > 0) {
        fseek(fPtr, sizeof(Contact) * (contact.id - 1), SEEK_SET);
        fwrite(&contact, sizeof(Contact), 1, fPtr);
    } else {
        printf("Kisi kimligi 0'dan buyuk olmalidir.\n");
        exit(EXIT_FAILURE);
    }
    return fPtr;
}

FILE* listContacts(FILE *fPtr) {
    Contact contact;
    rewind(fPtr);

    printf("Your phone book:\n");
    printf("--------------\n");

    while(fread(&contact, sizeof(Contact), 1, fPtr)) {
        if(contact.id > 0) {
            printf("Id: %d\nInline Phone: %d\nName: %s\nLast Name: %s\nDepartment: %s\n--------------\n",
                    contact.id, contact.inlinePhone, contact.name, contact.lName, contact.department);
        }
    }
    return fPtr;
}

FILE* exportContacts(FILE* fPtr, const char* filename) {
    FILE* outFile = fopen(filename, "w");
    if(outFile == NULL) {
        printf("Dışa aktarma dosyasi acilmadi.\n");
        exit(EXIT_FAILURE);
    }

    Contact contact;
    rewind(fPtr);

    fprintf(outFile, "Inline Phone,Name,Last Name,Department\n");

    while(fread(&contact, sizeof(Contact), 1, fPtr)) {
        if(contact.id > 0) {
            fprintf(outFile, "%d,%s,%s,%s\n", contact.inlinePhone, contact.name, contact.lName, contact.department);
        }
    }

    fclose(outFile);
    return fPtr;
}

FILE* searchContacts(FILE* fPtr, char option, const char* searchTerm) {
    Contact contact;
    int found = 0;

    rewind(fPtr);

    while(fread(&contact, sizeof(Contact), 1, fPtr)) {
        if (contact.id > 0) {
            switch(option) {
                case 'n':
                    if (strcmp(contact.name, searchTerm) == 0) {
                        found = 1;
                        printf("Id: %d\nInline phone: %d\nName: %s\nLast Name: %s\nDepartment: %s\n",
                               contact.id, contact.inlinePhone, contact.name, contact.lName, contact.department);
                    }
                    break;
                case 'l':
                    if (strcmp(contact.lName, searchTerm) == 0) {
                        found = 1;
                        printf("Id: %d\nInline phone: %d\nName: %s\nLast Name: %s\nDepartment: %s\n",
                               contact.id, contact.inlinePhone, contact.name, contact.lName, contact.department);
                    }
                    break;
                case 'p':
                    if (contact.inlinePhone == atoi(searchTerm)) {
                        found = 1;
                        printf("Id: %d\nInline phone: %d\nName: %s\nLast Name: %s\nDepartment: %s\n",
                               contact.id, contact.inlinePhone, contact.name, contact.lName, contact.department);
                    }
                    break;
                case 'd':
                    if (strcmp(contact.department, searchTerm) == 0) {
                        found = 1;
                        printf("Id: %d\nInline phone: %d\nName: %s\nLast Name: %s\nDepartment: %s\n",
                               contact.id, contact.inlinePhone, contact.name, contact.lName, contact.department);
                    }
                    break;
                default:
                    printf("Geçersiz arama kriteri.\n");
                    return fPtr;
            }
        }
    }

    if (!found) {
        printf("Belirtilen kriterle eslesen kisi bulunamadi.\n");
    }

    return fPtr;
}


FILE* updateContact(FILE* fPtr, int inlinePhone, const char* newName, const char* newLastName, const char* newDepartment) {
    Contact contact;
    int found = 0;
    rewind(fPtr);

    while(fread(&contact, sizeof(Contact), 1, fPtr)) {
        if(contact.id > 0 && contact.inlinePhone == inlinePhone) {
            found = 1;
            printf("Old name: %s\n", contact.name);
            printf("Enter new name: ");
            scanf("%s", contact.name);
            printf("Old Last name: %s\n", contact.lName);
            printf("Enter new last name: ");
            scanf("%s", contact.lName);
            printf("Old department: %s\n", contact.department);
            printf("Enter new department: ");
            scanf("%s", contact.department);

            fseek(fPtr, -(int)sizeof(Contact), SEEK_CUR);
            fwrite(&contact, sizeof(Contact), 1, fPtr);
            printf("Kisi guncellendi.\n");
            break;
        }
    }

    if (!found) {
        printf("Kisi bulunamadic.\n");
    }
    return fPtr;
}


FILE* deleteContact(FILE* fPtr, int inlinePhone) {
    Contact contact;
    int found = 0;
    rewind(fPtr);

    while(fread(&contact, sizeof(Contact), 1, fPtr)) {
        if(contact.id > 0 && contact.inlinePhone == inlinePhone) {
            found = 1;
            contact.id = 0;
            fseek(fPtr, -(int)sizeof(Contact), SEEK_CUR);
            fwrite(&contact, sizeof(Contact), 1, fPtr);
            printf("Kisi silindi.\n");
            break;
        }
    }

    if(!found) {
        printf("Kisi bulunamadi.\n");
    }
    return fPtr;
}

