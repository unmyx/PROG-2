#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

void sortItems(ITEM items[], int n, char type) {
    if (type == 'a') {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (items[j].price > items[j + 1].price) {
                    ITEM temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }

            }
        }
    } else if (type == 'd') {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (items[j].price < items[j + 1].price) {
                    ITEM temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }

            }
        }
    } else {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (strcmp(items[j].name, items[j+1].name) > 0) {
                    ITEM temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }

            }
        }
    }
}
int main(int argc, char *argv[])    {

    int W = 0,  N = 0, E = 0, A = 0, D = 0;
    char name[1001], item_name[1001], item_name2[1001];
    double lat, lon;
    int max_price;

    // switch validation - error conditions
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'w') {
                W = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    return 2;
                }
                else if (i+1 < argc) {
                    strcpy(name, argv[i+1]);
                    i+=1;
                }
                else return 2;
            }


            else if (argv[i][1] == 'n') {
                N = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    return 2;
                }
                else if (i+1 < argc){
                    lat = atof(argv[i+1]);
                    if (lat < -90 || lat > 90 ) {
                        // printf("return 4");
                        return 4;
                    }
                    i += 1;
                }
                else return 2;

            }
            else if (argv[i][1] == 'e') {
                E = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    return 2;
                }
                else if (i+1 < argc){
                    lon = atof(argv[i+1]);
                    if (lon < -180 || lon > 180) {
                        // printf("return 4");
                        return 4;
                    }
                    i += 1;
                }
                else return 2;

            }
            else if (argv[i][1] == 'a') {
                A = 1;
            }
            else if (argv[i][1] == 'd') {
                D = 1;
            }


            else {
                return 1;
            }
        }
    }
    if (N==1 && E ==0) {
        return 3;
    }
    if (N==0 && E==1) {
        return 3;
    }
    // printf("W: %d, Name: %s,  N: %f, E: %f, A: %d, D: %d\n", W,name,lat,lon, A,D);
    WAREHOUSE warehouse[1001];
    int wc = 0;
    FILE* file;
    char path[101] = "";
    //strcat(path,PATH_SEPARATOR);
    strcat(path,WAREHOUSE_DB_FILE);
    path[strlen(path)] = '\0';

    //printf("%s\n", path);
    file = fopen(path,"r");

    if (file == NULL) {
        printf("WAREHOUSE FILE NOT FOUND");
        return 0;
    }

    char buf[1001];
    while(fgets(buf, 101, file)) {
        char * token = strtok(buf, " \n");
        int i = 0;
        while( token != NULL ) {
            if (i == 0) {
                strcpy(warehouse[wc].name, token);
                i += 1;
            } else if (i == 1) {
                warehouse[wc].gps.lat = atof(token);
                i += 1;
            } else if (i == 2) {
                warehouse[wc].gps.lon = atof(token);
                i += 1;
            } else if (i == 3) {
                warehouse[wc].capacity = atoi(token);
                i += 1;
            }
            token = strtok(NULL, " \n");
        }
        wc += 1;
    }
    for (int i = 0; i < wc; i++) {
        printf("%s %.3f %.3f %d\n", warehouse[i].name, warehouse[i].gps.lat, warehouse[i].gps.lon, warehouse[i].capacity);
    }
    if (W == 1 || (N==1 && E==1)) {
        // show goods of only filtered warehouses
        WAREHOUSE warehouse_filter[1001];
        int wfc = 0;
        for (int i = 0; i < wc; i++) {
            if (strcmp(warehouse[i].name, name) == 0) {
                warehouse_filter[wfc++] = warehouse[i];
                break;
            }

        }
        if (N==1 && E == 1) {
            int ind = -1;
            double d = 1000001.0;
            GPS g;
            g.lat = lat; g.lon = lon;
            for (int i = 0; i < wc; i++) {
                double cur = distance(g, warehouse[i].gps);
                if (cur < d) {
                    d = cur;
                    ind = i;
                }
            }
            if (ind != -1)
                warehouse_filter[wfc++] = warehouse[ind];
        }

        for (int i = 0; i < wfc; i++) {

            char path2[101] = "";
            strcat(path2,ITEMS_FOLDER);
            strcat(path2,PATH_SEPARATOR);
            strcat(path2,warehouse_filter[i].name);
            strcat(path2,".txt");

            path2[strlen(path2)] = '\0';
            // printf("%s\n", path2);
            FILE *goods = fopen(path2,"r");
            if (!goods){
                fprintf( stderr, "FILE_ERROR %s.txt\n",warehouse_filter[i].name );
                continue;
            }
            ITEM items[warehouse_filter[i].capacity+1];
            int ic = 0;
            int cap_err = 0, file_err = 0, format_err = 0;
            while(fgets(buf, 101, goods)) {
                char * token = strtok(buf, " \n");
                int j = 0;
                while( token != NULL ) {
                    if (j == 0) {
                        if (strlen(token) == 0)
                            format_err = 1;
                        for (int k = 0; k < strlen(token); k++) {
                            if (token[k] < 'a' || token[k] > 'z') {
                                format_err = 1;
                                break;
                            }
                        }
                        if (format_err )
                            break;
                        strcpy(items[ic].name, token);
                        j += 1;
                    } else if (j == 1) {
                        if (strlen(token) == 0)
                            format_err = 1;
                        for (int k = 0; k < strlen(token); k++) {
                            if (token[k] < '0' || token[k] > '9') {
                                format_err = 1;
                                break;
                            }
                        }
                        if (format_err )
                            break;
                        items[ic].price = atoi(token);
                        j += 1;
                    }
                    token = strtok(NULL, " \n");
                }
                if (j != 2 || format_err == 1) {
                    format_err = 1;
                    fprintf( stderr, "FORMAT_ERROR %s.txt\n",warehouse_filter[i].name );
                    break;
                }
                ic += 1;
                if (ic > warehouse_filter[i].capacity) {
                    cap_err = 1;
                    fprintf( stderr, "CAPACITY_ERROR %s.txt\n",warehouse_filter[i].name );
                    break;
                }
            }
            fclose(goods);
            if (cap_err == 1 || format_err == 1)
                continue;

            printf("%s %.3f %.3f %d :\n", warehouse_filter[i].name, warehouse_filter[i].gps.lat, warehouse_filter[i].gps.lon, warehouse_filter[i].capacity);

            if (A == 1) {
                sortItems(items, ic, 'a');
            }
            else if (D == 1) {
                sortItems(items, ic, 'd');
            }
            else {
                sortItems(items, ic, 'n');
            }
            for (int j = 0; j < ic; j++) {
                printf("%d. %s %d\n", (j+1), items[j].name, items[j].price);
            }

        }
    }
    else {
        // show all goods of each warehouse
        for (int i = 0; i < wc; i++) {
            char path2[101] = "";
            strcat(path2,ITEMS_FOLDER);
            strcat(path2,PATH_SEPARATOR);
            strcat(path2,warehouse[i].name);
            strcat(path2,".txt");
            path2[strlen(path2)] = '\0';

            // printf("%s\n", path2);
            FILE *goods = fopen(path2,"r");
            if (!goods){
                fprintf( stderr, "FILE_ERROR %s.txt\n",warehouse[i].name );
                continue;
            }
            ITEM items[warehouse[i].capacity+1];
            int ic = 0;
            int cap_err = 0, file_err = 0, format_err = 0;
            while(fgets(buf, 101, goods)) {
                char * token = strtok(buf, " \n");
                int j = 0;
                while( token != NULL ) {
                    if (j == 0) {
                        if (strlen(token) == 0)
                            format_err = 1;
                        for (int k = 0; k < strlen(token); k++) {
                            if (token[k] < 'a' || token[k] > 'z') {
                                format_err = 1;
                                break;
                            }
                        }
                        if (format_err )
                            break;
                        strcpy(items[ic].name, token);
                        j += 1;
                    } else if (j == 1) {
                        if (strlen(token) == 0)
                            format_err = 1;
                        for (int k = 0; k < strlen(token); k++) {
                            if (token[k] < '0' || token[k] > '9') {
                                format_err = 1;
                                break;
                            }
                        }
                        if (format_err )
                            break;
                        items[ic].price = atoi(token);
                        j += 1;
                    }
                    token = strtok(NULL, " \n");
                }
                if (j != 2 || format_err == 1) {
                    format_err = 1;
                    fprintf( stderr, "FORMAT_ERROR %s.txt\n",warehouse[i].name );
                    break;
                }
                ic += 1;
                if (ic > warehouse[i].capacity) {
                    cap_err = 1;
                    fprintf( stderr, "CAPACITY_ERROR %s.txt\n",warehouse[i].name );
                    break;
                }
            }

            if (cap_err == 1 || format_err == 1)
                continue;
            printf("%s %.3f %.3f %d :\n", warehouse[i].name, warehouse[i].gps.lat, warehouse[i].gps.lon, warehouse[i].capacity);
            if (A == 1) {
                sortItems(items, ic, 'a');
            }
            else if (D == 1) {
                sortItems(items, ic, 'd');
            }
            else {
                sortItems(items, ic, 'n');
            }
            for (int j = 0; j < ic; j++) {
                printf("%d. %s %d\n", (j+1), items[j].name, items[j].price);
            }

        }
    }
    return 0;
}