#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <ctype.h>
#include<string.h>

char *tokens[10001];
int ct;
void tokenize(char *str) {

}
int main(int argc, char **argv ) {
	ct = 0;
	int numSwitch = 6;
	char *switches[] = {"-l", "-u", "-d", "-p", "-r", "-R"};

	// input contains valid switches from the CMD args
	char input[10];
	int ci = 0;
	
	int D = 0, P = 0, L = 0, U = 0, r = 0, R = 0;
	char replace_string[1001];
	char non[101][101];
	int cn = 0;
	for (int i = 1; i < argc; i++) {
		// printf("%s\n", argv[i]);
		int valid = 0;
		int is_switch = 0;
		for (int j = 0; j < numSwitch; j++) {
			// printf("%s ", switches[j]);
			if (argv[i][0] == '-') {
				is_switch = 1;
				if (strcmp(argv[i], switches[j]) == 0) {
					valid = 1;
					break;
				}
			}
			
		}
		if (is_switch) {
			if (valid == 0)
				return 1;
			input[ci++] = argv[i][1];
			if (argv[i][1] == 'd')
				D = 1;
			if (argv[i][1] == 'p')
				P = 1;
			if (argv[i][1] == 'l')
				L = 1;
			if (argv[i][1] == 'u')
				U = 1;
			if (argv[i][1] == 'r') {
				r = 1;
				if (i+1 < argc) {
					if (argv[i+1][0] != '-') {
						strcpy(replace_string, argv[i+1]);
						i+=1;
					}
					
				}
				
			}
			if (argv[i][1] == 'R') {
				R = 1;
				if (i+1 < argc) {
					if (argv[i+1][0] != '-') {
						strcpy(replace_string, argv[i+1]);
						i+=1;
					}
				}
				
			}
		}
		else {
			// non-switch args
			strcpy(non[cn++], argv[i]);
		}
		
	}

	if (r == 1 || R == 1) {
		if (strlen(replace_string) == 0) {
			// printf("2 returned\n");
			return 2;
		}
	}
	// printf("D; %d,  P: %d, U: %d, L: %d, r: %d, R: %d\n", D, P, U, L, r, R);
	// printf("Replace: %s\n", replace_string);
	// printf("Non-Args: ");
	// for(int i = 0; i < cn; i++) {
	// 	printf("%s ", non[i]);
	// }
	// printf("\n");

	char buf[10001];
	char cur[10001];
	int cc = 0;
	while (fgets(buf, sizeof buf, stdin)) {
		size_t eol = strcspn(buf, "\n"); 
	 	buf[eol] = '\0';  
	 	
	  	if(eol == 0)
	  		break;

	  	// now apply filters
	  	
	  	if (D==1 && P==1) {
	  		for (int i = 0; i < eol; i++) {
	  			cur[cc++] = buf[i];
	  		}
	  	}
	  	else if (D==1 && P == 0) {
	  		// remove Punctuations
	  		for (int i = 0; i < eol; i++) {
	  			if (ispunct(buf[i]))
	  				continue;
	  			cur[cc++] = buf[i];
	  		}
	  	}
	  	else if(D == 0 && P ==1) {
	  		// remove digits
	  		for (int i = 0; i < eol; i++) {
	  			if (isdigit(buf[i]))
	  				continue;
	  			cur[cc++] = buf[i];
	  		}
	  	}
	  	else {
	  		// default option - remove digits + remove puncts
	  		for (int i = 0; i < eol; i++) {
	  			if (isdigit(buf[i]) || ispunct(buf[i]))
	  				continue;
	  			cur[cc++] = buf[i];
	  		}

	  	}
	  	cur[cc++] = '\n';
	}
	if (L == 1) {
		// lowercase
		for (int i = 0; i < cc; i++) {
			if ( isalpha(cur[i]) ) {
				if (cur[i] >= 'A' && cur[i] <= 'Z')
					cur[i] += 32;
			}
		}
	}
	if (U == 1) {
		// uppercase
		for (int i = 0; i < cc; i++) {
			if ( isalpha(cur[i]) ) {
				if (cur[i] >= 'a' && cur[i] <= 'z')
					cur[i] -= 32;
			}
		}
	}
	if (r == 1) {
		// printf("%d\n", cn);
		if (cn == 0) {
			// replace every word with replace_string
			int l = 0;
			for (int i = 0; i < cc; i++) {
				if (isalnum(cur[i])) {
					if (l < strlen(replace_string)) {
						cur[i] = replace_string[l];
						l += 1;
					}
					
				}
				else {
					l = 0;
				}
			}
		} 
		else {
			// contains non-switch args
			// search ofr prefix with any of the args
			char temp[101];
			int ct = 0;
			for (int i = 0; i < cc; i++) {
				if (isalnum(cur[i])) {
					temp[ct++] = cur[i];
				}
				else {
					temp[ct] = '\0';
					// check if this word has any prefix
					for (int j = 0; j < cn; j++) {
						int itr = 0;
						while (itr < strlen(non[j])) {
							if (non[j][itr] == temp[itr])
								itr+=1;
							else break;
						}
						if (itr == strlen(non[j])) {
							// prefix match found
							itr = 0;
							for (int k = i - ct; k < i; k++) {
								if (itr < strlen(replace_string)){
									cur[k] = replace_string[itr];
									itr+=1;
								}
							}
							break;
						}
					}
					ct = 0;
				}
			}
		}
	}
	if (R == 1) {
		// printf("%d\n", cn);
		if (cn == 0) {
			// replace every word with replace_string - case-insensitive mode
			int l = 0;
			for (int i = 0; i < cc; i++) {
				if (isalnum(cur[i])) {
					if (l < strlen(replace_string)) {
						if (cur[i] >= 'A' && cur[i] <= 'Z')
						{
							// replace with Capital Letter
							if (replace_string[l] >= 'a' && replace_string[l] <= 'z')
								cur[i] = replace_string[l] - 32;
							else  
								cur[i] = replace_string[l];
						}
						else if (cur[i] >= 'a' && cur[i] <= 'z') {
							// replace with small Letter
							if (replace_string[l] >= 'A' && replace_string[l] <= 'Z')
								cur[i] = replace_string[l] + 32;
							else  
								cur[i] = replace_string[l];
						}
						else 
							cur[i] = replace_string[l];
						l += 1;
					}
					
				}
				else {
					l = 0;
				}
			}
		} 
		else {
			// contains non-switch args
			// search ofr prefix with any of the args
			char temp[101];
			int ct = 0;
			for (int i = 0; i < cc; i++) {
				if (isalnum(cur[i])) {
					temp[ct++] = cur[i];
				}
				else {
					temp[ct] = '\0';
					// check if this word has any prefix
					for (int j = 0; j < cn; j++) {
						int itr = 0;
						while (itr < strlen(non[j])) {
							if (tolower(non[j][itr]) == tolower(temp[itr]))
								itr+=1;
							else break;
						}
						if (itr == strlen(non[j])) {
							// prefix match found
							// printf("non: %s, actual: %s\n", non[j], temp);
							itr = 0;
							for (int k = i - ct; k < i; k++) {
								if (itr < strlen(replace_string)){
									if (cur[k] >= 'A' && cur[k] <= 'Z')
									{
										// replace with Capital Letter
										if (replace_string[itr] >= 'a' && replace_string[itr] <= 'z')
											cur[k] = replace_string[itr] - 32;
										else  
											cur[k] = replace_string[itr];
									}
									else if (cur[k] >= 'a' && cur[k] <= 'z') {
										// replace with small Letter
										if (replace_string[itr] >= 'A' && replace_string[itr] <= 'Z')
											cur[k] = replace_string[itr] + 32;
										else  
											cur[k] = replace_string[itr];
									}
									else 
										cur[k] = replace_string[itr];
									itr += 1;
								}
							}
							break;
						}
					}
					ct = 0;
				}
			}
		}

	}
	
	printf("%s", cur);
	return 0;

}