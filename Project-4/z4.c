#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "config.h"


#define pi 3.14159265358979323846

double degToRad(double deg) {
    return (deg * pi / 180);
}

double rad2deg(double rad) {
    return (rad * 180 / pi);
}

double mahalnobis(double lat1, double lon1, double lat2, double lon2) {
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    }
    else {
        theta = lon1 - lon2;
        dist = sin(degToRad(lat1)) * sin(degToRad(lat2)) + cos(degToRad(lat1)) * cos(degToRad(lat2)) * cos(degToRad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.1515;
        return (dist);
    }
}



int search_warehouse(WAREHOUSE w[], int ind, char *name) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(w[ind].items[i].name, name) == 0 ) {
            return 1;
        }

    }
    return 0;
}

int valid(char *s) {
    // printf("%s\n", s);
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '-' || s[i]=='.')
            continue;
        if (s[i] < '0' || s[i] >'9')
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]){



    int W = 0, I = 0, N = 0, E = 0, T = 0, P = 0, WF=0;
    char name[1001], item_name[1001], item_name2[1001];
    double lat, lon;
    int max_price;

    // switch validation - error conditions
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'w') {
                W = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc) {
                    strcpy(name, argv[i+1]);
                    i+=1;
                }
                else return 2;
            }
            else if (argv[i][1] == 'i') {
                I = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc) {
                    strcpy(item_name, argv[i+1]);
                    i+=1;
                }
                else return 2;

            }
            else if (argv[i][1] == 'W') {
                WF = 1;


            }
            else if (argv[i][1] == 'n') {
                N = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc){
                    if (!valid(argv[i+1])) {
                        // printf("return 4");
                        return 4;
                    }
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
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc){
                    if (!valid(argv[i+1])) {
                        // printf("return 4");
                        return 4;
                    }
                    lon = atof(argv[i+1]);
                    if (lon < -180 || lon > 180) {
                        // printf("return 4");
                        return 4;
                    }
                    i += 1;
                }
                else return 2;

            }
            else if (argv[i][1] == 't') {
                T = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc) {
                    strcpy(item_name2, argv[i+1]);
                    i+=1;
                }
                else return 2;

            }
            else if (argv[i][1] == 'p') {
                P = 1;
                if (i+1 < argc && argv[i+1][0] == '-') {
                    // mandatory param nor provided
                    // printf("return 2");
                    return 2;
                }
                else if (i+1 < argc) {
                    max_price = atoi(argv[i+1]);
                    i+=1;
                }
                else return 2;

            }
            else {
                // invalid switch
                // printf("return 1");
                return 1;
            }
        }
    }
    if (N==1 && E ==0) {
        // printf("return 3");
        return 3;
    }
    if (N==0 && E==1) {
        // printf("return 3");
        return 3;
    }

    // printf("W: %d, Name: %s, I: %d, Item-Name: %s, N: %f, E: %f\n", W,name,I,item_name,lat,lon);
    WAREHOUSE w_filter[DB_NUM];
    int wc = 0;

    int nearest = -1;
    float m = 10000001;
    if (N==1 && E==1) {
        GPS gps;
        gps.lat = lat; gps.lon = lon;
        for (int i = 0; i < DB_NUM; i++) {
            if (mahalnobis(lat, lon, db[i].gps.lat, db[i].gps.lon) < m) {
                m = mahalnobis(lat, lon, db[i].gps.lat, db[i].gps.lon);
                nearest = i;
            }


        }
    }

    for (int i = 0; i < DB_NUM; i++) {
        if (W == 1) {
            // check if current warehouse matches with the name
            if (strcmp(db[i].name, name) == 0) {
                w_filter[wc] = db[i];
                wc += 1;
            }
        }
        else if (I == 1) {
            // check if current warehouse contains any item with item_name
            if (search_warehouse(db, i, item_name))  {
                w_filter[wc] = db[i];
                wc += 1;
            }
        }
        else if (nearest == i) {
            // check nearest warehouse to provided coords
            w_filter[wc] = db[i];
            wc += 1;
        }
        else if (W==0 && I==0 && N==0 && E==0) {
            w_filter[wc] = db[i];
            wc += 1;
        }
    }

    // printf("%d\n",wc);
    int flag = 0;
    if (T || P) {
        flag = 1;
        int l = 0;
        if (WF == 0) {
            for (int i = 0; i < wc; i++) {
                if (P == 1) {
                    for (int j = 0; j < w_filter[i].n; j++) {
                        if (w_filter[i].items[j].price <= max_price) {
                            printf("%d. %s %d : %s %.3f %.3f %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price, w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);

                        }
                    }
                }
                if (T == 1) {
                    for (int j = 0; j < w_filter[i].n; j++) {
                        if ( strcmp( w_filter[i].items[j].name, item_name2) == 0 ) {
                            printf("%d. %s %d : %s %.3f %.3f %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price, w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);

                        }
                    }
                }
            }
        }

        else {
            for (int i = 0; i < wc; i++) {
                if (P == 1) {

                    int l = 0;
                    printf("%s %.3f %.3f %d :\n", w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);
                    for (int j = 0; j < w_filter[i].n; j++) {
                        if (w_filter[i].items[j].price <= max_price) {
                            printf("%d. %s %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price);

                        }
                    }


                }
                if (T == 1) {

                    int l = 0;
                    printf("%s %.3f %.3f %d :\n", w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);
                    for (int j = 0; j < w_filter[i].n; j++) {
                        if ( strcmp( w_filter[i].items[j].name, item_name2) == 0 ) {
                            printf("%d. %s %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price);
                        }
                    }



                }
            }
        }

    }

    if(flag == 0) {
        if (WF ==0) {
            int l = 0;
            for (int i = 0; i < wc; i++) {
                for (int j = 0; j < w_filter[i].n; j++) {
                    printf("%d. %s %d : %s %.3f %.3f %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price, w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);

                }
            }
        }
        else {



            for (int i = 0; i < wc; i++) {
                int l = 0;
                printf("%s %.3f %.3f %d :\n", w_filter[i].name, w_filter[i].gps.lat, w_filter[i].gps.lon, w_filter[i].n);
                for (int j = 0; j < w_filter[i].n; j++) {
                    printf("%d. %s %d\n", (++l), w_filter[i].items[j].name, w_filter[i].items[j].price);
                }
            }
        }


    }
    return 0;
}