#include "database.h"

// Setup table labels.
// Note that this function must be called before all other functions.
// Input:
// pLabelList: a list of sLabel.
// Return:
// 0: Success; -1: Error

typedef struct _string_list
{
    char *pStr;
    struct list_head list;
} string_list;

static char *get_item(struct list_head *pRecordList, int index)
{
    struct list_head *pos;
    int count = 0;
    list_for_each(pos, pRecordList)
    {
        if (count == index)
        {
            return list_entry(pos, string_list, pStr);
        }
        count++;
    }
    return NULL;
}
static struct list_head *current_table = NULL;
int32_t setup_table(const struct list_head *pLabelList)
{
    if (pLabelList == NULL)
    {
        return -1;
    }
    current_table = (struct list_head *)malloc(sizeof(struct list_head));
    return 0;
}
// Add a record.
// Input:
// pRecordList: record list head.
// pRecord: the new record;
// Return:
// 0: Success; -1: Error
int32_t add(struct list_head *pRecordList, sRecord *pRecord)
{
    if (pRecordList == NULL || pRecord == NULL)
    {
        return -1;
    }
    list_add_tail(&pRecord->list, pRecordList);
    return 0;
}

// Get the record list size.
// Input:
// pRecordList: record list head.
// Return:
// -1: Error; others, record size.
int32_t get_size(struct list_head *pRecordList)
{
    if (pRecordList == NULL)
    {
        return -1;
    }
    int count = 0;
    struct list_head *pos;
    list_for_each(pos, pRecordList)
    {
        count++;
    }
    return count;
}

// Query the database and get the result.
// Input:
// pRecordList: record list head.
// pCmd: the query command.
// Output:
// pResultList: the queried result. Note that you should put the queried
// item an ignore all unqueried items.
// Return:
// The number of query result.
int32_t query(struct list_head *pResultList, struct list_head *pRecordList, char *pCmd)
{
    if (pRecordList == NULL || pCmd == NULL)
    {
        return -1;
    }
    // split the command into tokens and into a list
    LIST_HEAD(char_list_head);
    char *token = strtok(pCmd, " ");
    while (token != NULL)
    {
        string_list *newComer = (string_list *)malloc(sizeof(string_list));
        if (!newComer)
        {
            return -1;
        }
        newComer->pStr = token;
        list_add_tail(&newComer->list, &char_list_head);
        token = strtok(NULL, " ");
    }
    // command: SELECT [labels] WHERE [label=”string”] [and|or]...

    struct list_head *pos;
    list_for_each(pos, &char_list_head)
    {

        if (strncmp(container_of(pos, string_list, list)->pStr, "SELECT", strlen("SELECT")) == 0)
        {
            continue;
        }
        if (strncmp(container_of(pos, string_list, list)->pStr, "WHERE", strlen("WHERE")) == 0)
        {
            break;
        }
    }

    // get the result and if label is not chosen, use "NULL" as the result
    pos = NULL;
    int counter = 2;
    list_for_each(pos, pRecordList)
    {
        sRecord *record = list_entry(pos, sRecord, list);
        struct list_head *pos2;
        list_for_each(pos2, &record->data)
        {
            sItem *item = list_entry(pos2, sItem, next);
            // printf("item: %s\n", item->pData);
            printf("%s",get_item(&char_list_head, counter));
        }
    }
    return 0;
}
