#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     
#include <stdlib.h>    
#include <stdbool.h>  
#include <string.h>

typedef enum _TOKEN_TYPE
{
	TOKEN_STRING,
	TOKEN_NUMBER,
}TOKEN_TYPE;
typedef struct _TOKEN
{
	TOKEN_TYPE type;
	union
	{
		char* string;
		double number;
	};
	bool isArray;
}TOKEN;
#define TOKEN_COUNT 40000
typedef struct _JSON
{
	TOKEN tokens[TOKEN_COUNT];
}JSON;
char* readfile(char* filename, int* readszie)
{
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) return NULL;
	int size;
	char* buffer;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = malloc(size + 1);
	memset(buffer, 0, size + 1);
	
	if (fread(buffer, size, 1, fp) < 1)
	{
		*readszie = 0;
		free(buffer);
		fclose(fp);
		return NULL;
	}
	//printf("%s\n",buffer);
	*readszie = size;
	fclose(fp);
	return buffer;

}
void parseJSON(char* doc, int size, JSON* json)
{
	int tokenIndex = 0;
	int pos = 0;
	if (doc[pos] != '[') return;

	pos++;
	while (pos < size)
	{
		//printf("%c",doc[pos]);
		switch (doc[pos])
		{
			case '"':
			{
				
				char* begin = doc + pos + 1;

				char* end = strchr(begin, '"');
				if (end == NULL) break;
				char* frame_id = end + 2;
				char* char_id = begin + 29;//29 == '/'
				int stringLength = end - begin;

				json->tokens[tokenIndex].type = TOKEN_STRING;
				json->tokens[tokenIndex].string = malloc(stringLength + 1);
				//printf("%s\n", json->tokens[tokenIndex].string);
				memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
				memcpy(json->tokens[tokenIndex].string, begin, stringLength);
				//printf("%d\n",tokenIndex);
				//printf("%s\n", json->tokens[tokenIndex].string);
				char b[20];
				memset(b, 0, sizeof(b));
				char* localval2 = 0;
				//printf("%c\n",*char_id);
				//printf("%c\n",*(char_id+1));
				if (strncmp(json->tokens[tokenIndex].string, "/home/hi/Downloads/object2_5/",29) == 0)
				{
					//printf("sdfsdgsdg\n");
					char localcnt = 0;
					//printf("%c\n", *char_id);
					while (1)
					{
						localval2 = char_id + localcnt;
						if (*localval2 == '"')
						{
							*(b + localcnt) = '\0';
							break;
						}
						//printf("%c %d\n",*localval,localcnt);
						*(b + localcnt) = *localval2;
						localcnt++;
					}
					printf("filename : %s\n", b);
				}

				char a[10];
				memset(a, 0, sizeof(a));
				char* localval = 0;
				if (strcmp(json->tokens[tokenIndex].string, "frame_id") == 0)
				{
					printf("\n");
					char localcnt = 0;
					while (1)
					{
						localval = frame_id + localcnt; 
						if (*localval == ',')
						{
							*(a + localcnt) = '\0';
							break;
						}
						//printf("%c %d\n",*localval,localcnt);
						*(a + localcnt) = *localval;
						localcnt++;
					}
					printf("frame id %s\n", a);
				}
				
				char c[10];
				memset(c, 0, sizeof(c));
				char* localval3 = 0;
				if (strcmp(json->tokens[tokenIndex].string, "class_id") == 0)
				{
					char localcnt = 0;
					while (1)
					{
						localval3 = frame_id + localcnt;
						if (*localval3 == ',')
						{
							*(c + localcnt) = '\0';
							break;
						}
						//printf("%c %d\n",*localval,localcnt);
						*(c + localcnt) = *localval3;
						localcnt++;
					}
					printf("class_id : %s\n", c);
				}

				tokenIndex++;
				pos = pos + stringLength + 1;
			}
			break;
		}
		pos++;
	}
}
void freeJSON(JSON* json)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].type == TOKEN_STRING)
			free(json->tokens[i].string);
	}
}
int main()
{
	int size;
	char* doc = readfile("result.json",&size);
	if (doc == NULL) return -1;

	JSON json = {0,};
	parseJSON(doc, size, &json);
	printf("Title: %s\n",json.tokens[1].string);
	printf("Genre: %s\n", json.tokens[3].string);
	printf("Director: %s\n", json.tokens[5].string);

	//freeJSON(&json);
	free(doc);
	return 0;
}