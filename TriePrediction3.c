// James Upchurch
// ja389873
// Functions for analyzing corpuses (or corpii, depending on your opinion on changing words to sound more exciting) and predicting text 
// I thank Professor Sean Szumlanski for his aide in the following functions: 
	// printSubTrie          buildTrie
	// printSubTrieHelper    insertString
	// printTrie             getMostFrequentWordHelper
	// printTrieHelper



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

//---------------------------------------------

TrieNode *createTrieNode(void)
{
	TrieNode *newNode = calloc(1, sizeof(TrieNode));
	return newNode;
}

//--------------------------------------------

void printSubTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("- %s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	// Builds each string individually
	for (i = 0; i < 26; i++)
	{
		buffer[k] = i + 'a';
		printSubTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

//---------------------------------------------

void printSubTrie(TrieNode *root)
{
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	buffer[0] = '\0';

	if (root == NULL)
	{
		printf("(EMPTY)\n");
		return;
	}

	printSubTrieHelper(root, buffer, 0);
}

//---------------------------------------------

void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	// Builds each string individually
	for (i = 0; i < 26; i++)
	{
		buffer[k] = i + 'a';
		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

//---------------------------------------------

void printTrie(TrieNode *root)
{
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	buffer[0] = '\0';

	printTrieHelper(root, buffer, 0);
}

//--------------------------------------------

void stripPunctuators(char *str)
{
	int i, k, len = strlen(str);

	for (i = 0; str[i] != '\0'; i++)
	{
		if (!isalpha(str[i]))
		{
			// Shifts characters over one space to overwrite punctuation
			for (k = i; str[k] != '\0'; k++)
				str[k] = str[k + 1];

			// To check skipped character
			i--;
		}
	}
}	

//--------------------------------------------
// Thank you, Professor Sean Szumlanski, for use of your insertString function.
// You lessen my burden.

TrieNode *insertString(TrieNode *root, char *str)
{
	int i, index, len;
	char *temp = malloc(sizeof(char) * (strlen(str) + 1));
	char *temp;
	TrieNode *wizard;
	
	strcpy(temp, str);

	stripPunctuators(temp);

	if (root == NULL)
		root = createTrieNode();

	wizard = root;
 
	len = strlen(temp);


	for (i = 0; i < len; i++)
	{
		index = tolower(temp[i]) - 'a';

		if (wizard->children[index] == NULL)
			wizard->children[index] = createTrieNode();

		wizard = wizard->children[index];
	}

	free(temp);
	wizard->count++;

	return root;
}

//--------------------------------------------
// Thank you, Professor Sean Szumlanski, for use of your buildTrie function.
// You lessen my burden.

TrieNode *buildTrie(char *filename)
{
	int cnt = 0, len = 0, isEnd = 0, len2, i;
	TrieNode *root = NULL, *temp;
	char buffer[MAX_CHARACTERS_PER_WORD + 1], buffer2[MAX_CHARACTERS_PER_WORD + 1];

	FILE *ifp;

	if ((ifp = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in buildTrie().\n", filename);
		return NULL;
	}

	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		root = insertString(root, buffer);

		if (cnt != 0)
		{
			len = strlen(buffer2);

			// Checks if word is end of sentence
			if ((buffer2[len - 1] == '.') || (buffer2[len - 1] == '!') || (buffer2[len - 1] == '?'))
				isEnd = 1;
		}

		cnt++;

		if (cnt != 1 && isEnd != 1)
		{
			temp = getNode(root, buffer2);
			temp->subtrie = insertString(temp->subtrie, buffer);	
		}

		isEnd = 0;

		strcpy(buffer2, buffer);
	}

	fclose(ifp);
	return root;
}

//--------------------------------------------

TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	if (root == NULL)
		return NULL;

	for (i = 0; i < 26; i++)
	{
		destroyTrie(root->children[i]);
	}

	if (root->subtrie != NULL)
		destroyTrie(root->subtrie);
	else
		return NULL;

	free(root);

	return NULL;
}

//--------------------------------------------
// Finds the count of the most frequent word in a trie

int *findTheMax(TrieNode *root, int *max)
{
	int i;

	if (root == NULL)
		return max;

	if (root->count > *max)
		*max = root->count;

	for (i = 0; i < 26; i++)
	{
		max = findTheMax(root->children[i], max);
	}

	return max;
}

//--------------------------------------------

void getMostFrequentWordHelper(TrieNode *root, char *temp, char *str, int k, int max, int *cnt)
{
	int i;

	if (root == NULL)
		return;

	if (root->count == max && *cnt == 0)
	{
		strcpy(str, temp);
		*cnt = *cnt + 1;
	}

	temp[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		temp[k] = i + 'a';
		getMostFrequentWordHelper(root->children[i], temp, str, k + 1, max, cnt);
	}

	temp[k] = '\0';
}

//-----------------------------------------------

void getMostFrequentWord(TrieNode *root, char *str)
{
	int *cnt = calloc(1, sizeof(int)), *max = calloc(1, sizeof(int));
	char *temp = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));

	if (root == NULL)
	{
		str[0] = '\0';
		free(temp);
		free(cnt);
		free(max);		
		return;
	}
	
	max = findTheMax(root, max);

	getMostFrequentWordHelper(root, temp, str, 0, *max, cnt);

	free(temp);
	free(cnt);
	free(max);
}

//--------------------------------------------

