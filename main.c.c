#include <stdio.h>
#include <string.h>
#include <time.h>

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

void saveProducts(struct Product products[], int count, float totalSales) {

    FILE *fp;

    fp = fopen("products.txt", "w");

    if(fp == NULL) {
        printf("Error saving products!\n");
        return;
    }

    fprintf(fp, "%.2f\n", totalSales);

    for(int i = 0; i < count; i++) {

        fprintf(fp, "%d|%s|%.2f|%d\n",
                products[i].id,
                products[i].name,
                products[i].price,
                products[i].quantity);
    }

    fclose(fp);
}

int loadProducts(struct Product products[], float *totalSales) {

    FILE *fp = fopen("products.txt", "r");
    int count = 0;

    if(fp == NULL) {
        *totalSales = 0;
        return 0;
    }

    if(fscanf(fp, "%f", totalSales) != 1) {
        fclose(fp);
        *totalSales = 0;
        return 0;
    }

    while(count < 100 &&
          fscanf(fp, "%d|%49[^|]|%f|%d",
                 &products[count].id,
                 products[count].name,
                 &products[count].price,
                 &products[count].quantity) == 4) {
        count++;
    }

    fclose(fp);
    return count;
}



int main() {

    struct Product products[100];
    int count;
    int choice;
    float totalSales = 0;
    count = loadProducts(products, &totalSales);

printf("\n%d products loaded successfully!\n", count);

    do {

        printf("\n===== SHOP MANAGEMENT SYSTEM =====\n");

        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Search Product\n");
        printf("4. Update Stock\n");
        printf("5. Generate Bill\n");
        printf("6. Sales Report\n");
        printf("7. Exit\n");
        printf("8. View Transaction History\n");
        printf("9. Delete Product\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {


case 1: {

    int newID;
    int newQuantity;
    float newPrice;
    int found = 0;

    if(count >= 100) {
        printf("\nInventory is Full!\n");
        break;
    }

    printf("\nEnter Product ID: ");

    if(scanf("%d", &newID) != 1) {
        printf("Invalid ID!\n");
        while(getchar() != '\n');
        break;
    }

    if(newID <= 0) {
        printf("Product ID must be positive!\n");
        break;
    }

    for(int i = 0; i < count; i++) {
        if(products[i].id == newID) {
            found = 1;
            break;
        }
    }

    if(found) {
        printf("Product ID already exists!\n");
        break;
    }

    printf("Enter Product Name: ");
    scanf(" %49[^\n]", products[count].name);

    printf("Enter Product Price: ");

    if(scanf("%f", &newPrice) != 1 || newPrice <= 0) {
        printf("Invalid Price! Price must be positive.\n");
        while(getchar() != '\n');
        break;
    }

    printf("Enter Product Quantity: ");

    if(scanf("%d", &newQuantity) != 1 || newQuantity < 0) {
        printf("Invalid Quantity!\n");
        while(getchar() != '\n');
        break;
    }

    products[count].id = newID;
    products[count].price = newPrice;
    products[count].quantity = newQuantity;

    count++;

    printf("\nProduct Added Successfully!\n");

    break;
}


            case 2:

if(count == 0) {

        printf("\nNo Products Available!\n");

    }

    else {

        printf("\n========== PRODUCT LIST ==========\n");

        printf("%-10s %-15s %-10s %-10s\n",
               "ID", "NAME", "PRICE", "QUANTITY");

        printf("---------------------------------------------\n");

        for(int i = 0; i < count; i++) {

            printf("%-10d %-15s %-10.2f %-10d\n",
                   products[i].id,
                   products[i].name,
                   products[i].price,
                   products[i].quantity);

        }
        printf("\n========== LOW STOCK ALERTS ==========\n");

int lowStockFound = 0;

for(int i = 0; i < count; i++) {

    if(products[i].quantity <= 5) {

        printf("WARNING: %s has only %d units left!\n",
               products[i].name,
               products[i].quantity);

        lowStockFound = 1;
    }
}

if(lowStockFound == 0) {
    printf("All products have sufficient stock.\n");
}

        printf("---------------------------------------------\n");

    }

    break;

            case 3:{

    int searchID;
    int found = 0;

    printf("\nEnter Product ID to Search: ");
    scanf("%d", &searchID);

    for(int i = 0; i < count; i++) {

        if(products[i].id == searchID) {

            printf("\n===== PRODUCT FOUND =====\n");
            printf("Product ID: %d\n", products[i].id);
            printf("Product Name: %s\n", products[i].name);
            printf("Product Price: %.2f\n", products[i].price);
            printf("Product Quantity: %d\n", products[i].quantity);

            found = 1;
            break;
        }
    }

    if(found == 0) {
        printf("\nProduct Not Found!\n");
    }

    break;
}


case 4: {

    int updateID;
    int found = 0;
    int newQuantity;
    float newPrice;

    printf("\nEnter Product ID to Update: ");

    if(scanf("%d", &updateID) != 1) {
        printf("Invalid Product ID!\n");
        while(getchar() != '\n');
        break;
    }

    for(int i = 0; i < count; i++) {

        if(products[i].id == updateID) {

            found = 1;

            printf("\nProduct Found: %s\n", products[i].name);

            printf("Current Quantity: %d\n", products[i].quantity);
            printf("Enter New Quantity: ");

            if(scanf("%d", &newQuantity) != 1 || newQuantity < 0) {
                printf("Invalid Quantity!\n");
                while(getchar() != '\n');
                break;
            }

            printf("Current Price: %.2f\n", products[i].price);
            printf("Enter New Price: ");

            if(scanf("%f", &newPrice) != 1 || newPrice <= 0) {
                printf("Invalid Price!\n");
                while(getchar() != '\n');
                break;
            }

            products[i].quantity = newQuantity;
            products[i].price = newPrice;

            printf("\nProduct Updated Successfully!\n");

            break;
        }
    }

    if(found == 0) {
        printf("\nProduct Not Found!\n");
    }

    break;
}

case 5: {

    int billID, billQty;
    int found;
    int purchased[100] = {0};
    float billTotal = 0;

    printf("\n===== GENERATE BILL =====\n");
    printf("Enter 0 as Product ID to finish billing.\n");

    while(1) {

        printf("\nEnter Product ID: ");

if(scanf("%d", &billID) != 1) {
    printf("Invalid Product ID!\n");
    while(getchar() != '\n');
    continue;
}

if(billID < 0) {
    printf("Invalid Product ID!\n");
    continue;
}

        if(billID == 0) {
            break;
        }

        found = 0;

        for(int i = 0; i < count; i++) {

            if(products[i].id == billID) {

                found = 1;

                printf("Product: %s\n", products[i].name);
                printf("Price: %.2f\n", products[i].price);
                printf("Available Stock: %d\n", products[i].quantity);

                printf("Enter Quantity: ");

if(scanf("%d", &billQty) != 1) {
    printf("Invalid Quantity!\n");
    while(getchar() != '\n');
    break;
}

                if(billQty <= 0) {
                    printf("Invalid Quantity!\n");
                }

                else if(billQty > products[i].quantity) {
                    printf("Insufficient Stock!\n");
                }

                else {

                    float itemTotal = products[i].price * billQty;

                    purchased[i] += billQty;
                    billTotal += itemTotal;
                    totalSales += itemTotal;

                    products[i].quantity -= billQty;

                    printf("Added to bill successfully!\n");
                }

                break;
            }
        }

        if(found == 0) {
            printf("Product Not Found!\n");
        }
    }
    for(int i = 0; i < count; i++) {

        if(purchased[i] > 0) {

            printf("%-22s %-8d %-12.2f %-12.2f\n",
                   products[i].name,
                   purchased[i],
                   products[i].price,
                   products[i].price * purchased[i]);
        }
    }

    printf("---------------------------------------------\n");

    printf("GRAND TOTAL: Rs. %.2f\n", billTotal);

    printf("=============================================\n");


if(billTotal > 0) {

    FILE *history = fopen("sales_history.txt", "a");

    if(history == NULL) {
        printf("Error saving transaction history!\n");
    }

    else {

        time_t now = time(NULL);

fprintf(history, "\n========== NEW TRANSACTION ==========\n");

fprintf(history, "Transaction ID: %lld\n", (long long)now);

fprintf(history, "Date: %s", ctime(&now));

        for(int i = 0; i < count; i++) {

            if(purchased[i] > 0) {

                fprintf(history,
                        "Product: %s | Quantity: %d | Price: %.2f | Total: %.2f\n",
                        products[i].name,
                        purchased[i],
                        products[i].price,
                        products[i].price * purchased[i]);
            }
        }

        fprintf(history, "Grand Total: Rs. %.2f\n", billTotal);
        fprintf(history, "=====================================\n");

        fclose(history);

FILE *daily = fopen("daily_sales.txt", "a");

if(daily != NULL) {

    time_t now = time(NULL);
    struct tm *dateInfo = localtime(&now);

    char date[11];

    strftime(date, sizeof(date), "%Y-%m-%d", dateInfo);

    int totalItems = 0;

    for(int i = 0; i < count; i++) {
        totalItems += purchased[i];
    }

    fprintf(daily, "%s|%d|%.2f\n",
            date, totalItems, billTotal);

    fclose(daily);
}


        printf("Transaction saved successfully!\n");
    }
}


    break;
}


    printf("\n========== CUSTOMER BILL ==========\n");
    time_t billTime = time(NULL);

printf("Transaction ID: %lld\n", (long long)billTime);

    printf("%-22s %-8s %-12s %-12s\n",
           "PRODUCT", "QTY", "PRICE", "TOTAL");

case 6: {

    FILE *daily;
    char date[11];
    char today[11];

    int items;
    int totalItemsToday = 0;
    int transactionsToday = 0;

    float revenue;
    float todayRevenue = 0;

    time_t now = time(NULL);
    struct tm *dateInfo = localtime(&now);

    strftime(today, sizeof(today), "%Y-%m-%d", dateInfo);

    daily = fopen("daily_sales.txt", "r");

    if(daily == NULL) {
        printf("\nNo Daily Sales Records Found!\n");
    }

    else {

        while(fscanf(daily, "%10[^|]|%d|%f\n",
                     date, &items, &revenue) == 3) {

            if(strcmp(date, today) == 0) {

                todayRevenue += revenue;
                totalItemsToday += items;
                transactionsToday++;
            }
        }

        fclose(daily);

        printf("\n========== SALES REPORT ==========\n");

        printf("Today's Date: %s\n", today);

        printf("Today's Revenue: Rs. %.2f\n", todayRevenue);

        printf("Transactions Today: %d\n", transactionsToday);

        printf("Products Sold Today: %d\n", totalItemsToday);

        printf("Overall Lifetime Revenue: Rs. %.2f\n", totalSales);

        printf("==================================\n");
    }

    break;
}


            case 7:

                printf("\nExiting Program...\n");
                break;


            case 8: {

    FILE *history;
    char line[256];

    history = fopen("sales_history.txt", "r");

    if(history == NULL) {
        printf("\nNo Transaction History Found!\n");
    }

    else {

        printf("\n========== TRANSACTION HISTORY ==========\n");

        while(fgets(line, sizeof(line), history) != NULL) {
            printf("%s", line);
        }

        printf("\n=========================================\n");

        fclose(history);
    }

    break;
}

            case 9: {

    int deleteID;
    int found = 0;

    printf("\nEnter Product ID to Delete: ");
    scanf("%d", &deleteID);

    for(int i = 0; i < count; i++) {

        if(products[i].id == deleteID) {

            found = 1;

            printf("\nProduct Found: %s\n", products[i].name);

            printf("Are you sure you want to delete it? (1 = Yes, 0 = No): ");

            int confirm;
            scanf("%d", &confirm);

            if(confirm == 1) {

                for(int j = i; j < count - 1; j++) {
                    products[j] = products[j + 1];
                }

                count--;

                printf("\nProduct Deleted Successfully!\n");
            }

            else {
                printf("\nDeletion Cancelled!\n");
            }

            break;
        }
    }

    if(found == 0) {
        printf("\nProduct Not Found!\n");
    }

    break;
}



            default:

                printf("\nInvalid Choice! Try Again.\n");

        }
        saveProducts(products, count, totalSales);

    } while(choice != 7);

    return 0;
}

