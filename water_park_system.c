#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function prototypes
void administrator();
void customer_menu();
void register_new_user();

void delete_user();
void check_reservation();
void customer_update_ticket();
int generate_reservation_number();

// Entry point
int main() {
    int choice;

    do {
        printf("\n==== Water Park Main Menu ====\n");
        printf("1. Administrator\n");
        printf("2. Customer\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                administrator();
                break;
            case 2:
                customer_menu();
                break;
            case 0:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}

// -------------------- ADMIN SECTION --------------------

void administrator() {
    int choice;
    do {
        printf("\n--- Administrator Menu ---\n");
        printf("1. Register new user\n");
        printf("2. Update user\n");
        printf("3. Delete user\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: register_new_user(); break;
            case 2: customer_update_ticket(); break;
            case 3: delete_user(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

// -------------------- CUSTOMER SECTION --------------------

void customer_menu() {
    int choice;
    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. Check Reservation\n");
        printf("2. Update Ticket\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: check_reservation(); break;
            case 2: customer_update_ticket(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

int generate_reservation_number() {
    srand(time(NULL));
    return rand() % 1000000 + 100000; // 6-digit number
}

void register_new_user() {
    char name[100], email[100], password[100];
    int age, user_type;
    float ticket_price = 0.0;
    FILE *file;

    printf("Register as:\n1. Customer (Buy Ticket)\n2. Staff\nChoice: ");
    scanf("%d", &user_type);

    printf("Enter name: ");
    scanf(" %[^\n]", name);
    printf("Enter age: ");
    scanf("%d", &age);
    printf("Enter email: ");
    scanf(" %[^\n]", email);
    printf("Create password: ");
    scanf(" %[^\n]", password);

    if (user_type == 1) {
        int reservation_number = generate_reservation_number();
        if (age < 12) ticket_price = 15.0;
        else if (age < 60) ticket_price = 25.0;
        else ticket_price = 10.0;

        file = fopen("customer.txt", "a");
        if (file == NULL) {
            printf("File error!\n");
            return;
        }
        fprintf(file, "%d,%s,%d,%s,%s,%.2f\n", reservation_number, name, age, email, password, ticket_price);
        fclose(file);

        printf("Ticket bought successfully!\n");
        printf("Reservation Number: %d | Name: %s | Price: RM%.2f\n", reservation_number, name, ticket_price);
    } else if (user_type == 2) {
        file = fopen("staff.txt", "a");
        if (file == NULL) {
            printf("File error!\n");
            return;
        }
        fprintf(file, "%s,%d,%s,%s\n", name, age, email, password);
        fclose(file);

        printf("Staff registered successfully!\n");
    } else {
        printf("Invalid user type!\n");
    }
}

void check_reservation() {
    int reservation;
    char line[256], name[100], email[100], password[100];
    int age, res_no;
    float ticket;

    printf("Enter your reservation number: ");
    scanf("%d", &reservation);

    FILE *file = fopen("customer.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%99[^,],%d,%99[^,],%99[^,],%f", &res_no, name, &age, email, password, &ticket);
        if (res_no == reservation) {
            printf("\n--- Reservation Details ---\n");
            printf("Reservation #: %d\nName: %s\nAge: %d\nEmail: %s\nPrice: RM%.2f\n", res_no, name, age, email, ticket);
            found = 1;
            break;
        }
    }

    if (!found) printf("Reservation not found.\n");
    fclose(file);
}

void customer_update_ticket() {
    int reservation;
    char line[256], name[100], email[100], password[100];
    int age, res_no;
    float ticket;
    int found = 0;

    printf("Enter your reservation number: ");
    scanf("%d", &reservation);

    FILE *file = fopen("customer.txt", "r");
    FILE *temp = fopen("temp_customer.txt", "w");
    if (!file || !temp) {
        printf("File error!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%99[^,],%d,%99[^,],%99[^,],%f", &res_no, name, &age, email, password, &ticket);

        if (res_no == reservation) {
            found = 1;
            printf("\n--- Current Ticket Info ---\n");
            printf("Reservation #: %d\nName: %s\nAge: %d\nEmail: %s\nPrice: RM%.2f\n", res_no, name, age, email, ticket);

            int update_choice;
            printf("\nWhich field do you want to update?\n");
            printf("1. Name\n2. Age\n3. Email\n4. Password\nChoice: ");
            scanf("%d", &update_choice);

            switch (update_choice) {
                case 1:
                    printf("Enter new name: ");
                    scanf(" %[^\n]", name);
                    break;
                case 2:
                    printf("Enter new age: ");
                    scanf("%d", &age);
                    if (age < 12) ticket = 15.0;
                    else if (age < 60) ticket = 25.0;
                    else ticket = 10.0;
                    break;
                case 3:
                    printf("Enter new email: ");
                    scanf(" %[^\n]", email);
                    break;
                case 4:
                    printf("Enter new password: ");
                    scanf(" %[^\n]", password);
                    break;
                default:
                    printf("Invalid option. No changes made.\n");
            }

            fprintf(temp, "%d,%s,%d,%s,%s,%.2f\n", res_no, name, age, email, password, ticket);
            printf("Ticket updated successfully!\n");
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("customer.txt");
        rename("temp_customer.txt", "customer.txt");
    } else {
        remove("temp_customer.txt");
        printf("Reservation not found!\n");
    }
}

void delete_user() {
    char name[100];
    int user_type;
    char line[256], current_name[100], email[100], password[100];
    int age;
    float ticket;
    int res_no;
    int found = 0;

    printf("Delete from:\n1. Customer\n2. Staff\nChoice: ");
    scanf("%d", &user_type);
    printf("Enter name to delete: ");
    scanf(" %[^\n]", name);

    FILE *file = fopen(user_type == 1 ? "customer.txt" : "staff.txt", "r");
    FILE *temp = fopen(user_type == 1 ? "temp_customer.txt" : "temp_staff.txt", "w");

    if (!file || !temp) {
        printf("File error!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (user_type == 1)
            sscanf(line, "%d,%99[^,],%d,%99[^,],%99[^,],%f", &res_no, current_name, &age, email, password, &ticket);
        else
            sscanf(line, "%99[^,],%d,%99[^,],%99[^\n]", current_name, &age, email, password);

        if (strcmp(current_name, name) == 0) {
            found = 1;
            printf("User %s deleted.\n", current_name);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(user_type == 1 ? "customer.txt" : "staff.txt");
        rename(user_type == 1 ? "temp_customer.txt" : "temp_staff.txt", user_type == 1 ? "customer.txt" : "staff.txt");
    } else {
        remove(user_type == 1 ? "temp_customer.txt" : "temp_staff.txt");
        printf("User not found.\n");
    }
}
