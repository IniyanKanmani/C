pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

// Using wrlock at the start of the ServeClient function and unlocking it at the end of 
// the ServeClient function is 6 times faster than using wrlock on individual if-else cases.

void* ServeClient(char *client) {

	pthread_rwlock_wrlock(&lock);
	
	FILE *fp = fopen(client, "r");
	char ch = ' ';

	if (fp == NULL) {

		printf("CANNOT OPEN FILE: %s\n", client);
		exit(-1);

	}

	ch = getc(fp);

	while (ch != EOF) {

		char strChar[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
		char strNum[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
		int i = 0;
		int j = 0;
		int num = 0;

		while (ch != '\n') {
			
			while (ch != ' ' && ch != '\n') {

				strChar[i] = ch;
				ch = getc(fp);
				i++;

			}

			if (ch == ' ') {

				ch = getc(fp);

			}

			while (ch != '\n') {

				strNum[j] = ch;
				ch = getc(fp);
				j++;

			}

		}

		if (strcmp(strChar, "insertNode") == 0) {

			sscanf(strNum, "%d", &num);
			root = insertNode(root, num);
			printf("[%s]insertNode %d\n", client, num);

		} else if (strcmp(strChar, "deleteNode") == 0) {

			sscanf(strNum, "%d", &num);
			root = deleteNode(root, num);
			printf("[%s]deleteNode %d\n", client, num);

		} else if (strcmp(strChar, "countNodes") == 0) {

			num = countNodes(root);
			printf("[%s]countNodes = %d\n", client, num);

		} else if (strcmp(strChar, "sumSubtree") == 0) {

			num = sumSubtree(root);
			printf("[%s]sumSubtree = %d\n", client, num);

		}

		ch = getc(fp);
		
	}

	fclose(fp);

	pthread_rwlock_unlock(&lock);

	return NULL;

}