int containsWord(TrieNode *root, char *str)
{
	int i, k, len = strlen(str);
	char temp[len];

	for(i = 0, k = 0; i < len; i++, k++)
	{
		if (!isalpha(str[i]))
		{
			i++;
			if (i >= len)
				continue;
		}

		temp[k] = tolower(str[i]);

		if (root->children[temp[k] - 'a'] == NULL)
			return 0;
		else		
			root = root->children[temp[k] - 'a'];
	}

	if (root->count > 0)
		return 1;
	else
		return 0;
}

//--------------------------------------------

TrieNode *getNode(TrieNode *root, char *str)
{
	int i, len;
	char temp[MAX_CHARACTERS_PER_WORD + 1];

	stripPunctuators(str);

	if (!containsWord(root, str))
	{
		return NULL;
	}
	
	strcpy(temp, str);

	for (i = 0; temp[i] != '\0'; i++)
	{
		temp[i] = tolower(temp[i]);

		if (root->children[temp[i] - 'a'] == NULL)
			return NULL;
		else
			root = root->children[temp[i] - 'a'];

	}

	if (root->count > 0)
		return root;
	else
		return NULL;

}

//--------------------------------------------
// Counts all valid strings after root

int prefixCountHelper(TrieNode *root)
{
	int i, total = 0;

	for (i = 0; i < 26; i++)
	{
		if (root->children[i] == NULL)
			;
		else
			total += root->children[i]->count + prefixCountHelper(root->children[i]);
	}

	return total;
}

//--------------------------------------------

int prefixCount(TrieNode *root, char *str)
{
	int i, len = strlen(str), total = 0;

	if (root == NULL)
		return -1;

	for (i = 0; i < len; i++)
	{
		if (root->children[tolower(str[i]) - 'a'] != NULL)
			root = root->children[tolower(str[i]) - 'a'];
		else return 0;
	}

	if (root->count > 0)
		total += root->count;
	
	total += prefixCountHelper(root);

	return total;
}

//--------------------------------------------


void mostFrequentWordHelperFancy(TrieNode *root, char *temp, char *str, int k, int max, int *cnt)
{
	int i;

	if (root == NULL)
		return;

	if (root->count == max && *cnt == 0)
	{
		printf("%s", temp);
		strcpy(str, temp);
		*cnt = *cnt + 1;
	}

	temp[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		temp[k] = i + 'a';
		mostFrequentWordHelperFancy(root->children[i], temp, str, k + 1, max, cnt);
	}

	temp[k] = '\0';
}

//-----------------------------------------------

void getMostFrequentWordFancy(TrieNode *root, char *str)
{
	int *cnt = calloc(1, sizeof(int)), *max = calloc(1, sizeof(int));
	char *temp = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));

	max = findTheMax(root, max);

	mostFrequentWordHelperFancy(root, temp, str, 0, *max, cnt);

	free(temp);
	free(cnt);
	free(max);
}

//--------------------------------------------

void textPrediction(TrieNode *root, char *str, int n)
{
	int i = 0;
	char tmpStr[MAX_CHARACTERS_PER_WORD + 1];

	TrieNode *temp = getNode(root, str);

	if (temp == NULL)
	{
		printf("\n");
		return;
	}
	else
		printf(" ");
	
	if (n == 0)
	{
		getMostFrequentWordFancy(temp->subtrie, tmpStr);
		return;
	}

	while (temp->subtrie != NULL && (i < n))
	{
		getMostFrequentWordFancy(temp->subtrie, tmpStr);
		temp = getNode(root, tmpStr);

		i++;		
		if (temp->subtrie != NULL && (i < n))
			printf("%c", ' ');
	}

	printf("\n");
}

//--------------------------------------------

void stringCmd(TrieNode *root, char *str)
{
	int len = strlen(str);
	char tmp[len];
	TrieNode *temp;

	strcpy(tmp, str);


	if (containsWord(root, tmp))
	{
		temp = getNode(root, tmp);
		printf("%s\n", str);
	}
	else
	{
		printf("%s\n", str);
		printf("(INVALID STRING)\n");
		return;
	}

	printSubTrie(temp->subtrie);
}

//--------------------------------------------

void executeInput(TrieNode *root, char *filename)
{
	int i = 0, n;
	char cmd[MAX_CHARACTERS_PER_WORD];
	FILE *ifp;

	if ((ifp = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in executeInput().\n", filename);
		return;
	}

	while (fscanf(ifp,"%s", cmd) != EOF)
	{
		if (cmd[i] == '!')
			printTrie(root);

		else if (cmd[i] == '@')
		{
			fscanf(ifp, "%s", cmd);
			fscanf(ifp, "%d", &n);
			printf("%s", cmd);	
			textPrediction(root, cmd, n);
		}

		else
			stringCmd(root, cmd);
	}
	fclose(ifp);
}

//---------------------------------------------

double difficultyRating(void)
{
	double difficulty = 4.468;
	return difficulty;
}

//--------------------------------------------

double hoursSpent(void)
{
	double hours = 45;
	return hours;
}

//--------------------------------------------

int main(int argc, char **argv)
{
	TrieNode *root, *temp;
	char *str = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));
	int i, *max = calloc(1, sizeof(int));

	root = buildTrie(argv[1]);

	// Reads input file and executes commands
	executeInput(root, argv[2]);

	root = destroyTrie(root);
	free(str);
	free(max);

	return 0;
}